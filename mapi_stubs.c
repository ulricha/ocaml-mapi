#include <stdio.h>
#include <assert.h>

#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/mlvalues.h>

#include <monetdb/mapi.h>

#define Val_none Val_int(0)

static inline value Val_some(value v)
{
    CAMLparam1(v);
    CAMLlocal1(some);
    some = caml_alloc(1, 0);
    Store_field(some, 0, v);
    CAMLreturn(some);
}

int index_of_msg(MapiMsg msg)
{
    int index = 0;

    switch (msg) {
    case MOK: index = 0; break;
    case MERROR: index = 1; break;
    case MTIMEOUT: index = 2; break;
    case MMORE: index = 3; break;
    case MSERVER: index = 4; break;
    }

    return index;
}

CAMLprim value mapi_connect_stub_native(value host, value port, value user, 
					value passwd, value lang, value db)
{
    const char *host_str = String_val(host);
    const int port_i = Int_val(port);
    assert(port_i > 0 && port_i < 65536);
    const char *user_str = String_val(user);
    const char *passwd_str = String_val(passwd);
    const char *lang_str = String_val(lang);
    const char *db_str = String_val(db);
    Mapi conn = mapi_connect(host_str, port_i, user_str, passwd_str, lang_str, db_str);
    if (conn) {
	return Val_some((value) conn);
    } else {
	return Val_none;
    }
}

CAMLprim value mapi_connect_stub_bc(value *argv, int argn)
{
    assert(argn == 6);
    const char *host_str = String_val(argv[0]);
    const int port_i = Int_val(argv[1]);
    assert(port_i > 0 && port_i < 65536);
    const char *user_str = String_val(argv[2]);
    const char *passwd_str = String_val(argv[3]);
    const char *lang_str = String_val(argv[4]);
    const char *db_str = String_val(argv[5]);
    Mapi conn = mapi_connect(host_str, port_i, user_str, passwd_str, lang_str, db_str);
    if (conn) {
	return Val_some((value) conn);
    } else {
	return Val_none;
    }
}

CAMLprim value mapi_destroy_stub(value conn)
{
    MapiMsg msg = mapi_destroy((Mapi) conn);
    return Val_int(index_of_msg(msg));
}

CAMLprim value mapi_disconnect_stub(value conn)
{
    MapiMsg msg = mapi_disconnect((Mapi) conn);
    return Val_int(index_of_msg(msg));
}

CAMLprim value mapi_reconnect_stub(value conn)
{
    MapiMsg msg = mapi_reconnect((Mapi) conn);
    return Val_int(index_of_msg(msg));
}

CAMLprim value mapi_ping_stub(value conn)
{
    MapiMsg msg = mapi_ping((Mapi) conn);
    return Val_int(index_of_msg(msg));
}

CAMLprim value mapi_error_stub(value conn)
{
    MapiMsg msg = mapi_error((Mapi) conn);
    return Val_int(index_of_msg(msg));
}

CAMLprim value mapi_error_str_stub(value conn)
{
    return caml_copy_string(mapi_error_str((Mapi) conn));
}

CAMLprim value mapi_result_error_stub(value handle)
{
    return caml_copy_string(mapi_result_error((MapiHdl) handle));
}

CAMLprim value mapi_query_stub(value conn, value query)
{
    MapiHdl hdl = mapi_query((Mapi) conn, String_val(query));
    if (hdl == NULL) {
	return Val_int(0);
    } else {
	return Val_some((value) hdl);
    }
}

CAMLprim value mapi_fetch_row_stub(value hdl)
{
    int nr_fields = mapi_fetch_row((MapiHdl) hdl);
    if (nr_fields == 0) {
	return Val_int(0);
    } else {
	return Val_some(Val_int(nr_fields));
    }
}

CAMLprim value mapi_fetch_field_stub(value hdl, value index)
{
    char *res = mapi_fetch_field((MapiHdl) hdl, Int_val(index));
    if (res) {
	return Val_some(caml_copy_string(res));
    } else {
	return Val_int(0);
    }
}

CAMLprim value mapi_close_handle_stub(value hdl)
{
    MapiMsg msg = mapi_close_handle((MapiHdl) hdl);
    return Val_int(index_of_msg(msg));
}

CAMLprim value mapi_seek_row_stub(value handle, value rownr, value whence)
{
    MapiMsg msg = mapi_seek_row((MapiHdl) handle, Int64_val(rownr), Int_val(whence));
    return Val_int(index_of_msg(msg));
}

CAMLprim value mapi_fetch_all_rows_stub(value handle)
{
    mapi_int64 nr_rows = (mapi_fetch_all_rows((MapiHdl) handle));

    if (nr_rows == 0) {
	return Val_int(0);
    } else {
	return Val_some(caml_copy_int64(nr_rows));
    }
}

CAMLprim value mapi_get_field_count_stub(value handle)
{
    return Val_int(mapi_get_field_count((MapiHdl) handle));
}

CAMLprim value mapi_get_row_count_stub(value handle)
{
    mapi_int64 nr_rows = mapi_get_row_count((MapiHdl) handle);
    return caml_copy_int64(nr_rows);
}

CAMLprim value mapi_rows_affected_stub(value handle)
{
    mapi_int64 nr_rows = mapi_rows_affected((MapiHdl) handle);
    return caml_copy_int64(nr_rows);
}

CAMLprim value mapi_fetch_field_list_stub(value handle)
{
    CAMLparam1(handle);
    CAMLlocal2(cli, cons);
    char **fields = mapi_fetch_field_array((MapiHdl) handle);
    int nr_fields = mapi_get_field_count((MapiHdl) handle);

    if (fields != NULL) {
	cli = Val_emptylist;

	int i = 0;

	for (i = (nr_fields - 1); i >= 0; --i) {
	    cons = caml_alloc(2, 0);
	    Store_field(cons, 0, caml_copy_string(fields[i]));
	    Store_field(cons, 1, cli);
	    cli = cons;
	}
    
	CAMLreturn(Val_some(cli));
    } else {
	CAMLreturn(Val_none);
    }
}

CAMLprim value mapi_quote_stub(value ml_str)
{
    CAMLparam1(ml_str);
    CAMLlocal1(ml_quoted_str);
    char *s = String_val(ml_str);
    int len = caml_string_length(ml_str);
    char *quoted_str = mapi_quote(s, len);
    ml_quoted_str = caml_copy_string(s);
    free(quoted_str);
    CAMLreturn(ml_quoted_str);
}

CAMLprim value mapi_unquote_stub(value ml_str)
{
    CAMLparam1(ml_str);
    CAMLlocal1(ml_unquoted_str);
    char *s = String_val(ml_str);
    int len = caml_string_length(ml_str);
    char *unquoted_str = mapi_quote(s, len);
    ml_unquoted_str = caml_copy_string(s);
    free(unquoted_str);
    CAMLreturn(ml_unquoted_str);
}

CAMLprim value mapi_get_name_stub(value ml_handle, value ml_index)
{
    int index = Int_val(ml_index);
    MapiHdl handle = (MapiHdl) ml_handle;
    assert(index < mapi_get_field_count(handle));
    char *fname = mapi_get_name(handle, index);
    if (fname) {
	return Val_some(caml_copy_string(fname));
    } else {
	return Val_none;
    }
}
