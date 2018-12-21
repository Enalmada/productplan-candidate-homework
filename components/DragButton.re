let component = ReasonReact.statelessComponent("DragButtons");
/* https://medium.freecodecamp.org/reactjs-implement-drag-and-drop-feature-without-using-external-libraries-ad8994429f1a */
let make = (~title, ~onDragStart, children) => {
  ...component,
  render: _self =>
    <div
      onDragStart={onDragStart(true)}
      draggable=true
      style={ReactDOMRe.Style.make(~padding="2px", ~backgroundColor="white", ~borderRadius="4px", ())}>
      <Antd_Button
        style={ReactDOMRe.Style.make(
          ~color="#73777B",
          ~backgroundColor="#E4E6E8",
          ~paddingLeft="7px",
          ~borderColor="transparent",
          ~width="100%",
          ~textAlign="left",
          (),
        )}>
        <img
          src="/static/images/expand2.png"
          style={ReactDOMRe.Style.make(~paddingRight="7px", ~marginTop="-2px", ())}
        />
        {ReasonReact.string(title)}
      </Antd_Button>
    </div>,
};
