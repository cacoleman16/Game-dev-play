"""Draw the FAIRY type icon and TM disc palette for the pokefirered hack.

- graphics/interface/menu_info.png: a 32x12 "FAIRY" badge is composed in the
  free slot at tile offset 0x04 (x=32, y=0) by reusing the NORMAL badge frame
  and letter glyphs cut from the FIRE, DARK and FLYING badges.
- graphics/interface/pokemon_types.pal: entry 10 (unused by every badge) becomes
  the Fairy pink used as the badge fill.
- graphics/tm_case/disc_types_2.pal: grows from 16 to 32 colours; the second
  16 are a pink recolour of the Psychic disc palette for Fairy TMs.

Usage: python3 -I make_fairy_gfx.py <pokefirered root>
"""
import sys
from PIL import Image

root = sys.argv[1]

BG = 0             # transparent / outside the badge
FILL_NORMAL = 25   # NORMAL badge fill colour index (16 + pal entry 9)
FILL_FAIRY = 26    # 16 + pal entry 10, recoloured to pink below
DARK, LIGHT = 30, 31
FAIRY_PINK = (238, 153, 172)


def tile_xy(offset):
    return (offset % 16) * 8, (offset // 16) * 8


def glyph(px, offset, x0, width):
    """Return the letter columns [x0, x0+width) from rows 2..9 of a badge."""
    ox, oy = tile_xy(offset)
    return [[px[ox + x0 + dx, oy + y] for dx in range(width)] for y in range(2, 10)]


def main():
    png_path = f"{root}/graphics/interface/menu_info.png"
    im = Image.open(png_path)
    px = im.load()

    letters = [
        glyph(px, 0x24, 7, 5),   # F from FIRE
        glyph(px, 0x8C, 11, 5),  # A from DARK
        glyph(px, 0x24, 12, 4),  # I from FIRE
        glyph(px, 0x24, 16, 5),  # R from FIRE
        glyph(px, 0x60, 12, 4),  # Y from FLYING
    ]
    text_w = sum(len(g[0]) for g in letters)

    dst_x, dst_y = tile_xy(0x04)
    for x in range(32):
        for y in range(12):
            if px[dst_x + x, dst_y + y] != BG:
                raise SystemExit("Fairy icon slot 0x04 is not empty")

    # Copy the NORMAL frame, recoloured and with its text cleared.
    src_x, src_y = tile_xy(0x20)
    for x in range(32):
        for y in range(12):
            c = px[src_x + x, src_y + y]
            if c == BG:
                out = BG
            elif 1 <= x <= 30 and 2 <= y <= 9:
                out = FILL_FAIRY
            else:
                out = FILL_FAIRY if c == FILL_NORMAL else c
            px[dst_x + x, dst_y + y] = out

    # Stamp the letters, centred.
    cur = (32 - text_w) // 2
    for g in letters:
        for y, row in enumerate(g):
            for dx, c in enumerate(row):
                if c in (DARK, LIGHT):
                    px[dst_x + cur + dx, dst_y + 2 + y] = c
        cur += len(g[0])
    im.save(png_path)

    # Recolour the unused type palette entry to Fairy pink.
    pal_path = f"{root}/graphics/interface/pokemon_types.pal"
    lines = open(pal_path).read().splitlines()
    assert lines[0] == "JASC-PAL" and lines[2] == "16"
    lines[3 + 10] = " ".join(map(str, FAIRY_PINK))
    open(pal_path, "w").write("\n".join(lines) + "\n")

    # TM case: add a 16-colour Fairy disc palette after Dragon.
    disc1 = open(f"{root}/graphics/tm_case/disc_types_1.pal").read().splitlines()
    psychic = [tuple(map(int, l.split())) for l in disc1[3 + 13 * 16: 3 + 14 * 16]]
    # Keep transparency/outline/shading slots from the Psychic disc, replace
    # the body gradient (3..9) and highlight (14) with a bubblegum pink.
    body = [(255, 214, 236), (255, 198, 226), (252, 182, 216), (248, 166, 206),
            (244, 150, 196), (238, 134, 186), (232, 118, 176)]
    fairy = list(psychic)
    fairy[3:10] = body
    fairy[14] = (255, 248, 252)
    disc2_path = f"{root}/graphics/tm_case/disc_types_2.pal"
    disc2 = open(disc2_path).read().splitlines()
    assert disc2[2] == "16"
    disc2[2] = "32"
    disc2 = disc2[:3 + 16] + [" ".join(map(str, c)) for c in fairy]
    open(disc2_path, "w").write("\n".join(disc2) + "\n")
    print("fairy gfx written; text width", text_w)


if __name__ == "__main__":
    main()
