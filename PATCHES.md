# Changes from upstream

This is the Asciiverse packaging of the Roguelike Restoration Project's
games: Nicholas J. Kisseberth's (Yendor) restorations, for modern
systems, of Rogue 3.6, Rogue 5.2, Super-Rogue 9.0, Advanced Rogue 5.8,
UltraRogue and XRogue 8.0. In the Asciiverse catalog each game is its
own title, credited as part of the Roguelike Restoration Project.

## What was imported

Each game sits in a folder of its own, imported unchanged:

- `rogue3.6/`: github.com/rogueforge/rogue3.6 (remote `rogue3.6`) at
  `5611381` (2022-12-11) with its whole history, from the 1981 Berkeley
  source through releases 3.6.1 to 3.6.5 (tag `rogue3.6.5`) and three
  fixes from 2022; moved into its folder by the first commit after it.
- `rogue5.2/`: github.com/rogueforge/rogue5.2 (remote `rogue5.2`) at
  `a7c5acb`, release 5.2.3 (tag `rogue5.2.3`), with its history.
- `srogue/`: `srogue9.0-1-src.tar.gz` (113319 bytes, sha256
  `d307975aecffef48ae034b1f9acfe84fa822db59835927ef698fda61e0072cc6`),
  the release of 2005-03-22 from rogue.rogueforge.net (Wayback Machine;
  britzl/roguearchive has the same file), tag `srogue9.0-1`; then the
  project's Subversion trunk at r1490, merged from
  github.com/RoguelikeRestorationProject/srogue (remote `rrp-srogue`).
- `arogue5.8/`: `arogue5.8.2-src.tar.gz` (208014 bytes, sha256
  `41da7942efde823dcdca6b6088fdcb27957d0e9280c8aa4eed4a6a39d4fa414a`),
  the release of 2006-02-13 (Wayback Machine), tag `arogue5.8.2`; then
  trunk r1490 from github.com/RoguelikeRestorationProject/arogue5.8
  (remote `rrp-arogue5.8`).
- `urogue/`: `urogue1.0.7-src.tar.gz` (249041 bytes, sha256
  `ce58e25234e75f3920052e28b809513fe5a73dcd308bb8a48aec180059f930fe`),
  the release of 2005-02-07 (Wayback Machine and britzl/roguearchive),
  tag `urogue1.0.7`; then trunk r1490 from
  github.com/RoguelikeRestorationProject/urogue (remote `rrp-urogue`).
- `xrogue/`: github.com/rogueforge/xrogue8.0 (remote `xrogue8.0`) at
  `4b66eb2` with its history: releases 8.0.1 to 8.0.4 (tags
  `xrogue8.0.3`, `xrogue8.0.4`), the 2023 portability work and four
  fixes after 8.0.4.

rogueforge.github.io names github.com/rogueforge/arogue5.8, srogue and
urogue as their homes, but those repositories do not exist (yet); the
Subversion trunk is the newest source of the three. Everything after
the imports is ours.

## Shared

- `asciiverse/build.sh` runs one script per game in `asciiverse/build/`
  (or only the games named), one compiler run each, as gnu89 with
  `-fcommon` and `-fwrapv` (the random number generators multiply a
  signed seed and count on it wrapping); with clang, the errors newer
  clangs make of 1980s C stay warnings. `asciiverse/package.sh`, from
  NetBSD games, packs one archive per game from `asciiverse/archive/`;
  `.github/workflows/release.yml` builds static musl Linux (x86_64,
  aarch64) and macOS (x86_64, arm64).
