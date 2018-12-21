type state = {lanePopoverOpen: bool};

type action =
  | LanePopoverOpen(bool);

let component = ReasonReact.reducerComponent("DragButtons");
let close = [%raw {|'&times;'|}];
let popoverPadding = "15px";

let draggableButtonWithPopup = (key, visible, handleVisible, content, onDragStart) => {
  <ReactIntl.IntlInjector>
    ...{intl =>
      <li key>
        <DragButtonPopover visible handleVisible content>
          <div style={ReactDOMRe.Style.make(~paddingLeft=popoverPadding, ())}>
            <DragButton onDragStart title={intl.formatMessage({"id": key, "defaultMessage": "Undefined"})} />
          </div>
        </DragButtonPopover>
      </li>
    }
  </ReactIntl.IntlInjector>;
};

let draggableButtonWithTwoPopup =
    (key, oneVisible, oneHandleVisible, oneContent, twoVisible, twoHandleVisible, twoContent, onDragStart) => {
  <ReactIntl.IntlInjector>
    ...{intl =>
      <li key>
        <DragButtonPopover visible=oneVisible handleVisible=oneHandleVisible content=oneContent>
          <DragButtonPopover visible=twoVisible handleVisible=twoHandleVisible content=twoContent>
            <div style={ReactDOMRe.Style.make(~paddingLeft=popoverPadding, ())}>
              <DragButton onDragStart title={intl.formatMessage({"id": key, "defaultMessage": "Undefined"})} />
            </div>
          </DragButtonPopover>
        </DragButtonPopover>
      </li>
    }
  </ReactIntl.IntlInjector>;
};

let make =
    (
      ~showFirstBarPopover,
      ~handleFirstBarPopover,
      ~showSecondBarPopover,
      ~handleSecondBarPopover,
      ~handleLaneDrag,
      ~handleBarDrag,
      ~timeout,
      children,
    ) => {
  ...component,
  initialState: () => {lanePopoverOpen: false},
  reducer: (action, state) =>
    switch (action) {
    | LanePopoverOpen(lanePopoverOpen) => ReasonReact.Update({...state, lanePopoverOpen})
    },
  didMount: self => {
    Js.Global.setTimeout(() => self.send(LanePopoverOpen(true)), timeout) |> ignore;
  },
  render: self =>
    <div className="dragging d-flex">
      <ul className="dragbuttons-list">
        {draggableButtonWithPopup(
           "addbutton.AddLane",
           self.state.lanePopoverOpen,
           _event => self.send(LanePopoverOpen(false)),
           {<LanePopover handleClick={_event => self.send(LanePopoverOpen(false))} />},
           handle => handleLaneDrag(true),
         )}
        <li key="spacer" style={ReactDOMRe.Style.make(~height="5px", ())} />
        {draggableButtonWithTwoPopup(
           "addbutton.AddBar",
           showFirstBarPopover,
           _event => handleFirstBarPopover(false),
           {<BarPopover handleClick={_event => handleFirstBarPopover(false)} />},
           showSecondBarPopover,
           _event => handleSecondBarPopover(false),
           {<BarPopover2 handleClick={_event => handleSecondBarPopover(false)} />},
           handle => handleBarDrag(true),
         )}
      </ul>
    </div>,
};
