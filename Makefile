#  Copyright (C) 2010  Florent Monnier
#  Contact:  <fmonnier(AT-SIGN)linux-nantes(DOT-ORG)>
#  This file is distributed under the terms of the GNU Lesser General Public
#  License, with the special exception on linking described in file LICENSE.txt

OCAMLC := ocamlc.opt -g
OCAMLOPT := ocamlopt.opt -g
OCAMLMKLIB := ocamlmklib
OCAMLDOC := ocamldoc.opt
OCAML_PATH := /opt/godi/lib/ocaml/site-lib/
MAPI_LIBS := -lmapi
MAPI_DIR := mapi
PREFIX := $(OCAML_PATH)/$(MAPI_DIR)
SO_PREFIX := $(PREFIX)
#SO_PREFIX := $(OCAML_PATH)/stublibs/
DOC_DIR := doc
MONET_PREFIX := /usr/local/monetdb

all: cma cmxa cmxs
byte cma: mapi.cma
opt cmxa: mapi.cmxa
shared cmxs: mapi.cmxs
.PHONY: all byte cma opt cmxa

mapi.mli: mapi.ml
	$(OCAMLC) -i $< > $@

mapi.cmi: mapi.mli
	$(OCAMLC) -c $<

mapi.cmx: mapi.ml mapi.cmi
	$(OCAMLOPT) -c $<

mapi.cmo: mapi.ml mapi.cmi
	$(OCAMLC) -c $<

mapi_stubs.o: mapi_stubs.c
	$(OCAMLC) -c -ccopt -I$(MONET_PREFIX)/include $<

dllmapi_stubs.so libmapi_stubs.a: mapi_stubs.o
	$(OCAMLMKLIB) -oc mapi_stubs -L$(MONET_PREFIX)/lib $(MAPI_LIBS) $< 

mapi.cmxa mapi.a: mapi.cmx dllmapi_stubs.so
	$(OCAMLMKLIB) -o mapi -L. -L$(MONET_PREFIX)/lib $(MAPI_LIBS) -ccopt -lmapi_stubs $< 

mapi.cma: mapi.cmo  dllmapi_stubs.so
	$(OCAMLC) -a -o $@ -ccopt $(MAPI_LIBS) -dllib -lmapi_stubs $<

mapi.cmxs: mapi.cmxa
	$(OCAMLOPT) -shared -linkall -o $@ $<

DIST_FILES=           \
    libmapi_stubs.a   \
    mapi.a            \
    mapi.o            \
    mapi.cma          \
    mapi.cmi          \
    mapi.cmo          \
    mapi.cmx          \
    mapi.cmxa         \
    mapi.ml           \
#EOL
SO_DIST_FILES=        \
    dllmapi_stubs.so  \
    mapi.cmxs         \
#EOL

.PHONY: install uninstall
install: $(DIST_FILES)  $(SO_DIST_FILES) META
	if [ ! -d $(PREFIX) ]; then install -d $(PREFIX) ; fi
	for file in $(DIST_FILES);    do if [ -f $$file ]; then install -m 0644 $$file $(PREFIX)/; fi; done
	for file in $(SO_DIST_FILES); do if [ -f $$file ]; then install -m 0755 $$file $(SO_PREFIX)/; fi; done
	install -m 0644 META $(PREFIX)/
uninstall:
	ocamlfind remove mapi

.PHONY: clean cleanmli cleandoc
clean:
	rm -f *.[oa] *.cm[ioxa] *.{so,cmxa,cmxs} *.{opt,byte}
cleanmli:
	rm -f mapi.mli
cleandoc:
	rm -f $(DOC_DIR)/*
	rmdir $(DOC_DIR)

