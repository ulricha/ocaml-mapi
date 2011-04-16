type t

type handle

type lang = SQL | MAL

type msg = 
    MOK
  | MERROR
  | MTIMEOUT
  | MMORE
  | MSERVER

let string_of_lang = function
  | SQL -> "sql"
  | MAL -> "mal"

(* mapi_connect *)
external raw_connect : string -> int -> string -> string -> string -> string -> t = "mapi_connect_stub_bc" "mapi_connect_stub_native"

let connect ?(host="localhost") ?(port=50000) ?(user="monetdb") ?(passwd="monetdb") ~lang ~db =
  raw_connect host port user passwd (string_of_lang lang) db

external error : t -> msg = "mapi_error_stub"
external query : t -> string -> handle option = "mapi_query_stub"
external fetch_row : handle -> int option = "mapi_fetch_row_stub"
external fetch_field : handle -> int -> string option = "mapi_fetch_field_stub"
external close_handle : handle -> msg = "mapi_close_handle_stub"
external destroy : t -> msg = "mapi_destroy_stub"

