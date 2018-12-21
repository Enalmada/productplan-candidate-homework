/* eslint camelcase: "off", react/jsx-no-undef: 0,  no-unused-vars: 0, no-console: 0, no-underscore-dangle: 0 */
import App, {Container} from "next/app";
import React from "react";
import {ApolloProvider} from "react-apollo";
import NextSeo from "next-seo";
import {LocaleProvider} from "antd";
import en_US from "antd/lib/locale-provider/en_US";

import {config, library as fontawesome} from "@fortawesome/fontawesome-svg-core";
import {faSearch, faSortDown} from "@fortawesome/free-solid-svg-icons";
import "@fortawesome/fontawesome-svg-core/styles.css";
import {UserAgentProvider} from "@quentin-sommer/react-useragent";
import {addLocaleData, IntlProvider} from "react-intl";
import withApollo from "../util/withApollo";
// import your default seo configuration
import SEO from "../next-seo.config";
import {captureException} from "../util/sentry";
import SiteContext from "../util/context";

config.autoAddCss = false;
fontawesome.add(faSearch, faSortDown);


// Register React Intl's locale data for the user's locale in the browser. This
// locale data was added to the page by `pages/_document.js`. This only happens
// once, on initial page load in the browser.
if (typeof window !== "undefined" && window.ReactIntlLocaleData) {
    Object.keys(window.ReactIntlLocaleData).forEach((lang) => {
        addLocaleData(window.ReactIntlLocaleData[lang]);
    });
}

class MyApp extends App {
    static async getInitialProps({Component, ctx}) {
        let pageProps = {};

        // https://github.com/zeit/next.js/pull/5727/files
        try {
            if (Component.getInitialProps) {
                pageProps = await Component.getInitialProps(ctx);
            }
        } catch (e) {
            captureException(e, ctx);
            throw e; // you can also skip re-throwing and set property on pageProps
        }

        const ua = ctx.req
            ? ctx.req.headers["user-agent"]
            : navigator.userAgent;


        // Get the `locale` and `messages` from the request object on the server.
        // In the browser, use the same values that the server serialized.
        const {req} = ctx;
        const {locale, messages} = req || window.__NEXT_DATA__.props;

        const iframe = (req && req.iframe === "true") || (process.browser && window.gell !== undefined);

        const isIframe = iframe ? "true" : "false";
        // console.log(`isIframe: ${isIframe}`);
        return {
            pageProps, locale, messages, ua, iframe,
        };
    }

    // This reports errors thrown while rendering components
    componentDidCatch(error, errorInfo) {
        captureException(error, {errorInfo});
        super.componentDidCatch(error, errorInfo);
    }

    render() {
        const {
            Component, pageProps, apolloClient, locale, messages, ua, iframe,
        } = this.props;
        const now = Date.now();

        return (
            <Container>
                <SiteContext.Provider value={{popoverTimeout: 1000}}>
                    <UserAgentProvider ua={ua}>
                        <IntlProvider locale={locale} messages={messages} initialNow={now}>
                            <LocaleProvider locale={en_US}>
                                <ApolloProvider client={apolloClient}>
                                    {/* Here we call NextSeo and pass our default configuration to it  */}
                                    <NextSeo config={SEO}/>
                                    <Component {...pageProps} />
                                </ApolloProvider>
                            </LocaleProvider>
                        </IntlProvider>
                    </UserAgentProvider>
                </SiteContext.Provider>
            </Container>
        );
    }
}

export default withApollo(MyApp);
