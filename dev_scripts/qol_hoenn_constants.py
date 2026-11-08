#!/usr/bin/env python3
"""[QoL] Merged regions: give Hoenn its own flags, vars and trainer IDs in the
FireRed build.

The FireRed build includes the Hoenn (Emerald) maps and scripts. Upstream, the
Emerald-only flags are 0 there and the Emerald vars and trainer IDs overlap the
Kanto ones, so Hoenn scripts would clobber Kanto progress. This script rewrites
the constant headers so that, in FRLG builds:

  * every Emerald-only flag is QOL_HOENN_FLAG(<its Emerald number>), a flag in a
    separate block stored in SaveBlock3 (src/qol_hoenn.c);
  * every Emerald-only var is QOL_HOENN_VAR(<its Emerald number>), likewise;
  * every Emerald trainer is HOENN_TRAINER(<its Emerald ID>), an ID after the
    Kanto trainers, so both trainer tables fit in gTrainers.

Emerald builds are unchanged (the macros give back the Emerald numbers).
The script is idempotent: run it again after merging upstream changes.

Usage (from the repo root): python3 dev_scripts/qol_hoenn_constants.py
"""
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFINE = re.compile(r'^(#define\s+)(\w+)(\s+)(\S.*?)(\s*//.*)?$')


def path(p):
    return os.path.join(ROOT, p)


def evaluate(expr, known):
    expr = expr.strip()
    expr = re.sub(r'\b\w+\b', lambda m: str(known[m.group(0)]) if m.group(0) in known else m.group(0), expr)
    if re.search(r'[A-Za-z_]', re.sub(r'0x[0-9A-Fa-f]+', '', expr)):
        return None
    try:
        return int(eval(expr, {}, {}))
    except Exception:
        return None


def emerald_flags():
    """Name -> value for the flags in flags.h's Emerald branch."""
    lines = open(path('include/constants/flags.h'), encoding='utf-8').read().split('\n')
    known = {'TEMP_FLAGS_START': 0}
    for line in lines:
        m = DEFINE.match(line)
        if m and m.group(2).startswith('FLAG_TEMP_'):
            v = evaluate(m.group(4), known)
            if v is not None:
                known[m.group(2)] = v
    # MAX_TRAINERS_COUNT is the Emerald one in that branch
    known['MAX_TRAINERS_COUNT'] = 864
    start = lines.index('#if IS_FRLG')
    i = start + 1
    while lines[i] != '#else':
        i += 1
    i += 1
    flags = {}
    while lines[i] != '#endif':
        m = DEFINE.match(lines[i])
        if m:
            v = evaluate(m.group(4), dict(known, **flags))
            if v is not None:
                flags[m.group(2)] = v
            elif 'REMATCH_' in m.group(4):
                flags[m.group(2)] = m.group(4)  # match call flags: keep the expression
        i += 1
    return flags


def patch_flags():
    em = emerald_flags()
    p = path('include/constants/flags_frlg.h')
    lines = open(p, encoding='utf-8').read().split('\n')
    start = lines.index('// emerald flags')
    changed = 0
    for i in range(start, len(lines)):
        m = DEFINE.match(lines[i])
        if not m or m.group(4) != '0' or m.group(2) not in em:
            continue
        v = em[m.group(2)]
        if isinstance(v, str):
            new = v
        elif v == 0:
            continue
        elif v < 0x20:
            new = '(TEMP_FLAGS_START + 0x%X)' % v
        elif v >= 0x4000:
            new = '(SPECIAL_FLAGS_START + 0x%X)' % (v - 0x4000)
        else:
            new = 'QOL_HOENN_FLAG(0x%03X)' % v
        lines[i] = m.group(1) + m.group(2) + m.group(3) + new + (m.group(5) or '')
        changed += 1
    open(p, 'w', encoding='utf-8').write('\n'.join(lines))
    print('flags: %d Emerald flags moved to the Hoenn block' % changed)


