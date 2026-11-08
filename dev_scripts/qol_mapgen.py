#!/usr/bin/env python3
"""[QoL] Map generator: paint FireRed-style outdoor maps from shapes.

A Canvas holds a terrain class per cell (grass, tall grass, path, water, tree,
...) plus fixed blocks (stamps cut from existing maps: buildings, fences,
signs). resolve() turns it into map.bin blocks:
  - grass: the 2x2 tuft pattern Kanto uses, with plain grass mixed in
  - path / water: edges learned from Kanto's own maps (8-neighbour autotile)
  - trees: FireRed's 2x2 trees, stacked and merged like Route 1's borders
Blocks keep the collision/elevation bits the original maps use.
"""
import collections
import json
import os
import random
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import qol_maptools as mt  # noqa: E402

ROOT = mt.ROOT
CACHE = os.path.join(ROOT, 'dev_scripts/seika/autotile_frlg.json')
PRIMARY = 'gTileset_General_Frlg'
NPRI = 640

SAND = (238, 230, 139)
WATER = {(74, 123, 222), (74, 148, 222)}

# FireRed's general tileset (see dev_scripts/seika/README in docs/SEIKA.md)
GRASS_PATTERN = [[0x008, 0x009], [0x010, 0x011]]
GRASS_PLAIN = 0x001
FLOWERS = 0x004
TALL_GRASS = 0x00D
TREE = {  # (row, side, merged) -> metatile; row: top/body/base, side L/R
    ('top', 'L', False): 0x01E, ('top', 'L', True): 0x01C,
    ('top', 'R', False): 0x01F, ('top', 'R', True): 0x01D,
    ('body', 'L', False): 0x016, ('body', 'L', True): 0x014,
    ('body', 'R', False): 0x017, ('body', 'R', True): 0x015,
    ('base', 'L', False): 0x026, ('base', 'L', True): 0x024,
    ('base', 'R', False): 0x027, ('base', 'R', True): 0x025,
}
TREE_CAP = (0x00E, 0x00F)
LEDGE = (0x0B0, 0x087, 0x0B1)  # left end, middle, right end (jump south)
SIGN = 0x002
WOOD_SIGN = 0x003

NEIGH = [(-1, -1), (0, -1), (1, -1), (-1, 0), (1, 0), (-1, 1), (0, 1), (1, 1)]


def _pixels(pair, mid):
    im = pair.draw(mid)
    return [im.getpixel((x, y)) for y in range(16) for x in range(16)]


def classify(pair, mid):
    """'path', 'water' or None for a primary metatile, from its colours."""
    px = _pixels(pair, mid)
    sand = sum(1 for p in px if p == SAND)
    water = sum(1 for p in px if p in WATER)
    if water >= 24:
        return 'water'
    if sand >= 24:
        return 'path'
    return None


