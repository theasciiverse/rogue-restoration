#
# Makefile for xrogue
#
DISTNAME=xrogue8.0.4
PROGRAM=xrogue

O=o

HDRS=	rogue.h mach_dep.h network.h

OBJS1 =	vers.$(O) actions.$(O) bolt.$(O) chase.$(O) command.$(O) daemon.$(O) \
        daemons.$(O) eat.$(O) effects.$(O) fight.$(O) encumb.$(O) help.$(O) \
        init.$(O) io.$(O) list.$(O) main.$(O) maze.$(O) misc.$(O) monsters.$(O)
OBJS2 = mons_def.$(O) move.$(O) n_level.$(O) options.$(O) outside.$(O) pack.$(O) \
        passages.$(O) player.$(O) potions.$(O) rings.$(O) rip.$(O) rooms.$(O) \
        save.$(O) scrolls.$(O) sticks.$(O) things.$(O) trader.$(O) util.$(O) \
        weapons.$(O) wear.$(O) wizard.$(O) rogue.$(O) state.$(O) xcrypt.$(O)
OBJS  = $(OBJS1) $(OBJS2)

CFILES=	vers.c actions.c bolt.c chase.c command.c daemon.c daemons.c eat.c \
	effects.c fight.c encumb.c help.c init.c io.c list.c main.c maze.c \
	misc.c monsters.c mons_def.c move.c n_level.c options.c outside.c \
	pack.c passages.c player.c potions.c rings.c rip.c rooms.c save.c \
	scrolls.c sticks.c things.c trader.c util.c weapons.c wear.c wizard.c \
	rogue.c state.c xcrypt.c

MISC  = Makefile README.md LICENSE $(PROGRAM).sln $(PROGRAM).vcxproj $(PROGRAM).vcxproj.filters $(PROGRAM).vcxproj.user

CC    = cc
CFLAGS= -O3
CRLIB = -lcurses
RM    = rm -f
TAR   = tar 
.SUFFIXES: .obj

.c.obj:
	$(CC) $(CFLAGS) /c $*.c

$(PROGRAM): $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(CRLIB) -o $@

clean:
	$(RM) $(OBJS1)
	$(RM) $(OBJS2)
	$(RM) core a.exe a.out a.exe.stackdump $(PROGRAM) $(PROGRAM).exe $(PROGRAM).tar $(PROGRAM).tar.gz $(PROGRAM).zip

dist.src:
	make clean
	tar cf $(DISTNAME)-src.tar $(CFILES) $(HDRS) $(MISC)
	gzip -f $(DISTNAME)-src.tar

dist.linux:
	make clean
	make CC=gcc CFLAGS="-O3 -Wall -pedantic" $(PROGRAM)
	tar cf $(DISTNAME)-linux.tar $(PROGRAM) README.md LICENSE
	gzip -f $(DISTNAME)-linux.tar

dist.djgpp:
	make clean
	make CC=gcc CFLAGS="-I$(DJDIR)/include -O3 -Wall -pedantic" LDFLAGS="-L$(DJDIR)/lib" CRLIB="-lpdcurses" $(PROGRAM)
	rm -f $(DISTNAME)-djgpp.zip
	zip $(DISTNAME)-djgpp.zip $(PROGRAM).exe README.md LICENSE
 
dist.cygwin:
	make clean
	make CC=gcc LDFLAGS="-static" CFLAGS="-DNCURSES_STATIC -O3 -Wall -pedantic" $(PROGRAM)
	tar cf $(DISTNAME)-cygwin.tar $(PROGRAM).exe README.md LICENSE
	gzip -f $(DISTNAME)-cygwin.tar

dist.win32:
	nmake O="obj" RM="-del" clean
	nmake O="obj" CC="CL" CRLIB="..\pdcurses.lib shell32.lib user32.lib Advapi32.lib Ws2_32.lib" CFLAGS="-nologo -I.. -Ox -wd4033 -wd4716" $(PROGRAM)
	-del $(DISTNAME)-win32.zip
	zip $(DISTNAME)-win32.zip $(PROGRAM).exe README.md LICENSE

# Must use MSYS2 32-bit build host 
dist.msys2-i686:
	$(MAKE) clean
	$(MAKE) CC=gcc CRLIB="-lcurses -lWs2_32" CFLAGS="-O3 -Wall -pedantic" $(PROGRAM)
	tar cf $(DISTNAME)-msys2-i686.tar $(PROGRAM).exe README.md LICENSE
	gzip -f $(DISTNAME)-msys2-i686.tar

# Must use MSYS2 64-bit build host 
dist.msys2-x86_64:
	$(MAKE) clean
	$(MAKE) CC=gcc CRLIB="-lcurses -lWs2_32" CFLAGS="-O3 -Wall -pedantic" $(PROGRAM)
	tar cf $(DISTNAME)-msys2-x86_x64.tar $(PROGRAM).exe README.md LICENSE
	gzip -f $(DISTNAME)-msys2-x86_x64.tar

# Must use MSYS2 mingw32 build environment (https://www.msys2.org)
dist.mingw-w64-i686:
	make clean
	make CC=gcc CRLIB="-lncurses -lws2_32" LDFLAGS="-L/mingw32/lib" CFLAGS="-DNCURSES_STATIC -I/mingw32/include/ncurses -O3 -Wall -pedantic" $(PROGRAM)
	rm $(DISTNAME)-mingw-w64-i686.zip
	zip $(DISTNAME)-mingw-w64-i686.zip $(PROGRAM).exe README.md LICENSE
	
# Must use MSYS2 mingw64 build environment (https://www.msys2.org)
dist.mingw-w64-x86_64:
	make clean
	make CC=gcc CRLIB="-lncurses -lws2_32" LDFLAGS="-L/mingw64/lib" CFLAGS="-DNCURSES_STATIC -I/mingw64/include/ncurses -O3 -Wall -pedantic" $(PROGRAM)
	rm -f $(DISTNAME)-mingw-w64-x86_64.zip
	zip $(DISTNAME)-mingw-w64-x86_64.zip $(PROGRAM).exe README.md LICENSE

