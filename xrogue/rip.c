/* Print flags for scoring */

#define REALLIFE 1      /* Print out machine and logname */
#define EDITSCORE 2     /* Edit the current score file */
#define ADDSCORE 3      /* Add a new score */

#define NAMELEN 80

/*
 * File for the fun ends
 * Death or a total win
 */

#include <curses.h>
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#ifdef BSD
#include <sys/time.h>
#else
#include <time.h>
#endif
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#ifndef _WIN32
#include <pwd.h>
#endif
#include <fcntl.h>
#include "mach_dep.h"
#include "network.h"
#include "rogue.h"
#ifdef PC7300
#include <sys/window.h>
extern struct uwdata wdata, oldwin;
extern char oldtext[WTXTNUM][WTXTLEN];
#endif

#ifndef SYSTEM
#include <sys/utsname.h>

struct utsname ourname;
#endif

#ifdef NUMNET
/* Network machines (for mutual score keeping) */
struct network Network[NUMNET] = {
    { "NULL", "NULL" },
};
#endif

/*
 * If you change this structure, change the compatibility routines
 * scoreout() and scorein() to reflect the change.  Also update SCORELEN.
 */

struct sc_ent {
    unsigned long       sc_score;
    char        sc_name[NAMELEN];
    char        sc_system[SYSLEN];
    char        sc_login[LOGLEN];
    short       sc_flags;
    short       sc_level;
    short       sc_ctype;
    short       sc_monster;
    short       sc_quest;
};
#define SCORELEN \
    (sizeof(unsigned long) + NAMELEN + SYSLEN + LOGLEN + 5*sizeof(short))

static void scorein(unsigned char *input, struct sc_ent scores[], long num_bytes);
static void scoreout(struct sc_ent scores[], unsigned char *output);
static int update(struct sc_ent top_ten[], unsigned long amount, short quest, char *whoami, short flags, short level, short monst, short ctype, char *system, char *login);

static char *rip[] = {
"                      ___________",
"                     /           \\",
"                    /             \\",
"                   /    R. I. P.   \\",
"                  /                 \\",
"                 /                   \\",
"                 |                   |",
"                 |                   |",
"                 |     killed by     |",
"                 |                   |",
"                 |                   |",
"                 |                   |",
"                *|      *  *  *      |*",
"       _________)|//\\\\///\\///\\//\\//\\/|(_________",
NULL
};

/*UNUSED*/
void
byebye(int n)
{
#ifdef BSD
    extern bool _endwin;

    if (!_endwin)
#endif
    {
        clear();
        endwin();
    }
#ifdef PC7300
    endhardwin();
#endif
    printf("\n");
    exit(0);
}


/*
 * death:
 *      Do something really fun when he dies
 */

void
death(register short monst)
{
    register char **dp = rip, *killer;
    register struct tm *lt;
    time_t date;
    char buf[LINELEN];

    time(&date);
    lt = localtime(&date);
    clear();
    move(8, 0);
    while (*dp)
        printw("%s\n", *dp++);
    mvaddstr(14, 28-(((int)strlen(whoami)+1)/2), whoami);
    sprintf(buf, "%lu Points", pstats.s_exp );
    mvaddstr(15, 28-(((int)strlen(buf)+1)/2), buf);
    killer = killname(monst);
    mvaddstr(17, 28-(((int)strlen(killer)+1)/2), killer);
    mvaddstr(18, 25, (sprintf(prbuf, "%4d", 1900+lt->tm_year), prbuf));
    move(lines-1, 0);
    refresh();
    score(pstats.s_exp, KILLED, monst);
    endwin();
#ifdef PC7300
    endhardwin();
#endif
    exit(0);
}

#ifdef PC7300

/*
 * Restore window characteristics on a hard window terminal (PC7300).
 */

endhardwin()
{
    register int i;
    struct utdata labelbuf;

    /* Restore the old window size */
    if (oldwin.uw_width) ioctl(1, WIOCSETD, &oldwin);

    /* Restore the old window text */
    for (i=0; i<WTXTNUM; i++) {
        labelbuf.ut_num = i;
        strcpy(labelbuf.ut_text, oldtext[i]);
        ioctl(1, WIOCSETTEXT, &labelbuf);
    }
}
#endif