def learn(force=False):
    """Learn autotiles + usual collision bits from FRLG outdoor maps.

    Tables (8-neighbour key -> block):
      path_grass   sand path in grass (Route 1 style)
      beach_grass  beach sand meeting grass (Vermilion style)
      sea_sand     sea water meeting sand (rims are in the water cells)
      water_grass  water meeting grass
    """
    if os.path.exists(CACHE) and not force:
        return json.load(open(CACHE))
    L = mt.layouts()
    examples = [l for l in L.values() if l.get('layout_version') == 'frlg'
                and l['primary_tileset'] == PRIMARY and l['width'] > 2]
    pair = mt.TilesetPair(PRIMARY, 'gTileset_PalletTown')
    cls = {mid: classify(pair, mid) for mid in range(NPRI)}
    grassish = {0x001, 0x008, 0x009, 0x010, 0x011, 0x004, TALL_GRASS}
    tables = collections.defaultdict(lambda: collections.defaultdict(collections.Counter))
    bits = collections.defaultdict(collections.Counter)
    for l in examples:
        g = mt.read_blocks(l['blockdata_filepath'], l['width'], l['height'])
        H, W = len(g), len(g[0])

        def kind(x, y):
            if not (0 <= x < W and 0 <= y < H):
                return None
            mid = g[y][x] & 0x3FF
            if mid >= NPRI:
                return 'other'
            if cls[mid] == 'water':
                return 'water'
            if cls[mid] == 'path':
                return 'sand'
            return 'grass' if mid in grassish else 'other'
        # sand families by connected region
        fam = {}
        for y in range(H):
            for x in range(W):
                if kind(x, y) == 'sand' and (x, y) not in fam:
                    comp, stack = [], [(x, y)]
                    fam[(x, y)] = None
                    while stack:
                        cx, cy = stack.pop()
                        comp.append((cx, cy))
                        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                            n = (cx + dx, cy + dy)
                            if kind(*n) == 'sand' and n not in fam:
                                fam[n] = None
                                stack.append(n)
                    ids = collections.Counter(g[cy][cx] & 0x3FF for cx, cy in comp)
                    f = 'beach' if ids[0x115] > ids[0x0DC] else 'path'
                    for c in comp:
                        fam[c] = f
        for y in range(H):
            for x in range(W):
                b = g[y][x]
                mid = b & 0x3FF
                bits[mid][b] += 1
                k = kind(x, y)
                if k not in ('water', 'sand'):
                    continue
                key, outside = 0, set()
                for i, (dx, dy) in enumerate(NEIGH):
                    nk = kind(x + dx, y + dy)
                    if nk is None:
                        nk = k
                    if nk == k:
                        key |= 1 << i
                    else:
                        outside.add(nk)
                if k == 'water':
                    if outside <= {'sand'}:
                        tables['sea_sand'][key][b] += 1
                    if outside <= {'grass'}:
                        tables['water_grass'][key][b] += 1
                else:
                    f = fam[(x, y)]
                    if outside <= {'grass'}:
                        tables[f + '_grass'][key][b] += 1
    out = {'bits': {}, 'ground': sorted(m for m in range(NPRI) if cls[m] or m in grassish)}
    for t, keys in tables.items():
        out[t] = {str(key): cnt.most_common(1)[0][0] for key, cnt in keys.items()}
    for mid, cnt in bits.items():
        out['bits'][str(mid)] = cnt.most_common(1)[0][0]
    os.makedirs(os.path.dirname(CACHE), exist_ok=True)
    json.dump(out, open(CACHE, 'w'), indent=0, sort_keys=True)
    return out


def _nearest(table, key):
    """Closest learned neighbourhood; orthogonal neighbours weigh more."""
    if str(key) in table:
        return table[str(key)]
    w = [1, 4, 1, 4, 4, 1, 4, 1]
    best, bd = None, 1e9
    for k, v in table.items():
        k = int(k)
        d = sum(w[i] for i in range(8) if ((k >> i) & 1) != ((key >> i) & 1))
        if d < bd:
            best, bd = v, d
    return best


