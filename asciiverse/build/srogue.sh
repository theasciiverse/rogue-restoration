# srogue: Super-Rogue 9.0, Robert D. Kindelberger's Bell Labs game
cd srogue
# K&R code that calls the C library undeclared: its headers first
$CC $CFLAGS -include stdlib.h -include string.h \
  vers.c armor.c chase.c command.c daemon.c daemons.c disply.c encumb.c \
  fight.c global.c init.c io.c list.c main.c misc.c monsters.c move.c \
  new_leve.c options.c pack.c passages.c potions.c pstats.c rings.c rip.c \
  rooms.c save.c scrolls.c state.c sticks.c things.c trader.c weapons.c \
  wizard.c xcrypt.c \
  "$COMPAT" $LDFLAGS $LIBS -o "$OUT/srogue"
echo "built srogue"
