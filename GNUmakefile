# Makefile
# $Id: GNUmakefile,v 1.4 2006-08-24 09:01:05 hkuno Exp $
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
OUTDIR=~/Sites/remotecam

#----- target ------------------------------------------------------------
all: $(TARGET)

install: $(TARGET)
	install $(TARGET) /usr/bin/sunmoon

setup:
	-mkdir $(OUTDIR)
	-mkdir $(OUTDIR)/result
	-mkdir $(OUTDIR)/param
	cp -i ../sample/normal.order   $(OUTDIR)/param
	cp -i ../sample/remotecam.conf ~/.remomtecam.conf

# for MacOS-X, Linux and Cygwin
sunmoon.out: sunmoon.o astro.a
	$(CXX) $(CFLAGS) -o $@ $^

astro.a: $(OBJ)
	ar cru $@ $^
	ranlib $@

$(OBJ): defs.h Makefile

#----- test --------------------------------------------------------------
test: jday.test atime.test acoord.test planets.test

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

planets.test: planets.out
	perl -n -e "print if /^1991/../^conv: l=229/;" planets.cpp >in.tmp
	./planets.out <in.tmp >out.tmp
	diff -w in.tmp out.tmp

jday.out atime.out acoord.out planets.out: astro.a

#----- util --------------------------------------------------------------
clean:
	$(RM) *.obj *.lib  *.o *.a  *.bak *.tmp

cleanall: clean
	$(RM) *.exe *.map *.out

#end
