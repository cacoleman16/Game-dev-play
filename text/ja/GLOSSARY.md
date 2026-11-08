# Japanese translation guide (FireRed QoL)

The game's Japanese mode uses the Gen 3 Japanese font: **kana only, no kanji**,
like the original Japanese FireRed. Translations live in `text/ja/` as JSON and
are written into the sources with `python3 dev_scripts/qol_japanese.py apply`.
Check every file with `python3 dev_scripts/qol_japanese.py check FILE.json`.

## Format

- Script files (`text/ja/data/.../scripts.json`): `{ "TextLabel": "日本語" }`.
- C files (`text/ja/src/xxx.json`): `{ "English literal exactly as in the source": "日本語" }`.
- Pages: keep a `\p` wherever the English starts a new page (roughly one
  Japanese page per English page). In JSON a backslash is written `\\p`.
- Lines: don't add `\n` or `\l`; `apply` wraps at 24 characters on the spaces
  you put between phrases. Use `\n` only to force a break (lists, menus).
- Keep every `{CODE}` exactly: `{PLAYER}`, `{RIVAL}`, `{STR_VAR_1}`..., battle
  codes like `{B_ATK_NAME_WITH_PREFIX}`, `{COLOR ...}`, `{PAUSE ...}`,
  `{PLAY_SE ...}` etc. The checker compares the name/number placeholders.
- A leading `{COLOR ...}{SHADOW ...}` stays at the start (some menus recolor it).
- Money: `¥{STR_VAR_1}` -> `{STR_VAR_1}円`.
- Untranslatable or meaningless strings (debug text, single symbols): leave them out.

## Style

