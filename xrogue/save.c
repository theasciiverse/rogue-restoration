/*
 * save game and restore routines
 */

#include <curses.h>
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include "rogue.h"
#include "mach_dep.h"
#ifdef PC7300
#include <sys/window.h>
extern struct uwdata wdata;
#endif
#ifdef MSDOS
#include <dos.h>
#endif

#if u370 || uts || i386
#define ENCREAD(b,n,fd) read(fd,b,n)
#define ENCWRITE(b,n,fd) write(fd,b,n)
#endif
#ifndef ENCREAD
#define ENCREAD encread
#define ENCWRITE encwrite
#endif

typedef struct stat STAT;

extern char version[];
extern unsigned char encstr[];
/* extern bool _endwin; */

static STAT sbuf;

bool
save_game()
{
    register int savefd;
    register int c;
    char buf[LINELEN];

    /*
     * get file name
     */
    mpos = 0;
    if (file_name[0] != '\0')
    {
        msg("Save file (%s)? ", file_name);
        do
        {
            c = wgetch(cw);
            if (c == ESC) return(0);
        } while (c != 'n' && c != 'N' && c != 'y' && c != 'Y');
        mpos = 0;
        if (c == 'y' || c == 'Y')
        {
            msg("File name: %s", file_name);
            goto gotfile;
        }
    }

    do
    {
        msg("File name: ");
        mpos = 0;
        buf[0] = '\0';
        if (get_str(buf, msgw) == QUIT)
        {
            msg("");
            return FALSE;
        }
        strcpy(file_name, buf);
gotfile:
#ifdef BSD
        if ((savefd = open(file_name, O_WRONLY, 0666)) < 0)
#else
        if ((savefd = open(file_name, O_WRONLY|O_CREAT|O_TRUNC,0666)) < 0)
#endif
            msg(strerror(errno));    /* fake perror() */
    } while (savefd < 0);

    /*
     * write out encrpyted file (after a stat)
     */
    if (save_file(savefd) == FALSE) {
        msg("Cannot create save file.");
        unlink(file_name);
        return(FALSE);
    }
    else return(TRUE);
}

/*
 * automatically save a file.  This is used if a HUP signal is recieved
 */

void
auto_save(int sig)
{
    register int savefd;
    register int i;

    for (i = 0; i < NSIG; i++)
        signal(i, SIG_IGN);
    if (file_name[0] != '\0'    && 
        pstats.s_hpt > 0        &&
#if MSDOS
        (savefd = open(file_name, O_WRONLY|O_CREAT|O_TRUNC)) >= 0)
#else
#ifdef BSD
        (savefd = open(file_name, O_WRONLY, 0600)) >= 0)
#else
        (savefd = open(file_name, O_WRONLY|O_CREAT|O_TRUNC, 0600)) >= 0)
#endif
#endif
        save_file(savefd);
    endwin();
#ifdef PC7300
    endhardwin();
#endif
    exit(1);
}

/*
 * write the saved game on the file
 */

bool
save_file(register int savefd)
{
#if defined(MSDOS) && !defined(__DJGPP__)
    extern unsigned int _asegr, _asegn; /* for MSDOS access to end of prog */
    register unsigned long num_to_write;
#endif
    int slines = LINES;
    int scols = COLS;
    int ret = FALSE;

    wmove(cw, lines-1, 0);
    draw(cw);
    lseek(savefd, 0L, 0);
    fstat(savefd, &sbuf);
#if defined(MSDOS) && !defined(__DJGPP__)
    msg ("version: seg %x offs %x", FP_SEG(&version),FP_OFF(&version));
    msg ("asegr = %x",_asegr);
    msg ("asegn = %x",_asegn);
    msg ("endaddr = %lx", (unsigned long)(((unsigned long)_asegr+0x100L)<<4L));
    num_to_write = (((unsigned long)_asegr+0x100L)<<4L) -
                   (((unsigned long)FP_SEG(version) << 4L) + FP_OFF(version));
    msg("num to write = %lx\n", num_to_write);
#endif
    encwrite(version,(unsigned int)strlen(version)+1,savefd);
    encwrite((char *)&sbuf.st_ino,sizeof(sbuf.st_ino),savefd);
    encwrite((char *)&sbuf.st_dev,sizeof(sbuf.st_dev),savefd);
    encwrite((char *)&sbuf.st_ctime,sizeof(sbuf.st_ctime),savefd);
    encwrite((char *)&sbuf.st_mtime,sizeof(sbuf.st_mtime),savefd);
    encwrite((char *)&slines,sizeof(slines),savefd);
    encwrite((char *)&scols,sizeof(scols),savefd);

    ret = rs_save_file(savefd);

    close(savefd);
    return(ret);
}

