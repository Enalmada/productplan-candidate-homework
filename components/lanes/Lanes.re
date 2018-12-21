type state = {dropHere: bool};

type action =
  | DropHere(bool);

let component = ReasonReact.reducerComponent("Timeline");

let make =
    (
      ~handleFirstBarPopover,
      ~handleSecondBarPopover,
      ~laneDrag: bool,
      ~showLane: bool,
      ~handleLaneDrag,
      ~handleShowLane,
      children,
    ) => {
  ...component,
  initialState: () => {dropHere: false},
  reducer: (action, state) =>
    switch (action) {
    | DropHere(dropHere) => ReasonReact.Update({...state, dropHere})
    },
  render: self =>
    <SiteContext.Consumer>
      ...{context =>
        <div>
          {showLane ? <Lane handleSecondBarPopover /> : ReasonReact.null}
          {laneDrag ?
             <div
               className="drop-here"
               onDragOver={e => ReactEvent.Mouse.preventDefault(e)}
               onDrop={e => {
                 let data = [%bs.raw {|  JSON.parse(event.dataTransfer.getData("text")) |}];
                 Js.log(data##dragtype);

                 handleLaneDrag(false);
                 handleShowLane(true);
                 Js.Global.setTimeout(() => handleFirstBarPopover(true), context##popoverTimeout) |> ignore;
               }}>
               <span className="task-header"> <M id="lanes.DropHere" /> </span>
             </div> :
             ReasonReact.null}
        </div>
      }
    </SiteContext.Consumer>,
};
