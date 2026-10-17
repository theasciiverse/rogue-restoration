# urogue: UltraRogue, Herb Chong's game of 1985 to 1995
cd urogue
# the Makefile's files, and mdport.c and xcrypt.c, which the trunk needs
# and its Makefile does not list
$CC $CFLAGS \
  armor.c artifact.c bag.c chase.c command.c daemon.c daemons.c dict.c \
  dictutil.c encumb.c fight.c getplay.c ident.c init.c io.c list.c magic.c \
  main.c maze.c memory.c misc.c monsdata.c monsters.c move.c newlvl.c \
  options.c pack.c passages.c player.c potions.c random.c rings.c rip.c \
  rooms.c save.c scrolls.c state.c status.c sticks.c things.c trader.c \
  verify.c vers.c weapons.c wizard.c mdport.c xcrypt.c \
  "$COMPAT" $LDFLAGS $LIBS -o "$OUT/urogue"
echo "built urogue"
