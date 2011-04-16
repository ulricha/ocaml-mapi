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
    puts("mapi_connect_stub_native");
    const char *host_str = String_val(host);
    const int port_i = Int_val(port);
    assert(port > 0 && port < 65536);
    const char *user_str = String_val(user);
    const char *passwd_str = String_val(passwd);
    const char *lang_str = String_val(lang);
    const char *db_str = String_val(db);
    Mapi conn = mapi_connect(host_str, port_i, user_str, passwd_str, lang_str, db_str);
    return (value) conn;
}

CAMLprim value mapi_connect_stub_bc(value *argv, int argn)
{
    puts("mapi_connect_stub_bc");
    assert(argn == 6);
    const char *host_str = String_val(argv[0]);
    const int port_i = Int_val(argv[1]);
    assert(port_i > 0 && port_i < 65536);
    const char *user_str = String_val(argv[2]);
    const char *passwd_str = String_val(argv[3]);
    const char *lang_str = String_val(argv[4]);
    const char *db_str = String_val(argv[5]);
    Mapi conn = mapi_connect(host_str, port_i, user_str, passwd_str, lang_str, db_str);
    return (value) conn;
}

CAMLprim value mapi_error_stub(value conn)
{
    MapiMsg msg = mapi_error((Mapi) conn);
    return Val_int(index_of_msg(msg));
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

CAMLprim value mapi_destroy_stub(value handle)
{
    MapiMsg msg = mapi_destroy((Mapi) handle);
    return Val_int(index_of_msg(msg));
    
}
