[@bs.deriving jsConverter]
type navValue = [
  | [@bs.as "roadmap"] `roadmap
  | [@bs.as "planningBoard"] `planningBoard
  | [@bs.as "parkingLot"] `parkingLot
];
