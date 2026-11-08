#!/usr/bin/env python3
"""[QoL] Starfall: make every starter catchable (docs/STARFALL.md).

In the Starfall story, Beldum is your partner, Blue takes Charmander and
Bulbasaur and Squirtle run off into the wild. This script puts each starter
into a fitting rare encounter slot (it replaces whatever was in that slot):

  Bulbasaur  Viridian Forest          Treecko   Petalburg Woods
  Charmander Route 4                  Torchic   Route 117
  Squirtle   Vermilion City, fishing  Mudkip    Route 102, fishing
  Gen 2 and Gen 4 starters: Kanto's Safari Zone (Center, East, West)
  Gen 5 to 9 starters: Hoenn's Safari Zone

Levels are the slot's own. Run from the repo root (idempotent):
  python3 dev_scripts/qol_add_starter_encounters.py
"""
import json
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PATH = os.path.join(ROOT, 'src/data/wild_encounters.json')

# (base label prefix, field, slot index, species)
# Land slots: 6/7 are 5%, 8/9 are 4%. Water slot 3 is 4%.
# Fishing: Old Rod = slots 0-1, Good Rod = 2-4, Super Rod = 5-9.
PLACEMENTS = [
    ('sViridianForest_', 'land_mons', 7, 'SPECIES_BULBASAUR'),
    ('sRoute4_', 'land_mons', 7, 'SPECIES_CHARMANDER'),
    ('sVermilionCity_', 'fishing_mons', 1, 'SPECIES_SQUIRTLE'),
    ('sVermilionCity_', 'fishing_mons', 4, 'SPECIES_SQUIRTLE'),
    ('gPetalburgWoods', 'land_mons', 7, 'SPECIES_TREECKO'),
    ('gRoute117', 'land_mons', 7, 'SPECIES_TORCHIC'),
    ('gRoute102', 'fishing_mons', 1, 'SPECIES_MUDKIP'),
    ('gRoute102', 'fishing_mons', 4, 'SPECIES_MUDKIP'),
    # Kanto Safari Zone: Johto and Sinnoh
    ('sSafariZoneCenter_', 'land_mons', 8, 'SPECIES_CHIKORITA'),
    ('sSafariZoneCenter_', 'land_mons', 9, 'SPECIES_TURTWIG'),
    ('sSafariZoneEast_', 'land_mons', 8, 'SPECIES_CYNDAQUIL'),
    ('sSafariZoneEast_', 'land_mons', 9, 'SPECIES_CHIMCHAR'),
    ('sSafariZoneWest_', 'land_mons', 8, 'SPECIES_TOTODILE'),
    ('sSafariZoneWest_', 'land_mons', 9, 'SPECIES_PIPLUP'),
    # Hoenn Safari Zone: Unova to Paldea
    ('gSafariZone_Southwest', 'land_mons', 8, 'SPECIES_SNIVY'),
    ('gSafariZone_Southwest', 'land_mons', 9, 'SPECIES_TEPIG'),
    ('gSafariZone_Southwest', 'water_mons', 3, 'SPECIES_OSHAWOTT'),
    ('gSafariZone_South', 'land_mons', 7, 'SPECIES_FROAKIE'),
    ('gSafariZone_South', 'land_mons', 8, 'SPECIES_CHESPIN'),
    ('gSafariZone_South', 'land_mons', 9, 'SPECIES_FENNEKIN'),
    ('gSafariZone_Northwest', 'land_mons', 8, 'SPECIES_ROWLET'),
    ('gSafariZone_Northwest', 'land_mons', 9, 'SPECIES_LITTEN'),
    ('gSafariZone_Northwest', 'water_mons', 3, 'SPECIES_POPPLIO'),
    ('gSafariZone_North', 'land_mons', 7, 'SPECIES_SOBBLE'),
    ('gSafariZone_North', 'land_mons', 8, 'SPECIES_GROOKEY'),
    ('gSafariZone_North', 'land_mons', 9, 'SPECIES_SCORBUNNY'),
    ('gSafariZone_Northeast', 'land_mons', 7, 'SPECIES_QUAXLY'),
    ('gSafariZone_Northeast', 'land_mons', 8, 'SPECIES_SPRIGATITO'),
    ('gSafariZone_Northeast', 'land_mons', 9, 'SPECIES_FUECOCO'),
]


def label_matches(label, prefix):
    if prefix.endswith('_'):  # FireRed and LeafGreen tables
        return label in (prefix + 'FireRed', prefix + 'LeafGreen')
    return label == prefix


def main():
    data = json.load(open(PATH, encoding='utf-8'))
    group = data['wild_encounter_groups'][0]
    done = 0
    for prefix, field, slot, species in PLACEMENTS:
        hits = [e for e in group['encounters'] if label_matches(e['base_label'], prefix)]
        if not hits:
            raise SystemExit('no encounter table %s' % prefix)
        for e in hits:
            mons = e[field]['mons']
            mons[slot]['species'] = species
            done += 1
    with open(PATH, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
        f.write('\n')
    print('%d encounter slots set' % done)


if __name__ == '__main__':
    main()
