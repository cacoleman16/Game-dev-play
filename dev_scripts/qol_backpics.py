#!/usr/bin/env python3
"""[QoL] Skyrift companions' battle back sprites: recoloured copies of
existing back sprites (docs/SKYRIFT.md). Idempotent.

  python3 dev_scripts/qol_backpics.py
"""
import os
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
D = os.path.join(ROOT, 'graphics/trainers/back_pics')

# new sprite: (source, {palette index: new colour})
RECOLOURS = {
    'qol_altair': ('pokedude', {   # khaki work shirt, brown hair
        5: (206, 190, 140), 6: (158, 138, 94), 7: (108, 94, 60), 8: (184, 164, 116),
        11: (74, 50, 34), 12: (118, 84, 54)}),
    'qol_deneb': ('leaf', {        # black hat and coat with violet, dark hair, gold band
        1: (70, 52, 92), 4: (40, 30, 56), 8: (26, 20, 36),
        9: (112, 72, 142), 10: (76, 46, 102),
        5: (62, 42, 82), 6: (32, 22, 46), 7: (124, 92, 164),
        11: (232, 192, 82), 12: (170, 130, 40)}),
    'qol_mira': ('may', {          # starry blue bandana, white top, silver hair
        10: (124, 172, 255), 11: (72, 112, 212),
        12: (238, 234, 255), 13: (182, 172, 232),
        7: (202, 202, 218), 8: (140, 140, 162)}),
}


def main():
    for name, (src, changes) in RECOLOURS.items():
        im = Image.open(os.path.join(D, src + '.png'))
        pal = im.getpalette()[:48]
        for i, rgb in changes.items():
            pal[i * 3:i * 3 + 3] = list(rgb)
        out = im.copy()
        out.putpalette(pal + [0] * (768 - len(pal)))
        out.save(os.path.join(D, name + '.png'))
        print('wrote', name)


if __name__ == '__main__':
    main()
