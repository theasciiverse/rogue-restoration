/*
 * Various installation dependent routines
 *
 * @(#)mach_dep.c	4.23 (Berkeley) 5/19/82
 */

/*
 * The various tuneable defines are:
 *
 *	SCOREFILE	Where/if the score file should live.
 *	MAXLOAD		What (if any) the maximum load average should be
 *			when people are playing.  If defined, then
 *		LOADAV		Should rogue define it's own routine to
 *				get the load average?
 *		NAMELIST	If so, where does the system namelist hide?
 *	MAXUSERS	What (if any) the maximum user count should be
 *			when people are playing.  If defined, then
 *		UCOUNT		Should rogue define it's own routine to
 *				count users?
 *		UTMP		If so, where does the user list hide?
 *	CHECKTIME	How often/if rogue should check during the game
 *			for high load average.
 */

#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "rogue.h"

#ifdef SCOREFILE
static char *lockfile = "/tmp/.roguelock";
#endif

#ifdef CHECKTIME
int num_checks;		/* times we've gone over in checkout() */
#endif

/*
 * init_check:
 *	Check out too see if it is proper to play the game now
 */
void
init_check()
{
#if defined(MAXLOAD) || defined(MAXUSERS)
    if (too_much())
    {
	printf("Sorry, %s, but the system is too loaded now.\n", whoami);
	printf("Try again later.  Meanwhile, why not enjoy a%s %s?\n",
	    vowelstr(fruit), fruit);
	if (author())
	    printf("However, since you're a good guy, it's up to you\n");
	else
	    exit(1);
    }
#endif
}

/*
 * open_score:
 *	Open up the score file for future use, and then
 *	setuid(getuid()) in case we are running setuid.
 */
void
open_score()
{
#ifdef SCOREFILE
    fd = open(SCOREFILE, 2);
#else
    fd = -1;
#endif
   
    md_normaluser();
}

/*
 * setup:
 *	Get starting setup for all games
 */
void
setup()
{
    /*
     * make sure that large terminals don't overflow the bounds
     * of the program
     */
    if (LINES > MAXLINES)
	LINES = MAXLINES;
    if (COLS > MAXCOLS)
	COLS = MAXCOLS;

#ifdef SIGHUP
    signal(SIGHUP, auto_save);
#endif
#ifndef DUMP
    signal(SIGILL, auto_save);
#ifdef SIGTRAP
    signal(SIGTRAP, auto_save);
#endif
#ifdef SIGIOT
    signal(SIGIOT, auto_save);
#endif
#ifdef SIGEMT
    signal(SIGEMT, auto_save);
#endif
    signal(SIGFPE, auto_save);
#ifdef SIGBUS
    signal(SIGBUS, auto_save);
#endif
    signal(SIGSEGV, auto_save);
#ifdef SIGSYS
    signal(SIGSYS, auto_save);
#endif
    signal(SIGTERM, auto_save);
#endif

    signal(SIGINT, quit);
#ifndef DUMP
#ifdef SIGQUIT
    signal(SIGQUIT, endit);
#endif
#endif
#ifdef CHECKTIME
    signal(SIGALRM, checkout);
    alarm(CHECKTIME * 60);
    num_checks = 0;
#endif
    crmode();				/* Cbreak mode */
    noecho();				/* Echo off */
}

/*
 * start_score:
 *	Start the scoring sequence
 */
void
start_score()
{
#ifdef CHECKTIME
#ifdef SIGALRM
    signal(SIGALRM, SIG_IGN);
#endif
#endif
}

/*
 * issymlink:
 *	See if the file has a symbolic link
 */
int
issymlink(sp)
char *sp;
{
#ifdef S_IFLNK
    struct stat sbuf2;

    if (lstat(sp, &sbuf2) < 0)
	return FALSE;
    else
	return ((sbuf2.st_mode & S_IFMT) != S_IFREG);
#else
    return FALSE;
#endif
}

#if defined(MAXLOAD) || defined(MAXUSERS)
/*
 * too_much:
 *	See if the system is being used too much for this game
 */
int
too_much()
{
#ifdef MAXLOAD
    double avec[3];

    if (md_getloadavg(avec) == 0)
        if (avec[2] > (MAXLOAD / 10.0))
	    return(1);
#else
#ifdef MAXUSERS
    if (md_ucount() > MAXUSERS)
	return(1) ;
#endif
#endif
    return(0);
}

/*
 * author:
 *	See if a user is an author of the program
 */
int
author()
{
#ifdef WIZARD
    if (wizard)
	return TRUE;
#endif
    switch (md_getuid())
    {
	case 162:
	    return TRUE;
	default:
	    return FALSE;
    }
}
#endif

#ifdef CHECKTIME
/*
 * checkout:
 *	Check each CHECKTIME seconds to see if the load is too high
 */
