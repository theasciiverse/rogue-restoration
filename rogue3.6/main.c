/*
 * Rogue
 * Exploring the dungeons of doom
 * Copyright (C) 1980 by Michael Toy and Glenn Wichman
 * All rights reserved
 *
 * @(#)main.c	3.27 (Berkeley) 6/15/81
 */

#include "curses.h"
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <fcntl.h>
#include "mach_dep.h"
#include "rogue.h"

#ifdef CHECKTIME
int num_checks;		/* times we've gone over in checkout() */
#endif

WINDOW *cw;			/* Window that the player sees */
WINDOW *hw;			/* Used for the help command */
WINDOW *mw;			/* Used to store mosnters */

int
main(argc, argv, envp)
int argc;
char **argv;
char **envp;
{
    register char *env;
    register struct passwd *pw;
    register struct linked_list *item;
    register struct object *obj;
    int lowtime;
    time_t now;

    md_init(MD_STRIP_CTRL_KEYPAD);

    /*
     * check for print-score option
     */
    if (argc == 2 && strcmp(argv[1], "-s") == 0)
    {
	waswizard = TRUE;
	score(0, -1, 0);
	exit(0);
    }
    /*
     * Check to see if he is a wizard
     */
    if (argc >= 2 && argv[1][0] == '\0')
	if (strcmp(PASSWD, crypt(md_getpass("Wizard's password: "), "mT")) == 0)
	{
	    wizard = TRUE;
	    argv++;
	    argc--;
	}

    /*
     * get home and options from environment
     */
    strncpy(home, md_gethomedir(), 80);
    home[79] = '\0';


    strcpy(file_name, home);
    strcat(file_name, "rogue.save");

    if ((env = getenv("ROGUEOPTS")) != NULL)
	parse_opts(env);
    if (env == NULL || whoami[0] == '\0')
	strucpy(whoami, md_getusername(), strlen(md_getusername()));
    if (env == NULL || fruit[0] == '\0')
	strcpy(fruit, "slime-mold");

#if MAXLOAD|MAXUSERS
    if (too_much() && !wizard && !author())
    {
	printf("Sorry, %s, but the system is too loaded now.\n", whoami);
	printf("Try again later.  Meanwhile, why not enjoy a%s %s?\n",
	    vowelstr(fruit), fruit);
	exit(1);
    }
#endif
    if (argc == 2)
	if (!restore(argv[1], envp)) /* Note: restore will never return */
	    exit(1);
    time(&now);
    lowtime = (int) now;

    env = getenv("SEED");

    if (env)
        seed = atoi(env);
    else
        seed = 0;

    if (seed > 0)
        dnum = seed;
    else
        dnum = lowtime + getpid();

    if (wizard || env)
	printf("Hello %s, welcome to dungeon #%d", whoami, dnum);
    else
	printf("Hello %s, just a moment while I dig the dungeon...", whoami);
    fflush(stdout);
    seed = dnum;

    init_player();			/* Roll up the rogue */
    init_things();			/* Set up probabilities of things */
    init_names();			/* Set up names of scrolls */
    init_colors();			/* Set up colors of potions */
    init_stones();			/* Set up stone settings of rings */
    init_materials();			/* Set up materials of wands */
    initscr();				/* Start up cursor package */

    if (COLS < 70)
    {
	endwin();
	printf("\n\nSorry, %s, but your terminal window has too few columns.\n", whoami);
	printf("Your terminal has %d columns, needs 70.\n",COLS);
	exit(1);
    }

    if (LINES < 22)
    {
	endwin();
	printf("\n\nSorry, %s, but your terminal window has too few lines.\n", whoami);
	printf("Your terminal has %d lines, needs 22.\n",LINES);
	exit(1);
    }

    setup();
    /*
     * Set up windows
     */
    cw = newwin(LINES, COLS, 0, 0);
    mw = newwin(LINES, COLS, 0, 0);
    hw = newwin(LINES, COLS, 0, 0);
    keypad(cw,1);
    waswizard = wizard;
    new_level();			/* Draw current level */
    /*
     * Start up daemons and fuses
     */
    start_daemon(doctor, 0, AFTER);
    fuse(swander, 0, WANDERTIME, AFTER);
    start_daemon(stomach, 0, AFTER);
    start_daemon(runners, 0, AFTER);
    /*
     * Give the rogue his weaponry.  First a mace.
     */
    item = new_item(sizeof *obj);
    obj = (struct object *) ldata(item);
    obj->o_type = WEAPON;
    obj->o_which = MACE;
    init_weapon(obj, MACE);
    obj->o_hplus = 1;
    obj->o_dplus = 1;
    obj->o_flags |= ISKNOW;
    add_pack(item, TRUE);
    cur_weapon = obj;
    /*
     * Now a +1 bow
     */
    item = new_item(sizeof *obj);
    obj = (struct object *) ldata(item);
    obj->o_type = WEAPON;
    obj->o_which = BOW;
    init_weapon(obj, BOW);
    obj->o_hplus = 1;
    obj->o_dplus = 0;
    obj->o_flags |= ISKNOW;
    add_pack(item, TRUE);
    /*
     * Now some arrows
     */
    item = new_item(sizeof *obj);
    obj = (struct object *) ldata(item);
    obj->o_type = WEAPON;
    obj->o_which = ARROW;
    init_weapon(obj, ARROW);
    obj->o_count = 25+rnd(15);
    obj->o_hplus = obj->o_dplus = 0;
    obj->o_flags |= ISKNOW;
    add_pack(item, TRUE);
    /*
     * And his suit of armor
     */
    item = new_item(sizeof *obj);
    obj = (struct object *) ldata(item);
    obj->o_type = ARMOR;
    obj->o_which = RING_MAIL;
    obj->o_ac = a_class[RING_MAIL] - 1;
    obj->o_flags |= ISKNOW;
    cur_armor = obj;
    add_pack(item, TRUE);
    /*
     * Give him some food too
     */
    item = new_item(sizeof *obj);
    obj = (struct object *) ldata(item);
    obj->o_type = FOOD;
    obj->o_count = 1;
    obj->o_which = 0;
    add_pack(item, TRUE);
    playit();
}

