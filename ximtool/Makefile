OBJS = eps.o fitsio.o gifio.o iis.o irafio.o ism.o ism_wcspix.o	\
 load.o logo.o print.o quant.o rasio.o raster.o save.o tiffio.o	\
 util.o ximclient.o ximtool.o zscale.o

all: ximtool clients

.PHONY: clients

CFLAGS += -I../obm

ximtool: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) \
	    -L../obm -lobm \
	    -lXpm -ltcl -lXaw3d -lXmu -lXt -lXext -lX11 -lncurses -lm

clients:
	cd clients && mkpkg || true

clean:
	rm -f ximtool $(OBJS)
	cd clients && mkpkg clean || true
