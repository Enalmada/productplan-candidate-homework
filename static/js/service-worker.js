// Local development test file
// Real service worker will be generated during production mode

importScripts("https://storage.googleapis.com/workbox-cdn/releases/3.6.3/workbox-sw.js");

workbox.skipWaiting();
workbox.clientsClaim();

workbox.routing.registerRoute(/^https:\/\/fonts\.googleapis\.com/, workbox.strategies.cacheFirst({cacheName: "google-fonts-stylesheets", plugins: [new workbox.expiration.Plugin({maxEntries: 100, maxAgeSeconds: 2592000, purgeOnQuotaError: false}), new workbox.cacheableResponse.Plugin({statuses: [0, 200]})]}), "GET");
workbox.routing.registerRoute(/^https?.*/, workbox.strategies.networkOnly(), "GET");
