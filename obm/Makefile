OBJS = Obm.o client.o param.o server.o widget.o gterm.o marker.o geom.o

ALL_OBJS = ObmW/[A-Za-z]*.o $(OBJS)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/X11/include -D__DARWIN__
    LDFLAGS += -L/usr/X11/lib
endif

ifeq ($(OSI_COMPLIANT), YES)
    CFLAGS += -DOSI_COMPLIANT
else
    OBJS += html.o
endif

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
