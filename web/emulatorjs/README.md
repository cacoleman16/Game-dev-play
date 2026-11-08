# Vendored EmulatorJS

- EmulatorJS 4.2.3 `data/` folder (npm `@emulatorjs/emulatorjs@4.2.3`)
- mGBA core 4.2.3: `data/cores/mgba-wasm.data` and `mgba-legacy-wasm.data`
  (npm `@emulatorjs/core-mgba@4.2.3`), non-threaded builds, so no special
  server headers are needed
- RAR support (`libunrar`) removed; it isn't needed for `.gba` files
- Patched `data/src/GameManager.js`: the exit handler no longer unmounts
  `/data/saves`. Unmounting there let IDBFS autoPersist sync an empty folder
  right after, which deleted every stored save on each page reload.

License: GPL-3.0, see `LICENSE`. Upstream: https://github.com/EmulatorJS/EmulatorJS
