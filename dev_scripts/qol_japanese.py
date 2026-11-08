#!/usr/bin/env python3
"""[QoL] Japanese text tool.

Translations live in text/ja/, mirroring the source tree:

  data/maps/PalletTown_Frlg/scripts.inc -> text/ja/data/maps/PalletTown_Frlg/scripts.json
  src/battle_message.c                  -> text/ja/src/battle_message.json

Script sources (.inc): the JSON maps a text label to its Japanese text.
C sources (.c): the JSON maps an English string literal (as written between
the quotes) to its Japanese text.

`apply` writes each translation into the source right after the English text,
as an "alternate" that starts with two {JPN} codes:

  PalletTown_Text_Hello::
      .string "Hello!$"
      .string "{JPN}{JPN}こんにちは！$"

  const u8 gText_Yes[] = _("YES${JPN}{JPN}はい");

The English text ends at its '$' (end of string), so English mode never sees
the alternate. In Japanese mode, src/qol_language.c finds it right after the
end of the English text and prints it instead.

Writing translations: Gen 3 Japanese uses kana only (no kanji) and full-width
spaces instead of commas. Separate pages with \\p. Line breaks are added by
`apply` (24 characters per line), so write each page as one line with spaces
between phrases; explicit \\n line breaks are kept.

Commands:
  sources                list the translatable source files
  show SOURCE            print the English text of a source as JSON
  check [JSON ...]       validate translation files (default: all)
  apply [JSON ...]       write translations into the sources (default: all)
  strip                  remove every Japanese alternate from the sources
  coverage               translated / total per source
"""
import glob
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TEXT_DIR = os.path.join(ROOT, 'text', 'ja')
ALT = '{JPN}{JPN}'
WIDTH = 24  # characters per message line (message boxes are 27 tiles of 8px)

# Script files the FireRed build uses, besides the FRLG map scripts.
SHARED_SCRIPT_FILES = [
    'data/text/fame_checker_frlg.inc', 'data/text/trainer_card_frlg.inc', 'data/text/day_care_frlg.inc',
    'data/text/new_game_intro_frlg.inc', 'data/text/trainers_frlg.inc', 'data/text/ingame_trade_frlg.inc',
    'data/text/pc.inc', 'data/text/pkmn_center_nurse.inc', 'data/text/obtain_item.inc',
    'data/text/move_relearner.inc', 'data/text/surf.inc', 'data/text/save.inc', 'data/text/braille.inc',
    'data/scripts/trainer_tower.inc', 'data/scripts/fame_checker_frlg.inc', 'data/scripts/item_ball_scripts_frlg.inc',
    'data/scripts/silphco_doors.inc', 'data/scripts/move_tutors_frlg.inc', 'data/scripts/cable_club_frlg.inc',
    'data/scripts/trainer_card_frlg.inc', 'data/scripts/mystery_event_club.inc', 'data/scripts/day_care_frlg.inc',
    'data/scripts/seagallop.inc', 'data/scripts/static_pokemon.inc', 'data/scripts/aide.inc',
    'data/scripts/pokemon_mansion.inc', 'data/scripts/pokemon_league.inc', 'data/scripts/route23.inc',
    'data/scripts/trainers_frlg.inc', 'data/scripts/flavor_text.inc', 'data/scripts/pkmn_center_nurse_frlg.inc',
    'data/scripts/std_msgbox.inc', 'data/scripts/trainer_battle.inc', 'data/scripts/new_game.inc',
    'data/scripts/hall_of_fame.inc', 'data/scripts/hall_of_fame_frlg.inc', 'data/scripts/pkmn_center_nurse.inc',
    'data/scripts/obtain_item.inc', 'data/scripts/pc.inc', 'data/scripts/move_relearner.inc',
    'data/scripts/surf.inc', 'data/scripts/field_poison.inc', 'data/scripts/field_move_scripts.inc',
    'data/scripts/repel.inc', 'data/scripts/safari_zone.inc', 'data/scripts/follower.inc',
    'data/scripts/dexnav.inc', 'data/scripts/flash.inc', 'data/scripts/qol_quests.inc',
    'data/scripts/qol_regions.inc', 'data/scripts/qol_starfall.inc',
]

