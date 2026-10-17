# rogue36: Rogue 3.6, the 1981 Berkeley game
cd rogue3.6
$CC $CFLAGS -DSCOREFILE='"rogue36.scr"' \
  vers.c armor.c chase.c command.c daemon.c daemons.c fight.c init.c io.c \
  list.c main.c mdport.c misc.c monsters.c move.c new_level.c options.c \
  pack.c passages.c potions.c rings.c rip.c rooms.c save.c scrolls.c \
  state.c sticks.c things.c weapons.c wizard.c \
  "$COMPAT" $LDFLAGS $LIBS -o "$OUT/rogue36"
echo "built rogue36"
