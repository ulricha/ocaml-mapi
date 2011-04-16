type t

type handle

type lang = SQL | MAL

type msg = 
    MOK
  | MERROR
  | MTIMEOUT
  | MMORE
  | MSERVER

val connect : ?host:string -> ?port:int -> ?user:string -> ?passwd:string -> lang:lang -> db:string -> t
val error : t -> msg
val query : t -> string -> handle option
val fetch_row : handle -> int option
val fetch_field : handle -> int -> string option
val close_handle : handle -> msg
val destroy : t -> msg