# ---------------------------------------------------------------- characters

def load_charmap():
    chars = {}
    for line in open(os.path.join(ROOT, 'charmap.txt'), encoding='utf-8'):
        m = re.match(r"^'(.)'\s*=\s*([0-9A-Fa-f]{2})", line)
        if m:
            chars.setdefault(m.group(1), int(m.group(2), 16))
    return chars

CHARMAP = load_charmap()
KANA = set(c for c in CHARMAP if '぀' <= c <= 'ヿ')
# Characters that look right in the Japanese font (codes A1-EE and F0 are shared
# with English: digits, letters and some punctuation).
JP_PUNCT = set('　！？。ー・‥「」『』♂♀円×/:') | set('0123456789') | set('ABCDEFGHIJKLMNOPQRSTUVWXYZ') \
    | set('abcdefghijklmnopqrstuvwxyz') | {' ', '!', '?', '-', '.'}
ALLOWED = (KANA | JP_PUNCT) & (set(CHARMAP) | {' '})
NORMALIZE = {'、': '　', '，': '　', '…': '‥', '⋯': '‥', '〜': 'ー', '～': 'ー', 'ヴ': 'ブ', '．': '.',
             '：': ':', '／': '/', '－': 'ー', '―': 'ー', '‐': 'ー', '“': '『', '”': '』', '"': '』'}
for i in range(10):
    NORMALIZE[chr(0xFF10 + i)] = str(i)
for i in range(26):
    NORMALIZE[chr(0xFF21 + i)] = chr(65 + i)
    NORMALIZE[chr(0xFF41 + i)] = chr(97 + i)

# Width of placeholders, in characters. Control codes with arguments print nothing.
PLACEHOLDER_WIDTH = {'PLAYER': 7, 'RIVAL': 7, 'KUN': 3}
ZERO_WIDTH_PREFIXES = ('COLOR', 'HIGHLIGHT', 'SHADOW', 'PALETTE', 'FONT', 'RESET_FONT', 'PAUSE', 'WAIT_SE',
                       'PLAY_BGM', 'PLAY_SE', 'ESCAPE', 'SHIFT', 'FILL_WINDOW', 'CLEAR', 'SKIP', 'MIN_LETTER',
                       'JPN', 'ENG', 'PAUSE_MUSIC', 'RESUME_MUSIC', 'TEXT_COLORS', 'ACCENT', 'SPEED',
                       'CLEAR_TO', 'UNKNOWN', 'DYNAMIC', 'NAME_END', 'RESET', 'WAIT')

def token_width(tok):
    name = tok[1:-1].split(' ')[0]
    if name in PLACEHOLDER_WIDTH:
        return PLACEHOLDER_WIDTH[name]
    if name.startswith(ZERO_WIDTH_PREFIXES):
        return 0
    if name.startswith(('DPAD', 'A_BUTTON', 'B_BUTTON', 'L_BUTTON', 'R_BUTTON', 'START', 'SELECT', 'UP_', 'DOWN_',
                        'LEFT_', 'RIGHT_', 'EMOJI', 'CIRCLE', 'PLUS', 'CROSS', 'LV', 'PK', 'MN', 'PO', 'KE',
                        'BL', 'OC', 'K', 'NO', 'POKEBLOCK', 'ID', 'SUPER', 'LEFT', 'RIGHT', 'UP', 'DOWN')):
        return 2
    return 10  # strings filled in at runtime: names, numbers, items...

TOKEN_RE = re.compile(r'(\{[^}]*\}|\\[nlp]|.)')

def tokens(text):
    return TOKEN_RE.findall(text)

def normalize(text):
    return ''.join(NORMALIZE.get(c, c) for c in text)

