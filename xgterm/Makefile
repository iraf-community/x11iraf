OBJS = button.o charproc.o cursor.o data.o gtermio.o input.o main.o	\
 menu.o misc.o screen.o scrollbar.o tabs.o util.o VTPrsTbl.o

all: xgterm

CFLAGS += -I../obm -DALLOWLOGGING -Wno-implicit-function-declaration -Wno-implicit-int
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/X11/include
    LDFLAGS += -L/usr/X11/lib
endif

xgterm: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) \
	    -L../obm -lobm \
	    -lXpm $(TCL_LIB) -lXaw3d -lXmu -lXt -lXext -lX11 -lncurses -lm

clean:
	rm -f xgterm $(OBJS)
