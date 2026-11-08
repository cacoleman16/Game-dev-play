#!/usr/bin/env python3
"""[QoL] Build the Seika region (docs/SEIKA.md): maps, layouts, scripts,
trainers, flags, wild Pokémon, heal locations. Idempotent.

  python3 dev_scripts/seika/build.py            write everything
  python3 dev_scripts/seika/build.py render DIR [module...]
                                                draw the new maps to DIR
"""
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import importlib  # noqa: E402

import common as c  # noqa: E402

# Order matters once released: flags and trainer IDs are numbered in this order.
MODULES = ['lanternport', 'inkbrush', 'kiln', 'silkhollow', 'duskpeak', 'league', 'flymap',
           'skyrift', 'skyrift_story']  # Skyrift (docs/SKYRIFT.md) comes after, so Seika's numbers stay put


LOADED = []


def main():
    only = sys.argv[3:] if len(sys.argv) > 3 and sys.argv[1] == 'render' else None
    for name in MODULES:
        if only and name not in only:
            continue
        if not os.path.exists(os.path.join(os.path.dirname(os.path.abspath(__file__)), name + '.py')):
            continue
        LOADED.append(importlib.import_module(name))
        LOADED[-1].build()
    c.resolve_links()
    # Doors lead to the middle of a 3-wide entrance mat, like Kanto's.
    by_id = {m.id: m for m in c.MAPS}
    for m in c.MAPS:
        for w in m.warps:
            d = by_id.get(w['dest_map'])
            if d and w['dest_warp_id'] == '0' and len(d.warps) >= 3:
                a, b, e = d.warps[:3]
                if a['y'] == b['y'] == e['y'] and b['x'] == a['x'] + 1 and e['x'] == a['x'] + 2:
                    w['dest_warp_id'] = '1'
    # The map-name popup holds 20 characters: section name, a space, then
    # the floor ("2F", "B1F" or "ROOFTOP"). Longer names overrun it and crash.
    names = {s['id']: s['name'] for s in c.MAPSECS}
    for m in c.MAPS:
        fl = getattr(m, 'floor', 0)
        if fl and m.mapsec in names:
            text = names[m.mapsec] + ' ' + ('ROOFTOP' if fl == 127 else ('B%dF' % -fl if fl < 0 else '%dF' % fl))
            assert len(text) <= 20, 'map name popup too long for %s: %r' % (m.id, text)
    if len(sys.argv) > 2 and sys.argv[1] == 'render':
        os.makedirs(sys.argv[2], exist_ok=True)
        for m in c.MAPS:
            if not isinstance(m.layout, str):
                m.layout.render(os.path.join(sys.argv[2], m.name + '.png'))
        return
    c.write_flags()
    c.write_trainers()
    c.g.write_maps(c.MAPS, c.GROUP, c.REGION, 'data/maps/seika_scripts.inc',
                   mapsecs=c.MAPSECS, heal_locations=c.HEALS, wild=c.WILD)
    for mod in LOADED:
        if hasattr(mod, 'write'):
            mod.write()
    # Japanese text for any Seika map that has a translation
    for m in c.MAPS:
        js = os.path.join(c.ROOT, 'text/ja/data/maps', m.name, 'scripts.json')
        if os.path.exists(js):
            subprocess.run([sys.executable, os.path.join(c.ROOT, 'dev_scripts/qol_japanese.py'), 'apply', js], check=True)
    print('%d maps, %d trainers, %d flags' % (len(c.MAPS), len(c.TRAINERS), len(c.FLAGS)))


if __name__ == '__main__':
    main()