def placeholders(text):
    # Runtime strings that must survive translation (names, numbers, items...).
    out = []
    for t in tokens(text):
        if t.startswith('{') and token_width(t) >= 3:
            out.append(t)
    return sorted(out)

# ------------------------------------------------------------- line wrapping

def tok_width(t):
    return token_width(t) if t.startswith('{') else (0 if t in ('\\n', '\\l', '\\p') else 1)

def wrap_page(page, width):
    """Wrap one page. Breaks at spaces (never inside {CODES}); a space-free run
    longer than a line is cut between characters."""
    lines = []
    for hard in re.split(r'\\[nl]', page):
        words, cur = [], []
        for t in tokens(hard):
            if t in (' ', '　'):
                words.append(cur)
                cur = []
            else:
                cur.append(t)
        words.append(cur)
        line, linew = [], 0
        for w in words:
            ww = sum(tok_width(t) for t in w)
            if not w:
                continue
            if linew > 0 and linew + 1 + ww > width:
                lines.append(''.join(line))
                line, linew = [], 0
            elif linew > 0:
                line.append('　')
                linew += 1
            while ww > width:  # one long run: cut it
                part, pw = [], 0
                while w and pw + tok_width(w[0]) <= width:
                    pw += tok_width(w[0])
                    part.append(w.pop(0))
                if not part:
                    part.append(w.pop(0))
                lines.append(''.join(line) + ''.join(part))
                line, linew = [], 0
                ww = sum(tok_width(t) for t in w)
            line.extend(w)
            linew += ww
        lines.append(''.join(line))
    return [l for l in lines if l != ''] or ['']

def layout(text, wrap, newline_only=False):
    """Japanese text -> list of source string pieces with \\n, \\l and \\p in place.
    newline_only: every line break is \\n (windows that print a page at once
    and never scroll), as the English text does there."""
    text = normalize(text)
    if not wrap:
        return [text]  # short strings keep their spaces (e.g. a prefix like 「やせいの　」)
    text = text.strip()
    pages = text.split('\\p')
    out = []
    for pi, page in enumerate(pages):
        lines = wrap_page(page.strip(), WIDTH)
        for li, line in enumerate(lines):
            if li == 0:
                out.append(line)
            elif li == 1 or newline_only:
                out[-1] += '\\n'
                out.append(line)
            else:
                out[-1] += '\\l'
                out.append(line)
        if pi < len(pages) - 1:
            out[-1] += '\\p'
    return out

def uses_newline_only(en):
    # English pages with 3+ lines and no \\l scroll are printed in windows
    # that show a whole page at once: keep plain \\n breaks there too.
    if '\\l' in en:
        return False
    return any(page.count('\\n') >= 2 for page in en.split('\\p'))

def needs_wrap(en, jp):
    if any(code in en for code in ('\\n', '\\l', '\\p')):
        return True
    w = sum(token_width(t) if t.startswith('{') else 1 for t in tokens(normalize(jp)))
    return w > WIDTH

# ------------------------------------------------------------ script sources

LABEL_RE = re.compile(r'^(\w+)::?\s*(?:@.*)?$')
STRING_RE = re.compile(r'^\s*\.string\s+"(.*)"\s*(?:@.*)?$')

def script_sources():
    files = sorted(glob.glob(os.path.join(ROOT, 'data/maps/*_Frlg/scripts.inc')))
    files = [os.path.relpath(f, ROOT) for f in files]
    return files + [f for f in SHARED_SCRIPT_FILES if os.path.exists(os.path.join(ROOT, f))]

