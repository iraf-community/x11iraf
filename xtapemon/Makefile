OBJS = classnames.o types.o xtapemon.o

all: xtapemon

xtapemon: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) -lXaw3d -lXmu -lXt -lX11 -lm

clean:
	rm -f xtapemon $(OBJS)
