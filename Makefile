all: xgterm ximtool xtapemon obmsh

.PHONY: xgterm ximtool xtapemon obmsh

obm/libobm.a:
	$(MAKE) -C obm

xgterm: obm/libobm.a
	$(MAKE) -C xgterm

ximtool: obm/libobm.a
	$(MAKE) -C ximtool

xtapemon:
	$(MAKE) -C xtapemon

obmsh:
	$(MAKE) -C obmsh

clean:
	$(MAKE) -C xtapemon clean
	$(MAKE) -C ximtool clean
	$(MAKE) -C xgterm clean
	$(MAKE) -C obmsh clean
	$(MAKE) -C obm clean
