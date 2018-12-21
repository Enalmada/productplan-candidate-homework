open ReactIntl;

let component = ReasonReact.statelessComponent("M");

let make = (~id, ~defaultMessage=?, ~values=?, _children) => {
  ...component,
  render: _self =>
    switch (defaultMessage) {
    | None => <FormattedMessage id defaultMessage="Undefined" />
    | Some(nonOptionalMessage) => <FormattedMessage id defaultMessage=nonOptionalMessage ?values />
    },
};
