# arogue: Advanced Rogue 5.8, the Bell Labs game of 1984 and 1985
cd arogue5.8
# much of it calls the C library undeclared: its headers first
$CC $CFLAGS -include stdlib.h -include string.h \
  vers.c chase.c command.c daemon.c daemons.c encumb.c fight.c init.c io.c \
  list.c main.c maze.c mdport.c misc.c monsters.c move.c new_level.c \
  options.c outside.c pack.c passages.c player.c potions.c rings.c rip.c \
  rogue.c rooms.c save.c scrolls.c state.c sticks.c things.c trader.c \
  util.c weapons.c wear.c wizard.c xcrypt.c \
  "$COMPAT" $LDFLAGS $LIBS -o "$OUT/arogue"
echo "built arogue"