def patch_vars():
    frlg = set()
    for line in open(path('include/constants/vars_frlg.h'), encoding='utf-8'):
        m = DEFINE.match(line.rstrip('\n'))
        if m:
            frlg.add(m.group(2))
    p = path('include/constants/vars.h')
    lines = open(p, encoding='utf-8').read().split('\n')
    changed = 0
    for i, line in enumerate(lines):
        m = DEFINE.match(line)
        if not m or m.group(2) in frlg or m.group(2).startswith('VAR_QOL_') or m.group(2) in ('VARS_END', 'VAR_KANTO_STARTER'):
            continue
        if not re.fullmatch(r'0x[0-9A-Fa-f]+', m.group(4)):
            continue
        v = int(m.group(4), 16)
        if not (0x4020 <= v <= 0x40FF):
            continue
        lines[i] = m.group(1) + m.group(2) + m.group(3) + 'QOL_HOENN_VAR(0x%04X)' % v + (m.group(5) or '')
        changed += 1
    open(p, 'w', encoding='utf-8').write('\n'.join(lines))
    print('vars: %d Emerald vars moved to the Hoenn block' % changed)


def patch_trainers():
    p = path('include/constants/opponents.h')
    lines = open(p, encoding='utf-8').read().split('\n')
    changed = 0
    for i, line in enumerate(lines):
        m = DEFINE.match(line)
        if not m or not m.group(2).startswith('TRAINER_') or m.group(2) == 'TRAINER_NONE':
            continue
        if not re.fullmatch(r'\d+', m.group(4)):
            continue
        lines[i] = m.group(1) + m.group(2) + m.group(3) + 'HOENN_TRAINER(%s)' % m.group(4) + (m.group(5) or '')
        changed += 1
    open(p, 'w', encoding='utf-8').write('\n'.join(lines))
    print('trainers: %d Emerald trainer IDs moved after the Kanto ones' % changed)


# Scripts that only Hoenn maps use, besides the Hoenn maps' own scripts.
HOENN_SHARED_SCRIPTS = ['data/scripts/players_house.inc', 'data/scripts/prof_birch.inc',
                        'data/scripts/secret_base.inc', 'data/scripts/hall_of_fame.inc']


def patch_hoenn_scripts():
    """Hoenn scripts get Hoenn's own badges and Hall of Fame flag."""
    import glob
    import json
    files = list(HOENN_SHARED_SCRIPTS)
    for m in glob.glob(path('data/maps/*/map.json')):
        if json.load(open(m, encoding='utf-8')).get('region') == 'REGION_HOENN':
            for f in ('scripts.inc', 'scripts.pory'):
                f = os.path.join(os.path.dirname(m), f)
                if os.path.exists(f):
                    files.append(os.path.relpath(f, ROOT))
    changed = 0
    for f in files:
        s = open(path(f), encoding='utf-8').read()
        t = re.sub(r'\bFLAG_BADGE0(\d)_GET\b', r'FLAG_HOENN_BADGE0\1_GET', s)
        t = re.sub(r'\bFLAG_SYS_GAME_CLEAR\b', 'FLAG_HOENN_GAME_CLEAR', t)
        if t != s:
            open(path(f), 'w', encoding='utf-8').write(t)
            changed += 1
    print('scripts: %d Hoenn script files use the Hoenn badges' % changed)


def patch_kanto_scripts():
    """Kanto scripts keep their starter in VAR_KANTO_STARTER (VAR_STARTER_MON is Hoenn's)."""
    import glob
    changed = 0
    for f in glob.glob(path('data/maps/*_Frlg/scripts.inc')):
        s = open(f, encoding='utf-8').read()
        t = re.sub(r'\bVAR_STARTER_MON\b', 'VAR_KANTO_STARTER', s)
        if t != s:
            open(f, 'w', encoding='utf-8').write(t)
            changed += 1
    print('scripts: %d Kanto script files use VAR_KANTO_STARTER' % changed)


if __name__ == '__main__':
    patch_flags()
    patch_vars()
    patch_trainers()
    patch_hoenn_scripts()
    patch_kanto_scripts()