char *
killname(register short monst)
{
    static char mons_name[LINELEN/2];
    int i;

    if (monst > NUMMONST) return("a strange monster");

    if (monst >= 0) {
        switch (monsters[monst].m_name[0]) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                sprintf(mons_name, "an %s", monsters[monst].m_name);
                break;
            default:
                sprintf(mons_name, "a %s", monsters[monst].m_name);
        }
        return(mons_name);
    }
    for (i = 0; i< DEATHNUM; i++) {
        if (deaths[i].reason == monst)
            break;
    }
    if (i >= DEATHNUM)
        return ("strange death");
    return (deaths[i].name);
}

/*
 * score -- figure score and post it.
 */

/* VARARGS2 */
void
score(unsigned long amount, int flags, short monst)
{
    static struct sc_ent top_ten[NUMSCORE] = { 0 };
    register struct sc_ent *scp;
    register int i;
    register struct sc_ent *sc2;
    register int outfd;
    register char *killer;
    register int prflags = 0;
    register int fd;
    long num_read;      
    short upquest=0, wintype=0, uplevel=0, uptype=0;    /* For network updating */
    char upsystem[SYSLEN], uplogin[LOGLEN];
    char *thissys;      /* Holds the name of this system */
    char *compatstr=NULL; /* Holds scores for writing compatible score files */
#define REASONLEN 3
    static char *reason[] = {
        "killed",
        "quit",
        "A total winner",
        "somehow left",
    };
    char *packend;

    signal(SIGINT, byebye);
    if (level == 0 && max_level == 0) 
        amount = 0; /*don't count if quit early */
    if (flags != WINNER && flags != SCOREIT && flags != UPDATE) {
        if (flags == CHICKEN) {
            packend = "when you quit";
	    amount = amount / 100;
	    }
        else
            packend = "at your untimely demise";
        mvaddstr(lines - 1, 0, retstr);
        refresh();
        getstr(prbuf);
        showpack(packend);
    }
    purse = 0;  /* Steal all the gold */

    /*
     * Open file and read list
     */

#ifdef BSD
    if ((fd = open(score_file, O_RDWR, 0666)) < 0) return;
#else
    if ((fd = open(score_file, O_CREAT|O_RDWR, 0666)) < 0) return;
#endif
    outfd = fd;

    thissys = md_gethostname();

    for (scp = top_ten; scp < &top_ten[NUMSCORE]; scp++)
    {
        scp->sc_score = 0L;
        for (i = 0; i < NAMELEN; i++)
            scp->sc_name[i] = rnd(255);
        scp->sc_quest= RN;
        scp->sc_flags = RN;
        scp->sc_level = RN;
        scp->sc_monster = RN;
        scp->sc_ctype = 0;
        strxcpy(scp->sc_system, thissys, SYSLEN);
        scp->sc_login[0] = '\0';
    }

    /*
     * If this is a SCOREIT optin (rogue -s), don't call byebye.  The
     * endwin() calls in byebye() will and this results in a core dump.
     */
    if (flags == SCOREIT) signal(SIGINT, SIG_DFL);
    else signal(SIGINT, byebye);

    if (flags != SCOREIT && flags != UPDATE)
    {
        mvaddstr(lines - 1, 0, retstr);
        refresh();
        fflush(stdout);
        getstr(prbuf);
    }

    /* Check for special options */
    if (strcmp(prbuf, "names") == 0)
        prflags = REALLIFE;
#ifdef WIZARD
    else if (wizard) {
        if (strcmp(prbuf, "edit") == 0) prflags = EDITSCORE;
        else if (strcmp(prbuf, "add") == 0) {
            prflags = ADDSCORE;
            waswizard = FALSE;  /* We want the new score recorded */
        }
    }
#endif

    /* Read the score and convert it to a compatible format */
    compatstr = malloc(NUMSCORE * SCORELEN);
    if (compatstr == NULL) return;
    num_read = encread(compatstr, (long)(NUMSCORE * SCORELEN), fd); 
    scorein((unsigned char *)compatstr, top_ten, num_read);      /* Convert it */

#if NUMNET
    /* Get some values if this is an update */
    if (flags == UPDATE) {
        unsigned long netread();
        int errcheck, errors = 0;

        upquest = (short) netread(&errcheck, sizeof(short), stdin);
        if (errcheck) errors++;

        if (fread(whoami, 1, NAMELEN, stdin) != NAMELEN) errors++;

        wintype = (short) netread(&errcheck, sizeof(short), stdin);
        if (errcheck) errors++;

        uplevel = (short) netread(&errcheck, sizeof(short), stdin);
        if (errcheck) errors++;

        uptype = (short) netread(&errcheck, sizeof(short), stdin);
        if (errcheck) errors++;

        if (fread(upsystem, 1, SYSLEN, stdin) != SYSLEN)
                errors++;
        if (fread(uplogin, 1, LOGLEN, stdin) != LOGLEN)
                errors++;
        
        if (errors) {
            close(outfd);
            free(compatstr);
            return;
        }
    }
#endif

    /*
     * Insert player in list if need be
     */
    if (!waswizard) {
        char *login;

        if (flags != UPDATE) {
#if !MSDOS && !_WIN32
            struct passwd *pp;
            if ((pp = getpwuid(getuid())) == NULL) login = "";
            else login = pp->pw_name;
#else
            login = "another rogue fiend";
#endif
        }

        if (flags == UPDATE)
            (void) update(top_ten, amount, upquest, whoami, wintype,
                   uplevel, monst, uptype, upsystem, uplogin);
        else {
#ifdef WIZARD
            if (prflags == ADDSCORE) {  /* Overlay characteristic by new ones */
                char buffer[LINELEN];

                clear();
                mvaddstr(1, 0, "Score: ");
                mvaddstr(2, 0, "Quest (number): ");
                mvaddstr(3, 0, "Name: ");
                mvaddstr(4, 0, "System: ");
                mvaddstr(5, 0, "Login: ");
                mvaddstr(6, 0, "Level: ");
                mvaddstr(7, 0, "Char type: ");
                mvaddstr(8, 0, "Result: ");

                /* Get the score */
                move(1, 7);
                get_str(buffer, stdscr);
                amount = atol(buffer);

                /* Get the character's quest -- must be a number */
                move(2, 16);
                get_str(buffer, stdscr);
                quest_item = atoi(buffer);

                /* Get the character's name */
                move(3, 6);
                get_str(buffer, stdscr);
                strxcpy(whoami, buffer, NAMELEN);

                /* Get the system */
                move(4, 8);
                get_str(buffer, stdscr);
                strxcpy(thissys, buffer, SYSLEN);

                /* Get the login */
                move(5, 7);
                get_str(buffer, stdscr);
                strxcpy(login, buffer, LOGLEN);

                /* Get the level */
                move(6, 7);
                get_str(buffer, stdscr);
                level = max_level = (short) atoi(buffer);

                /* Get the character type */
                move(7, 11);
                get_str(buffer, stdscr);
                for (i=0; i<NUM_CHARTYPES; i++) {
                    if (EQSTR(buffer, char_class[i].name, strlen(buffer)))
                        break;
                }
                player.t_ctype = i;

                /* Get the win type */
                move(8, 8);
                get_str(buffer, stdscr);
                switch (buffer[0]) {
                    case 'W':
                    case 'w':
                    case 'T':
                    case 't':
                        flags = WINNER;
                        break;

                    case 'Q':
                    case 'q':
                        flags = CHICKEN;
                        break;

                    case 'k':
                    case 'K':
                    default:
                        flags = KILLED;
                        break;
                }

                /* Get the monster if player was killed */
                if (flags == KILLED) {
                    mvaddstr(9, 0, "Death type: ");
                    get_str(buffer, stdscr);
                    if (buffer[0] == 'M' || buffer[0] == 'm')
                        do {
                            monst = makemonster(TRUE, "Editing", "choose");
                        } while (monst < 0); /* Force a choice */
                    else monst = getdeath();
                }
            }
#endif

            if (update(top_ten, amount, (short) quest_item, whoami, flags,
                    (flags == WINNER) ? (short) max_level : (short) level,
                    monst, player.t_ctype, thissys, login)
#ifdef NUMNET
                && fork() == 0          /* Spin off network process */
#endif
                ) {
#if defined(NUMNET) && defined(NETCOMMAND)
                /* Send this update to the other systems in the network */
                int i, j;
                char cmd[256];  /* Command for remote execution */
                FILE *rmf, *popen();    /* For input to remote command */

                for (i=0; i<NUMNET; i++)
                    if (Network[i].system[0] != '!' &&
                        strcmp(Network[i].system, thissys)) {
                        sprintf(cmd, NETCOMMAND,
                                Network[i].system, Network[i].rogue);

                        /* Execute the command */
                        if ((rmf=popen(cmd, "w")) != NULL) {
                            unsigned long temp; /* Temporary value */

                            /* Write out the parameters */
                            (void) netwrite((unsigned long) amount,
                                          sizeof(unsigned long), rmf);

                            (void) netwrite((unsigned long) monst,
                                          sizeof(short), rmf);

                            (void) netwrite((unsigned long) quest_item,
                                        sizeof(short), rmf);

                            (void) fwrite(whoami, 1, strlen(whoami), rmf);
                            for (j=strlen(whoami); j<NAMELEN; j++)
                                putc('\0', rmf);

                            (void) netwrite((unsigned long) flags,
                                          sizeof(short), rmf);

                            temp = (unsigned long)
                                (flags==WINNER ? max_level : level);
                            (void) netwrite(temp, sizeof(short), rmf);

                            (void) netwrite((unsigned long) player.t_ctype,
                                          sizeof(short), rmf);

                            (void) fwrite(thissys, 1,
                                                strlen(thissys), rmf);
                            for (j=strlen(thissys); j<SYSLEN; j++)
                                putc('\0', rmf);

                            (void) fwrite(login, 1, strlen(login), rmf);
                            for (j=strlen(login); j<LOGLEN; j++)
                                putc('\0', rmf);

                            /* Close off the command */
                            (void) pclose(rmf);
                        }
                    }
                    _exit(0);   /* Exit network process */
#endif
            }
        }
    }

    /*
     * SCOREIT -- rogue -s option.  Never started curses if this option.
     * UPDATE -- network scoring update.  Never started curses if this option.
     * EDITSCORE -- want to delete or change a score.
     */
/*   if (flags != SCOREIT && flags != UPDATE && prflags != EDITSCORE)
        endwin();       */

    if (flags != UPDATE) {
        if (flags != SCOREIT) {
            clear();
            refresh();
#if USG5_2 
            endwin();
#endif
        }
        /*
        * Print the list
        */
        printf("\nTop %d Adventurers:\nRank     Score\tName\n",
                NUMSCORE);
        for (scp = top_ten; scp < &top_ten[NUMSCORE]; scp++) {
            char *class;

            if (scp->sc_score != 0) {
                class = char_class[scp->sc_ctype].name;

                /* Make sure we have an in-bound reason */
                if (scp->sc_flags > REASONLEN) scp->sc_flags = REASONLEN;

                printf("%3ld %10lu\t%s (%s)", (long) (scp - top_ten + 1),
                    scp->sc_score, scp->sc_name, class);
                    
#if !MSDOS
                if (prflags == REALLIFE) printf(" [in real life %.*s!%.*s]",
                                SYSLEN, scp->sc_system, LOGLEN, scp->sc_login);
#endif
                printf(":\n\t\t%s on level %d", reason[scp->sc_flags],
                            scp->sc_level);

                switch (scp->sc_flags) {
                    case KILLED:
                        printf(" by");
                        killer = killname(scp->sc_monster);
                        printf(" %s", killer);
                        break;

                    case WINNER:
                        printf(" with the %s",
                                rel_magic[scp->sc_quest].mi_name);
                        break;
                }

                if (prflags == EDITSCORE)
                {
                    fflush(stdout);
                    getstr(prbuf);
                    printf("\n");
                    if (prbuf[0] == 'd') {
                        for (sc2 = scp; sc2 < &top_ten[NUMSCORE-1]; sc2++)
                            *sc2 = *(sc2 + 1);
                        top_ten[NUMSCORE-1].sc_score = 0;
                        for (i = 0; i < NAMELEN; i++)
                            top_ten[NUMSCORE-1].sc_name[i] = rnd(255);
                        top_ten[NUMSCORE-1].sc_flags = RN;
                            top_ten[NUMSCORE-1].sc_level = RN;
                            top_ten[NUMSCORE-1].sc_monster = RN;
                            scp--;
                    }
                    else if (prbuf[0] == 'e') {
                        printf("Death type: ");
                        getstr(prbuf);
                        if (prbuf[0] == 'M' || prbuf[0] == 'm')
                            do {
                                scp->sc_monster =
                                    makemonster(TRUE, "Editing", "choose");
                            } while (scp->sc_monster < 0); /* Force a choice */
                        else scp->sc_monster = getdeath();
                        clear();
                        refresh();
                    }
                }
                else printf("\n");
            }
        }
/*      if (prflags == EDITSCORE) endwin();    *//* End editing windowing */
    }
    lseek(outfd, 0L, 0);
    /*
     * Update the list file
     */
    scoreout(top_ten, (unsigned char *)compatstr); /* Get a compatibile format score string */
    encwrite(compatstr, (long)(NUMSCORE * SCORELEN), outfd);
    free(compatstr);
    close(outfd);
}

