OBJS = Arrow.o Board.o Button.o Common.o DrawIString.o DrawString.o	\
 DrawingArea.o Frame.o Group.o Gterm.o Icon.o iconutil.o Label.o	\
 MenuBar.o MultiList.o RadioGrp.o RowCol.o Scrollbar.o		\
 Slider2.o Tablist2Tabs.o TextWidth.o Toggle.o cvtLong.o scroll.o	\
 strnchr.o Tabs.o Gcs.o ListTree.o Container.o color.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/usr/X11/include
    LDFLAGS += -L/usr/X11/lib
endif

ifeq ($(OSI_COMPLIANT), YES)
      CFLAGS += -DOSI_COMPLIANT
else
      OBJS += HTML-PSformat.o HTML.o HTMLformat.o HTMLimages.o HTMLjot.o     \
         HTMLlists.o HTMLparse.o HTMLwidgets.o Separator.o Table.o Table3d.o \
	 TableUtil.o
endif

all: $(OBJS)

Gterm.o: Gterm.c GtermCnv.c GtermGraphics.c GtermImaging.c		\
 GtermCmap.c GtermMapping.c GtermMarker.c GtermUtil.c GtermDebug.c

clean:
	rm -f $(OBJS)
