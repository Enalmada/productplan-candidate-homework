type bar = {
  id: int,
  x: int,
  y: int,
};

type state = {
  bars: list(bar),
  barNumber: int,
  barLines: int,
};

type action =
  | AddBar(bar)
  | SetBars(list(bar))
  | IncrementBar
  | IncrementLine;

let component = ReasonReact.reducerComponent("Lane");

let dot = [%raw {|'\u2022'|}];

let makeLine = header => <div className="bar-unit"> <div className="timeline-line" /> </div>;

let listElements = bars => {
  ReasonReact.array(
    Array.of_list(
      List.map(
        item =>
          <div
            key={string_of_int(item.id)}
            className="lane-bar-wrapper d-flex"
            style={ReactDOMRe.Style.make(
              ~position="fixed",
              ~top=string_of_int(item.y) ++ "px",
              ~left=string_of_int(item.x) ++ "px",
              ~zIndex="10",
              (),
            )}>
            <div
              className="lane-bar"
              draggable=true
              onDragStart={event => {
                /*
                  let dict = Js.Dict.empty();
                                 Js.Dict.set(dict, "dragtype", "existingbar");
                                 Js.Dict.set(dict, "id", string_of_int(item.id));

                                 %bs.raw
                                 {| event.dataTransfer.setData("text", dict) |};
                                 Js.log("dropped " ++ string_of_int(item.id));
                 */

                let itemId = item.id;
                Js.log(itemId);
                %bs.raw
                {| event.dataTransfer.setData("text", JSON.stringify({"dragtype": "existingbar", "id": itemId})) |};
                Js.log("dropped " ++ string_of_int(item.id));
              }}>
              {ReasonReact.string("Roadmap Item " ++ string_of_int(item.id))}
            </div>
          </div>,
        bars,
      ),
    ),
  );
};

let make = (~handleSecondBarPopover, children) => {
  ...component,
  initialState: () => {bars: [], barNumber: 0, barLines: 0},
  reducer: (action, state) =>
    switch (action) {
    | AddBar(bar) => ReasonReact.Update({...state, bars: List.append(state.bars, [bar])})
    | SetBars(bars) => ReasonReact.Update({...state, bars})

    | IncrementBar => ReasonReact.Update({...state, barNumber: state.barNumber + 1})
    | IncrementLine => ReasonReact.Update({...state, barLines: state.barLines + 1})
    },
  render: self =>
    <SiteContext.Consumer>
      ...{context =>
        <div className="show-lane">
          <div className="show-lane-top d-flex justify-content-start">
            <FontAwesomeIcon icon=["fas", "sort-down"] style={ReactDOMRe.Style.make(~fontSize="1em", ())} />
            <span className="show-lane-header "> <M id="lanes.Lane" /> </span>
          </div>
          <div
            className="show-lane-bottom d-flex"
            style={ReactDOMRe.Style.make(~height=string_of_int(70 * (self.state.barLines + 1)) ++ "px", ())}
            onDragOver={e => ReactEvent.Mouse.preventDefault(e)}
            onDrop={event => {
              Js.Global.setTimeout(() => handleSecondBarPopover(true), context##popoverTimeout) |> ignore;
              Js.log("dropped");
              let data = [%bs.raw {|  JSON.parse(event.dataTransfer.getData("text")) |}];
              Js.log(data);
              let x = [%bs.raw {|  event.clientX - 75 |}];
              let y = [%bs.raw {|  event.clientY - 25 |}];
              Js.log("x: " ++ string_of_int(x) ++ " y: " ++ string_of_int(y));
              if (data##dragtype == "newbar") {
                self.send(IncrementBar);
                let newbar: bar = {id: self.state.barNumber + 1, x, y};
                self.send(AddBar(newbar));

                if (self.state.barLines == 0) {
                  self.send(IncrementLine);
                };
                /* self.send(ShowBar(true)); */
              } else {
                Js.log("data##id:" ++ data##id);
                let newList = List.filter(f => f.id != data##id, self.state.bars);
                let newbar: bar = {id: data##id, x, y};
                self.send(SetBars(List.append(newList, [newbar])));
              };
            }}>
            {listElements(self.state.bars)}
            /*
             {self.state.showBar ?
                <div className="lane-bar-wrapper d-flex">
                  <div
                    className="lane-bar"
                    draggable=true
                    onDragStart={event => {
                      %bs.raw
                      {| event.dataTransfer.setData("text", JSON.stringify({"dragtype": "existingbar"})) |};
                      Js.log("dropped");
                    }}>
                    {ReasonReact.string("Roadmap Item " ++ string_of_int(self.state.barNumber))}
                  </div>
                </div> :
                ReasonReact.null}
                */
            <div className="d-flex justify-content-around lines-container">
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
              {makeLine("")}
            </div>
          </div>
        </div>
      }
    </SiteContext.Consumer>,
};
