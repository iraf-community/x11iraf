OBJS = eps.o fitsio.o gifio.o iis.o irafio.o ism.o ism_wcspix.o	\
 load.o logo.o print.o quant.o rasio.o raster.o save.o tiffio.o	\
 util.o ximclient.o ximtool.o zscale.o

all: ximtool clients

.PHONY: clients

CFLAGS += -I../obm
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/X11/include -D__DARWIN__
    LDFLAGS += -L/usr/X11/lib
endif

ifeq ($(OSI_COMPLIANT), YES)
    CFLAGS += -DOSI_COMPLIANT
else
    ximclient.o: ximtool.html.h
endif

ximtool.o: ximtool.gui.h

ximtool.gui.h: ximtool.gui
	sed -f gui2c.sed ximtool.gui > ximtool.gui.h

ximtool.html.h: ximtool.html
	sed -f gui2c.sed ximtool.html > ximtool.html.h

ximtool: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) \
	    -L../obm -lobm \
	    -lXpm -ltcl -lXaw3d -lXmu -lXt -lXext -lX11 -lncurses -lm

clients:
	cd clients && mkpkg || true

clean:
	rm -f ximtool ximtool.gui.h ximtool.html.h $(OBJS)
	cd clients && mkpkg clean || true
