#
#  Makefile for campus Linux computers
#

INCLUDE = -I/lusr/include -I/lusr/X11/include -I/p/graft/fltk-1.1.10/include
LIBDIR = -L/lusr/lib -L/lusr/X11/lib -L/p/graft/fltk-1.1.10/lib

LIBS = -lfltk -lfltk_gl -lfltk_images -lGL -ljpeg -lpng -lz

CFLAGS = -g

CC = g++

all: impressionist

ALL.O = bitmap.o impBrush.o \
	impressionist.o impressionistDoc.o impressionistUI.o \
	originalView.o paintView.o pointBrush.o filledCircleBrush.o \
	scatteredCircleBrush.o scatteredLineBrush.o scatteredPointBrush.o \
	singleLineBrush.o splineBrush.o
ALL.CPP = $(subst .o,.cpp,$(ALL.O)) 

MAKEDEPEND = gcc -M $(CPPFLAGS) $(INCLUDE) -o $*.d $<

%.P : %.cpp
	$(MAKEDEPEND)
	@sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $*.d > $@; \
		rm -f $*.d; [ -s $@ ] || rm -f $@

-include $(ALL.O:.o=.P)

%.o: %.cpp 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

impressionist: $(ALL.O)
	$(CC) $(CFLAGS) -o $@ $(ALL.O) $(INCLUDE) $(LIBDIR) $(LIBS)

clean:
	rm -f *.o *.P impressionist *~ ._*
