/* eslint no-shadow: "off", "no-console":  off, no-unused-vars: 0, no-param-reassign: 0,
guard-for-in: 0, no-restricted-syntax: 0,  no-underscore-dangle: 0, global-require: 0, import/no-dynamic-require: 0,
prefer-destructuring: 0, no-bitwise: 0,  prefer-rest-params: 0, no-unused-expressions: 0, func-names: 0
 */

// Polyfill Node with `Intl` that has data for all locales.
// See: https://formatjs.io/guides/runtime-environments/#server
const IntlPolyfill = require("intl");

Intl.NumberFormat = IntlPolyfill.NumberFormat;
Intl.DateTimeFormat = IntlPolyfill.DateTimeFormat;

const dev = process.env.NODE_ENV !== "production";
// use "npm run start-dev" to enable local dotenv in production mode for testing
if (dev || process.env.LOCAL_ENV) {
    const dotenv = require("dotenv");
    dotenv.config();
}

// Allow https in dev mode
const https = require("https");
const devcert = require("devcert");


// const tamper = require("tamper");

const {readFileSync} = require("fs");
const {basename} = require("path");
const accepts = require("accepts");
const glob = require("glob");
const express = require("express");
const next = require("next");
const expressHealthcheck = require("express-healthcheck");
const fs = require("fs");
const cors = require("cors");

const port = parseInt(process.env.PORT, 10) || 3000;
const devSslPort = parseInt(process.env.SSL_PORT, 10) || 3443;
const app = next({dev});
const path = require("path");
const helmet = require("helmet");
// const shrinkRay = require("shrink-ray-current");

const env = process.env.ENV;

const handle = app.getRequestHandler();
const {CDN_URL} = process.env;
const cdnPrefix = CDN_URL ? `${CDN_URL}` : "";

const cookieParser = require("cookie-parser");
const Sentry = require("@sentry/node");
const uuidv4 = require("uuid/v4");
require("./util/sentry");


// Put every origin that would ever connect here.
const whitelist = [`http://localhost:${port}`, `https://www.myweb.com:${devSslPort}`];
const corsOptions = {
    origin(origin, callback) {
        if (origin === undefined || whitelist.indexOf(origin) !== -1) {
            callback(null, true);
        } else {
            callback(new Error(`Not allowed by CORS - origin:${origin}`));
        }
    },
};


// Get the supported languages by looking for translations in the `lang/` dir.
const languages = glob.sync("./static/locale/*.json").map(f => basename(f, ".json"));

// We need to expose React Intl's locale data on the request for the user's
// locale. This function will also cache the scripts by lang in memory.
const localeDataCache = new Map();
const getLocaleDataScript = (locale) => {
    const lang = locale.split("-")[0];
    if (!localeDataCache.has(lang)) {
        const localeDataFile = require.resolve(`react-intl/locale-data/${lang}`);
        const localeDataScript = readFileSync(localeDataFile, "utf8");
        localeDataCache.set(lang, localeDataScript);
    }
    return localeDataCache.get(lang);
};

// We need to load and expose the translations on the request for the user's
// locale. These will only be used in production, in dev the `defaultMessage` in
// each message description in the source code will be used.
const getMessages = locale => require(`./static/locale/${locale}.json`);


const reactIntlLocaleMessages = function (req, res, next) {
    const accept = accepts(req);
    const locale = accept.language(languages) || "en";
    req.locale = locale;
    req.localeDataScript = getLocaleDataScript(locale);
    req.messages = getMessages(locale);
    next();
};