class Canvas:
    def __init__(self, w, h, secondary, base='grass', seed=1):
        self.w, self.h = w, h
        self.secondary = secondary
        self.cls = [[base] * w for _ in range(h)]
        self.fixed = {}       # (x, y) -> u16 block, wins over terrain
        self.trees = set()    # top-left cells of 2x2 trees
        self.tree_merge = {}  # tree cell -> merge with neighbours?
        self.warps = []       # imported from stamps: (x, y, info)
        self.rng = random.Random(seed)
        self.auto = learn()
        self._ground = set(self.auto.get('ground', []))

    # ---- painting
    def rect(self, x, y, w, h, c):
        for yy in range(max(0, y), min(self.h, y + h)):
            for xx in range(max(0, x), min(self.w, x + w)):
                self.cls[yy][xx] = c
                self.fixed.pop((xx, yy), None)
                self._untree(xx, yy)

    def cells(self, pts, c):
        for (x, y) in pts:
            if 0 <= x < self.w and 0 <= y < self.h:
                self.cls[y][x] = c
                self.fixed.pop((x, y), None)

    def _untree(self, x, y):
        for t in list(self.trees):
            if t[0] <= x <= t[0] + 1 and t[1] <= y <= t[1] + 1:
                self.trees.discard(t)

    def forest(self, x, y, w, h, merge=True):
        """Fill with trees on a 2x2 grid starting at (x, y). w and h in tiles."""
        for ty in range(y, y + h - 1, 2):
            for tx in range(x, x + w - 1, 2):
                self.tree(tx, ty, merge)

    def tree(self, x, y, merge=False):
        for yy in (y, y + 1):
            for xx in (x, x + 1):
                if 0 <= xx < self.w and 0 <= yy < self.h:
                    self.cls[yy][xx] = 'tree'
                    self.fixed.pop((xx, yy), None)
        self.trees.add((x, y))
        self.tree_merge[(x, y)] = merge

    def pond(self, x, y, w, h):
        """Rectangular pond with FireRed's rim (top/left/right in the water,
        the bottom lip in the row below)."""
        for yy in range(h):
            for xx in range(w):
                if yy == 0:
                    mid = 0x1A5 if xx == 0 else 0x1A7 if xx == w - 1 else 0x1A6
                else:
                    mid = 0x1AD if xx == 0 else 0x1AF if xx == w - 1 else 0x1AE
                self.put(x + xx, y + yy, mid)
                self.cls[y + yy][x + xx] = 'fixed'
        for xx in range(w):
            self.put(x + xx, y + h, 0x133)
            self.cls[y + h][x + xx] = 'fixed'

    def put(self, x, y, mid, bits=None):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.fixed[(x, y)] = self.block(mid) if bits is None else (mid | bits)

    def block(self, mid):
        """Metatile with the collision/elevation it usually has."""
        return int(self.auto['bits'].get(str(mid), mid | (3 << 12)))

    def ledge(self, x, y, n):
        self.put(x, y, LEDGE[0])
        for i in range(1, n - 1):
            self.put(x + i, y, LEDGE[1])
        self.put(x + n - 1, y, LEDGE[2])

    def stamp(self, lid, sx, sy, w, h, x, y, skip=(), ground=False):
        """Copy a rectangle of an existing layout. Ground cells (grass, sand,
        water) are left out unless ground=True, so only the building lands."""
        L = mt.layouts()[lid]
        if L['secondary_tileset'] != self.secondary:
            # primary-only stamps are fine from any FRLG map
            pass
        g = mt.read_blocks(L['blockdata_filepath'], L['width'], L['height'])
        for yy in range(h):
            for xx in range(w):
                b = g[sy + yy][sx + xx]
                if (b & 0x3FF) >= NPRI and L['secondary_tileset'] != self.secondary:
                    raise ValueError('stamp %s uses %s tiles' % (lid, L['secondary_tileset']))
                if (xx, yy) in skip:
                    continue
                if not ground and (b & 0x3FF) in self._ground:
                    continue
                tx, ty = x + xx, y + yy
                if 0 <= tx < self.w and 0 <= ty < self.h:
                    self.fixed[(tx, ty)] = b
                    self.cls[ty][tx] = 'fixed'
                    self._untree(tx, ty)

    # ---- resolving
    def _is(self, x, y, c):
        if not (0 <= x < self.w and 0 <= y < self.h):
            return self.cls[min(max(y, 0), self.h - 1)][min(max(x, 0), self.w - 1)] == c
        return self.cls[y][x] == c

    def resolve(self):
        out = [[0] * self.w for _ in range(self.h)]
        for y in range(self.h):
            for x in range(self.w):
                c = self.cls[y][x]
                if c in ('grass', 'flowers', 'plain', 'tree', 'fixed'):
                    mid = GRASS_PATTERN[y & 1][x & 1]
                    if self.rng.random() < 0.08:
                        mid = GRASS_PLAIN
                    if c == 'flowers':
                        mid = FLOWERS
                    if c == 'plain':
                        mid = GRASS_PLAIN
                    out[y][x] = self.block(mid)
                elif c == 'tall':
                    out[y][x] = self.block(TALL_GRASS)
                elif c in ('path', 'beach'):
                    # sea next to a beach counts as beach: the rim is drawn in the water
                    same = (c, 'sea', 'path') if c == 'beach' else (c, 'beach')
                    key = 0
                    for i, (dx, dy) in enumerate(NEIGH):
                        if any(self._is(x + dx, y + dy, s) for s in same):
                            key |= 1 << i
                    out[y][x] = _nearest(self.auto[c + '_grass'], key)
                elif c == 'sea':
                    key, sandy, grassy = 0, 0, 0
                    for i, (dx, dy) in enumerate(NEIGH):
                        if self._is(x + dx, y + dy, 'sea'):
                            key |= 1 << i
                        elif any(self._is(x + dx, y + dy, s) for s in ('beach', 'path')):
                            sandy += 1
                        else:
                            grassy += 1
                    # Kanto draws the coast rim inside the water cells, the same
                    # way next to grass and next to beach sand
                    out[y][x] = _nearest(self.auto['water_grass'], key)
        # trees
        for (x, y) in sorted(self.trees):
            merge = self.tree_merge.get((x, y), False)
            left = merge and ((x - 2, y) in self.trees or x - 2 < 0 and x == 0) or (merge and x <= 0)
            right = merge and ((x + 2, y) in self.trees or x + 2 >= self.w)
            below = (x, y + 2) in self.trees or y + 2 >= self.h
            above = (x, y - 2) in self.trees or y - 1 < 0
            rows = [('top', y), ('body' if below else 'base', y + 1)]
            for row, yy in rows:
                for side, xx, mg in (('L', x, left), ('R', x + 1, right)):
                    if 0 <= xx < self.w and 0 <= yy < self.h and (xx, yy) not in self.fixed:
                        out[yy][xx] = self.block(TREE[(row, side, bool(mg))])
            if not above and y - 1 >= 0:
                for i, xx in enumerate((x, x + 1)):
                    if (xx, y - 1) not in self.fixed and self.cls[y - 1][xx] != 'tree' and 0 <= xx < self.w:
                        out[y - 1][xx] = self.block(TREE_CAP[i])
        for (x, y), b in self.fixed.items():
            out[y][x] = b
        return out

    def render(self, path, scale=1, marks=None):
        pair = mt.TilesetPair(PRIMARY, self.secondary)
        mt.render_grid(pair, self.resolve(), scale, marks).save(path)


