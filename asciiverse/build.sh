#!/bin/sh
# Builds every game into out/: each asciiverse/build/<id>.sh compiles one
# game with the compiler, flags and curses library from the environment,
# so one script serves every target (see .github/workflows/release.yml):
#
#   asciiverse/build.sh                                   # Debian, Ubuntu
#   LIBS=-lncursesw LDFLAGS=-static asciiverse/build.sh   # Alpine, static
#   LIBS=-lncurses asciiverse/build.sh                    # macOS
#
# The games are 1980s C: built as gnu89, with globals in common blocks as
# their compilers had them. compat/centre.h, included before every source
# file, plays each on a fixed 80x24 field centred in the terminal and
# gives it its data folder (compat/centre.c).
set -eu
cd "$(dirname "$0")/.."
export CC="${CC:-cc}"
export CFLAGS="${CFLAGS:--O2} -std=gnu89 -fcommon -include $PWD/compat/centre.h -I$PWD/compat"
export LDFLAGS="${LDFLAGS:-}"
export LIBS="${LIBS:--lncursesw}"
export OUT="${OUT:-$PWD/out}"
export COMPAT="$PWD/compat/centre.c"
mkdir -p "$OUT"
for s in asciiverse/build/*.sh; do
  sh -eu "$s"
done
