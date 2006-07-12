# Makefile
# $Id: Makefile,v 1.2 2006-07-12 07:57:24 hkuno Exp $
#----- rules -------------------------------------------------------------
RM=del
TARGET=sunmoon.exe
CFLAGS=-W3 -MD -GX -O1
.cpp.obj:
	cl $(CFLAGS) -c $<
.cpp.exe:
	cl $(CFLAGS) -DTEST -Fotest.obj -Fe$*.exe $< astro.lib

#----- objects and sources -----------------------------------------------
SRC=vec3.cpp degree.cpp jday.cpp \
	acoord.cpp atime.cpp \
	planets.cpp
OBJ=$(SRC:.cpp=.obj)

#----- target ------------------------------------------------------------
all: $(TARGET)

# for Windows
sunmoon.exe: sunmoon.obj astro.lib
	cl $(CFLAGS) $**

astro.lib: $(OBJ)
	lib /OUT:$@ $**

$(OBJ): $*.h defs.h Makefile

#----- test --------------------------------------------------------------
test: jday.exe atime.exe acoord.exe
	jday self
	atime self
	acoord self

jday.exe atime.exe acoord.exe : astro.lib

#----- util --------------------------------------------------------------
clean:
	$(RM) *.obj *.lib  *.o *.l  *.bak *.tmp $$*

cleanall: clean
	$(RM) *.exe *.map *.out

#end