if __name__ == '__main__':
    if sys.argv[1:] == ['learn']:
        t = learn(force=True)
        print({k: len(v) for k, v in t.items()})


# ---------------------------------------------------------------- map files

def obj(gfx, x, y, script, local_id=None, move='MOVEMENT_TYPE_FACE_DOWN', rx=1, ry=1,
        trainer='TRAINER_TYPE_NONE', sight='0', flag='0', elevation=3):
    o = {'type': 'object', 'graphics_id': gfx, 'x': x, 'y': y, 'elevation': elevation,
         'movement_type': move, 'movement_range_x': rx, 'movement_range_y': ry,
         'trainer_type': trainer, 'trainer_sight_or_berry_tree_id': str(sight),
         'script': script, 'flag': flag}
    if local_id:
        o = {'local_id': local_id, **o}
    return o


def warp(x, y, dest_map, dest_warp, elevation=3):
    return {'x': x, 'y': y, 'elevation': elevation, 'dest_map': dest_map, 'dest_warp_id': str(dest_warp)}


def sign(x, y, script, facing='BG_EVENT_PLAYER_FACING_NORTH', elevation=0):
    return {'type': 'sign', 'x': x, 'y': y, 'elevation': elevation,
            'player_facing_dir': facing, 'script': script}


def hidden_item(x, y, item, flag, elevation=3):
    return {'type': 'hidden_item', 'x': x, 'y': y, 'elevation': elevation, 'item': item,
            'flag': flag, 'quantity': 1, 'underfoot': False}


def coord(x, y, script, var='VAR_TEMP_1', value='0', elevation=3):
    return {'type': 'trigger', 'x': x, 'y': y, 'elevation': elevation, 'var': var,
            'var_value': str(value), 'script': script}


