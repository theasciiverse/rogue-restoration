/*
 * Asciiverse: the fixed, centred field behind compat/centre.h, which has
 * the full description.
 */
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "centre.h"

/* the real calls from here on */
#undef LINES
#undef COLS
#undef stdscr
#undef initscr
#undef newwin
#undef subwin
#undef delwin
#undef mvwin
#undef wgetch
#undef getch

#define AV_FIELD_LINES 24
#define AV_FIELD_COLS 80
#define AV_MAX_WINDOWS 64

int av_lines = AV_FIELD_LINES, av_cols = AV_FIELD_COLS;
WINDOW *av_stdscr;

/* where the field's corner is on the terminal */
static int av_oy, av_ox;
static volatile sig_atomic_t av_winch;

/* every window the game made: its place and size on the field */
static struct {
	WINDOW *win;
	int y, x, lines, cols;
} av_wins[AV_MAX_WINDOWS];

static void
av_on_winch(int sig)
{
	(void)sig;
	av_winch = 1;
}

static void
av_remember(WINDOW *win, int y, int x, int lines, int cols)
{
	int i;

	for (i = 0; i < AV_MAX_WINDOWS; ++i) {
		if (av_wins[i].win == NULL || av_wins[i].win == win) {
			av_wins[i].win = win;
			av_wins[i].y = y;
			av_wins[i].x = x;
			av_wins[i].lines = lines;
			av_wins[i].cols = cols;
			return;
		}
	}
}

static void
av_place(void)
{
	av_oy = LINES > av_lines ? (LINES - av_lines) / 2 : 0;
	av_ox = COLS > av_cols ? (COLS - av_cols) / 2 : 0;
}

WINDOW *
av_initscr(void)
{
	if (av_stdscr == NULL) {
		struct sigaction sa;

		/*
		 * Our own SIGWINCH handler, set before curses starts, keeps
		 * ncurses from resizing the game's windows by itself: they
		 * are moved, never shrunk (av_follow_resize).
		 */
		memset(&sa, 0, sizeof sa);
		sa.sa_handler = av_on_winch;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGWINCH, &sa, NULL);
		if (initscr() == NULL)
			return NULL;
		av_lines = LINES < AV_FIELD_LINES ? LINES : AV_FIELD_LINES;
		av_cols = COLS < AV_FIELD_COLS ? COLS : AV_FIELD_COLS;
		av_place();
		av_stdscr = newwin(av_lines, av_cols, av_oy, av_ox);
		if (av_stdscr == NULL)
			av_stdscr = stdscr;
		else
			av_remember(av_stdscr, 0, 0, av_lines, av_cols);
	} else {
		/* a game that calls initscr() again after endwin() */
		refresh();
	}
	return av_stdscr;
}

WINDOW *
av_newwin(int lines, int cols, int y, int x)
{
	WINDOW *win;

	if (lines <= 0)
		lines = av_lines - y;
	if (cols <= 0)
		cols = av_cols - x;
	win = newwin(lines, cols, y + av_oy, x + av_ox);
	if (win != NULL)
		av_remember(win, y, x, lines, cols);
	return win;
}

WINDOW *
av_subwin(WINDOW *orig, int lines, int cols, int y, int x)
{
	WINDOW *win;

	if (lines <= 0)
		lines = av_lines - y;
	if (cols <= 0)
		cols = av_cols - x;
	win = subwin(orig, lines, cols, y + av_oy, x + av_ox);
	if (win != NULL)
		av_remember(win, y, x, lines, cols);
	return win;
}

int
av_delwin(WINDOW *win)
{
	int i;

	for (i = 0; i < AV_MAX_WINDOWS; ++i)
		if (av_wins[i].win == win)
			av_wins[i].win = NULL;
	if (win == av_stdscr)
		return OK;	/* the game's stdscr lives as long as curses */
	return delwin(win);
}

