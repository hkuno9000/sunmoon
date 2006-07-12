# Makefile
# $Id: GNUmakefile,v 1.2 2006-07-12 10:17:00 hkuno Exp $
#----- rules -------------------------------------------------------------
TARGET=sunmoon.out
CFLAGS=-Wall
.cpp.out:
	$(CXX) $(CFLAGS) -DTEST -o $@ $< astro.a

#----- objects and sources -----------------------------------------------
SRC=vec3.cpp degree.cpp jday.cpp \
	acoord.cpp atime.cpp \
	planets.cpp
OBJ=$(SRC:.cpp=.o)

#----- target ------------------------------------------------------------
all: $(TARGET)

# for MacOS-X, Linux and Cygwin
sunmoon.out: sunmoon.o astro.a
	$(CXX) $(CFLAGS) -o $@ $^

astro.a: $(OBJ)
	ar cru $@ $^
	ranlib $@

$(OBJ): defs.h Makefile

#----- test --------------------------------------------------------------
test: jday.test atime.test acoord.test

jday.test: jday.out
	perl -n -e "print if /^j/,/^g2000/;" jday.cpp >in.tmp
	./jday.out <in.tmp >out.tmp
	diff -w in.tmp out.tmp

atime.test: atime.out
	perl -n -e "print if /^1997/../^1978/;" atime.cpp >in.tmp
	./atime.out <in.tmp >out.tmp
	diff -w in.tmp out.tmp

acoord.test: acoord.out
	perl -n -e "print if /^1996/../^1997.11/;" acoord.cpp >in.tmp
	./acoord.out <in.tmp >out.tmp
	diff -w in.tmp out.tmp

jday.out atime.out acoord.out: astro.a

#----- util --------------------------------------------------------------
clean:
	$(RM) *.obj *.lib  *.o *.a  *.bak *.tmp

cleanall: clean
	$(RM) *.exe *.map *.out

#end