def parse_script(path):
    """Return {label: (english, first_line, last_en_line, last_alt_line)} for single-string text blocks."""
    lines = open(os.path.join(ROOT, path), encoding='utf-8').read().split('\n')
    texts = {}
    i = 0
    while i < len(lines):
        m = LABEL_RE.match(lines[i])
        if not m:
            i += 1
            continue
        label = m.group(1)
        j = i + 1
        parts = []
        while j < len(lines) and STRING_RE.match(lines[j]):
            s = STRING_RE.match(lines[j]).group(1)
            if s.startswith(ALT):
                break
            parts.append(s)
            j += 1
        en = ''.join(parts)
        if parts and en.endswith('$') and en.count('$') == 1:
            k = j  # existing alternate block
            if k < len(lines) and STRING_RE.match(lines[k]) and STRING_RE.match(lines[k]).group(1).startswith(ALT):
                while k < len(lines) and STRING_RE.match(lines[k]):
                    if STRING_RE.match(lines[k]).group(1).endswith('$'):
                        k += 1
                        break
                    k += 1
            texts[label] = (en[:-1], i, j, k)
        i = j if j > i + 1 else i + 1
    return texts, lines

# ----------------------------------------------------------------- C sources

C_DECL_RE = re.compile(r'(const u8 \w+\[\]\s*=\s*_\(")((?:[^"\\]|\\.)*)("\))')
C_COMPOUND_RE = re.compile(r'(COMPOUND_STRING\(")((?:[^"\\]|\\.)*)("\))')

def c_strings(path):
    src = open(os.path.join(ROOT, path), encoding='utf-8').read()
    out = []
    for rx in (C_DECL_RE, C_COMPOUND_RE):
        for m in rx.finditer(src):
            body = m.group(2)
            en = body.split('$' + ALT)[0]
            if '$' in en:
                continue
            out.append(en)
    return out

# ------------------------------------------------------------- translations

def translation_path(source):
    base = os.path.splitext(source)[0]
    return os.path.join(TEXT_DIR, base + '.json')

def source_of(json_path):
    rel = os.path.relpath(os.path.abspath(json_path), TEXT_DIR)
    base = os.path.splitext(rel)[0]
    for ext in ('.inc', '.c', '.h'):
        if os.path.exists(os.path.join(ROOT, base + ext)):
            return base + ext
    return None

def all_translation_files():
    return sorted(glob.glob(os.path.join(TEXT_DIR, '**', '*.json'), recursive=True))

def load_json(p):
    return json.load(open(p, encoding='utf-8'))

def check_entry(key, en, jp, errors):
    jp = normalize(jp)
    for t in tokens(jp):
        if t.startswith('{'):
            name = t[1:-1].split(' ')[0]
            if name not in KNOWN_CODES:
                errors.append('%s: unknown code %s' % (key, t))
        elif t in ('\\n', '\\l', '\\p'):
            pass
        elif t not in ALLOWED:
            errors.append('%s: character %r is not in the Japanese font' % (key, t))
    if en is not None and placeholders(en) != placeholders(jp):
        errors.append('%s: placeholders differ: %s vs %s' % (key, placeholders(en), placeholders(jp)))
    if en is not None and jp.strip() == '':
        errors.append('%s: empty translation' % key)

def load_known_codes():
    codes = set()
    for line in open(os.path.join(ROOT, 'charmap.txt'), encoding='utf-8'):
        m = re.match(r'^([A-Z_0-9]+)\s*=', line)
        if m:
            codes.add(m.group(1))
    return codes

KNOWN_CODES = load_known_codes()

def cmd_check(files):
    files = files or all_translation_files()
    bad = 0
    for f in files:
        source = source_of(f)
        errors = []
        if source is None:
            errors.append('no source file for %s' % f)
        else:
            try:
                data = load_json(f)
            except Exception as e:
                print('%s: invalid JSON: %s' % (f, e))
                bad += 1
                continue
            if source.endswith('.inc'):
                texts, _ = parse_script(source)
                for label, jp in data.items():
                    if label not in texts:
                        errors.append('%s: no such text label in %s' % (label, source))
                        continue
                    check_entry(label, texts[label][0], jp, errors)
            else:
                known = set(c_strings(source))
                for en, jp in data.items():
                    if en not in known:
                        errors.append('%r: no such string in %s' % (en[:40], source))
                        continue
                    check_entry(en[:40], en, jp, errors)
                    # C strings are sometimes copied into small buffers.
                    jl = len(tokens(normalize(jp)))
                    el = len(tokens(en))
                    if el <= 16 and jl > el + 4:
                        errors.append('%r: too long for a short C string (%d > %d + 4 characters)' % (en[:40], jl, el))
        for e in errors:
            print('%s: %s' % (os.path.relpath(f, ROOT), e))
        bad += len(errors)
    print('%d problem(s)' % bad)
    return 1 if bad else 0

