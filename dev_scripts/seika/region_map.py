#!/usr/bin/env python3
"""[QoL] Draw Seika's Fly map in the style of FireRed's Kanto map.

The picture is drawn with the Kanto map's own palette (sea stripes, green
land shading, yellow routes, red town dots), then cut into 8x8 tiles for the
region map's 8bpp background. Writes graphics/pokenav/region_map/map_seika.*
and src/data/region_map/region_map_layout_seika.h (which cell is which map
section). Cell (x, y) of the 28x15 grid sits at tile (x+1, y+2).

  python3 dev_scripts/seika/region_map.py [preview.png]
"""
import math
import os
import sys

from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
RM = os.path.join(ROOT, 'graphics/pokenav/region_map')
W_CELLS, H_CELLS = 28, 15
PAL_BASE = 112  # the palette is loaded at BG palette 7

# colour indices in map_kanto.pal
SEA_A, SEA_B = 10, 13
LAND = [9, 15, 20, 22, 26]          # coast -> inland
ROCK = [16, 23, 25]
ROUTE, ROUTE_EDGE = 4, 17
WATER_ROUTE = 18

# map sections: (id, x, y, w, h, kind) kind: town / route / water
SECTIONS = [
    ('MAPSEC_SEIKA_LANTERNPORT', 17, 14, 1, 1, 'town'),
    ('MAPSEC_SEIKA_LANTERN_ROAD', 17, 12, 1, 2, 'route'),
    ('MAPSEC_SEIKA_INKBRUSH', 17, 11, 1, 1, 'town'),
    ('MAPSEC_SEIKA_MOSSGROVE_WOODS', 13, 11, 4, 1, 'route'),
    ('MAPSEC_SEIKA_MOSSGROVE', 12, 11, 1, 1, 'town'),
    ('MAPSEC_SEIKA_KILN_ROAD', 12, 9, 1, 2, 'route'),
    ('MAPSEC_SEIKA_KILN_CITY', 12, 8, 1, 1, 'town'),
    ('MAPSEC_SEIKA_TIDE_ROAD', 13, 8, 7, 1, 'route'),
    ('MAPSEC_SEIKA_TIDEWATCH', 20, 8, 1, 1, 'town'),
    ('MAPSEC_SEIKA_HOLLOW_PATH', 20, 6, 1, 2, 'route'),
    ('MAPSEC_SEIKA_SILKHOLLOW', 20, 5, 1, 1, 'town'),
    ('MAPSEC_SEIKA_WEAVERS_ROAD', 15, 5, 5, 1, 'route'),
    ('MAPSEC_SEIKA_FROSTMERE', 14, 5, 1, 1, 'town'),
    ('MAPSEC_SEIKA_FROST_ROAD', 14, 4, 1, 1, 'route'),
    ('MAPSEC_SEIKA_DUSKPEAK', 14, 3, 1, 1, 'town'),
    ('MAPSEC_SEIKA_SKYREACH_PASS', 14, 2, 1, 1, 'route'),
    ('MAPSEC_SEIKA_STARWATCH_PLATEAU', 14, 1, 1, 1, 'town'),
    ('MAPSEC_SEIKA_SKY_SHRINE', 14, 0, 1, 1, 'route'),
]


def load_pal(p):
    lines = open(p).read().split('\n')[3:]
    return [tuple(int(v) for v in l.split()[:3]) for l in lines if l.strip()]


def kanto_tile(n):
    """8x8 tile n of the Kanto map graphics, as palette indices."""
    im = Image.open(os.path.join(RM, 'map_kanto.png'))
    px = im.load()
    tx, ty = n % 16, n // 16
    return [[(px[tx * 8 + x, ty * 8 + y] - PAL_BASE) % 256 for x in range(8)] for y in range(8)]


def centre(x, y):
    return ((x + 1) * 8 + 4, (y + 2) * 8 + 4)


