let component = ReasonReact.statelessComponent("ConsumerHeader");
open ReactIntl;
let styles = ReactDOMRe.Style.make(~marginRight="10px", ());

let logoStyle =
  ReactDOMRe.Style.make(
    ~width="120px",
    ~height="14px",
    ~background="rgba(255,255,255,.2)",
    ~margin="16px 28px 16px 0",
    ~float="left",
    (),
  );
[@bs.val] external nodeEnv: string = "process.env.NODE_ENV";

let make = (~nav: NavValues.navValue, ~header: string, _children) => {
  ...component,
  render: _self =>
    <ReactIntl.IntlInjector>
      ...{intl =>
        <div>
          <div
            className="d-flex justify-content-between align-items-center header-one"
            style={ReactDOMRe.Style.make(~marginBottom="12px", ())}>
            <div className="d-flex justify-content-start align-items-center">
              <Link className="logo" href="/" title={intl.formatMessage({"id": "logo", "defaultMessage": "Logo"})}>
                <Img
                  src="/static/images/logohack.png"
                  height=14
                  width=120
                  alt={intl.formatMessage({"id": "logo", "defaultMessage": "ProductPlan Logo"})}
                  style={ReactDOMRe.Style.make(~marginRight="20px", ())}
                />
              </Link>
              <div style={ReactDOMRe.Style.make(~marginLeft="20px", ~fontSize="18px", ~color="black", ())}>
                {ReasonReact.string(header)}
              </div>
            </div>
            <FontAwesomeIcon icon=["fas", "search"] style={ReactDOMRe.Style.make(~fontSize="1.4em", ())} />
          </div>
          <div>
            <div
              className="d-flex justify-content-between align-items-center bold-header header-two"
              style={ReactDOMRe.Style.make(~padding="0 20px", ())}>
              <div style={ReactDOMRe.Style.make(~fontSize="32px", ~color="black", ~marginLeft="15px", ())}>
                <M id="header.ProductRoadmap" />
              </div>
              <Antd_Menu
                theme=`Light
                mode={nodeEnv == "test" ? `Vertical : `Horizontal}
                selectedKeys=[NavValues.navValueToJs(nav)]
                style={ReactDOMRe.Style.make(
                  ~display="inline-block",
                  ~marginBottom="-25px",
                  ~fontSize="16px",
                  ~color="#62666A",
                  ~lineHeight="45px",
                  ~marginRight="250px",
                  ~backgroundColor="transparent",
                  (),
                )}>
                <Antd_Menu.Item key="roadmap"> <Link href="/"> <M id="nav.roadmap" /> </Link> </Antd_Menu.Item>
                <Antd_Menu.Item key="planningBoard">
                  <Link href="/planningBoard"> <M id="nav.planningBoard" /> </Link>
                </Antd_Menu.Item>
                <Antd_Menu.Item key="parkingLot">
                  <Link href="/parkingLot"> <M id="nav.parkingLot" /> </Link>
                </Antd_Menu.Item>
              </Antd_Menu>
            </div>
          </div>
        </div>
      }
    </ReactIntl.IntlInjector>,
};
