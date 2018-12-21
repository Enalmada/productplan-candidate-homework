let component = ReasonReact.statelessComponent("BarPopover2");

let make = (~handleClick, children) => {
  ...component,
  render: _self =>
    <div style={ReactDOMRe.Style.make(~maxWidth="391px", ())}>
      /* image ultimately served from cdn and processed with browser specific optimization like webp */

        <div className="bold-header popover-header"> <M id="addbutton.AddBar.popup2.header" /> </div>
        <div> <M id="addbutton.AddBar.popup.body1" /> </div>
        <div className="popover-button-gotit"> <Button onClick=handleClick> <M id="GotIt" /> </Button> </div>
      </div>,
};
