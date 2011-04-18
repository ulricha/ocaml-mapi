type t

type handle

type lang = SQL | MAL

type msg = 
  | MOK
  | MERROR
  | MTIMEOUT
  | MMORE
  | MSERVER

type whence =
  | SEEK_SET
  | SEEK_CUR
  | SEEK_END

let string_of_lang = function
  | SQL -> "sql"
  | MAL -> "mal"

(* connection handling *)
external raw_connect : string -> int -> string -> string -> string -> string -> t option = "mapi_connect_stub_bc" "mapi_connect_stub_native"
external destroy : t -> msg = "mapi_destroy_stub"
external disconnect : t -> msg = "mapi_disconnect_stub"
external reconnect : t -> msg = "mapi_reconnect_stub"
external ping : t -> msg = "mapi_ping_stub"

let connect ?(host="localhost") ?(port=50000) ?(user="monetdb") ?(passwd="monetdb") ~lang ~db =
  raw_connect host port user passwd (string_of_lang lang) db

external error : t -> msg = "mapi_error_stub"
external error_str : t -> string option = "mapi_error_str_stub"
external result_error : handle -> string option = "mapi_result_error_stub"

(* query functions *)
external query : t -> string -> handle option = "mapi_query_stub"

(* fetch results *)
external get_field_count : handle -> int = "mapi_get_field_count_stub"
external get_row_count : handle -> int64 = "mapi_get_row_count_stub"
external rows_affected : handle -> int64 = "mapi_rows_affected_stub"

external fetch_row : handle -> int option = "mapi_fetch_row_stub"
external fetch_all_rows : handle -> int64 option = "mapi_fetch_all_rows_stub"
external seek_row : handle -> int64 -> whence -> msg = "mapi_seek_row_stub"
external fetch_field : handle -> int -> string option = "mapi_fetch_field_stub"
external fetch_field_list : handle -> string list option = "mapi_fetch_field_list_stub"
external close_handle : handle -> msg = "mapi_close_handle_stub"

external quote : string -> string = "mapi_quote_stub"
external unquote : string -> string = "mapi_unquote_stub"

external field_name : handle -> int -> string option = "mapi_get_name_stub"

let connection_ok conn =
  match error conn with
    | MOK -> true
    | _ -> false 