def draw():
    S = 64 * 8
    img = [[SEA_A if ((x + y // 2) % 4) < 2 else SEA_B for x in range(S)] for y in range(S)]
    # real sea pattern: Kanto's sea tile, repeated
    sea = kanto_tile(9)
    for y in range(S):
        for x in range(S):
            img[y][x] = sea[y % 8][x % 8]

    outline = [(10.6, -1.0), (13.8, -1.6), (16.6, -1.0), (19.2, 0.4), (21.6, 1.8), (22.6, 4.4),
               (21.6, 6.6), (22.8, 8.6), (21.6, 11.0), (19.8, 12.6), (18.6, 15.6), (16.2, 15.4),
               (14.6, 13.8), (11.8, 13.6), (10.0, 12.2), (10.8, 9.8), (9.6, 7.4), (11.0, 5.0),
               (10.0, 2.8)]

    def inside(cx, cy):
        n, hit = len(outline), False
        for i in range(n):
            (x1, y1), (x2, y2) = outline[i], outline[(i + 1) % n]
            if (y1 > cy) != (y2 > cy) and cx < x1 + (cy - y1) * (x2 - x1) / (y2 - y1):
                hit = not hit
        return hit

    def noise(px, py):
        return (0.30 * math.sin(px / 4.3 + py / 9.1) + 0.22 * math.sin(py / 3.7 - px / 11.0)
                + 0.12 * math.sin((px + py) / 2.1))

    def land_at(px, py):
        cx, cy = px / 8.0 - 1, py / 8.0 - 2      # cell coordinates
        if not inside(cx + noise(px, py), cy + noise(py + 17, px)):
            return False
        # the bay south of Tide Road
        if ((cx - 16.8) / 3.2) ** 2 + ((cy - 9.8) / 1.05) ** 2 < 1.0 + 0.25 * noise(px, py):
            return False
        # Frostmere's lake
        if ((cx - 13.0) / 0.8) ** 2 + ((cy - 6.4) / 0.55) ** 2 < 1.0:
            return False
        return True
    mask = [[land_at(x, y) for x in range(S)] for y in range(S)]
    # distance to the coast (chessboard, capped) for the green shading
    INF = 99
    dist = [[0 if not mask[y][x] else INF for x in range(S)] for y in range(S)]
    for _ in range(2):
        for y in range(S):
            for x in range(S):
                if dist[y][x]:
                    best = dist[y][x]
                    for dx, dy in ((-1, 0), (0, -1), (-1, -1), (1, -1)):
                        nx, ny = x + dx, y + dy
                        if 0 <= nx < S and 0 <= ny < S:
                            best = min(best, dist[ny][nx] + 1)
                    dist[y][x] = best
        for y in range(S - 1, -1, -1):
            for x in range(S - 1, -1, -1):
                if dist[y][x]:
                    best = dist[y][x]
                    for dx, dy in ((1, 0), (0, 1), (1, 1), (-1, 1)):
                        nx, ny = x + dx, y + dy
                        if 0 <= nx < S and 0 <= ny < S:
                            best = min(best, dist[ny][nx] + 1)
                    dist[y][x] = best
    for y in range(S):
        for x in range(S):
            if mask[y][x]:
                d = dist[y][x]
                img[y][x] = LAND[0 if d < 2 else 1 if d < 5 else 2 if d < 9 else 3 if d < 14 else 4]
    # Mt. Skyreach: a grey range across the north
    for y in range(S):
        for x in range(S):
            cx, cy = x / 8.0 - 1, y / 8.0 - 2
            r = ((cx - 15.2) / 4.2) ** 2 + ((cy + 0.4) / 2.4) ** 2 + 0.35 * noise(x * 2, y * 2)
            if mask[y][x] and r < 1.0:
                img[y][x] = ROCK[0 if r > 0.7 else 1 if r > 0.35 else 2]
    def line(p, q, colour, width):
        (x0, y0), (x1, y1) = p, q
        n = max(abs(x1 - x0), abs(y1 - y0), 1)
        for i in range(n + 1):
            x = round(x0 + (x1 - x0) * i / n)
            y = round(y0 + (y1 - y0) * i / n)
            for dy in range(-(width // 2), width - width // 2):
                for dx in range(-(width // 2), width - width // 2):
                    img[y + dy][x + dx] = colour

    # routes between the sections, in order
    path = [(17, 14), (17, 11), (12, 11), (12, 8), (20, 8), (20, 5), (14, 5), (14, 0)]
    pts = [centre(*c) for c in path]
    for a, b in zip(pts, pts[1:]):
        line(a, b, ROUTE_EDGE, 5)
    for a, b in zip(pts, pts[1:]):
        line(a, b, ROUTE, 3)
    # the S.S. TIDAL's sea lane south of Lanternport
    lx, ly = centre(17, 14)
    line((lx, ly + 6), (lx, ly + 14), WATER_ROUTE, 2)
    # town dots: Kanto's dot tile
    dot = kanto_tile(54)
    for sid, x, y, w, h, kind in SECTIONS:
        if kind == 'town':
            ox, oy = (x + 1) * 8, (y + 2) * 8
            for yy in range(8):
                for xx in range(8):
                    img[oy + yy][ox + xx] = dot[yy][xx]
    return img


def cut(img):
    """-> (tiles, tilemap): unique 8x8 tiles and the 64x64 byte map."""
    tiles, index, tmap = [], {}, []
    sea = tuple(tuple(r) for r in kanto_tile(9))
    tiles.append(sea)
    index[sea] = 0
    for ty in range(64):
        for tx in range(64):
            t = tuple(tuple(img[ty * 8 + y][tx * 8:tx * 8 + 8]) for y in range(8))
            if t not in index:
                index[t] = len(tiles)
                tiles.append(t)
            tmap.append(index[t])
    return tiles, tmap


def write(tiles, tmap):
    assert len(tiles) <= 256, len(tiles)
    rows = (len(tiles) + 15) // 16
    im = Image.new('P', (128, rows * 8))
    kanto = Image.open(os.path.join(RM, 'map_kanto.png'))
    im.putpalette(kanto.getpalette())
    px = im.load()
    for n, t in enumerate(tiles):
        tx, ty = n % 16, n // 16
        for y in range(8):
            for x in range(8):
                px[tx * 8 + x, ty * 8 + y] = PAL_BASE + t[y][x]
    im.save(os.path.join(RM, 'map_seika.png'))
    open(os.path.join(RM, 'map_seika.bin'), 'wb').write(bytes(tmap))
    open(os.path.join(RM, 'map_seika.pal'), 'w').write(open(os.path.join(RM, 'map_kanto.pal')).read())
    # which cell is which map section
    grid = [['MAPSEC_NONE'] * W_CELLS for _ in range(H_CELLS)]
    import json
    known = {m['id'] for m in json.load(open(os.path.join(ROOT, 'src/data/region_map/region_map_sections.json')))['map_sections']}
    for sid, x, y, w, h, kind in SECTIONS:
        if sid not in known:   # not built yet
            continue
        for yy in range(y, y + h):
            for xx in range(x, x + w):
                grid[yy][xx] = sid
    with open(os.path.join(ROOT, 'src/data/region_map/region_map_layout_seika.h'), 'w') as f:
        f.write('// [QoL] Generated by dev_scripts/seika/region_map.py\n')
        f.write('static const u8 sRegionMapSections_Seika[MAP_HEIGHT][MAP_WIDTH] = {\n')
        for row in grid:
            f.write('    {' + ', '.join(row) + '},\n')
        f.write('};\n')


def preview(img, path):
    pal = load_pal(os.path.join(RM, 'map_kanto.pal'))
    out = Image.new('RGB', (30 * 8, 18 * 8))
    p = out.load()
    for y in range(18 * 8):
        for x in range(30 * 8):
            p[x, y] = pal[img[y][x]]
    out.resize((out.width * 3, out.height * 3), Image.NEAREST).save(path)


if __name__ == '__main__':
    img = draw()
    tiles, tmap = cut(img)
    print('%d tiles' % len(tiles))
    if len(sys.argv) > 1:
        preview(img, sys.argv[1])
    else:
        write(tiles, tmap)