- Hiragana for Japanese words, katakana for loanwords and names.
- No 、 (it isn't in the font): separate phrases with a full-width space,
  as Gen 3 did: `ポケモン センターへ ようこそ！`
- Allowed punctuation: `！ ？ 。 ー ・ ‥ 「 」 『 』 ♂ ♀ 円 × / :` plus A-Z a-z 0-9.
  Not available: 、 （ ） ～ ＆ ％ ＋ ＝ ； and kanji.
- Ellipsis: `‥` (one character). Long vowel: `ー`.
- Tone: friendly, short. NPCs talk like the originals (ぼく/わたし/おれ as fits,
  ～だよ/～です). Professor Oak: 〜じゃ style (「わしは オーキド！」).
- Keep it about as long as the English or shorter.

## Names and terms

| English | Japanese |
| --- | --- |
| POKéMON | ポケモン |
| POKéDEX | ポケモンずかん (ずかん) |
| TRAINER | トレーナー |
| PROF. OAK | オーキドはかせ |
| MOM | ママ |
| DAISY | ナナミ |
| BILL | マサキ |
| MR. FUJI | フジろうじん |
| TEAM ROCKET | ロケットだん |
| BROCK / MISTY / LT. SURGE / ERIKA | タケシ / カスミ / マチス / エリカ |
| KOGA / SABRINA / BLAINE / GIOVANNI | キョウ / ナツメ / カツラ / サカキ |
| LORELEI / BRUNO / AGATHA / LANCE | カンナ / シバ / キクコ / ワタル |
| CELIO | ニシキ |
| GYM / GYM LEADER | ジム / ジムリーダー |
| BADGE | バッジ |
| ELITE FOUR / CHAMPION | してんのう / チャンピオン |
| POKéMON LEAGUE | ポケモンリーグ |
| POKéMON CENTER | ポケモンセンター |
| POKé MART | フレンドリィショップ |
| PC / BOX | パソコン / ボックス |
| BILL's PC / SOMEONE's PC | マサキの パソコン / だれかの パソコン |
| party | てもち |
| item / KEY ITEM | どうぐ / だいじな もの |
| BAG | バッグ |
| SAVE (the game) | レポートを かく |
| TM / HM | わざマシン / ひでんマシン |
| POKé BALL | モンスターボール |
| POTION | キズぐすり |
| REPEL | むしよけスプレー |
| ESCAPE ROPE | あなぬけのヒモ |
| OLD / GOOD / SUPER ROD | ボロのつりざお / いいつりざお / すごいつりざお |
| BICYCLE | じてんしゃ |
| TOWN MAP | タウンマップ |
| S.S. TICKET / S.S. ANNE | ふねのチケット / サントアンヌごう |
| SILPH SCOPE / SILPH CO. | シルフスコープ / シルフカンパニー |
| POKé FLUTE | ポケモンのふえ |
| CUT / FLY / SURF / STRENGTH / FLASH | いあいぎり / そらをとぶ / なみのり / かいりき / フラッシュ |
| ROCK SMASH / WATERFALL | いわくだき / たきのぼり |
| EXP. / LEVEL | けいけんち / レベル |
| Fainted | ひんし |
| money | おかね |
| Fame Checker | ひとしりフォン |
| VS Seeker | バトルサーチャー |
| Teachy TV | おしえてテレビ |
| Tri-Pass / Rainbow Pass | トライパス / レインボーパス |

| Place | Japanese |
| --- | --- |
| PALLET TOWN / VIRIDIAN CITY / PEWTER CITY | マサラタウン / トキワシティ / ニビシティ |
| CERULEAN CITY / VERMILION CITY / LAVENDER TOWN | ハナダシティ / クチバシティ / シオンタウン |
| CELADON CITY / SAFFRON CITY / FUCHSIA CITY | タマムシシティ / ヤマブキシティ / セキチクシティ |
| CINNABAR ISLAND / INDIGO PLATEAU | グレンじま / セキエイこうげん |
| ROUTE 1 | 1ばんどうろ |
| VIRIDIAN FOREST / MT. MOON / ROCK TUNNEL | トキワのもり / おつきみやま / イワヤマトンネル |
| DIGLETT'S CAVE / POKéMON TOWER / POWER PLANT | ディグダのあな / ポケモンタワー / むじんはつでんしょ |
| SAFARI ZONE / SEAFOAM ISLANDS / POKéMON MANSION | サファリゾーン / ふたごじま / ポケモンやしき |
| VICTORY ROAD / CERULEAN CAVE | チャンピオンロード / ハナダのどうくつ |
| GAME CORNER / CYCLING ROAD / UNDERGROUND PATH | ゲームコーナー / サイクリングロード / ちかつうろ |
| DEPT. STORE / DAY CARE / BIKE SHOP | デパート / そだてや / じてんしゃや |
| POKéMON FAN CLUB | ポケモンだいすきクラブ |
| SEVII ISLANDS / ONE ISLAND ... SEVEN ISLAND | ナナシマ / 1のしま ... 7のしま |
| MT. EMBER / KINDLE ROAD / TREASURE BEACH | ともしびやま / ほてりのみち / たからのはま |
| BERRY FOREST / ICEFALL CAVE / LOST CAVE | きのみのもり / いてだきのどうくつ / かえらずのあな |
| TANOBY RUINS / TRAINER TOWER / NAVEL ROCK / BIRTH ISLAND | アスカナいせき / トレーナータワー / へそのいわ / たんじょうのしま |

This hack's own features:

| English | Japanese |
| --- | --- |
| NUZLOCKE | ナズロック |
| RANDOMIZER | ランダマイザー |
| HARD MODE | ハードモード |
| LEVEL CAPS | レベルキャップ |
| FOLLOWERS / following POKéMON | つれあるき |
| DexNav | ずかんナビ |
| EXP. SHARE | がくしゅうそうち |
| auto-run | オートダッシュ |
| Hyper Training | すごいとっくん |
| BOTTLE CAP / GOLD BOTTLE CAP | ぎんのおうかん / きんのおうかん |
| ABILITY CAPSULE / PATCH | とくせいカプセル / とくせいパッチ |
| MEGA STONE / Z-CRYSTAL | メガストーン / Zクリスタル |
| MEGA RING / DYNAMAX BAND / TERA ORB | メガリング / ダイマックスバンド / テラスタルオーブ |

The Starfall story (`data/scripts/qol_starfall.inc`, see `docs/STARFALL.md`):

| English | Japanese |
| --- | --- |
| STARFALL (the event, and the title) | スターフォール |
| STAR SHARD / shard | スターシャード / かけら (not ほしのかけら: that is STAR PIECE) |
| meteor / meteorite | ながれぼし / いんせき |
| TEAM NOVA | ノバだん (ヴ is not in the font; it would print as ブ) |
| VEGA / ALTAIR / DENEB | ベガ / アルタイル / デネブ (grunts call her ベガさま) |
| Nova leader / admin / grunt | ボス / かんぶ / したっぱ |
| TEAM MAGMA / TEAM AQUA / MAXIE / ARCHIE | マグマだん / アクアだん / マツブサ / アオギリ |
| STEVEN (STONE) | ダイゴ (ツワブキ　ダイゴ); Oak calls him ダイゴくん |
| PROF. BIRCH / PROF. COZMO / NORMAN | オダマキはかせ / ソライシはかせ / センリ |
| S.S. TIDAL | タイドリップごう |
| LITTLEROOT / PETALBURG / MOSSDEEP | ミシロタウン / トウカシティ / トクサネシティ |
| METEOR FALLS / MT. CHIMNEY / SPACE CENTER | りゅうせいのたき / えんとつやま / (トクサネ)うちゅうセンター |
| MOON STONE / MASTER BALL | つきのいし / マスターボール |

Voices: ALTAIR rough (おれ, ～だぜ/～さ); DENEB playful (うふふ, ～わ/～よ,
calls the player ダーリン); VEGA an heiress (わたくし, ～なさい/～わ).
Speakers are written `ナマエ『...』` like the Oak's Lab text; letters and
notes use `「...」`.

Pokémon, moves, items and abilities use their official Japanese names; the game
already has those tables (`src/data/text/qol_ja_names.h`), so in running text
just write the official name.