class MapDef:
    """One map: either a new outdoor Canvas or an existing layout id."""

    def __init__(self, name, map_id, mapsec, layout, music, map_type='MAP_TYPE_TOWN',
                 weather='WEATHER_SUNNY', show_name=True, cycling=True, running=True,
                 escaping=False, battle_scene='MAP_BATTLE_SCENE_NORMAL', border=None, floor=0):
        self.name, self.id, self.mapsec = name, map_id, mapsec
        self.layout = layout
        self.music, self.map_type, self.weather = music, map_type, weather
        self.show_name, self.cycling, self.running, self.escaping = show_name, cycling, running, escaping
        self.battle_scene, self.border, self.floor = battle_scene, border, floor
        self.objects, self.warps, self.coords, self.bgs = [], [], [], []
        self.connections = []
        self.map_scripts = []   # (MAP_SCRIPT_*, label)
        self.scripts = ''       # assembly after the MapScripts table

    @property
    def layout_id(self):
        return self.layout if isinstance(self.layout, str) else 'LAYOUT_' + self.id[4:]

    def connect(self, other, direction, offset):
        self.connections.append({'map': other.id, 'offset': offset, 'direction': direction})
        back = {'up': 'down', 'down': 'up', 'left': 'right', 'right': 'left'}[direction]
        other.connections.append({'map': self.id, 'offset': -offset, 'direction': back})


TREE_BORDER = [0x01C, 0x01D, 0x014, 0x015]
SEA_BORDER = [0x12B, 0x12B, 0x12B, 0x12B]


def _update_json(path, fn):
    full = os.path.join(ROOT, path)
    d = json.load(open(full, encoding='utf-8'))
    fn(d)
    with open(full, 'w', encoding='utf-8') as f:
        json.dump(d, f, indent=2, ensure_ascii=False)
        f.write('\n')


def write_maps(maps, group, region, scripts_inc, mapsecs=(), heal_locations=(), wild=()):
    """Write every map of a region and register it. Idempotent."""
    layouts_new = []
    for m in maps:
        if not isinstance(m.layout, str):
            c = m.layout
            d = 'data/layouts/%s' % m.name
            mt.write_blocks(d + '/map.bin', c.resolve())
            border = m.border or (TREE_BORDER if isinstance(c, Canvas) else [0] * 4)
            blk = [c.block(b) for b in border] if isinstance(c, Canvas) else border
            mt.write_blocks(d + '/border.bin', [blk[:2], blk[2:]])
            layouts_new.append({
                'id': m.layout_id, 'name': m.name + '_Layout', 'width': c.w, 'height': c.h,
                'border_width': 2, 'border_height': 2, 'primary_tileset': getattr(c, 'primary', PRIMARY),
                'secondary_tileset': c.secondary, 'border_filepath': d + '/border.bin',
                'blockdata_filepath': d + '/map.bin', 'layout_version': 'frlg'})
        j = {
            'id': m.id, 'name': m.name, 'layout': m.layout_id, 'music': m.music, 'region': region,
            'region_map_section': m.mapsec, 'requires_flash': False, 'weather': m.weather,
            'map_type': m.map_type, 'allow_cycling': m.cycling, 'allow_escaping': m.escaping,
            'allow_running': m.running, 'show_map_name': m.show_name, 'floor_number': m.floor,
            'battle_scene': m.battle_scene, 'connections': m.connections or None,
            'object_events': m.objects, 'warp_events': m.warps, 'coord_events': m.coords,
            'bg_events': m.bgs}
        os.makedirs(os.path.join(ROOT, 'data/maps', m.name), exist_ok=True)
        with open(os.path.join(ROOT, 'data/maps', m.name, 'map.json'), 'w', encoding='utf-8') as f:
            json.dump(j, f, indent=2, ensure_ascii=False)
            f.write('\n')
        s = '%s_MapScripts::\n' % m.name
        for kind, label in m.map_scripts:
            s += '\tmap_script %s, %s\n' % (kind, label)
        s += '\t.byte 0\n\n' + m.scripts.strip('\n') + '\n'
        with open(os.path.join(ROOT, 'data/maps', m.name, 'scripts.inc'), 'w', encoding='utf-8') as f:
            f.write(s)

    def upd_layouts(d):
        ids = {l['id'] for l in layouts_new}
        d['layouts'] = [l for l in d['layouts'] if l.get('id') not in ids] + layouts_new
    _update_json('data/layouts/layouts.json', upd_layouts)

    def upd_groups(d):
        if group not in d['group_order']:
            d['group_order'].append(group)
        d[group] = [m.name for m in maps]
    _update_json('data/maps/map_groups.json', upd_groups)

    with open(os.path.join(ROOT, scripts_inc), 'w', encoding='utf-8') as f:
        f.write('@ [QoL] Generated: the Seika region\'s map scripts.\n')
        for m in maps:
            f.write('\t.include "data/maps/%s/scripts.inc"\n' % m.name)

    def upd_secs(d):
        have = {s['id']: s for s in d['map_sections']}
        for sec in mapsecs:
            if sec['id'] in have:
                have[sec['id']].update(sec)
            else:
                d['map_sections'].append(sec)
    _update_json('src/data/region_map/region_map_sections.json', upd_secs)

    def upd_heal(d):
        ids = {h['id'] for h in heal_locations}
        d['heal_locations'] = [h for h in d['heal_locations'] if h['id'] not in ids] + list(heal_locations)
    _update_json('src/data/heal_locations.json', upd_heal)

    def upd_wild(d):
        enc = d['wild_encounter_groups'][0]['encounters']
        labels = {w['base_label'] for w in wild}
        enc[:] = [e for e in enc if e['base_label'] not in labels] + list(wild)
    _update_json('src/data/wild_encounters.json', upd_wild)