/*
 * endit:
 *	Exit the program abnormally.
 */

void
endit(signum)
int signum;
{
    fatal("Ok, if you want to exit that badly, I'll have to allow it\n");
}

/*
 * fatal:
 *	Exit the program, printing a message.
 */

int
fatal(s)
char *s;
{
    clear();
    move(LINES-2, 0);
    printw("%s", s);
    draw(stdscr);
    endwin();
    exit(0);
}

/*
 * rnd:
 *	Pick a very random number.
 */

int
rnd(range)
register int range;
{
    return range == 0 ? 0 : abs(RN) % range;
}

/*
 * roll:
 *	roll a number of dice
 */

int
roll(number, sides)
register int number, sides;
{
    register int dtotal = 0;

    while(number--)
	dtotal += rnd(sides)+1;
    return dtotal;
}
# ifdef SIGTSTP
/*
 * handle stop and start signals
 */
void
tstp(int signum)
{
#ifdef SIGTSTP
    signal(SIGTSTP, SIG_IGN);
#endif
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
    fflush(stdout);
#ifdef SIGTSTP
    signal(SIGTSTP, SIG_DFL);
    kill(0, SIGTSTP);
    signal(SIGTSTP, tstp);
#endif
    crmode();
    noecho();
    clearok(curscr, TRUE);
    touchwin(cw);
    draw(cw);
    flushinp();	/* flush input */
}
# endif

