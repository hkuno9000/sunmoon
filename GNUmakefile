# Makefile
# $Id: GNUmakefile,v 1.5 2006-08-24 09:14:43 hkuno Exp $
#----- rules -------------------------------------------------------------
TARGET=sunmoon.out
ifndef PREFIX
PREFIX=/usr/local
endif
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

install: $(TARGET)
	install $(TARGET) $(PREFIX)/bin/sunmoon

# for MacOS-X, Linux and Cygwin
sunmoon.out: sunmoon.o astro.a
	$(CXX) $(CFLAGS) -o $@ $^

astro.a: $(OBJ)
	ar cru $@ $^
	ranlib $@

$(OBJ): defs.h GNUmakefile

#----- test --------------------------------------------------------------
test: jday.test atime.test acoord.test planets.test sunmoon.test

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

define SUNMOON_TEXT
LT: +35d 10m 00s
LG: E136d 55m 00s
SEA: 0m
UTC: 2026-01-01T00:00:00
LOC: 2026-01-01 09:00:00 JST
LST: 15h 50m 19s
SUN-ALT: +18.47d
MOON-ALT: -29.02d
MOON-PHASE: +145.68d
SUN     : RADEC[18h 45m 59s(281.49d)  -23d 01m 03s(-23.02d)], J2000[18h 44m 24s(281.10d)  -23d 02m 38s(-23.04d)], AZALT[317.70d(SE)  +18.47d]
MOON    : RADEC[04h 15m 17s(063.82d)  +25d 31m 29s(+25.52d)], J2000[04h 13m 42s(063.42d)  +25d 27m 29s(+25.46d)], AZALT[173.56d(North)  -29.02d]
MERCURY : RADEC[17h 54m 06s(268.52d)  -24d 00m 08s(-24.00d)], J2000[17h 52m 30s(268.12d)  -23d 59m 44s(-24.00d)], AZALT[329.07d(SSE)  +23.97d]
VENUS   : RADEC[18h 40m 13s(280.06d)  -23d 37m 23s(-23.62d)], J2000[18h 38m 38s(279.66d)  -23d 38m 44s(-23.65d)], AZALT[319.20d(SE)  +18.76d]
MARS    : RADEC[18h 55m 31s(283.88d)  -23d 43m 13s(-23.72d)], J2000[18h 53m 56s(283.48d)  -23d 45m 09s(-23.75d)], AZALT[316.32d(SE)  +16.58d]
JUPITER : RADEC[07h 32m 30s(113.12d)  +21d 58m 45s(+21.98d)], J2000[07h 30m 56s(112.74d)  +22d 02m 02s(+22.03d)], AZALT[128.50d(NW)  -12.32d]
SATURN  : RADEC[23h 49m 31s(357.38d)  -03d 35m 47s(-03.60d)], J2000[23h 48m 11s(357.05d)  -03d 44m 30s(-03.74d)], AZALT[254.86d(ENE)  -26.21d]
URANUS  : RADEC[03h 42m 57s(055.74d)  +19d 30m 34s(+19.51d)], J2000[03h 41m 26s(055.36d)  +19d 25m 31s(+19.43d)], AZALT[182.13d(North)  -35.30d]
NEPTUNE : RADEC[00h 00m 20s(000.08d)  -01d 24m 57s(-01.42d)], J2000[23h 59m 00s(359.75d)  -01d 33m 40s(-01.56d)], AZALT[251.07d(ENE)  -26.96d]
PLUTO   : RADEC[20h 23m 58s(305.99d)  -23d 13m 22s(-23.22d)], J2000[20h 22m 25s(305.61d)  -23d 18m 22s(-23.31d)], AZALT[301.18d(ESE)  +02.83d]
SUN-CULM: 2026-01-01 11:55:44 JST
MOON-RISE: 2026-01-01 14:17:25 JST
SUN-SET: 2026-01-01 16:51:30 JST
MOON-CULM: 2026-01-01 21:59:12 JST
MOON-SET: 2026-01-02 05:45:30 JST
SUN-RISE: 2026-01-02 07:00:15 JST
endef
sunmoon.test: $(TARGET)
	$(file >in.tmp,$(SUNMOON_TEXT))
	./$(TARGET) utc=2026-1-1T0:0:0 leap=37 table=1 -p >out.tmp
	diff -w in.tmp out.tmp

jday.out atime.out acoord.out planets.out: astro.a

#----- util --------------------------------------------------------------
clean:
	$(RM) *.obj *.lib  *.o *.a  *.bak *.tmp

cleanall: clean
	$(RM) *.exe *.map *.out

#end
