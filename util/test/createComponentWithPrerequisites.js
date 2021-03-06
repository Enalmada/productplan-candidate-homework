// This file

// https://www.apollographql.com/docs/react/recipes/testing.html
// https://github.com/quentin-sommer/react-useragent/blob/master/tests/index-test.js
// https://github.com/yahoo/react-intl/wiki/Testing-with-React-Intl#jest

import React from "react";
import renderer from "react-test-renderer";
import {IntlProvider} from "react-intl";
import {UserAgentProvider} from "@quentin-sommer/react-useragent";
import "babel-polyfill";

import Router from "next/router";

import {config, library as fontawesome} from "@fortawesome/fontawesome-svg-core";
import {fas} from "@fortawesome/free-solid-svg-icons";
import "@fortawesome/fontawesome-svg-core/styles.css";
import SiteContext from "../context";

const mockedRouter = {push: () => {}, prefetch: () => {}};
Router.router = mockedRouter;

config.autoAddCss = false;
fontawesome.add(fas);

const createComponentWithPrerequisites = (children, props = {locale: "en"}) => renderer.create(
    <SiteContext.Provider value={{popoverTimeout: 1000}}>
        <UserAgentProvider
            ua="Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.71 Safari/537.36 Edge/12.0">
            <IntlProvider {...props}>
                {children}
            </IntlProvider>
        </UserAgentProvider>
    </SiteContext.Provider>,
);

export default createComponentWithPrerequisites;