class Room:
    """Indoor map built from explicit blocks (copied from an existing room)."""

    def __init__(self, w, h, primary, secondary, source=None):
        self.w, self.h = w, h
        self.primary, self.secondary = primary, secondary
        self.grid = [[0] * w for _ in range(h)]
        self.src = None
        if source:
            L = mt.layouts()[source]
            assert L['secondary_tileset'] == secondary and L['primary_tileset'] == primary, source
            self.src = mt.read_blocks(L['blockdata_filepath'], L['width'], L['height'])

    def at(self, sx, sy):
        """Block of the source room at (sx, sy)."""
        return self.src[sy][sx]

    def set(self, x, y, b):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.grid[y][x] = b

    def fill(self, x, y, w, h, b):
        for yy in range(y, y + h):
            for xx in range(x, x + w):
                self.set(xx, yy, b)

    def copy(self, sx, sy, w, h, x, y):
        for yy in range(h):
            for xx in range(w):
                self.set(x + xx, y + yy, self.src[sy + yy][sx + xx])

    def stamp(self, lid, sx, sy, w, h, x, y):
        L = mt.layouts()[lid]
        g = mt.read_blocks(L['blockdata_filepath'], L['width'], L['height'])
        for yy in range(h):
            for xx in range(w):
                self.set(x + xx, y + yy, g[sy + yy][sx + xx])

    def block(self, b):
        return b

    def resolve(self):
        return [row[:] for row in self.grid]

    def render(self, path, scale=1, marks=None):
        pair = mt.TilesetPair(self.primary, self.secondary)
        mt.render_grid(pair, self.resolve(), scale, marks).save(path)


def pier(c, x, y, w, h):
    """Wooden pier (Vermilion's boardwalk planks) with its shadow row below."""
    for yy in range(h):
        for xx in range(w):
            mid = 0x1C5 if xx == 0 else 0x1C7 if xx == w - 1 else 0x1C6
            c.put(x + xx, y + yy, mid, 3 << 12)
            c.cls[y + yy][x + xx] = 'fixed'
    for xx in range(w):
        c.put(x + xx, y + h, 0x1CF if xx == w - 1 else 0x1CE, 1 << 12)
        c.cls[y + h][x + xx] = 'fixed'
