#!/usr/bin/env python3
"""[QoL] Build the Seika region (docs/SEIKA.md): maps, layouts, scripts,
trainers, flags, wild Pokémon, heal locations. Idempotent.

  python3 dev_scripts/seika/build.py            write everything
  python3 dev_scripts/seika/build.py render DIR draw every new map to DIR
"""
import os
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common as c  # noqa: E402
import lanternport  # noqa: E402

TOWNS = [lanternport]


def main():
    for t in TOWNS:
        t.build()
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
    # Japanese text for any Seika map that has a translation
    for m in c.MAPS:
        js = os.path.join(c.ROOT, 'text/ja/data/maps', m.name, 'scripts.json')
        if os.path.exists(js):
            subprocess.run([sys.executable, os.path.join(c.ROOT, 'dev_scripts/qol_japanese.py'), 'apply', js], check=True)
    print('%d maps, %d trainers, %d flags' % (len(c.MAPS), len(c.TRAINERS), len(c.FLAGS)))


if __name__ == '__main__':
    main()
