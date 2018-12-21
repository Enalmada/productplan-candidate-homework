type state = {
  laneDrag: bool,
  barDrag: bool,
  showLane: bool,
  showFirstBarPopover: bool,
  showSecondBarPopover: bool,
  shownSecondBarPopover: bool,
};

type action =
  | LaneDrag(bool)
  | BarDrag(bool)
  | ShowLane(bool)
  | ShowFirstBarPopover(bool)
  | ShowSecondBarPopover(bool);

let component = ReasonReact.reducerComponent("Index");

let make = _children => {
  ...component,
  initialState: () => {
    laneDrag: false,
    barDrag: false,
    showLane: false,
    showFirstBarPopover: false,
    showSecondBarPopover: false,
    shownSecondBarPopover: false,
  },
  reducer: (action, state) =>
    switch (action) {
    | LaneDrag(laneDrag) => ReasonReact.Update({...state, laneDrag})
    | BarDrag(barDrag) => ReasonReact.Update({...state, barDrag})
    | ShowLane(showLane) => ReasonReact.Update({...state, showLane})
    | ShowFirstBarPopover(showFirstBarPopover) => ReasonReact.Update({...state, showFirstBarPopover})
    | ShowSecondBarPopover(showSecondBarPopover) =>
      ReasonReact.Update({
        ...state,
        showSecondBarPopover: showSecondBarPopover && state.shownSecondBarPopover === false,
        shownSecondBarPopover: true,
      })
    },
  render: self =>
    <SiteContext.Consumer>
      ...{context =>
        <ReactIntl.IntlInjector>
          ...{intl =>
            <Layout
              nav=`roadmap
              header={intl.formatMessage({"id": "header.roadmap", "defaultMessage": "Candidate Roadmap"})}>
              <NextSeo title={intl.formatMessage({"id": "meta.title", "defaultMessage": "Candidate Roadmap"})} />
              <div className="d-flex justify-content-between main-content-wrapper">
                <div className="timelines">
                  <Timeline />
                  <Lanes
                    laneDrag={self.state.laneDrag}
                    showLane={self.state.showLane}
                    handleLaneDrag={laneDrag => self.send(LaneDrag(laneDrag))}
                    handleShowLane={showLane => self.send(ShowLane(showLane))}
                    handleFirstBarPopover={showFirstBarPopover =>
                      self.send(ShowFirstBarPopover(showFirstBarPopover))
                    }
                    handleSecondBarPopover={showSecondBarPopover =>
                      self.send(ShowSecondBarPopover(showSecondBarPopover))
                    }
                  />
                </div>
                <DragButtons
                  showFirstBarPopover={self.state.showFirstBarPopover}
                  handleFirstBarPopover={showFirstBarPopover => self.send(ShowFirstBarPopover(showFirstBarPopover))}
                  showSecondBarPopover={self.state.showSecondBarPopover}
                  handleSecondBarPopover={showSecondBarPopover =>
                    self.send(ShowSecondBarPopover(showSecondBarPopover))
                  }
                  handleLaneDrag={(laneDrag, event) => {
                    self.send(LaneDrag(laneDrag));

                    %bs.raw
                    {| event.dataTransfer.setData("text", JSON.stringify({"dragtype": "newlane"})) |};
                  }}
                  handleBarDrag={(barDrag, _event) => {
                    self.send(BarDrag(barDrag));
                    %bs.raw
                    {| event.dataTransfer.setData("text", JSON.stringify({"dragtype": "newbar"})) |};
                  }}
                  timeout=context##popoverTimeout
                />
              </div>
            </Layout>
          }
        </ReactIntl.IntlInjector>
      }
    </SiteContext.Consumer>,
};

/* Convert reason-react element to js page for Next.js */
let default = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));
