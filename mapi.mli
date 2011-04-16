type t

type handle

type lang = SQL | MAL

type msg = 
    MOK
  | MERROR
  | MTIMEOUT
  | MMORE
  | MSERVER

type whence =
  | SEEK_SET
  | SEEK_CUR
  | SEEK_END

val connect : ?host:string -> ?port:int -> ?user:string -> ?passwd:string -> lang:lang -> db:string -> t
val destroy : t -> msg
val disconnect : t -> msg
val reconnect : t -> msg
val ping : t -> msg

val error : t -> msg

val query : t -> string -> handle option
val close_handle : handle -> msg

val get_field_count : handle -> int
val get_row_count : handle -> int64
val rows_affected : handle -> int64

val fetch_row : handle -> int option
val fetch_all_rows : handle -> int64 option
val seek_row : handle -> int64 -> whence -> msg
val fetch_field : handle -> int -> string option

