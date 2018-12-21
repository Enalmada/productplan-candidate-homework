/* eslint no-param-reassign: 0, global-require: 0, no-undef: 0, no-unused-vars: 0 */
const {CDN_URL} = process.env;
const withOffline = require("next-offline");
const {
    PHASE_DEVELOPMENT_SERVER, PHASE_PRODUCTION_BUILD,
} = require("next/constants");
const path = require("path");
const fs = require("fs");
const nib = require("nib"); // TODO: this should be only used during dev/build
// const TargetsPlugin = require("targets-webpack-plugin");
const webpack = require("webpack");

const {ANALYZE} = process.env;

// styled jsx will fail without this hack
if (typeof require !== "undefined") {
    require.extensions[".less"] = () => {};
    require.extensions[".css"] = () => {};
}


const nextConfig = {
    // Will be available on both server and client
    publicRuntimeConfig: {
        GRAPHQL_API: process.env.GRAPHQL_API,
        SENTRY_DSN: process.env.SENTRY_DSN,
        ENV: process.env.ENV,
    },
    // Allow us to compile reason-react to es6 which is a bit more modern and readable
    transpileModules: [
        "bs-platform",
        "reason-react",
        "reason-apollo",
        "bs-ant-design-alt",
        "bs-ant-design-mobile",
        "bs-css",
        "bs-fontawesome",
        "bs-react-useragent",
        "bs-next-seo",
        "bs-react-intl",
        "bs-next-alt"],
    // Remove the unused css that we should have cleaned up manually but didn't
    purgeCss: {
        whitelist: ["ant-layout"],
        whitelistPatterns: [/^ant-/, /^fade-/, /^move-/, /^slide-/, /^zoom-/, /^svg-/, /^fa-/],
        whitelistPatternsChildren: [/^ant-/, /^fade-/, /^move-/, /^slide-/, /^zoom-/, /^svg-/, /^fa-/],
    },
    // Gives stylus some extra power
    stylusLoaderOptions: {
        use: [nib()],
    },
    // devSwSrc: 'static/js/service-worker.js',
    poweredByHeader: false,
    // Prefix our static assets with cdn url for optimal performance
    assetPrefix: CDN_URL ? `${CDN_URL}` : "",
    pageExtensions: ["jsx", "js", "bs.js"],
    lessLoaderOptions: {
        javascriptEnabled: true,
        // theme antd here
        modifyVars: {"@primary-color": "#1973BA"},
    },
    workboxOpts: {
        runtimeCaching: [
            {
                urlPattern: /^https:\/\/fonts\.(googleapis|gstatic)\.com/,
                handler: "cacheFirst",
                options: {
                    cacheName: "google-fonts-stylesheets",
                    expiration: {maxEntries: 100, maxAgeSeconds: 60 * 60 * 24 * 30},
                    cacheableResponse: {
                        statuses: [0, 200],
                    },
                },
            },
            {urlPattern: /^https?.*/, handler: "networkFirst"},
        ],
        // Add extra stuff to service worker like push notifications
        // importScripts: ['static/js/service-worker-extras.js'],
        skipWaiting: true,
        clientsClaim: true,
    },
    webpack: (config, {dev, isServer, buildId}) => {
        // Experimental plugin to ensure site is spiderable in googlebot
        // https://github.com/zeit/next.js/pull/5727#issuecomment-440795436
        // https://github.com/zeit/next.js/issues/3205#issuecomment-384673971
        // possibly https://github.com/zeit/next.js/pull/3181#issuecomment-393643297
        // Note: may need to build with "cross-env NODE_OPTIONS=--max_old_space_size=4096 next build"
        /*
        if (!dev) {
            config.plugins.push(new TargetsPlugin({
                browsers: ["last 2 versions", "chrome >= 41"]
            }))
        }
        */


        // https://github.com/zeit/next.js/pull/5727/files
        if (!dev) {
            config.plugins.push(
                new webpack.DefinePlugin({
                    "process.env.SENTRY_RELEASE": JSON.stringify(buildId),
                }),
            );
        }

        if (!isServer) {
            config.resolve.alias["@sentry/node"] = "@sentry/browser";
        }


        // To make sure nothing huge got into our code splitting
        if (ANALYZE) {
            const {BundleAnalyzerPlugin} = require("webpack-bundle-analyzer");

            config.plugins.push(new BundleAnalyzerPlugin({
                analyzerMode: "server",
                analyzerPort: isServer ? 8888 : 8889,
                openAnalyzer: true,
            }));
        }

        return config;
    },
};
//  apparently next-less, next-css, etc only need require duing dev/build
// https://github.com/zeit/next.js/issues/4248#issuecomment-386038283
// Obviously having css, less, and stylus at same time is overkill but there were cases where I needed them
// TODO: nib should only be loaded when withStylus is loaded
module.exports = (phase) => {
    if (phase === PHASE_DEVELOPMENT_SERVER || phase === PHASE_PRODUCTION_BUILD) {
        const withLess = require("@zeit/next-less");
        const withCSS = require("@zeit/next-css");
        const withStylus = require("@zeit/next-stylus");
        const withPurgeCss = require("next-purgecss");
        const nextSourceMaps = require("@zeit/next-source-maps")();
        const withTM = require("next-plugin-transpile-modules");

        return withOffline(withStylus(withLess(withCSS(withPurgeCss(nextSourceMaps(withTM(nextConfig)))))));
    }
    return withOffline(nextConfig);
};
