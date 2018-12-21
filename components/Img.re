type retainedProps = {src: string};

let component = ReasonReact.statelessComponentWithRetainedProps("Img");

let replace = str => Js.String.replace("https://", "", Js.String.replace("http://", "", str));
/*
 Removing https to work around thumbor image bug in nginx.
 TODO: full real solution is to make thumbor have its own cdn so we dont do the /thumbor path which screws stuff up
 */
let make = (~style=?, ~src, ~width, ~height, ~alt, _children) => {
  ...component,
  retainedProps: {
    src: src,
  },
  shouldUpdate: ({oldSelf, newSelf}) => oldSelf.retainedProps.src !== newSelf.retainedProps.src,
  render: _self =>
    <img src height={string_of_int(height)} width={string_of_int(width)} alt crossOrigin="anonymous" ?style />,
};
