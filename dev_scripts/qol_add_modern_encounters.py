"""[QoL] Add Gen 4-9 Pokémon to the FireRed/LeafGreen wild encounter tables.

Rules:
  * Only the `_FireRed` / `_LeafGreen` tables in src/data/wild_encounters.json change.
  * A slot is replaced only if its species also appears in another slot of
    the same table, so every vanilla Pokémon stays catchable.
  * Rare slots are replaced first (land: 1%, 1%, 4%, 4%...; surf: 1%, 4%, 5%;
    fishing: the rarest Good Rod and Super Rod slots). The new Pokémon keeps
    that slot's level range.
  * Each area type has its own pool of thematic Pokémon; picks are deterministic
    per map, so FireRed and LeafGreen get the same additions.

Run once from the repo root:  python3 dev_scripts/qol_add_modern_encounters.py
"""
import hashlib
import json

PATH = "src/data/wild_encounters.json"

POOLS = {
    "forest": ["COMBEE", "KRICKETOT", "SEWADDLE", "VENIPEDE", "KARRABLAST", "SHELMET", "SCATTERBUG",
               "GRUBBIN", "CUTIEFLY", "BLIPBUG", "NYMBLE", "TAROUNTULA", "PIKIPEK", "SKWOVET", "BURMY"],
    "early": ["STARLY", "BIDOOF", "LILLIPUP", "PIDOVE", "FLETCHLING", "BUNNELBY", "YUNGOOS", "PIKIPEK",
              "SKWOVET", "ROOKIDEE", "WOOLOO", "LECHONK", "TANDEMAUS", "PATRAT"],
    "grass": ["SHINX", "BUNEARY", "BUDEW", "COTTONEE", "PETILIL", "DEERLING", "LITLEO", "FLABEBE",
              "SKIDDO", "ROCKRUFF", "MUDBRAY", "PAWMI", "FIDOUGH", "SMOLIV", "NICKIT", "YAMPER",
              "GLAMEOW", "CHATOT", "MINCCINO", "PURRLOIN", "PANCHAM", "ESPURR"],
    "late": ["STARAVIA", "LUXIO", "HERDIER", "TRANQUILL", "FLETCHINDER", "PAWMO", "DACHSBUN",
             "RIOLU", "HAWLUCHA", "SCRAGGY", "MIENFOO", "PAWNIARD", "SANDILE", "TRUBBISH"],
    "badlands": ["RIOLU", "AXEW", "SCRAGGY", "MIENFOO", "PAWNIARD", "HONEDGE", "HAWLUCHA",
                 "FALINKS", "TINKATINK", "CUFANT"],
    "cave": ["ROGGENROLA", "WOOBAT", "DRILBUR", "BRONZOR", "NACLI", "GLIMMET", "ROLYCOLY",
             "CARBINK", "NOIBAT", "TYRUNT", "AMAURA"],
    "ghost": ["DRIFLOON", "LITWICK", "YAMASK", "PHANTUMP", "PUMPKABOO", "SINISTEA", "GREAVARD",
              "GOLETT", "SANDYGAST", "IMPIDIMP"],
    "mansion": ["LITWICK", "SIZZLIPEDE", "SALANDIT", "TOXEL", "CROAGUNK", "SKORUPI", "STUNKY", "TRUBBISH"],
    "electric": ["TYNAMO", "JOLTIK", "PACHIRISU", "EMOLGA", "HELIOPTILE", "DEDENNE", "TOXEL", "PAWMI",
                 "TADBULB", "ROTOM"],
    "ice": ["SNOVER", "CUBCHOO", "VANILLITE", "BERGMITE", "SNOM", "CETODDLE", "CRYOGONAL", "EISCUE",
            "FRIGIBAX"],
    "volcano": ["LITLEO", "SALANDIT", "SIZZLIPEDE", "ROLYCOLY", "HEATMOR", "LARVESTA", "TURTONATOR",
                "CHARCADET"],
    "safari": ["HIPPOPOTAS", "SKORUPI", "CROAGUNK", "CARNIVINE", "RIOLU", "MUNCHLAX", "HAPPINY",
               "BONSLY", "MIME_JR", "DEERLING", "SKIDDO", "MUDBRAY", "PASSIMIAN", "ORANGURU",
               "BOUFFALANT", "GOOMY", "AXEW"],
    "dragon_den": ["GIBLE", "DEINO", "GOOMY", "JANGMO_O", "DREEPY", "FRIGIBAX", "AXEW", "RIOLU",
                   "ZORUA", "LARVESTA"],
    "sevii": ["ROCKRUFF", "MUDBRAY", "SALANDIT", "STUFFUL", "BOUNSWEET", "COMFEY", "PASSIMIAN",
              "ORANGURU", "MORELULL", "FOMANTIS", "DEWPIDER", "SANDYGAST", "PYUKUMUKU",
              "MAREANIE", "PINCURCHIN"],
    "surf": ["BUIZEL", "SHELLOS", "FINNEON", "DUCKLETT", "FRILLISH", "WIGLETT", "FINIZEN", "CHEWTLE",
             "ARROKUDA", "MAREANIE", "DEWPIDER", "WISHIWASHI", "SKRELP", "CLAUNCHER", "TYMPOLE",
             "ALOMOMOLA", "BRUXISH", "PYUKUMUKU", "VELUZA"],
    "fish": ["FINNEON", "BASCULIN", "ARROKUDA", "CHEWTLE", "TYMPOLE", "BRUXISH", "CLAUNCHER",
             "SKRELP", "WISHIWASHI", "VELUZA", "ALOMOMOLA"],
}

