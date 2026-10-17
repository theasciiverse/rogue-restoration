/*
 * Asciiverse: play a Restoration Project game on a fixed 80x24 field,
 * centred in the terminal.
 *
 * Included before every source file of a game (-include). The games lay
 * their dungeon out to fill LINES x COLS and keep it in curses windows
 * made with newwin(LINES, COLS, 0, 0): the real map on stdscr, what the
 * player sees on cw, the monsters on mw. Here LINES and COLS read 24 and
 * 80 (or the terminal's size, where that is smaller), every window the
 * game makes is placed at an offset that centres that field, and stdscr
 * becomes a window of the field's size at the same place. A resized
 * terminal moves the windows to its new centre and shows the picture
 * again; it never shrinks them, so nothing of the dungeon is lost, and a
 * terminal too small for the field shows its top left part until it is
 * made big enough again.
 *
 * The games' own logic is unchanged: they still see a terminal of
 * LINES x COLS with its corner at 0,0.
 */
#ifndef ASCIIVERSE_CENTRE_H
#define ASCIIVERSE_CENTRE_H

#include <curses.h>

/* the size of the field the game plays on, at most 24 x 80 */
extern int av_lines, av_cols;
/* the game's stdscr: a window of that size, centred */
extern WINDOW *av_stdscr;

WINDOW *av_initscr(void);
WINDOW *av_newwin(int lines, int cols, int y, int x);
WINDOW *av_subwin(WINDOW *orig, int lines, int cols, int y, int x);
int av_delwin(WINDOW *win);
int av_mvwin(WINDOW *win, int y, int x);
int av_wgetch(WINDOW *win);
/* follow a resized terminal now (also done while waiting for a key) */
void av_follow_resize(void);

#undef LINES
#define LINES av_lines
#undef COLS
#define COLS av_cols
#undef stdscr
#define stdscr av_stdscr
#undef initscr
#define initscr() av_initscr()
#undef newwin
#define newwin(l, c, y, x) av_newwin(l, c, y, x)
#undef subwin
#define subwin(w, l, c, y, x) av_subwin(w, l, c, y, x)
#undef delwin
#define delwin(w) av_delwin(w)
#undef mvwin
#define mvwin(w, y, x) av_mvwin(w, y, x)
#undef wgetch
#define wgetch(w) av_wgetch(w)
#undef getch
#define getch() av_wgetch(av_stdscr)
/* curses' printw() and scanw() are functions that use its own stdscr */
#undef printw
#define printw(...) wprintw(av_stdscr, __VA_ARGS__)
#undef mvprintw
#define mvprintw(y, x, ...) mvwprintw(av_stdscr, y, x, __VA_ARGS__)
#undef scanw
#define scanw(...) wscanw(av_stdscr, __VA_ARGS__)
#undef mvscanw
#define mvscanw(y, x, ...) mvwscanw(av_stdscr, y, x, __VA_ARGS__)

/*
 * The folder for the game's files (scores, saved games), made on first
 * use: <data home>/rogue-restoration/<game>/ with a trailing slash, where
 * the data home is $XDG_DATA_HOME, else ~/.local/share. Never NULL; the
 * empty string (the current folder) when there is no home folder.
 */
const char *av_datadir(const char *game);
/* av_datadir(game) followed by name, in a static buffer of its own */
const char *av_datafile(const char *game, const char *name);

#endif
