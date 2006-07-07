# Makefile
#	Copyright (C) 1997,1998 hkuno
#	mailto:hkuno.kuno@nifty.ne.jp
#----- rules -------------------------------------------------------------
!ifdef MSVCDIR
## Visual C++ 5.0
MYLIB=mylibvc.lib
CC = cl
AR = lib
CFLAGS = -W3 -O1
.cpp.obj:
	$(CC) $(CFLAGS) -c $<
.cpp.asm:
	$(CC) $(CFLAGS) -Fa $<
.cpp.cod:
	$(CC) $(CFLAGS) -Fc $<
.cpp.exe:
	$(CC) $(CFLAGS) -DTEST $< $(MYLIB)
!else
## C++Builder
MYLIB=mylibbc.lib
CC = bcc32
AR = tlib
CFLAGS = -w -O1
.cpp.obj:
	$(CC) $(CFLAGS) -c { $< }
.cpp.asm:
	$(CC) $(CFLAGS) -S $<
.cpp.exe:
	$(CC) $(CFLAGS) -DTEST -o$$ $< $(MYLIB)
!endif

#----- objects and sources -----------------------------------------------
OBJ=cfile.obj strx.obj fname.obj \
	degree.obj vec3.obj \
	jday.obj atime.obj acoord.obj

#----- target ------------------------------------------------------------
$(MYLIB): $(OBJ)
!ifdef MSVCDIR
	$(AR) -out:$@ $(OBJ)
!else
	$(AR) $@ -+$(?: = -+)
!endif

$(OBJ): defs.h

#----- test --------------------------------------------------------------
test: jday.exe atime.exe acoord.exe

jday.exe atime.exe acoord.exe : $(MYLIB)

#----- util --------------------------------------------------------------
clean:
	del $$*
	del *.obj
	del *.tds
	del pch.*
	del9 *.~*
	del9 *.bak

cleanall: clean
	del *.lib
	del *.exe

save:
	zcopy *.cpp *.h makefile* *.htm *.txt fix\ /u/b

lzh:
	lha32 u -o2 ..\src *.cpp *.h makefile *.html *.txt
