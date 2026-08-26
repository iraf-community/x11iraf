OBJS = classnames.o types.o xtapemon.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/X11/include -D__APPLE__
    LDFLAGS += -L/usr/X11/lib
endif

all: xtapemon

xtapemon: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -lXaw -lXmu -lXt -lX11 -lm

clean:
	rm -f xtapemon $(OBJS)
