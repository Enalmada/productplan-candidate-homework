let component = ReasonReact.statelessComponent("BarPopover");

let make = (~handleClick, children) => {
  ...component,
  render: _self =>
    <div style={ReactDOMRe.Style.make(~maxWidth="391px", ())}>
      /* image ultimately served from cdn and processed with browser specific optimization like webp */

        <img src="/static/images/barPopoverOne.png" className="popover-image" />
        <div className="bold-header popover-header"> <M id="addbutton.AddBar.popup.header" /> </div>
        <div> <M id="addbutton.AddBar.popup.body1" /> </div>
        <div style={ReactDOMRe.Style.make(~marginTop="5px", ())}> <M id="addbutton.AddBar.popup.body2" /> </div>
        <div className="popover-button-gotit"> <Button onClick=handleClick> <M id="GotIt" /> </Button> </div>
      </div>,
};
