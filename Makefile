all: xgterm ximtool

PREFIX = /usr/local

export TCL_INCLUDE_DIR = /usr/include/tcl

.PHONY: xgterm ximtool xtapemon obmsh

	
obm/libobm.a:
	$(MAKE) -C obm

xgterm: obm/libobm.a
	$(MAKE) -C xgterm

ximtool: obm/libobm.a
	$(MAKE) -C ximtool

xtapemon:
	$(MAKE) -C xtapemon

obmsh: obm/libobm.a
	$(MAKE) -C obmsh

clean:
	$(MAKE) -C xtapemon clean
	$(MAKE) -C ximtool clean
	$(MAKE) -C xgterm clean
	$(MAKE) -C obmsh clean
	$(MAKE) -C obm clean

install: xgterm ximtool
	mkdir -p ${PREFIX}/bin ${PREFIX}/man/man1
	install -m755 xgterm/xgterm ${PREFIX}/bin
	install -m755 xgterm/xgterm.man ${PREFIX}/man/man1/xgterm.1
	tic xgterm/xgterm.terminfo
	install -m755 ximtool/ximtool ${PREFIX}/bin
	install -m755 ximtool/ximtool.man ${PREFIX}/man/man1/ximtool.1
	if [ -x ximtool/clients/ism_wcspix.e ] ; then \
	    install -m755 ximtool/clients/ism_wcspix.e ${PREFIX}/bin ; \
	fi
