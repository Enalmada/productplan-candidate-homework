let component = ReasonReact.statelessComponent("DrapButtonPopover");

let make = (~visible, ~handleVisible, ~content, children) => {
  ...component,
  render: self =>
    <Antd_Popover
      overlayClassName="dragging-popover"
      content
      placement=`leftTop
      title={
        <div className="width-100">
          <a onClick={_event => handleVisible(false)} className="close">
            <span dangerouslySetInnerHTML={"__html": "&times;"} />
          </a>
        </div>
      }
      trigger=`click
      visible>
      ...children
    </Antd_Popover>,
};
