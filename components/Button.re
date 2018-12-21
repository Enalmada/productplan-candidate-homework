let component = ReasonReact.statelessComponent("LanePopover");

let make = (~onClick, children) => {
  ...component,
  render: _self =>
    <Antd_Button _type=`primary onClick style={ReactDOMRe.Style.make(~paddingLeft="35px", ~paddingRight="35px", ())}>
      ...children
    </Antd_Button>,
};
