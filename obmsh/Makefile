OBJS = obmsh.o

all: obmsh

CFLAGS += -I../obm

obmsh: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) \
	    -L../obm -lobm \
	    -lXpm -ltcl -lXaw3d -lXmu -lXt -lXext -lX11 -lncurses -lm

clean:
	rm -f obmsh $(OBJS)