void
checkout(int sig)
{
    static char *msgs[] = {
	"The load is too high to be playing.  Please leave in %0.1f minutes",
	"Please save your game.  You have %0.1f minutes",
	"Last warning.  You have %0.1f minutes to leave",
    };
    int checktime;

#ifdef SIGALRM
    signal(SIGALRM, checkout);
#endif
    if (too_much())
    {
	if (author())
	{
	    num_checks = 1;
	    chmsg("The load is rather high, O exaulted one");
	}
	else if (num_checks++ == 3)
	    fatal("Sorry.  You took to long.  You are dead\n");
	checktime = (CHECKTIME * 60) / num_checks;
#ifdef SIGALRM
	alarm(checktime);
#endif
	chmsg(msgs[num_checks - 1], ((double) checktime / 60.0));
    }
    else
    {
	if (num_checks)
	{
	    num_checks = 0;
	    chmsg("The load has dropped back down.  You have a reprieve");
	}
#ifdef SIGALRM
	alarm(CHECKTIME * 60);
#endif
    }
}

/*
 * chmsg:
 *	checkout()'s version of msg.  If we are in the middle of a
 *	shell, do a printf instead of a msg to avoid the refresh.
 */
void
chmsg(char *fmt, ...)
{
    va_list ap;

    if (in_shell)
    {
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	putchar('\n');
	fflush(stdout);
    }
    else
    {
	if (*fmt == '\0')
	{
	    move(0, 0);
	    clrtoeol();
	    mpos = 0;
	    return;
	}
	va_start(ap, fmt);
	doadd(fmt, ap);
	va_end(ap);
	endmsg();
    }
}
#endif

#ifdef LOADAV

#ifdef BSD
#include <nlist.h>

struct nlist avenrun = {
    "_avenrun"
};
#endif

/*
 * loadav:
 *	Looking up load average in core (for system where the loadav()
 *	system call isn't defined
 */
void
loadav(avg)
register double *avg;
{
#ifdef BSD
    register int kmem;

    if ((kmem = open("/dev/kmem", 0)) < 0)
	goto bad;
    nlist(NAMELIST, &avenrun);
    if (avenrun.n_type == 0)
    {
bad:
	avg[0] = avg[1] = avg[2] = 0.0;
	return;
    }

    lseek(kmem, (long) avenrun.n_value, 0);
    read(kmem, (char *) avg, 3 * sizeof (double));
#else
    getloadavg(avg,3);
#endif
}
#endif

#ifdef UCOUNT
/*
 * ucount:
 *	Count number of users on the system
 */
#include <utmp.h>

struct utmp buf;

int
ucount()
{
    register struct utmp *up;
    register FILE *utmp;
    register int count;

    if ((utmp = fopen(UTMP, "r")) == NULL)
	return 0;

    up = &buf;
    count = 0;

    while (fread(up, 1, sizeof (*up), utmp) > 0)
	if (buf.ut_name[0] != '\0')
	    count++;
    fclose(utmp);
    return count;
}
#endif

/*
 * lock_sc:
 *	lock the score file.  If it takes too long, ask the user if
 *	they care to wait.  Return TRUE if the lock is successful.
 */
int
lock_sc()
{
#ifdef SCOREFILE
    register int cnt;
    static struct stat sbuf;
    time_t time();

over:
    if (creat(lockfile, 0000) > 0)
	return TRUE;
    for (cnt = 0; cnt < 5; cnt++)
    {
	md_sleep(1);
	if (creat(lockfile, 0000) > 0)
	    return TRUE;
    }
    if (stat(lockfile, &sbuf) < 0)
    {
	creat(lockfile, 0000);
	return TRUE;
    }
    if (time(NULL) - sbuf.st_mtime > 10)
    {
	if (md_unlink(lockfile) < 0)
	    return FALSE;
	goto over;
    }
    else
    {
	printf("The score file is very busy.  Do you want to wait longer\n");
	printf("for it to become free so your score can get posted?\n");
	printf("If so, type \"y\"\n");
	if (fgets(prbuf, MAXSTR, stdin) == NULL)
	    prbuf[0] = '\0';
	if (prbuf[0] == 'y')
	    for (;;)
	    {
		if (creat(lockfile, 0000) > 0)
		    return TRUE;
		if (stat(lockfile, &sbuf) < 0)
		{
		    creat(lockfile, 0000);
		    return TRUE;
		}
		if (time(NULL) - sbuf.st_mtime > 10)
		{
		    if (md_unlink(lockfile) < 0)
			return FALSE;
		}
		md_sleep(1);
	    }
	else
	    return FALSE;
    }
#endif
}

/*
 * unlock_sc:
 *	Unlock the score file
 */
void
unlock_sc()
{
#ifdef SCOREFILE
    md_unlink(lockfile);
#endif
}

/*
 * flush_type:
 *	Flush typeahead for traps, etc.
 */
void
flush_type()
{
    register int flag;

#if !defined(_XOPEN_CURSES) && !defined(__NCURSES_H)
    flag = _tty.sg_flags;
    _tty.sg_flags |= RAW;
    stty(_tty_ch, &_tty);
    _tty.sg_flags = flag;
    stty(_tty_ch, &_tty);
#else
    flushinp();
#endif
}