/*
 * scorein:
 *      Convert a character string that has been translated from a
 *      score file by scoreout() back to a score file structure.
 */

static void
scorein(unsigned char *input, struct sc_ent scores[], long num_bytes)
/* num_bytes - Number of bytes of input that we want to convert */
{
    register long i, j;
    unsigned long *lptr;
    unsigned short *sptr;
    unsigned char *cptr;

    /* Convert a maximum of NUMSCORE entries */
    for (i=0; num_bytes > 0 && i < NUMSCORE; num_bytes -= SCORELEN, i++) {
        /* The long fields are first -- ordered low to high byte in input */
        lptr = &scores[i].sc_score;
        *lptr  = ((unsigned long) *input++) & 0x000000ffL;
        *lptr |= (((unsigned long) *input++) << 8) & 0x0000ff00L;
        *lptr |= (((unsigned long) *input++) << 16) & 0x00ff0000L;
        *lptr |= (((unsigned long) *input++) << 24) & 0xff000000L;

        /* The short fields are next -- ordered low to high byte in input */
        sptr = (unsigned short *) &scores[i].sc_flags;
        *sptr = ((unsigned short) *input++) & 0xff;
        *sptr |= (((unsigned short) *input++) << 8) & 0xff00;

        sptr = (unsigned short *) &scores[i].sc_level;
        *sptr = ((unsigned short) *input++) & 0xff;
        *sptr |= (((unsigned short) *input++) << 8) & 0xff00;

        sptr = (unsigned short *) &scores[i].sc_ctype;
        *sptr = ((unsigned short) *input++) & 0xff;
        *sptr |= (((unsigned short) *input++) << 8) & 0xff00;

        sptr = (unsigned short *) &scores[i].sc_monster;
        *sptr = ((unsigned short) *input++) & 0xff;
        *sptr |= (((unsigned short) *input++) << 8) & 0xff00;

        sptr = (unsigned short *) &scores[i].sc_quest;
        *sptr = ((unsigned short) *input++) & 0xff;
        *sptr |= (((unsigned short) *input++) << 8) & 0xff00;

        /* Finally comes the char fields -- they're easy */
        cptr = (unsigned char *) scores[i].sc_name;
        for (j = 0; j < NAMELEN; j++) *cptr++ = *input++;

        cptr = (unsigned char *) scores[i].sc_system;
        for (j = 0; j < SYSLEN; j++) *cptr++ = *input++;

        cptr = (unsigned char *) scores[i].sc_login;
        for (j = 0; j < LOGLEN; j++) *cptr++ = *input++;
    }
}

