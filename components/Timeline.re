let component = ReasonReact.statelessComponent("Timeline");

let dot = [%raw {|'\u2022'|}];

let makeDot = header =>
  <div className="timeline-unit">
    {ReasonReact.string(header)}
    <br />
    <div className="timeline-dot"> {ReasonReact.string(dot)} </div>
  </div>;

let make = children => {
  ...component,
  render: _self =>
    /* Quick hack as this will be a more advanced component in production */
    <div className="d-flex justify-content-around">
      {makeDot("2018")}
      {makeDot("")}
      {makeDot("Q1 2019")}
      {makeDot("")}
      {makeDot("")}
      {makeDot("Q2")}
      {makeDot("")}
      {makeDot("")}
      {makeDot("Q3")}
      {makeDot("")}
      {makeDot("")}
      {makeDot("Q4")}
    </div>,
};