const createServer = () => {
    const server = express();


    // Compressing all assets in dev slows things down.
    // Only use this in production if your assets are cdn cached and proxy doesn't do br natively
    // if (!dev) {
    //    server.use(shrinkRay());
    // }

    // It is important to have real cors value so service worker caches proper response code
    // server.use(cors(corsOptions));
    // In real production it is important to use the above with a proper whitelist but for demo we will allow *
    server.use(cors());

    server.use(helmet()); // Basic best practice security settings
    server.use(helmet.dnsPrefetchControl({allow: true})); // Performance desired in this case
    server.use(helmet.hsts({includeSubDomains: false})); // Lets not force our summary domain to https

    // https://github.com/zeit/next.js/pull/5727#issuecomment-441181614
    // Must only use on node 8 for now ... https://github.com/zeit/next.js/pull/5727#issuecomment-443279483
    server.use(Sentry.Handlers.requestHandler());

    server.use(cookieParser());
    server.use((req, res, next) => {
        const htmlPage = !req.path.match(/^\/(_next|static)/)
            && !req.path.match(/\.(js|map)$/)
            && req.accepts("text/html", "text/css", "image/png") === "text/html";

        if (!htmlPage) {
            next();
            return;
        }

        if (!req.cookies.sid || req.cookies.sid.length === 0) {
            req.cookies.sid = uuidv4();
            res.cookie("sid", req.cookies.sid);
        }

        next();
    });

    // In production we don't want to serve sourcemaps for anyone
    if (!dev) {
        const hasSentryToken = !!process.env.SENTRY_TOKEN;
        server.get(/\.map$/, (req, res, next) => {
            if (hasSentryToken && req.headers["x-sentry-token"] !== process.env.SENTRY_TOKEN) {
                res
                    .status(401)
                    .send(
                        "Authentication access token is required to access the source map.",
                    );
                return;
            }
            next();
        });
    }

    // The error handler must be before any other error middleware
    // Unfortunately Next error handler is in front blocking this but that is being looked into.
    // https://github.com/zeit/next.js/pull/5727#issuecomment-441181614
    // Must only use on node 8 for now ... https://github.com/zeit/next.js/pull/5727#issuecomment-443279483
    server.use(Sentry.Handlers.errorHandler());

    server.options("*", cors()); // include before other routes

    server.use("/health", expressHealthcheck());

    // Handle the silly browsers that ignore manifest and meta tags and look here
    server.get("/favicon.ico", (req, res) => {
        res.set("Cache-Control", "public,max-age=31536000,immutable"); // files from static need explicit cache control
        app.serveStatic(req, res, path.resolve("./static/icons/favicon.ico"));
    });

    // manifest needs to be served locally due to relative start_url
    server.get("/static/manifest-v1.json", (req, res) => {
        res.set("Cache-Control", "public,max-age=31536000,immutable"); // files from static need explicit cache control
        res.set("Content-Type", "application/json");
        app.serveStatic(req, res, path.resolve("./static/manifest-v1.json"));
    });

    /*
    // Local fonts served immutable - usually these would be served from cdn
    // just here for convenience during demo
    // if (!dev) {
    server.get(
        /^\/static\/fonts\//,
        (_, res, handle) => {
            res.setHeader(
                "Cache-Control",
                "public, max-age=31536000, immutable",
            );
            handle();
        },
    );
    // }
    */

    // Service worker file gets created by next-offline
    // If you test in production mode, remember to manually unregister the production service worker after
    server.get("/service-worker.js", (req, res) => {
        // Don't cache service worker is a best practice (otherwise clients wont get emergency bug fix)
        res.set("Cache-Control", "no-store, no-cache, must-revalidate, proxy-revalidate");
        res.set("Content-Type", "application/javascript");
        // prefix manifest precache links with cdn so they are not downloaded twice by the browser from different places
        // fix links with windows style slashes so windows can local test (todo: replace with normalize-paths)
        res.send(
            Buffer.from(fs.readFileSync("./.next/service-worker.js", "utf8").toString()
                .replace(new RegExp("\"url\": \"/_next", "g"), `"url": "${cdnPrefix}/_next`)
                .replace(new RegExp("\"url\": \"static", "g"), `"url": "${cdnPrefix}/static`)
                .replace(new RegExp("\\\\\\\\", "g"), "/")),
        );

        app.serveStatic(req, res, path.resolve("./.next/service-worker.js"));
    });

    const robotsOptions = {
        root: `${__dirname}/static/`,
        headers: {
            "Content-Type": "text/plain;charset=UTF-8",
        },
    };
    server.get("/robots.txt", (req, res) => {
        if (env === "production") {
            res.status(200).sendFile("robots.txt", robotsOptions);
        } else {
            res.status(200).sendFile("robots-dev.txt", robotsOptions);
        }
    });


    // Put language and messages for react intl
    server.use(reactIntlLocaleMessages);

    server.get("*", (req, res) => {
        // TODO: figure out how to add "no-cache" to only text/html pages so we can put CloudFront CDN
        // in front of our entire domain. This would be useful in serverless for a few reasons:
        // redirect http -> https, enable the CloudFront WAF
        // if (!cachable) {
        // res.set("Cache-Control", "no-store, no-cache, must-revalidate, proxy-revalidate");
        // }
        handle(req, res);
    });


    return server;
};

const server = createServer();
if (!process.env.LAMBDA) {
    app.prepare()
        .then(() => {
            if (dev || process.env.LOCAL_ENV) {
                // Always support localhost as it is what people expect
                server.listen(port, (err) => {
                    if (err) throw err;
                    // eslint-disable-next-line
                    console.log(`> Ready on http://localhost:${port}`);
                });

                // This will do a one time certificate password request the first time you start.
                // It will also attempt to add your certificateFor domain to hosts.  If you
                // get access errors, add "www.myweb.com 127.0.0.1" to hosts file manually.
                // devcert.certificateFor("www.myweb.com", {installCertutil: true}).then((ssl) => {
                devcert.certificateFor("www.myweb.com", {installCertutil: true}).then((ssl) => {
                    https.createServer(ssl, server).listen(devSslPort, (err) => {
                        if (err) throw err;
                        // eslint-disable-next-line
                        console.log(`> Ready on https://www.myweb.com:${devSslPort}`);
                    });
                });
            } else {
                server.listen(port, (err) => {
                    if (err) throw err;
                    // eslint-disable-next-line
                    console.log(`> Ready on http://localhost:${port}`);
                });
            }
        });
}

exports.app = app;
exports.server = server;