# First matching substring wins. Maps not listed keep their vanilla land table.
LAND_RULES = [
    ("TANOBY", None),  # Unown chambers stay as they are
    ("CERULEAN_CAVE", "dragon_den"),
    ("VIRIDIAN_FOREST", "forest"), ("BERRY_FOREST", "forest"), ("PATTERN_BUSH", "forest"),
    ("POKEMON_TOWER", "ghost"), ("LOST_CAVE", "ghost"),
    ("POKEMON_MANSION", "mansion"),
    ("POWER_PLANT", "electric"),
    ("SEAFOAM", "ice"), ("ICEFALL", "ice"),
    ("MT_EMBER", "volcano"), ("KINDLE_ROAD", "volcano"),
    ("SAFARI_ZONE", "safari"),
    ("MT_MOON", "cave"), ("ROCK_TUNNEL", "cave"), ("DIGLETTS_CAVE", "cave"), ("VICTORY_ROAD", "cave"),
    ("ALTERING_CAVE", "cave"), ("SEVAULT", "cave"),
    ("ISLAND", "sevii"),
    ("ROUTE23", "badlands"),
    ("ROUTE1", "early"), ("ROUTE2", "early"), ("ROUTE22", "early"), ("PALLET", "early"), ("VIRIDIAN", "early"),
    ("ROUTE12", "late"), ("ROUTE13", "late"), ("ROUTE14", "late"), ("ROUTE15", "late"),
    ("ROUTE16", "late"), ("ROUTE17", "late"), ("ROUTE18", "late"),
    ("ROUTE", "grass"),
]


def pool_for(map_name, rules):
    for key, pool in rules:
        if key in map_name:
            return pool
    return None


def pick(pool, map_name, k, taken):
    names = POOLS[pool]
    start = int(hashlib.sha1(map_name.encode()).hexdigest(), 16)
    for i in range(len(names)):
        cand = "SPECIES_" + names[(start + k * 7 + i) % len(names)]
        if cand not in taken:
            return cand
    return None


def replace_slots(mons, candidates, how_many, pool, map_name, tag):
    if pool is None or not mons:
        return 0
    taken = {m["species"] for m in mons}
    done = 0
    for idx in candidates:
        if done >= how_many or idx >= len(mons):
            continue
        sp = mons[idx]["species"]
        if sum(1 for m in mons if m["species"] == sp) < 2:
            continue  # would remove the last slot of a vanilla species
        new = pick(pool, map_name + tag, done, taken)
        if new is None:
            break
        mons[idx]["species"] = new
        taken.add(new)
        done += 1
    return done


def main():
    data = json.load(open(PATH))
    encounters = data["wild_encounter_groups"][0]["encounters"]
    total = 0
    for enc in encounters:
        label = enc["base_label"]
        if not (label.endswith("_FireRed") or label.endswith("_LeafGreen")):
            continue
        map_name = enc["map"]
        if "land_mons" in enc:
            total += replace_slots(enc["land_mons"]["mons"], [11, 10, 9, 8, 7, 6], 2,
                                   pool_for(map_name, LAND_RULES), map_name, "land")
        if "water_mons" in enc and "TANOBY" not in map_name:
            total += replace_slots(enc["water_mons"]["mons"], [4, 3, 2], 1, "surf", map_name, "surf")
        if "fishing_mons" in enc:
            mons = enc["fishing_mons"]["mons"]
            total += replace_slots(mons, [4, 3], 1, "fish", map_name, "good")
            total += replace_slots(mons, [9, 8, 7], 1, "fish", map_name, "super")
    with open(PATH, "w") as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
        f.write("\n")
    print("replaced", total, "slots")


if __name__ == "__main__":
    main()