int
av_mvwin(WINDOW *win, int y, int x)
{
	int i;

	for (i = 0; i < AV_MAX_WINDOWS; ++i) {
		if (av_wins[i].win == win) {
			av_wins[i].y = y;
			av_wins[i].x = x;
		}
	}
	return mvwin(win, y + av_oy, x + av_ox);
}

void
av_follow_resize(void)
{
#ifdef TIOCGWINSZ
	struct winsize ws;
	WINDOW *snap;
	int i, lines, cols, old_oy = av_oy, old_ox = av_ox;

	if (!av_winch || av_stdscr == NULL || isendwin())
		return;
	av_winch = 0;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 || ws.ws_row == 0 ||
	    ws.ws_col == 0)
		return;
	/*
	 * curses is told a size of at least the field's, so it never cuts
	 * a window down; a smaller terminal shows the top left part.
	 */
	lines = ws.ws_row > av_lines ? ws.ws_row : av_lines;
	cols = ws.ws_col > av_cols ? ws.ws_col : av_cols;
	if (lines == LINES && cols == COLS)
		return;
	/* what is on the screen now, to show again at the new place */
	snap = newwin(av_lines, av_cols, 0, 0);
	if (snap != NULL)
		copywin(curscr, snap, old_oy, old_ox, 0, 0, av_lines - 1,
		    av_cols - 1, FALSE);
	resize_term(lines, cols);
	av_place();
	for (i = 0; i < AV_MAX_WINDOWS; ++i) {
		if (av_wins[i].win == NULL)
			continue;
		/* windows that filled the old screen were grown with it */
		wresize(av_wins[i].win, av_wins[i].lines, av_wins[i].cols);
		mvwin(av_wins[i].win, av_wins[i].y + av_oy,
		    av_wins[i].x + av_ox);
	}
	/* a blank screen (curses' own stdscr), and the picture on it */
	werase(stdscr);
	wnoutrefresh(stdscr);
	if (snap != NULL) {
		/* it filled a screen of the field's size: grown with it */
		wresize(snap, av_lines, av_cols);
		mvwin(snap, av_oy, av_ox);
		touchwin(snap);
		wnoutrefresh(snap);
		delwin(snap);
	}
	clearok(curscr, TRUE);
	doupdate();
#endif
}

int
av_wgetch(WINDOW *win)
{
	int c;
	int delay;

#if defined(NCURSES_VERSION_MAJOR) && \
    (NCURSES_VERSION_MAJOR > 5 || NCURSES_VERSION_MINOR >= 8)
	delay = wgetdelay(win);
#else
	delay = win->_delay;
#endif
	av_follow_resize();
	if (delay >= 0)
		return wgetch(win);	/* the game waits on its own terms */
	/*
	 * Wait for the key in short steps, so a resized terminal is
	 * followed at once rather than at the next key.
	 */
	for (;;) {
		wtimeout(win, 200);
		c = wgetch(win);
		wtimeout(win, -1);
		if (c != ERR)
			return c;
		av_follow_resize();
	}
}

const char *
av_datadir(const char *game)
{
	static char dir[1024];
	static char made[64];
	const char *base = getenv("XDG_DATA_HOME"), *home = getenv("HOME");
	char *p;

	if (strcmp(made, game) == 0)
		return dir;
	if (base != NULL && *base == '/')
		snprintf(dir, sizeof dir, "%s/rogue-restoration/%s/", base,
		    game);
	else if (home != NULL && *home != '\0')
		snprintf(dir, sizeof dir,
		    "%s/.local/share/rogue-restoration/%s/", home, game);
	else
		dir[0] = '\0';
	/* make each folder on the way, as mkdir -p does */
	for (p = dir + 1; *p != '\0'; ++p) {
		if (*p == '/') {
			*p = '\0';
			if (mkdir(dir, 0755) != 0 && errno != EEXIST) {
				*p = '/';
				break;
			}
			*p = '/';
		}
	}
	snprintf(made, sizeof made, "%s", game);
	return dir;
}

const char *
av_datafile(const char *game, const char *name)
{
	static char path[1100];

	snprintf(path, sizeof path, "%s%s", av_datadir(game), name);
	return path;
}
