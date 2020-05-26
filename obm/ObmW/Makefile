OBJS = Arrow.o Board.o Button.o Common.o DrawIString.o DrawString.o	\
 DrawingArea.o Frame.o Group.o Gterm.o Icon.o iconutil.o Label.o	\
 Layout.o MenuBar.o MultiList.o RadioGrp.o RowCol.o Scrollbar.o		\
 Slider2.o Tablist2Tabs.o TextWidth.o Toggle.o cvtLong.o scroll.o	\
 strnchr.o Tabs.o Gcs.o ListTree.o Separator.o Container.o color.o	\
 laygram.o laylex.o

# Non-free
OBJS += HTML-PSformat.o HTML.o HTMLformat.o HTMLimages.o HTMLjot.o	\
 HTMLlists.o HTMLparse.o HTMLwidgets.o Table.o Table3d.o TableUtil.o

CFLAGS += -I.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/X11/include -D__DARWIN__
    LDFLAGS += -L/usr/X11/lib
endif

all: $(OBJS)

Gterm.o: Gterm.c GtermCnv.c GtermGraphics.c GtermImaging.c		\
 GtermCmap.c GtermMapping.c GtermMarker.c GtermUtil.c GtermDebug.c

laygram.c: laygram.y
	yacc -d laygram.y
	sed 's/yy/LayYY/g' y.tab.h > laygram.h
	sed 's/yy/LayYY/g' y.tab.c > laygram.c
	rm y.tab.c y.tab.h

laylex.c: laylex.l laygram.c
	$(LEX) laylex.l
	sed 's/yy/LayYY/g' lex.yy.c > laylex.c
	rm lex.yy.c

clean:
	rm -f $(OBJS) laygram.c laygram.h laylex.c
