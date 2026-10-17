#!/bin/sh
# Builds every game into out/: each asciiverse/build/<id>.sh compiles one
# game with the compiler, flags and curses library from the environment,
# so one script serves every target (see .github/workflows/release.yml):
#
#   asciiverse/build.sh                                   # Debian, Ubuntu
#   LIBS=-lncursesw LDFLAGS=-static asciiverse/build.sh   # Alpine, static
#   LIBS=-lncurses asciiverse/build.sh                    # macOS
#   asciiverse/build.sh rogue52 xrogue                    # only these
#
# Each game is one compiler run, one game after another.
#
# The games are 1980s C: built as gnu89, with globals in common blocks as
# their compilers had them, and with -fwrapv, since their random number
# generators count on signed overflow wrapping. compat/centre.h, included
# before every source file, plays each on a fixed 80x24 field centred in
# the terminal and gives it its data folder (compat/centre.c).
set -eu
cd "$(dirname "$0")/.."
export CC="${CC:-cc}"
# Newer clang makes some of what 1980s C does errors even in gnu89 mode
# (a bare return in an int function, mismatched function pointers);
# they stay warnings, as they were.
LENIENT="-Wno-error=return-mismatch -Wno-error=incompatible-function-pointer-types -Wno-error=int-conversion -Wno-error=implicit-function-declaration -Wno-error=implicit-int -Wno-unknown-warning-option"
case "$($CC --version 2>/dev/null)" in *clang*) ;; *) LENIENT= ;; esac
export CFLAGS="${CFLAGS:--O2} -std=gnu89 -fcommon -fwrapv $LENIENT -include $PWD/compat/centre.h -I$PWD/compat"
export LDFLAGS="${LDFLAGS:-}"
export LIBS="${LIBS:--lncursesw}"
export OUT="${OUT:-$PWD/out}"
export COMPAT="$PWD/compat/centre.c"
mkdir -p "$OUT"
if [ $# -gt 0 ]; then
  for id in "$@"; do sh -eu "asciiverse/build/$id.sh"; done
else
  for s in asciiverse/build/*.sh; do sh -eu "$s"; done
fi