def apply_script(source, data):
    texts, lines = parse_script(source)
    edits = []
    for label, (en, start, en_end, alt_end) in texts.items():
        jp = data.get(label)
        new = []
        if jp:
            pieces = layout(jp, needs_wrap(en, jp), uses_newline_only(en))
            pieces[0] = ALT + pieces[0]
            pieces[-1] += '$'
            new = ['\t.string "%s"' % p for p in pieces]
        edits.append((en_end, alt_end, new))
    for en_end, alt_end, new in sorted(edits, reverse=True):
        lines[en_end:alt_end] = new
    open(os.path.join(ROOT, source), 'w', encoding='utf-8').write('\n'.join(lines))

def apply_c(source, data):
    path = os.path.join(ROOT, source)
    src = open(path, encoding='utf-8').read()
    def repl(m):
        body = m.group(2)
        en = body.split('$' + ALT)[0]
        jp = data.get(en)
        if not jp:
            return m.group(1) + en + m.group(3)
        pieces = layout(jp, needs_wrap(en, jp), uses_newline_only(en))
        return m.group(1) + en + '$' + ALT + ''.join(pieces) + m.group(3)
    for rx in (C_DECL_RE, C_COMPOUND_RE):
        src = rx.sub(repl, src)
    open(path, 'w', encoding='utf-8').write(src)

def cmd_apply(files, strip=False):
    if strip:
        for source in script_sources():
            apply_script(source, {})
        for f in all_translation_files():
            s = source_of(f)
            if s and not s.endswith('.inc'):
                apply_c(s, {})
        return 0
    files = files or all_translation_files()
    for f in files:
        source = source_of(f)
        if not source:
            print('skip %s: no source' % f)
            continue
        data = {k: v for k, v in load_json(f).items() if isinstance(v, str) and v.strip()}
        if source.endswith('.inc'):
            apply_script(source, data)
        else:
            apply_c(source, data)
        print('applied %d text(s) to %s' % (len(data), source))
    return 0

def cmd_show(source):
    if not source.endswith('.inc'):
        print(json.dumps({en: '' for en in c_strings(source)}, ensure_ascii=False, indent=1))
    else:
        texts, _ = parse_script(source)
        print(json.dumps({k: v[0] for k, v in texts.items()}, ensure_ascii=False, indent=1))
    return 0

def cmd_coverage():
    total = done = 0
    for source in script_sources():
        texts, lines = parse_script(source)
        n = len(texts)
        t = sum(1 for v in texts.values() if v[3] > v[2])
        total += n
        done += t
    print('script text: %d / %d translated (%.0f%%)' % (done, total, 100.0 * done / max(total, 1)))
    for f in all_translation_files():
        s = source_of(f)
        if s and not s.endswith('.inc'):
            src = open(os.path.join(ROOT, s), encoding='utf-8').read()
            n = len(c_strings(s))
            t = src.count('$' + ALT)
            print('%s: %d / %d' % (s, t, n))
    return 0

def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1
    cmd, args = sys.argv[1], sys.argv[2:]
    if cmd == 'sources':
        for s in script_sources():
            print(s)
        return 0
    if cmd == 'show':
        return cmd_show(args[0])
    if cmd == 'check':
        return cmd_check(args)
    if cmd == 'apply':
        return cmd_apply(args)
    if cmd == 'strip':
        return cmd_apply([], strip=True)
    if cmd == 'coverage':
        return cmd_coverage()
    print(__doc__)
    return 1

if __name__ == '__main__':
    sys.exit(main())
