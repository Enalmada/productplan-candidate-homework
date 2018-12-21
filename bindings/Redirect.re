[@bs.module "../util/redirect"] external redirect: ('a, string) => unit = "default";
[@bs.module "../util/redirect"] external redirectWithCode: ('a, string, int) => unit = "default";

/*
 Avoids this nonsense
  %bs.raw
  {|
   require("../util/redirect").default(context, '/') |};
   */
