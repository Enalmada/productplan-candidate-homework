let component = ReasonReact.statelessComponent("ConsumerPage");
open Antd;

[%bs.raw {| require("../../../assets/styles.styl") |}];
[%bs.raw {| require("../../../assets/productplan.styl") |}];

/* https://stackoverflow.com/questions/49039433/how-to-add-a-copyright-symbol-in-reason-react-component */
let copy = [%raw {|'\u00a9'|}];
let linkStyles = ReactDOMRe.Style.make(~marginRight="10px", ());

let make = (~nav: NavValues.navValue, ~header: string, children) => {
  ...component,
  render: _self =>
    <Layout>
      <Layout.Header> <HomeworkHeader nav header /> </Layout.Header>
      <Layout.Content> ...children </Layout.Content>
      <Layout.Footer />
    </Layout>,
};
