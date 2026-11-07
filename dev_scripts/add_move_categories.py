"""Insert a `.category = DAMAGE_CATEGORY_*` line into every gBattleMoves entry.

Classification follows the Gen 4+ physical/special split:
  - power == 0            -> STATUS
  - name in SPECIAL_MOVES -> SPECIAL
  - everything else       -> PHYSICAL
Usage: python3 -I add_move_categories.py path/to/src/data/battle_moves.h
"""
import re
import sys

SPECIAL_MOVES = {
    "RAZOR_WIND", "GUST", "SONIC_BOOM", "ACID", "EMBER", "FLAMETHROWER",
    "WATER_GUN", "HYDRO_PUMP", "SURF", "ICE_BEAM", "BLIZZARD", "PSYBEAM",
    "BUBBLE_BEAM", "AURORA_BEAM", "HYPER_BEAM", "ABSORB", "MEGA_DRAIN",
    "SOLAR_BEAM", "PETAL_DANCE", "DRAGON_RAGE", "FIRE_SPIN", "THUNDER_SHOCK",
    "THUNDERBOLT", "THUNDER", "CONFUSION", "PSYCHIC", "NIGHT_SHADE", "SMOG",
    "SLUDGE", "FIRE_BLAST", "SWIFT", "DREAM_EATER", "BUBBLE", "PSYWAVE",
    "TRI_ATTACK", "AEROBLAST", "SNORE", "POWDER_SNOW", "SLUDGE_BOMB",
    "MUD_SLAP", "OCTAZOOKA", "ZAP_CANNON", "ICY_WIND", "GIGA_DRAIN",
    "DRAGON_BREATH", "HIDDEN_POWER", "TWISTER", "MIRROR_COAT",
    "ANCIENT_POWER", "SHADOW_BALL", "FUTURE_SIGHT", "WHIRLPOOL", "UPROAR",
    "SPIT_UP", "HEAT_WAVE", "ERUPTION", "LUSTER_PURGE", "MIST_BALL",
    "HYPER_VOICE", "BLAST_BURN", "HYDRO_CANNON", "WEATHER_BALL", "AIR_CUTTER",
    "OVERHEAT", "SILVER_WIND", "WATER_SPOUT", "SIGNAL_BEAM", "EXTRASENSORY",
    "SHEER_COLD", "MUDDY_WATER", "FRENZY_PLANT", "MUD_SHOT", "MAGICAL_LEAF",
    "SHOCK_WAVE", "WATER_PULSE", "DOOM_DESIRE", "PSYCHO_BOOST",
}

ENTRY_RE = re.compile(r"(    \[MOVE_(\w+)\] =\n    \{\n)(.*?)(\n    \},?)", re.S)


def main(path):
    src = open(path).read()
    seen = set()
    counts = {"PHYSICAL": 0, "SPECIAL": 0, "STATUS": 0}

    def repl(m):
        head, name, body, tail = m.groups()
        if ".category" in body:
            raise SystemExit(f"MOVE_{name} already has a category")
        seen.add(name)
        power = int(re.search(r"\.power = (\d+)", body).group(1))
        if power == 0:
            cat = "STATUS"
        elif name in SPECIAL_MOVES:
            cat = "SPECIAL"
        else:
            cat = "PHYSICAL"
        counts[cat] += 1
        body = re.sub(r"(        \.type = \w+,\n)",
                      r"\1        .category = DAMAGE_CATEGORY_" + cat + ",\n",
                      body, count=1)
        return head + body + tail

    out = ENTRY_RE.sub(repl, src)
    missing = SPECIAL_MOVES - seen
    if missing:
        raise SystemExit(f"Unknown special moves: {sorted(missing)}")
    open(path, "w").write(out)
    print(f"tagged {len(seen)} moves: {counts}")


if __name__ == "__main__":
    main(sys.argv[1])
