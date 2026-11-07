# dev_scripts

Small helpers used while building the QoL hack. None of them are part of the
ROM build.

| Script | What it does |
| --- | --- |
| `syntax_check.sh` | Type-checks C files with the host `gcc` (front end only, C89 rules like agbcc). Handy when you can't run the full build. Needs `gcc-multilib`. |
| `gen_headers.py` | Writes stand-ins for the build-generated headers (`map_groups.h`, `layouts.h`, `map_event_ids.h`, `region_map_sections.h`, `data/items.h`) so `syntax_check.sh` can find them. |
| `add_move_categories.py` | One-shot script that added `.category` to every move in `src/data/battle_moves.h` (already applied). |
| `make_fairy_gfx.py` | One-shot script that drew the FAIRY type badge and TM disc palette (already applied). |

```sh
python3 dev_scripts/gen_headers.py . /tmp/gen
dev_scripts/syntax_check.sh . /tmp/gen src/follower_pokemon.c src/pokemon.c
```

For a real build, follow `INSTALL.md` (agbcc + `make`).