int
restore(register char *file, char **envp)
{
    register int inf;
    extern char **environ;
    char buf[LINELEN];
#if USG5_0 || BSD
    char *newterm;                   /* For new terminal type */
#endif
    STAT sbuf2;
    int oldcol, oldline;        /* Old number of columns and lines */

    if (strcmp(file, "-r") == 0)
        file = file_name;
    if ((inf = open(file, O_RDONLY)) < 0)
    {
        perror(file);
        return FALSE;
    }

    fflush(stdout);
    lseek(inf, 0L, 0);
    ENCREAD(buf, (long)(strlen(version) + 1), inf); /* check which version */
    if (strcmp(buf, version) != 0)
    {
        printf("Sorry, saved game is out of date.\n");
        return FALSE;
    }

    fstat(inf, &sbuf2);
    fflush(stdout);
#if BSD || USG5_0 || USG5_2
    if (brk(version + sbuf2.st_size))
        return FALSE;
#endif
    /*
     * Added the following #ifdef to re-initialize the screen
     * on i386 system for playing saved games 10/30/90.
     */
#ifdef i386
    initscr();
    typeahead(-1);
#endif

    initscr();
    lseek(inf, 0L, 0);
    encread((char *)&sbuf.st_ino,sizeof(sbuf.st_ino), inf);
    encread((char *)&sbuf.st_dev,sizeof(sbuf.st_dev), inf);
    encread((char *)&sbuf.st_ctime,sizeof(sbuf.st_ctime), inf);
    encread((char *)&sbuf.st_mtime,sizeof(sbuf.st_mtime), inf);
    encread((char *)&lines,sizeof(lines),inf);
    encread((char *)&cols,sizeof(cols),inf);

    /*
     * Get the lines and columns from the previous game
     */
    oldcol = cols;
    oldline = lines;

    /*
     * Set the new terminal and make sure we aren't going to a smaller screen.
     */
#if USG5_0 || BSD
    /* Set columns and lines to 0 so that setterm() will reset the values */
    COLS = 0;
    LINES = 0;

    if ((newterm = getenv("TERM")) == NULL ||
        setterm(newterm) == ERR) {
        printf("Bad or missing terminal type.\n");
        return FALSE;
    }
#endif
/* 
 * this code should work but does not because of curses bug 
 * curses fails to turn echo off
 */
#if 0  /* this is effectively commented out. */
    if ((newterm = getenv("TERM")) == NULL) {
        printf("Bad or missing terminal type.\n");
        return FALSE;
    }
    restartterm(newterm, 1, 0);
#endif
    if (COLS < oldcol || LINES < oldline) {
        printf("Cannot restart the game on a smaller screen.\n");
        return FALSE;
    }
/*
    cols = COLS;
    lines = LINES;
    if (cols > 85) cols = 85;
    if (lines > 24) lines = 24;
*/

    /*
     * we do not close the file so that we will have a hold of the
     * inode for as long as possible
     */
#if !MSDOS
    if (!wizard) {
        if (sbuf2.st_ino != sbuf.st_ino || sbuf2.st_dev != sbuf.st_dev) {
            printf("Sorry, saved game is not in the same file.\n");
#ifdef i386
	endwin();  /* added because of initscr() above */
#endif
            return FALSE;
        }
#ifndef NOCHECKACCESS
        else if (sbuf2.st_ctime - sbuf.st_ctime > FUDGE_TIME) {
            printf("Sorry, file has been touched.\n");
#ifdef i386
	endwin();  /* added because of initscr() above */
#endif
            return FALSE;
        }
#endif
    }
#endif

    /*
     * defeat multiple restarting from the same place
     */
    if (!wizard) {
        if (sbuf2.st_nlink != 1) {
            printf("Cannot restore from a linked file\n");
#ifdef i386
	endwin();  /* added because of initscr() above */
#endif
            return FALSE;
        }
        else if (unlink(file) < 0) {
            printf("Cannot unlink file\n");
#ifdef i386
	endwin();  /* added because of initscr() above */
#endif
            return FALSE;
        }
    }

    typeahead(-1);
    cw = newwin(LINES, COLS, 0, 0);
    mw = newwin(LINES, COLS, 0, 0);
    hw = newwin(LINES, COLS, 0, 0);
    msgw = newwin(4, cols, 0, 0);

    keypad(cw, TRUE);
    keypad(hw, TRUE);

    if (rs_restore_file(inf) == FALSE)
    {
        endwin();
        printf("Cannot restore file\n");
        close(inf);
        return(FALSE);
    }
    mpos = 0;
    environ = envp;
    strcpy(file_name, file);
    setup();
    clearok(curscr, TRUE);
    touchwin(cw);
    msg("Welcome back!  --More-- ");
    wait_for(' ');
    playit();
    /*NOTREACHED*/
    return FALSE;
}

#define ENCWBSIZ        1024

/*
 * perform an encrypted write
 */

long
encwrite(register char *start, register unsigned long size, register int outf)
{
    register unsigned char *ep;
    register int i = 0;
    unsigned long num_written = 0;
    char buf[ENCWBSIZ];

    ep = encstr;

    while (size--)
    {
        buf[i++] = *start++ ^ *ep++;
        if (*ep == '\0')
           ep = encstr;

        if (i == ENCWBSIZ || size == 0) {
            if (write(outf, buf, (unsigned)i) < i) 
                 return(num_written);
            else {
                num_written += i;
                i = 0;
            }
        }
    }
    return(num_written);
}

#define ENCRBSIZ        (2^15)

/*
 * perform an encrypted read
 */

long
encread(register char *start, register unsigned long size, int inf)
{
    register unsigned char *ep;
    register int rd_siz;
    register unsigned long total_read;

    total_read = 0;
    while (total_read < size) {
        rd_siz = size-total_read > ENCRBSIZ ? ENCRBSIZ : size-total_read;
        if((rd_siz=read(inf,&start[total_read],rd_siz))==-1 || rd_siz==0)
                break;
        total_read += rd_siz;
    }
    ep = encstr;

    size = total_read;
    while (size--)
    {
        *start++ ^= *ep++;
        if (*ep == '\0')
            ep = encstr;
    }
    return total_read;
}

