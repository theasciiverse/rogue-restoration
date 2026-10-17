# xrogue: XRogue 8.0, Robert Pietkivitch's game of 1989 to 1992
cd xrogue
$CC $CFLAGS \
  vers.c actions.c bolt.c chase.c command.c daemon.c daemons.c eat.c \
  effects.c fight.c encumb.c help.c init.c io.c list.c main.c maze.c misc.c \
  monsters.c mons_def.c move.c n_level.c options.c outside.c pack.c \
  passages.c player.c potions.c rings.c rip.c rooms.c save.c scrolls.c \
  sticks.c things.c trader.c util.c weapons.c wear.c wizard.c rogue.c \
  state.c xcrypt.c \
  "$COMPAT" $LDFLAGS $LIBS -o "$OUT/xrogue"
echo "built xrogue"
