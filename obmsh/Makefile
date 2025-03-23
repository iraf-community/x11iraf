OBJS = obmsh.o

all: obmsh

CFLAGS += -I../obm
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/X11/include
    LDFLAGS += -L/usr/X11/lib
endif

obmsh: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) \
	    -L../obm -lobm \
	    -lXpm $(TCL_LIB) -lXaw -lXmu -lXt -lXext -lX11 -lncurses -lm

clean:
	rm -f obmsh $(OBJS)
