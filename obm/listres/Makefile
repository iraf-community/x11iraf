OBJS = listres.o AllWidgets.o

CFLAGS += -I.. -I../ObmW
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/X11/include
    LDFLAGS += -L/usr/X11/lib
endif

all: listres

listres: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) \
	    -L.. -lobm \
	    -lXpm -ltcl -lXaw3d -lXmu -lXt -lXext -lX11 -lm

clean:
	rm -f listres $(OBJS)
