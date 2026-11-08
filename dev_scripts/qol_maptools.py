#!/usr/bin/env python3
"""[QoL] Map tools for building new regions without Porymap.

Library + CLI:
  python3 dev_scripts/qol_maptools.py render LAYOUT_ID out.png   draw a layout
  python3 dev_scripts/qol_maptools.py info LAYOUT_ID             size/tilesets
  python3 dev_scripts/qol_maptools.py sheet TILESET out.png      numbered metatile sheet

Formats (pokeemerald-expansion):
  map.bin: u16 per block = metatile id (bits 0-9) | collision (10-11) | elevation (12-15)
  metatiles.bin: 8 u16 tile entries per metatile (bottom layer 2x2, top layer 2x2);
    entry = tile (0-9) | hflip (10) | vflip (11) | palette (12-15)
  FRLG primary tilesets: 640 tiles/metatiles, 7 palettes; Emerald: 512, 6.
"""
import json
import os
import re
import struct
import sys

from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def _read(path):
    return open(os.path.join(ROOT, path), encoding='utf-8').read()


_TILESETS = None


def tilesets():
    """gTileset_X -> {'secondary', 'tiles', 'pals', 'metatiles', 'attrs'} (file paths)."""
    global _TILESETS
    if _TILESETS:
        return _TILESETS
    headers = _read('src/data/tilesets/headers.h')
    graphics = _read('src/data/tilesets/graphics.h')
    metas = _read('src/data/tilesets/metatiles.h')
    tiles = dict(re.findall(r'const u32 (\w+)\[\] = INCGFX_U32\("([^"]+?)/tiles\.png"', graphics))
    pals = {}
    for m in re.finditer(r'const u16 (?:ALIGNED\(4\) )?(\w+)\[\]\[16\] =\s*\{(.*?)\};', graphics, re.S):
        p = re.findall(r'"([^"]+?)/palettes/\d+\.pal"', m.group(2))
        if p:
            pals[m.group(1)] = p[0]
    mt = dict(re.findall(r'const u16 (\w+)\[\] = INCBIN_U16\("([^"]+?)/metatiles\.bin"\)', metas))
    at = dict(re.findall(r'const u(?:16|32) (\w+)\[\] = INCBIN_U(?:16|32)\("([^"]+?)/metatile_attributes\.bin"\)', metas))
    out = {}
    for m in re.finditer(r'const struct Tileset (\w+) =\s*\{(.*?)\};', headers, re.S):
        body = m.group(2)
        g = lambda k: (re.search(r'\.%s = (\w+)' % k, body) or [None, None])[1]
        sec = re.search(r'\.isSecondary = (\w+)', body).group(1) == 'TRUE'
        d = {'secondary': sec, 'tiles': tiles.get(g('tiles')), 'pals': pals.get(g('palettes')),
             'metatiles': mt.get(g('metatiles')), 'attrs': at.get(g('metatileAttributes'))}
        out[m.group(1)] = d
    _TILESETS = out
    return out