/*
 * scoreout:
 *      Convert a score file structure to a character string.  We do
 *      this for compatibility sake since some machines write out fields
 *      in different orders.
 */

static void
scoreout(struct sc_ent scores[], unsigned char *output)
{
    register long i, j;
    unsigned long *lptr;
    unsigned short *sptr;
    unsigned char *cptr;

    for (i=0; i<NUMSCORE; i++) {
        /* The long fields are first -- ordered low to high byte in input */
        lptr = &scores[i].sc_score;
        for (j = 0; j < sizeof(unsigned long); j++)
            *output++ = (unsigned char) ((*lptr >> 8*j) & 0xff);

        /* The short fields are next -- ordered low to high byte in input */
        sptr = (unsigned short *) &scores[i].sc_flags;
        *output++ = (unsigned char) (*sptr & 0xff);
        *output++ = (unsigned char) ((*sptr >> 8) & 0xff);

        sptr = (unsigned short *) &scores[i].sc_level;
        *output++ = (unsigned char) (*sptr & 0xff);
        *output++ = (unsigned char) ((*sptr >> 8) & 0xff);

        sptr = (unsigned short *) &scores[i].sc_ctype;
        *output++ = (unsigned char) (*sptr & 0xff);
        *output++ = (unsigned char) ((*sptr >> 8) & 0xff);

        sptr = (unsigned short *) &scores[i].sc_monster;
        *output++ = (unsigned char) (*sptr & 0xff);
        *output++ = (unsigned char) ((*sptr >> 8) & 0xff);

        sptr = (unsigned short *) &scores[i].sc_quest;
        *output++ = (unsigned char) (*sptr & 0xff);
        *output++ = (unsigned char) ((*sptr >> 8) & 0xff);

        /* Finally comes the char fields -- they're easy */
        cptr = (unsigned char *) scores[i].sc_name;
        for (j = 0; j < NAMELEN; j++) *output++ = *cptr++;

        cptr = (unsigned char *) scores[i].sc_system;
        for (j = 0; j < SYSLEN; j++) *output++ = *cptr++;

        cptr = (unsigned char *) scores[i].sc_login;
        for (j = 0; j < LOGLEN; j++) *output++ = *cptr++;
    }
}