- `compat/centre.h` and `compat/centre.c`, included before every source
  file: the games lay their dungeon out to fill `LINES` x `COLS` and keep
  it in curses windows, the real map on `stdscr`. `LINES` and `COLS`
  read 24 and 80 (or the terminal's size where that is smaller), every
  window is made at the offset that centres that field, `stdscr` is a
  window of the field's size there, and `printw()` and `scanw()` go to
  it. A SIGWINCH handler of ours keeps ncurses from cutting the windows
  down; a resized terminal moves them to its new centre and shows the
  picture again while the game waits for a key, and a terminal smaller
  than the field shows its top left part until it grows again.
- `av_datadir()` and `av_datafile()` give each game its folder,
  `rogue-restoration/<id>` under `$XDG_DATA_HOME`, else
  `~/.local/share`. Nothing is setuid and nothing is written to system
  folders.
- The player's name comes from `$LOGNAME` or `$USER` before the password
  database in every game, as in NetBSD rogue.
- `asciiverse/record/`: the preview drivers (`rec.py`, from NetBSD
  games', which with `REC_BIN` also plays an AddressSanitizer build as a
  test; `rlbot.py`, a player shared by the six; one driver per game) and
  the casts in `asciiverse/casts/`.

## rogue36 (Rogue 3.6)

- Build: the Makefile's files, score file `rogue36.scr`; without a
  `config.h`, mdport.c leaves out the shell escape and the setuid code.
- Files: score file and saved game (`rogue.save`) in the data folder;
  the score file is made at the first score (no `O_CREAT` kept no scores
  on a new install); `file_name` holds 1024 bytes. `rogue36 -r` restores
  the default saved game; a bare name not in the current folder is
  looked for in the data folder.
- `NCOLORS`, `NSTONES`, `NWOOD` and `NMETAL` divided arrays of `char[15]`
  by `sizeof(char *)`: every new game read and wrote past the name tables
  (AddressSanitizer: global-buffer-overflow in `init_colors()`).
- Restoring: the inode and change-time checks compared with a stat that
  is never saved, so every restore failed ("Sorry, saved game is not in
  the same file"); left out, as in 3.6.4. `rs_read_daemons()` cleared
  `d_list[20]`, one past the end; `rs_read_room_reference()` turned the
  -1 for no room into `&rooms[-1]`.
- `get_str()` stored typed characters past its 80-byte buffer.

## rogue52 (Rogue 5.2)

- Build: the Makefile's files with `WIZARD` and score file
  `rogue52.scr`, without `DUMP` and the load limits that read
  `/dev/kmem`; the suspend handler moves curses' cursor with `wmove()`
  instead of writing ncurses' opaque fields.
- Files: score file, its lock (was `/tmp/.roguelock`) and saved game
  (`rogue.save`) in the data folder; the score file is made at the first
  score; `file_name` and `save_game()`'s copy hold 1024 bytes; `$HOME` is
  cut to fit. `rogue52 -r` as before, bare names also from the data
  folder, and no `strcpy()` of `file_name` onto itself.
- `score()`: with no older score of the player's, the table was moved
  down starting at `top_ten[10]`, one past the end (AddressSanitizer:
  global-buffer-overflow at the first death).

## srogue (Super-Rogue 9.0)

- Build: the Makefile's files, with `<stdlib.h>` and `<string.h>` read
  first for its undeclared library calls.
- Files: score file (`srogue.scr`, was in `$ROGUEHOME`, the first of
  several system folders, or the current folder) and saved game
  (`srogue.sav`) in the data folder; 1024-byte paths; `$HOME` cut to
  fit. New: `srogue -r` restores the saved game, as in Rogue 3.6 and
  5.2; bare names also from the data folder.
- `roguehome()` copied `$ROGUEHOME` with a limit of `PATH_MAX - 20` into
  a 1024-byte buffer.
- 64-bit: `charge_str()` and `ring_num()` return pointers but were called
  undeclared, so names of identified wands and rings went through a
  pointer cut to 32 bits; the shell escape ended `execl()` with an int 0;
  the top ten printed a pointer difference with `%d`.

## arogue (Advanced Rogue 5.8)

- Build: the Makefile's files with `<stdlib.h>` and `<string.h>` first;
  mdport.c reads `<term.h>` (Debian and Ubuntu have no
  `<ncurses/term.h>`); `find_slot()` is declared.
- Files: score file (`arogue58.scr`) and saved game (`arogue58.sav`) in
  the data folder; 1024-byte paths; `arogue -r` as before, bare names
  also from the data folder, no `strcpy()` onto itself.
- 64-bit: daemons kept their argument in an int, so `&player` for
  `doctor()` was cut to 32 bits and the first turn crashed
  (AddressSanitizer: SEGV in `doctor()`). `d_arg` is a long, `daemon()`
  and `fuse()` have prototypes, and a restore points `doctor()` at the
  player again.
- Score file: logins are written as 80 bytes but were read into an
  8-byte field, past the end of the table for the last entry; five
  shorts were read with `%d`.
- `status()` built its line in 80 bytes; `md_getroguedir()` had the
  `PATH_MAX - 20` limit on a 1024-byte buffer; `md_shellescape()` ended
  `execl()` with an int 0.

## urogue (UltraRogue)

- Build: the Makefile's files plus mdport.c and xcrypt.c, which the
  trunk needs and its Makefile does not list. rogue.h reads `<unistd.h>`
  and renames the game's `daemon` type (it met `daemon(3)`);
  dictutil.c's `trace` is `dict_trace` (it met curses' `trace()`);
  `mem_validate()` is defined as declared; mdport.c reads `<term.h>`.
- Files: score file (was in the current folder), saved game (there was
  no default, so `-r` restored nothing) and character file (was
  `~/urogue.chr`) in the data folder; a bare name typed when saving is a
  file there, and bare names given to restore are looked for there.
  1024-byte paths; the name from `$LOGNAME`, then `$USER`, cut to fit.
- Saved games did not restore: `ur_read_room()` read an int as a short
  and put everything after it two bytes off (assertion failures,
  absurd allocations); `ur_write_object()` flagged stacks it did not
  write; `ur_write_window()` used ncurses' opaque `_maxy` and `_maxx`,
  the last line and column, so those were never saved.
- The tombstone printed the year as `19` and `tm_year` (2026 read
  "19126"); `md_getroguedir()` had the `PATH_MAX - 20` limit.

## xrogue (XRogue 8.0)

- Build: the Makefile's files.
- Files: score file (was `/usr/games/lib/xrogue_roll`) and saved game
  (`xrogue.save`, was in the home folder) in the data folder; 1024-byte
  paths; `$HOME` and the name cut to fit. `xrogue -r` as before, bare
  names also from the data folder, no `strcpy()` onto itself.
- Saved games did not restore: `restore()` seeked back to the start
  after the version string, from when the whole data segment was saved,
  and read every later field out of place ("Cannot restart the game on
  a smaller screen").
- The score file wrote each score in 8 bytes on 64-bit machines and
  read 4, so `xrogue -s` crashed once the file held a score
  (AddressSanitizer: SEGV in `score()`); 4 bytes again.

## Licences

Each archive carries its game's licence. Rogue 3.6, Rogue 5.2 and
XRogue are under the 3-clause BSD licence. Super-Rogue, Advanced Rogue
and UltraRogue add two clauses to it: their names may not be used to
endorse or promote derived products, and "Products derived from this
software may not be called" by their names "without prior written
permission".

Nicholas J. Kisseberth gave that permission on 2026-10-21: "You can use
the names if the only modifications are to get them to play in your
environment." Every change to these three listed above is of that kind
(building, file locations, the centred screen, 64-bit and memory fixes,
saved games that restore); keep it so. A change to how they play needs
a new name, or asking him again.
