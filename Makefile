all: checktcl xgterm ximtool

PREFIX = /usr/local

.PHONY: xgterm ximtool xtapemon obmsh checktcl

# Check the correct `tcl.h` path
TCL_CORRECT = 1
ifneq ($(shell test -e /usr/include/tcl/tcl.h && echo -n yes),yes)
	ifneq ($(shell test -e /usr/local/include/tcl/tcl.h && echo -n yes),yes)
		ifeq ($(shell test -e /usr/include/tcl.h && echo -n yes),yes)
			TCL_CORRECT = 0
		else ifeq ($(shell test -e /usr/local/include/tcl.h && echo -n yes),yes)
			TCL_CORRECT = 0
		endif
	endif
endif

checktcl:
	@sh -c "if [ $(TCL_CORRECT) -eq 0 ];   then grep -rli --exclude=Makefile 'tcl/tcl.h' ./ | xargs -i@ sed -i 's#tcl/tcl.h#tcl.h#g' @;        fi"
	
	
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
