// [QoL] Offline support for the FireRed QoL Player.
// Caches the page and the vendored emulator so it works without a connection
// after the first visit. Bump CACHE when web/ changes so phones pick it up.
const CACHE = "frqol-player-v3";
const PRECACHE = [
  "./",
  "apple-touch-icon.png",
  "emulatorjs/data/compression/extract7z.js",
  "emulatorjs/data/compression/extractzip.js",
  "emulatorjs/data/cores/mgba-legacy-wasm.data",
  "emulatorjs/data/cores/mgba-wasm.data",
  "emulatorjs/data/cores/reports/mgba.json",
  "emulatorjs/data/emulator.css",
  "emulatorjs/data/loader.js",
  "emulatorjs/data/localization/en-US.json",
  "emulatorjs/data/localization/ja-JA.json",
  "emulatorjs/data/src/GameManager.js",
  "emulatorjs/data/src/compression.js",
  "emulatorjs/data/src/emulator.js",
  "emulatorjs/data/src/gamepad.js",
  "emulatorjs/data/src/nipplejs.js",
  "emulatorjs/data/src/shaders.js",
  "emulatorjs/data/src/socket.io.min.js",
  "emulatorjs/data/src/storage.js",
  "emulatorjs/data/version.json",
  "icon-192.png",
  "icon-512.png",
  "icon-maskable-512.png",
  "index.html",
  "manifest.webmanifest"
];

self.addEventListener("install", (event) => {
  event.waitUntil(caches.open(CACHE).then((cache) => cache.addAll(PRECACHE)).then(() => self.skipWaiting()));
});

self.addEventListener("activate", (event) => {
  event.waitUntil(
    caches.keys()
      .then((keys) => Promise.all(keys.filter((k) => k !== CACHE).map((k) => caches.delete(k))))
      .then(() => self.clients.claim())
  );
});

self.addEventListener("fetch", (event) => {
  const req = event.request;
  const url = new URL(req.url);
  if (req.method !== "GET" || url.origin !== self.location.origin) return; // ROM blobs, CDN version checks, etc.
  if (url.pathname.startsWith("/aetherveil/")) return; // The 3D game has its own service worker.

  if (req.mode === "navigate") {
    // Page: network first so updates arrive, cache when offline.
    event.respondWith(
      fetch(req)
        .then((res) => {
          if (res.ok) { const copy = res.clone(); caches.open(CACHE).then((c) => c.put("index.html", copy)); }
          return res;
        })
        .catch(() => caches.match("index.html"))
    );
    return;
  }

  // Emulator files and icons: cache first.
  event.respondWith(
    caches.match(req).then((hit) => hit || fetch(req).then((res) => {
      if (res.ok) { const copy = res.clone(); caches.open(CACHE).then((c) => c.put(req, copy)); }
      return res;
    }))
  );
});
