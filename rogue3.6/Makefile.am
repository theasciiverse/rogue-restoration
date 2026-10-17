AUTOMAKE_OPTIONS = foreign

noinst_HEADERS = rogue.h mdport.h config-xsh.h mach_dep.h

bin_PROGRAMS = rogue
rogue_SOURCES = armor.c chase.c command.c daemon.c daemons.c fight.c init.c io.c list.c main.c mdport.c misc.c monsters.c move.c new_level.c options.c pack.c passages.c potions.c rings.c rip.c rooms.c save.c scrolls.c state.c sticks.c things.c vers.c weapons.c wizard.c

dist_man6_MANS = rogue.6

dist_doc_DATA = rogue.r rogue.doc rogue.txt rogue.pdf rogue.ps rogue.cat LICENSE.TXT

EXTRA_DIST = Makefile.xcu rogue.vcxproj rogue.sln TODO

rogue.doc: rogue.r
	$(GROFF) -P-c -t -ms -Tutf8 rogue.r > rogue.doc

rogue.txt: rogue.r
	$(GROFF) -P-c -t -ms -Tascii rogue.r > rogue.txt

rogue.pdf: rogue.r
	$(PDFROFF) --no-toc -t -ms rogue.r > rogue.pdf

rogue.ps: rogue.r
	$(GROFF) -t -ms -Tps rogue.r > rogue.ps

rogue.cat: rogue.6
	$(GROFF) -P-c -man -Tascii rogue.6 > rogue.cat

install-exec-hook:
	if test "x$(GROUP)" != "x"; then chgrp $(GROUP) $(DESTDIR)$(bindir)/$(PROGRAM); fi
	if test "x$(GROUP)" != "x"; then chmod g+s $(DESTDIR)$(bindir)/$(PROGRAM); fi
	touch $(DESTDIR)$(localstatedir)/$(SCOREFILE)
	if test "x$(GROUP)" != "x"; then chgrp $(GROUP) $(DESTDIR)$(localstatedir)/$(SCOREFILE); fi
	if test "x$(GROUP)" != "x"; then chmod g+w $(DESTDIR)$(localstatedir)/$(SCOREFILE); fi

