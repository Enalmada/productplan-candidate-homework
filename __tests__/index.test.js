/* eslint-env jest */

import React from "react";
import {MockedProvider} from "react-apollo/test-utils";
import createComponentWithPrerequisites from "../util/test/createComponentWithPrerequisites";
import App from "../pages/index.bs";
import "babel-polyfill";

// The component AND the query need to be exported
const mocks = [];


describe("With Snapshot Testing", () => {
    it("Index matches expected snapshot", () => {
        const component = createComponentWithPrerequisites(<MockedProvider mocks={mocks} addTypename={false}>
            <App/></MockedProvider>);
        const tree = component.toJSON();
        expect(tree).toMatchSnapshot();
    });
});
