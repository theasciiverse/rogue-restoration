#
# Makefile for xrogue
#

HDRS=	rogue.h mach_dep.h network.h pcmenu.h clrln.h

OBJS=	vers.o actions.o bolt.o chase.o command.o daemon.o daemons.o eat.o \
	effects.o fight.o encumb.o help.o init.o io.o list.o main.o maze.o \
	misc.o monsters.o mons_def.o move.o n_level.o options.o outside.o \
	pack.o passages.o player.o potions.o rings.o rip.o rooms.o save.o \
	scrolls.o state.o sticks.o things.o trader.o util.o weapons.o wear.o wizard.o \
	rogue.o xcrypt.o
#	rogue.o clrln.o

CFILES=	vers.c actions.c bolt.c chase.c command.c daemon.c daemons.c eat.c \
	effects.c fight.c encumb.c help.c init.c io.c list.c main.c maze.c \
	misc.c monsters.c mons_def.c move.c n_level.c options.c outside.c \
	pack.c passages.c player.c potions.c rings.c rip.c rooms.c save.c \
	scrolls.c state.c sticks.c things.c trader.c util.c weapons.c wear.c wizard.c \
	rogue.c xcrypt.c
#	rogue.c clrln.c

# Define -DUSG5_2 as an option to CFLAGS if you are running System V Release 2.
# Define -DUSG5_0 as an option to CFLAGS if you are running System V Release 0.
# For a BSD installation, the makefile will automatically set the -DBSD option.
#
# There are ifdefs in the code for PC7300.  Do NOT define this flag unless you
# have the modified libtam.a package.  This package is not currently available
# in the Toolchest or TOADS database.  Rogue will run on the UNIX PC without
# it, but it will not use the hardware windowing features.  Be sure, though, to
# set -DUSG5_0 (for PC releases at least through 3.0)..
#
# Use the -g option only if you want the symbol table available for debugging.
# Use the rtcc compiler to identify null-pointer probs.

#CC=rtcc
#CFLAGS=-c -DUSG5_2
#CFLAGS=-c -g -qp -DUSG5_2
#LDFLAGS=

# Sun systems
#CC=/usr/5bin/cc

CC=cc
CFLAGS=-DUSG5_2
LDFLAGS=

# For System V Release 2, curses and terminfo are available via the -lcurses
# library call.  If that is not available, set CURSESLIB to the path name
# of the archive file containing your curses library and TERMLIB to the path
# name of the archive file containing your termcap or terminfo library.
# With the UNIX PC, through release 3.0 (at least), set CURSESLIB to -lcurses
# and TERMLIB to -ltermlib
#
# Note that for System V Release 2, -lcurses automatically includes
# the terminfo library.  For BSD systems, the makefile takes care
# of including the curses and termcap libraries.

#CURSESLIB=-lcurses
CURSESLIB=-lcurses
TERMLIB=

# If you have fixes to the terminfo routines, define them in TERMOBJ.
# For instance, the standard terminfo library does not erase to the end of
# the line correctly for terminals without a built-in clear-to-end-of-line
# feature.  So we use a replacement:
#
# TERMOBJ=$(HOME)/src/rogue/terminfo/clrln.o
#
# We make clrln.o above.

#TERMOBJ=clrln.o
TERMOBJ=

a.out:	$(HDRS) $(OBJS) 
	$(CC) $(LDFLAGS) $(OBJS) $(TERMOBJ) $(CURSESLIB) $(TERMLIB)

#	@if test -f /vmunix \
#	;then echo $(CC) $(LDFLAGS) $(OBJS) -lcurses -ltermlib \
#	;$(CC) $(LDFLAGS) $(OBJS) -lcurses -ltermlib \
#	;else echo $(CC) $(LDFLAGS) $(OBJS) $(TERMOBJ) $(CURSESLIB) $(TERMLIB) \
#	;$(CC) $(LDFLAGS) $(OBJS) $(TERMOBJ) $(CURSESLIB) $(TERMLIB) \
#	;fi
#	#size a.out

xrogue:	a.out
	mv a.out xrogue

xrg:	a.out
	mv a.out xrg

install:	a.out
	cp a.out xrogue
	cpset -o xrogue /usr/games 2555 bin games
	> xrogue_roll
	cpset -o xrogue_roll /usr/games/lib 660 bin games
	rm -f xrogue_roll

lint:	$(HDRS) $(CFILES)
	lint -DUSG5_2 -x -b $(CFILES)  >> lintout

rtlint:	$(HDRS) $(CFILES)
	rtlint -DUSG5_2 -x -b $(CFILES)  >> rtlintout

clean:	clobber
shrink:	clobber
clobber:
	rm -f $(OBJS) xrogue xrg a.out

