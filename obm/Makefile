OBJS = Obm.o client.o param.o server.o widget.o gterm.o marker.o geom.o

# Non-free
OBJS += html.o

ALL_OBJS = ObmW/[A-Za-z]*.o $(OBJS)

CFLAGS += -I. -IObmW

all: libobm.a

.PHONY: ObmW listres

libobm.a: $(OBJS) ObmW
	ar cr $@ $(ALL_OBJS)

ObmW:
	$(MAKE) -C ObmW

listres:
	$(MAKE) -C listres

clean:
	$(MAKE) -C ObmW clean
	$(MAKE) -C listres clean
	rm -f $(OBJS) libobm.a