/*
 * showpack:
 *      Display the contents of the hero's pack
 */

void
showpack(char *howso)
{
        reg char *iname;
        reg int cnt, packnum;
        reg struct linked_list *item;
        reg struct object *obj;

        idenpack();
        cnt = 1;
        clear();
        mvprintw(0, 0, "Contents of your pack %s:\n",howso);
        packnum = 'a';
        for (item = pack; item != NULL; item = next(item)) {
                obj = OBJPTR(item);
                iname = inv_name(obj, FALSE);
                mvprintw(cnt, 0, "%c) %s\n",packnum++,iname);
                if (++cnt >= lines - 2 && 
                    next(item) != NULL) {
                        cnt = 1;
                        mvaddstr(lines - 1, 0, morestr);
                        refresh();
                        wait_for(' ');
                        clear();
                }
        }
        mvprintw(cnt + 1,0,"--- %ld  Gold Pieces ---",purse);
        refresh();
}

void
total_winner()
{
    register struct linked_list *item;
    register struct object *obj;
    register long worth;
    register char c;
    register long oldpurse;

    clear();
    standout();
    addstr("                                                               \n");
    addstr("  @   @               @   @           @          @@@  @     @  \n");
    addstr("  @   @               @@ @@           @           @   @     @  \n");
    addstr("  @   @  @@@  @   @   @ @ @  @@@   @@@@  @@@      @  @@@    @  \n");
    addstr("   @@@@ @   @ @   @   @   @     @ @   @ @   @     @   @     @  \n");
    addstr("      @ @   @ @   @   @   @  @@@@ @   @ @@@@@     @   @     @  \n");
    addstr("  @   @ @   @ @  @@   @   @ @   @ @   @ @         @   @  @     \n");
    addstr("   @@@   @@@   @@ @   @   @  @@@@  @@@@  @@@     @@@   @@   @  \n");
    addstr("                                                               \n");
    addstr("     Congratulations, you have made it to the light of day!    \n");
    standend();
    addstr("\nYou have joined the elite ranks of those who have escaped the\n");
    addstr("Dungeons of Doom alive.  You journey home and sell all your loot at\n");
    addstr("a great profit and are appointed ");
    switch (player.t_ctype) {
        case C_FIGHTER: addstr("Leader of the Fighter's Guild.\n");
        when C_RANGER:  addstr("King of the Northern Land.\n");
        when C_PALADIN: addstr("King of the Southern Land.\n");
        when C_MAGICIAN:addstr("High Wizard of the Sorcerer's Guild.\n");
        when C_CLERIC:  addstr("Bishop of the Monastery.\n");
        when C_THIEF:   addstr("Leader of the Thief's Guild.\n");
        when C_MONK:    addstr("Master of the Temple.\n");
        when C_ASSASSIN: addstr("Leader of the Assassin's Guild.\n");
        when C_DRUID:   addstr("High Priest of the Monastery.\n");
        otherwise:      addstr("Town Drunk in the Tavern.\n");
    }
    mvaddstr(lines - 1, 0, spacemsg);
    refresh();
    wait_for(' ');
    clear();
    mvaddstr(0, 0, "   Worth  Item");
    oldpurse = purse;
    for (c = 'a', item = pack; item != NULL; c++, item = next(item))
    {
        obj = OBJPTR(item);
        worth = get_worth(obj);
        if (obj->o_group == 0)
            worth *= obj->o_count;
        whatis(item);
        mvprintw(c-'a'+1, 0, "%c) %6ld  %s", c, worth, inv_name(obj, FALSE));
        purse += worth;
    }
    mvprintw(c - 'a' + 1, 0,"   %5ld  Gold Pieces          ", oldpurse);
    refresh();
    score(pstats.s_exp + (long) purse, WINNER, '\0');
    endwin();
#ifdef PC7300
    endhardwin();
#endif
    exit(0);
}

