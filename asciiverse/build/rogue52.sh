# rogue52: Rogue 5.2, the 1982 Berkeley game
cd rogue5.2
# the Makefile's WIZARD (its password-guarded wizard mode) and score file
# name; not its DUMP (core dumps instead of saving on a crash) nor its
# load limits (MAXLOAD, LOADAV, CHECKTIME read /dev/kmem)
$CC $CFLAGS -DWIZARD -DSCOREFILE='"rogue52.scr"' \
  vers.c extern.c armor.c chase.c command.c daemon.c daemons.c fight.c \
  init.c io.c list.c main.c mdport.c misc.c monsters.c move.c new_level.c \
  options.c pack.c passages.c potions.c rings.c rip.c rooms.c save.c \
  scrolls.c state.c sticks.c things.c weapons.c wizard.c xcrypt.c mach_dep.c \
  "$COMPAT" $LDFLAGS $LIBS -o "$OUT/rogue52"
echo "built rogue52"
