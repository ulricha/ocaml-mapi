MONET_PREFIX= /usr/local/monetdb
INSTALL_DIR= /opt/godi/lib/ocaml/site-lib

all: mapi.cma mapi.cmxa

install: mapi.cma mapi.cmxa
	mkdir -p $(INSTALL_DIR)/mapi
	cp -a mapi.cma mapi.cmxa mapi.mli mapi.a libmapi_stubs.a mapi.cmi dllmapi_stubs.so META $(INSTALL_DIR)/mapi/

mapi_stubs.o: mapi_stubs.c
	ocamlc -c -ccopt -I$(MONET_PREFIX)/include mapi_stubs.c

mapi.cmi: mapi.mli
	ocamlc -c mapi.mli

mapi.cmo: mapi.ml mapi.cmi
	ocamlc -c mapi.ml

dll_mapi_stubs.so: mapi_stubs.o
	ocamlmklib -o mapi_stubs mapi_stubs.o -L$(MONET_PREFIX)/lib -lmapi

mapi.cma: mapi.cmo dll_mapi_stubs.so
	ocamlc -a -o $@ mapi.cmo -dllib -lmapi_stubs \
		-ccopt -L$(MONET_PREFIX)/lib -cclib -lmapi

mapi.cmx: mapi.ml mapi.cmi
	ocamlopt -c mapi.ml

mapi.cmxa: mapi.cmx dll_mapi_stubs.so
	ocamlopt -a -o $@ mapi.cmx -cclib -lmapi_stubs \
		-ccopt -L$(MONET_PREFIX)/lib -cclib -lmapi

clean:
	rm -f *.cmi *.cmo *.so *.cma *.o *.a *.cmxa *.cma *.cmx