static int
update(struct sc_ent top_ten[], unsigned long amount, short quest, char *whoami, short flags, short level, short monst, short ctype, char *system, char *login)
{
    register struct sc_ent *scp, *sc2;
    int retval=0;       /* 1 if a change, 0 otherwise */

    for (scp = top_ten; scp < &top_ten[NUMSCORE]; scp++) {
        if (amount >= scp->sc_score)
            break;

#ifdef LIMITSCORE       /* Limits player to one entry per class per uid */
        /* If this good score is the same class and uid, then forget it */
        if (strncmp(scp->sc_login, login, LOGLEN) == 0 &&
            scp->sc_ctype == ctype &&
            strncmp(scp->sc_system, system, SYSLEN) == 0) return(0);
#endif
    }

    if (scp < &top_ten[NUMSCORE])
    {
        retval = 1;

#ifdef LIMITSCORE       /* Limits player to one entry per class per uid */
    /* If a lower scores exists for the same login and class, delete it */
    for (sc2 = scp ;sc2 < &top_ten[NUMSCORE]; sc2++) {
        if (sc2->sc_score == 0L) break; /* End of useful scores */

        if (strncmp(sc2->sc_login, login, LOGLEN) == 0 &&
            sc2->sc_ctype == ctype &&
            strncmp(sc2->sc_system, system, SYSLEN) == 0) {
            /* We want to delete this entry */
            while (sc2 < &top_ten[NUMSCORE-1]) {
                *sc2 = *(sc2+1);
                sc2++;
            }
            sc2->sc_score = 0L;
        }
    }
#endif

        for (sc2 = &top_ten[NUMSCORE-1]; sc2 > scp; sc2--)
            *sc2 = *(sc2-1);
        scp->sc_score = amount;
        scp->sc_quest = quest;
        strxcpy(scp->sc_name, whoami, NAMELEN);
        scp->sc_flags = flags;
        scp->sc_level = level;
        scp->sc_monster = monst;
        scp->sc_ctype = ctype;
        strxcpy(scp->sc_system, system, SYSLEN);
        strxcpy(scp->sc_login, login, LOGLEN);
    }

    return(retval);
}

