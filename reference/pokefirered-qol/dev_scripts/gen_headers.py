"""Generate stand-ins for pokefirered's build-generated headers so changed C
files can be syntax-checked with the host compiler (no decomp tools run).

Writes into <out>/: constants/map_groups.h, constants/layouts.h,
constants/map_event_ids.h, data/items.h
Usage: python3 -I gen_headers.py <pokefirered root> <out dir>
"""
import json
import os
import sys

root, out = sys.argv[1], sys.argv[2]
os.makedirs(f"{out}/constants", exist_ok=True)
os.makedirs(f"{out}/data", exist_ok=True)

# map_groups.h
groups = json.load(open(f"{root}/data/maps/map_groups.json"))
lines = ["#ifndef GUARD_CONSTANTS_MAP_GROUPS_H", "#define GUARD_CONSTANTS_MAP_GROUPS_H"]
event_lines = ["#ifndef GUARD_CONSTANTS_MAP_EVENT_IDS_H", "#define GUARD_CONSTANTS_MAP_EVENT_IDS_H"]
seen_local = set()
for g, group in enumerate(groups["group_order"]):
    for n, name in enumerate(groups[group]):
        m = json.load(open(f"{root}/data/maps/{name}/map.json"))
        lines.append(f"#define {m['id']} ({n} | ({g} << 8))")
        for i, obj in enumerate(m.get("object_events", [])):
            lid = obj.get("local_id")
            if lid and lid not in seen_local:
                seen_local.add(lid)
                event_lines.append(f"#define {lid} {i + 1}")
lines.append(f"#define MAP_GROUPS_COUNT {len(groups['group_order'])}")
lines.append("#endif")
open(f"{out}/constants/map_groups.h", "w").write("\n".join(lines) + "\n")
event_lines.append("#endif")
open(f"{out}/constants/map_event_ids.h", "w").write("\n".join(event_lines) + "\n")

# layouts.h
layouts = json.load(open(f"{root}/data/layouts/layouts.json"))
lines = ["#ifndef GUARD_CONSTANTS_LAYOUTS_H", "#define GUARD_CONSTANTS_LAYOUTS_H"]
for i, layout in enumerate(layouts["layouts"]):
    if layout.get("id"):
        lines.append(f"#define {layout['id']} {i + 1}")
lines.append("#endif")
open(f"{out}/constants/layouts.h", "w").write("\n".join(lines) + "\n")

# data/items.h (mirrors src/data/items.json.txt)
items = json.load(open(f"{root}/src/data/items.json"))["items"]
lines = []
for it in items:
    if it["pocket"] == "POCKET_TM_CASE":
        lines.append(f"extern const u8 gMoveDescription_{it['moveId']}[];")
    if it["itemId"] != "ITEM_NONE":
        lines.append(f"const u8 gItemDescription_{it['itemId']}[] = _(\"{it['description_english']}\");")
lines.append('const u8 gItemDescription_ITEM_NONE[] = _("?????");')
lines.append("const struct Item gItems[] = {")
for it in items:
    desc = f"gMoveDescription_{it['moveId']}" if it["pocket"] == "POCKET_TM_CASE" else f"gItemDescription_{it['itemId']}"
    lines.append(
        "    {"
        f".name = _(\"{it['english']}\"), .itemId = {it['itemId']}, .price = {it['price']}, "
        f".holdEffect = {it['holdEffect']}, .holdEffectParam = {it['holdEffectParam']}, .description = {desc}, "
        f".importance = {it['importance']}, .registrability = {it['registrability']}, .pocket = {it['pocket']}, "
        f".type = {it['type']}, .fieldUseFunc = {it['fieldUseFunc']}, .battleUsage = {it['battleUsage']}, "
        f".battleUseFunc = {it['battleUseFunc']}, .secondaryId = {it['secondaryId']}"
        "},"
    )
lines.append("};")
open(f"{out}/data/items.h", "w").write("\n".join(lines) + "\n")
print("generated headers in", out)

# constants/region_map_sections.h (mirrors region_map_sections.constants.json.txt)
secs = json.load(open(f"{root}/src/data/region_map/region_map_sections.json"))["map_sections"]
tmpl = open(f"{root}/src/data/region_map/region_map_sections.constants.json.txt").read()
body = "".join(f"    {s['id']},\n" for s in secs)
start = tmpl.index("## for map_section in map_sections\n")
end = tmpl.index("## endfor\n") + len("## endfor\n")
text = tmpl[:start] + body + tmpl[end:]
text = text.replace("{{ doNotModifyHeader }}\n", "")
open(f"{out}/constants/region_map_sections.h", "w").write(text)