def layouts():
    return {l['id']: l for l in json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts'] if 'id' in l}


def is_frlg_tileset(name):
    return name.endswith('Frlg') or tilesets()[name]['tiles'] and '_frlg' in tilesets()[name]['tiles']


def read_blocks(path, w, h):
    data = open(os.path.join(ROOT, path), 'rb').read()
    vals = struct.unpack('<%dH' % (len(data) // 2), data)
    return [list(vals[y * w:(y + 1) * w]) for y in range(h)]


def write_blocks(path, grid):
    full = os.path.join(ROOT, path)
    os.makedirs(os.path.dirname(full), exist_ok=True)
    with open(full, 'wb') as f:
        for row in grid:
            f.write(struct.pack('<%dH' % len(row), *row))


def load_pal(path):
    lines = open(os.path.join(ROOT, path)).read().split('\n')[3:19]
    return [tuple(int(v) for v in l.split()[:3]) for l in lines]


class TilesetPair:
    """Primary + secondary tileset, able to draw any metatile."""

    def __init__(self, primary, secondary):
        ts = tilesets()
        self.p, self.s = ts[primary], ts[secondary]
        self.frlg = '_frlg' in (self.p['tiles'] or '')
        self.ntiles = 640 if self.frlg else 512
        self.nmeta = 640 if self.frlg else 512
        self.npals = 7 if self.frlg else 6
        self.attr_size = 4 if self.frlg else 2
        self.tiles = [self._tiles(self.p), self._tiles(self.s)]
        pals = []
        for i in range(16):
            src = self.p if i < self.npals else self.s
            pals.append(load_pal('%s/palettes/%02d.pal' % (src['pals'], i)))
        self.pals = pals
        self.meta = [self._u16(self.p['metatiles']), self._u16(self.s['metatiles'])]
        self.attrs = [self._attrs(self.p), self._attrs(self.s)]
        self._cache = {}

    def _u16(self, path):
        data = open(os.path.join(ROOT, path + '/metatiles.bin'), 'rb').read()
        return struct.unpack('<%dH' % (len(data) // 2), data)

    def _attrs(self, ts):
        data = open(os.path.join(ROOT, ts['attrs'] + '/metatile_attributes.bin'), 'rb').read()
        fmt = 'I' if self.attr_size == 4 else 'H'
        return struct.unpack('<%d%s' % (len(data) // self.attr_size, fmt), data)

    def _tiles(self, ts):
        im = Image.open(os.path.join(ROOT, ts['tiles'] + '/tiles.png'))
        w, h = im.size
        px = im.load()
        out = []
        for ty in range(h // 8):
            for tx in range(w // 8):
                out.append([[px[tx * 8 + x, ty * 8 + y] & 15 for x in range(8)] for y in range(8)])
        return out

    def count(self, secondary=False):
        return len(self.meta[1 if secondary else 0]) // 8

    def behavior(self, mid):
        a = self.attrs[0][mid] if mid < self.nmeta else self.attrs[1][mid - self.nmeta]
        return a & (0x1FF if self.attr_size == 4 else 0xFF)

    def tile_entries(self, mid):
        if mid < self.nmeta:
            m = self.meta[0][mid * 8:mid * 8 + 8]
        else:
            m = self.meta[1][(mid - self.nmeta) * 8:(mid - self.nmeta) * 8 + 8]
        return m

    def draw(self, mid):
        if mid in self._cache:
            return self._cache[mid]
        im = Image.new('RGB', (16, 16), (255, 0, 255))
        px = im.load()
        ents = self.tile_entries(mid)
        if len(ents) == 8:
            for layer in range(2):
                for q in range(4):
                    e = ents[layer * 4 + q]
                    t, hf, vf, pal = e & 0x3FF, (e >> 10) & 1, (e >> 11) & 1, e >> 12
                    src = self.tiles[0][t] if t < self.ntiles else (
                        self.tiles[1][t - self.ntiles] if t - self.ntiles < len(self.tiles[1]) else None)
                    if src is None:
                        continue
                    ox, oy = (q & 1) * 8, (q >> 1) * 8
                    for y in range(8):
                        for x in range(8):
                            c = src[7 - y if vf else y][7 - x if hf else x]
                            if c == 0 and layer:
                                continue
                            if c == 0 and not layer:
                                c = 0
                            px[ox + x, oy + y] = self.pals[pal][c]
        self._cache[mid] = im
        return im


def render_grid(pair, grid, scale=1, marks=None):
    h, w = len(grid), len(grid[0])
    im = Image.new('RGB', (w * 16, h * 16))
    for y in range(h):
        for x in range(w):
            im.paste(pair.draw(grid[y][x] & 0x3FF), (x * 16, y * 16))
    if marks:
        from PIL import ImageDraw
        d = ImageDraw.Draw(im)
        for (x, y, col, label) in marks:
            d.rectangle([x * 16, y * 16, x * 16 + 15, y * 16 + 15], outline=col)
            if label:
                d.text((x * 16 + 2, y * 16 + 2), label, fill=col)
    if scale != 1:
        im = im.resize((im.width * scale, im.height * scale), Image.NEAREST)
    return im


def render_layout(lid, out, scale=1):
    l = layouts()[lid]
    pair = TilesetPair(l['primary_tileset'], l['secondary_tileset'])
    grid = read_blocks(l['blockdata_filepath'], l['width'], l['height'])
    render_grid(pair, grid, scale).save(out)


def sheet(primary, secondary, out, which='both'):
    """Numbered sheet of metatiles (hex ids) for picking."""
    from PIL import ImageDraw
    pair = TilesetPair(primary, secondary)
    ids = []
    if which in ('both', 'primary'):
        ids += list(range(pair.count()))
    if which in ('both', 'secondary'):
        ids += [pair.nmeta + i for i in range(pair.count(True))]
    cols = 16
    rows = (len(ids) + cols - 1) // cols
    cell = 40
    im = Image.new('RGB', (cols * cell, rows * cell), (40, 40, 40))
    d = ImageDraw.Draw(im)
    for k, mid in enumerate(ids):
        x, y = (k % cols) * cell, (k // cols) * cell
        im.paste(pair.draw(mid).resize((32, 32), Image.NEAREST), (x + 4, y))
        d.text((x + 2, y + 30), '%X' % mid, fill=(255, 255, 0))
    im.save(out)


if __name__ == '__main__':
    cmd = sys.argv[1]
    if cmd == 'render':
        render_layout(sys.argv[2], sys.argv[3], int(sys.argv[4]) if len(sys.argv) > 4 else 1)
    elif cmd == 'info':
        print(json.dumps(layouts()[sys.argv[2]], indent=1))
    elif cmd == 'sheet':
        sheet(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5] if len(sys.argv) > 5 else 'both')


def render_grid_labels(lid, out, step=5):
    """Render a layout with a coordinate grid (for picking stamp rectangles)."""
    from PIL import ImageDraw
    l = layouts()[lid]
    pair = TilesetPair(l['primary_tileset'], l['secondary_tileset'])
    grid = read_blocks(l['blockdata_filepath'], l['width'], l['height'])
    im = render_grid(pair, grid)
    d = ImageDraw.Draw(im)
    for x in range(0, l['width'], step):
        d.line([(x * 16, 0), (x * 16, im.height)], fill=(255, 0, 0))
        for y in range(0, l['height'], step):
            d.text((x * 16 + 2, y * 16 + 2), '%d,%d' % (x, y), fill=(255, 255, 255))
    for y in range(0, l['height'], step):
        d.line([(0, y * 16), (im.width, y * 16)], fill=(255, 0, 0))
    im.save(out)


def used_sheet(lids, out, cols=12):
    """Sheet of every metatile used by the given layouts (same tileset pair), with ids and counts."""
    from PIL import ImageDraw
    import collections
    L = layouts()
    l0 = L[lids[0]]
    pair = TilesetPair(l0['primary_tileset'], l0['secondary_tileset'])
    cnt = collections.Counter()
    for lid in lids:
        l = L[lid]
        for row in read_blocks(l['blockdata_filepath'], l['width'], l['height']):
            for b in row:
                cnt[b & 0x3FF] += 1
    ids = sorted(cnt)
    cell = 56
    rows = (len(ids) + cols - 1) // cols
    im = Image.new('RGB', (cols * cell, rows * cell), (30, 30, 30))
    d = ImageDraw.Draw(im)
    for k, mid in enumerate(ids):
        x, y = (k % cols) * cell, (k // cols) * cell
        im.paste(pair.draw(mid).resize((32, 32), Image.NEAREST), (x + 12, y + 2))
        d.text((x + 2, y + 36), '%03X:%d' % (mid, cnt[mid]), fill=(255, 255, 0))
    im.save(out)
    return cnt


def map_json_for_layout(lid):
    """The map.json files that use a layout."""
    out = []
    for d in os.listdir(os.path.join(ROOT, 'data/maps')):
        p = os.path.join(ROOT, 'data/maps', d, 'map.json')
        if os.path.exists(p):
            j = json.load(open(p))
            if j.get('layout') == lid:
                out.append(j)
    return out


def buildings(lid, min_cells=6):
    """Bounding boxes of solid secondary-tile clusters (houses, gyms...) and the
    warps inside them, to pick stamps from."""
    L = layouts()[lid]
    g = read_blocks(L['blockdata_filepath'], L['width'], L['height'])
    H, W = len(g), len(g[0])
    npri = 640 if L.get('layout_version') == 'frlg' else 512
    sec = [[(g[y][x] & 0x3FF) >= npri for x in range(W)] for y in range(H)]
    seen = set()
    warps = []
    for j in map_json_for_layout(lid)[:1]:
        warps = [(w['x'], w['y'], w['dest_map']) for w in j.get('warp_events', [])]
    out = []
    for y in range(H):
        for x in range(W):
            if sec[y][x] and (x, y) not in seen:
                stack, comp = [(x, y)], []
                seen.add((x, y))
                while stack:
                    cx, cy = stack.pop()
                    comp.append((cx, cy))
                    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                        nx, ny = cx + dx, cy + dy
                        if 0 <= nx < W and 0 <= ny < H and sec[ny][nx] and (nx, ny) not in seen:
                            seen.add((nx, ny))
                            stack.append((nx, ny))
                if len(comp) >= min_cells:
                    xs = [c[0] for c in comp]
                    ys = [c[1] for c in comp]
                    x0, y0, x1, y1 = min(xs), min(ys), max(xs), max(ys)
                    ws = [w for w in warps if x0 <= w[0] <= x1 and y0 <= w[1] <= y1 + 1]
                    out.append(((x0, y0, x1 - x0 + 1, y1 - y0 + 1), ws))
    return out


def crop_labeled(lid, x0, y0, w, h, out, sc=2):
    """Zoomed crop with absolute coordinates on each cell."""
    from PIL import ImageDraw
    L = layouts()[lid]
    g = read_blocks(L['blockdata_filepath'], L['width'], L['height'])
    pair = TilesetPair(L['primary_tileset'], L['secondary_tileset'])
    sub = [row[x0:x0 + w] for row in g[y0:y0 + h]]
    im = render_grid(pair, sub, sc)
    d = ImageDraw.Draw(im)
    for y in range(len(sub)):
        for x in range(len(sub[0])):
            if (x0 + x) % 2 == 0 and (y0 + y) % 2 == 0:
                d.text((x * 16 * sc + 1, y * 16 * sc + 1), '%d,%d' % (x0 + x, y0 + y), fill=(255, 0, 255))
    im.save(out)
