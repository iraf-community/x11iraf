OBJS = listres.o AllWidgets.o

CFLAGS += -I.. -I../ObmW

all: listres

listres: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) \
	    -L.. -lobm \
	    -lXpm -ltcl -lXaw3d -lXmu -lXt -lXext -lX11 -lm

clean:
	rm -f listres $(OBJS)
