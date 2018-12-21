module DragDropContext = {
  [@bs.module "react-beautiful-dnd"] external reactClass: ReasonReact.reactClass = "DragDropContext";

  [@bs.obj] external makeProps: (~onDragEnd: unit => unit=?, unit) => _ = "";

  let make = (~onDragEnd=?, children) =>
    ReasonReact.wrapJsForReason(~reactClass, ~props=makeProps(~onDragEnd?, ()), children);
};