int
setup()
{
#ifdef CHECKTIME
    void  checkout(int);
#endif

#ifndef DUMP
#ifdef SIGHUP
    signal(SIGHUP, auto_save);
#endif
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
#ifdef SIGPIPE
    signal(SIGPIPE, auto_save);
#endif
    signal(SIGTERM, auto_save);
#endif

    signal(SIGINT, quit);
#ifndef DUMP
#ifdef SIGQUIT
    signal(SIGQUIT, endit);
#endif
#endif
#ifdef SIGTSTP
    signal(SIGTSTP, tstp);
#endif
#ifdef CHECKTIME
    if (!author())
    {
#ifdef SIGALRM
	signal(SIGALRM, checkout);
	alarm(CHECKTIME * 60);
#endif
	num_checks = 0;
    }
#endif
    crmode();				/* Cbreak mode */
    noecho();				/* Echo off */
}

/*
 * playit:
 *	The main loop of the program.  Loop until the game is over,
 * refreshing things and looking at the proper times.
 */

int
playit()
{
    register char *opts;

    /*
     * set up defaults for slow terminals
     */

    if (baudrate() < 1200)
    {
	terse = TRUE;
	jump = TRUE;
    }

    /*
     * parse environment declaration of options
     */
    if ((opts = getenv("ROGUEOPTS")) != NULL)
	parse_opts(opts);


    oldpos = hero;
    oldrp = roomin(&hero);
    while (playing)
	command();			/* Command execution */
    endit(0);
}

#if MAXLOAD|MAXUSERS
/*
 * see if the system is being used too much for this game
 */
int
too_much()
{
#ifdef MAXLOAD
    double avec[3];
#else
    register int cnt;
#endif

#ifdef MAXLOAD
    md_getloadavg(avec);
    return (avec[2] > (MAXLOAD / 10.0));
#else
    return (ucount() > MAXUSERS);
#endif
}

/*
 * see if a user is an author of the program
 */
int
author()
{
    switch (md_getuid())
    {
	case 24601:
	    return TRUE;
	default:
	    return FALSE;
    }
}
#endif

#ifdef CHECKTIME
void
checkout(int signum)
{
    static char *msgs[] = {
	"The load is too high to be playing.  Please leave in %d minutes",
	"Please save your game.  You have %d minutes",
	"Last warning.  You have %d minutes to leave",
    };
    int checktime;
#ifdef SIGALRM
    signal(SIGALRM, checkout);
#endif
    if (too_much())
    {
	if (num_checks == 3)
	    fatal("Sorry.  You took to long.  You are dead\n");
	checktime = CHECKTIME / (num_checks + 1);
	chmsg(msgs[num_checks++], checktime);
#ifdef SIGALRM
	alarm(checktime * 60);
#endif
    }
    else
    {
	if (num_checks)
	{
	    chmsg("The load has dropped back down.  You have a reprieve.");
	    num_checks = 0;
	}
#ifdef SIGALRM
	alarm(CHECKTIME * 60);
#endif
    }
}

/*
 * checkout()'s version of msg.  If we are in the middle of a shell, do a
 * printf instead of a msg to avoid the refresh.
 */
void
chmsg(char *fmt, ...)
{
    va_list arg;

    if (in_shell)
    {
	vprintf(fmt, arg);
	putchar('\n');
	fflush(stdout);
    }
    else
    {
	if (*fmt == '\0')
	{
	    wmove(cw, 0, 0);
	    wclrtoeol(cw);
	    mpos = 0;
	    return;
	}

	doadd(fmt, arg);
    }
}
#endif

#ifdef LOADAV

int
loadav(avg)
register double *avg;
{
    return getloadavg(avg,3);
}
#endif

#ifdef UCOUNT

#ifdef __CYGWIN__
#include <utmp.h>

int
ucount()
{
    struct utmp *up=NULL;
    int count=0;

    setutent();
    do
    {
	up = getutent();
	if (up && up->ut_type == USER_PROCESS)
	    count++;
    } while(up != NULL);

   endutent();

   return(count);
}
#else
#include <utmpx.h>

int
ucount()
{
    struct utmpx *up=NULL;
    int count=0;

    setutxent();    
    do
    {
	up = getutxent();
	if (up && up->ut_type == USER_PROCESS)
	    count++;
    } while(up != NULL);
   
   endutxent();

   return(count);
}
#endif
#endif

