/*
    mdport.c - Machine Dependent Code for Porting Unix/Curses games

    Copyright (C) 2005 Nicholas J. Kisseberth
    All rights reserved.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdio.h>
#ifdef HAVE_PROCESS_H /* Only Windows probably */
#include <process.h>
#endif

#if defined(_WIN32)
#undef MOUSE_MOVED
#include <io.h>
#include <sys/locking.h>
#include <Windows.h>
#include <Lmcons.h>
#include <conio.h>
#ifndef __MINGW32__
#pragma warning( disable: 4201 ) 
#endif
#include <shlobj.h>
#ifndef __MINGW32__
#pragma warning( default: 4201 ) 
#endif
#include <Shlwapi.h>
#undef MOUSE_MOVED
#endif

#include "mdport.h"

#ifdef HAVE_TERM_H /* MSVC does not */
#include <term.h>
#elif HAVE_NCURSES_TERM_H
#include <ncurses/term.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#include <errno.h>
#include <limits.h>
#include <signal.h>

#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>

#ifdef HAVE_UTMPX_H /* DJGPP DOES NOT */
#include <utmpx.h>
#elif HAVE_UTMP_H
#include <utmp.h>
#endif

#define NOOP(x) (void) x

void unread(int c);
int reread(void);
int md_setsuspchar(int c);
int directory_exists(char *dirname);
char *md_getshell(void);
void md_putchar(int c);

static int pass_ctrl_keypad = 1;

void
md_init(int options)
{
#if defined(__INTERIX)
    if (getenv("TERM") == NULL)
        md_setenv("TERM","interix",0);
#elif defined(__DJGPP__)
    _fmode = _O_BINARY;
#elif defined(_WIN32)
    _fmode = _O_BINARY;
#endif

#ifdef HAVE_ESCDELAY
    ESCDELAY=64;
#else
    if (getenv("ESCDELAY") == NULL)
        md_setenv("ESCDELAY","64",0);
#endif

    if (options & MD_SETUP_SIGNALS)
#if defined(DUMP)
    md_onsignal_default();
#else
    md_onsignal_exit();
#endif

    if (options & MD_STRIP_CTRL_KEYPAD)
        pass_ctrl_keypad = 0;
    else
        pass_ctrl_keypad = 1;
}

int
md_setenv(const char *name, const char *value, int overwrite)
{
#ifdef HAVE_SETENV 
    setenv(name, value, overwrite); 
#else 
    int len, ret;
    char *str;

    if (!overwrite && getenv(name) != NULL)
        return 0;

    len = strlen(value)+1+strlen(value)+1; 

    str = malloc(len); 

    if (str == NULL)
    {
        errno = ENOMEM;
        return -1;
    }

    sprintf(str, "%s=%s", name, value); 

    ret = putenv(str); 

    free(str);

    return ret;
#endif 
}

void
md_onsignal_default(void)
{
#ifdef SIGHUP
    signal(SIGHUP, SIG_DFL);
#endif
#ifdef SIGQUIT
    signal(SIGQUIT, SIG_DFL);
#endif
#ifdef SIGILL
    signal(SIGILL, SIG_DFL);
#endif
#ifdef SIGTRAP
    signal(SIGTRAP, SIG_DFL);
#endif
#ifdef SIGIOT
    signal(SIGIOT, SIG_DFL);
#endif
#ifdef SIGEMT
    signal(SIGEMT, SIG_DFL);
#endif
#ifdef SIGFPE
    signal(SIGFPE, SIG_DFL);
#endif
#ifdef SIGBUS
    signal(SIGBUS, SIG_DFL);
#endif
#ifdef SIGSEGV
    signal(SIGSEGV, SIG_DFL);
#endif
#ifdef SIGSYS
    signal(SIGSYS, SIG_DFL);
#endif
#ifdef SIGTERM
    signal(SIGTERM, SIG_DFL);
#endif
}

void
md_onsignal_exit(void)
{
#ifdef SIGHUP
    signal(SIGHUP, SIG_DFL);
#endif
#ifdef SIGQUIT
    signal(SIGQUIT, exit);
#endif
#ifdef SIGILL
    signal(SIGILL, exit);
#endif
#ifdef SIGTRAP
    signal(SIGTRAP, exit);
#endif
#ifdef SIGIOT
    signal(SIGIOT, exit);
#endif
#ifdef SIGEMT
    signal(SIGEMT, exit);
#endif
#ifdef SIGFPE
    signal(SIGFPE, exit);
#endif
#ifdef SIGBUS
    signal(SIGBUS, exit);
#endif
#ifdef SIGSEGV
    signal(SIGSEGV, exit);
#endif
#ifdef SIGSYS
    signal(SIGSYS, exit);
#endif
#ifdef SIGTERM
    signal(SIGTERM, exit);
#endif
}

extern void auto_save(int sig);
extern void endit(int sig);
extern void quit(int sig);

void
md_onsignal_autosave(void)
{

#ifdef SIGHUP
    signal(SIGHUP, auto_save);
#endif
#ifdef SIGQUIT
	signal(SIGQUIT, endit);
#endif
#ifdef SIGILL
    signal(SIGILL, auto_save);
#endif
#ifdef SIGTRAP
    signal(SIGTRAP, auto_save);
#endif
#ifdef SIGIOT
    signal(SIGIOT, auto_save);
#endif
#ifdef SIGEMT
    signal(SIGEMT, auto_save);
#endif
#ifdef SIGFPE
    signal(SIGFPE, auto_save);
#endif
#ifdef SIGBUS
    signal(SIGBUS, auto_save);
#endif
#ifdef SIGSEGV
    signal(SIGSEGV, auto_save);
#endif
#ifdef SIGSYS
    signal(SIGSYS, auto_save);
#endif
#ifdef SIGTERM
    signal(SIGTERM, auto_save);
#endif
#ifdef SIGINT
    signal(SIGINT, quit);
#endif
}

int
md_hasclreol(void)
{
#ifdef clr_eol
#ifdef NCURSES_VERSION
    if (cur_term == NULL)
	return(0);
#ifdef CUR
    if (CUR Strings == NULL)
#else
    if (cur_term->type.Strings == NULL)
#endif
	return(0);
#endif
    return((clr_eol != NULL) && (*clr_eol != 0));
#elif defined(__PDCURSES__)
    return(TRUE);
#else
    return((CE != NULL) && (*CE != 0));
#endif
}

void
md_putchar(int c)
{
    putchar(c);
}

#ifdef _WIN32
static int md_standout_mode = 0;
#endif

void
md_raw_standout(void)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
    HANDLE hStdout;
    WORD fgattr,bgattr;

    if (md_standout_mode == 0)
    {
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
        GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
        fgattr = (csbiInfo.wAttributes & 0xF);
        bgattr = (csbiInfo.wAttributes & 0xF0);
        SetConsoleTextAttribute(hStdout,(fgattr << 4) | (bgattr >> 4));
        md_standout_mode = 1;
    }
#elif defined(SO)
    tputs(SO,0,md_putchar);
    fflush(stdout);
#endif
}

void
md_raw_standend(void)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
    HANDLE hStdout;
    WORD fgattr,bgattr;

    if (md_standout_mode == 1)
    {
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
        GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
        fgattr = (csbiInfo.wAttributes & 0xF);
        bgattr = (csbiInfo.wAttributes & 0xF0);
        SetConsoleTextAttribute(hStdout,(fgattr << 4) | (bgattr >> 4));
        md_standout_mode = 0;
    }
#elif defined(SE)
    tputs(SE,0,md_putchar);
    fflush(stdout);
#endif
}

int
md_unlink_open_file(const char *file, int inf)
{
#ifdef _WIN32
    _close(inf);
    (void) _chmod(file, 0600);
    return( _unlink(file) );
#else
    (void) inf;

    return(unlink(file));
#endif
}

int
md_unlink(char *file)
{
#ifdef _WIN32
    (void) _chmod(file, 0600);
    return( _unlink(file) );
#else
    return(unlink(file));
#endif
}

void
md_normaluser(void)
{
#if defined(HAVE_GETGID) && defined(HAVE_GETUID)
        gid_t realgid = getgid();
        uid_t realuid = getuid();

#if defined(HAVE_SETRESGID)
    if (setresgid(-1, realgid, realgid) != 0) {
#elif defined (HAVE_SETREGID)
    if (setregid(realgid, realgid) != 0) {
#elif defined (HAVE_SETGID)
        if (setgid(realgid) != 0) {
#else
        if (0) {
#endif
                perror("Could not drop setgid privileges.  Aborting.");
                exit(1);
    }

#if defined(HAVE_SETRESUID)
    if (setresuid(-1, realuid, realuid) != 0) {
#elif defined(HAVE_SETREUID)
    if (setreuid(realuid, realuid) != 0) {
#elif defined(HAVE_SETUID)
        if (setuid(realuid) != 0) {
#else
        if (0) {
#endif
        perror("Could not drop setuid privileges.  Aborting.");
        exit(1);
    }
#endif
}

uid_t
md_getuid(void)
{
#ifdef HAVE_GETUID
    return( getuid() );
#else
    return(42);
#endif
}

char *
md_getusername(void)
{
    static char login[80];
    const char *l = NULL;
#ifdef _WIN32
    LPSTR mybuffer;
    DWORD size = UNLEN + 1;
    TCHAR buffer[UNLEN + 1];

    mybuffer = buffer;
	GetUserName(mybuffer,&size);
    l = mybuffer;
#elif defined(HAVE_GETPWUID)&& !defined(__DJGPP__)
    struct passwd *pw;

    pw = getpwuid(getuid());

    l = pw->pw_name;
#endif

    if ((l == NULL) || (*l == '\0'))
        if ( (l = getenv("USERNAME")) == NULL )
            if ( (l = getenv("LOGNAME")) == NULL )
                if ( (l = getenv("USER")) == NULL )
                    return(NULL);

    strncpy(login,l,80);
    login[79] = 0;

    return(login);
}

char *
md_gethomedir(void)
{
    static char homedir[PATH_MAX];
    const char *h = NULL;
    size_t len;
#if defined(_WIN32)
    TCHAR szPath[PATH_MAX];
#endif
#if !defined(_WIN32) && !defined(DJGPP)
    struct passwd *pw;
    pw = getpwuid(getuid());

    h = pw->pw_dir;

    if (strcmp(h,"/") == 0)
        h = NULL;
#endif
    homedir[0] = 0;
#ifdef _WIN32
    if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, szPath)))
        h = szPath;
#endif

    if ( (h == NULL) || (*h == '\0') )
    {
        if ( (h = getenv("HOME")) == NULL )
	{
            if ( (h = getenv("HOMEDRIVE")) == NULL)
                h = "";
            else
            {
    strncpy(homedir,h,PATH_MAX-1);
                homedir[PATH_MAX-1] = 0;

                if ( (h = getenv("HOMEPATH")) == NULL)
                    h = "";
            }
	}
    }

    len = strlen(homedir);
    strncat(homedir,h,PATH_MAX-len-1);
    homedir[PATH_MAX-1] = 0;

    len = strlen(homedir);

#if defined(_WIN32)
    if ((homedir[0] != 0) && (homedir[len] != '\\') && (homedir[len] != '\\'))
        strncat(homedir,"\\",PATH_MAX-len-1);
#else
    if ((homedir[0] != 0) && (homedir[len] != '/'))
        strncat(homedir,"/",PATH_MAX-len-1);
#endif
    homedir[PATH_MAX-1] = 0;

    return(homedir);
}

void
md_sleep(int s)
{
#ifdef _WIN32
    Sleep(s);
#else
    sleep(s);
#endif
}

char *
md_getshell(void)
{
    static char shell[PATH_MAX];
    const char *s = NULL;
#ifdef _WIN32
    const char *def = "C:\\WINDOWS\\SYSTEM32\\CMD.EXE";
#elif defined(__DJGPP__)
    const char *def = "C:\\COMMAND.COM";
#else
    const char *def = "/bin/sh";
    struct passwd *pw;
    pw = getpwuid(getuid());
    s = pw->pw_shell;
#endif
    if ((s == NULL) || (*s == '\0'))
        if ( (s = getenv("COMSPEC")) == NULL)
            if ( (s = getenv("SHELL")) == NULL)
                if ( (s = getenv("SystemRoot")) == NULL)
                    s = def;

    strncpy(shell,s,PATH_MAX);
    shell[PATH_MAX-1] = 0;

    return(shell);
}

int
md_shellescape(void)
{
#if defined(HAVE_WORKING_FORK)
    int ret_status;
    int pid;
    void (*myquit)(int);
    void (*myend)(int);
    char *sh;

    sh = md_getshell();

    while((pid = fork()) < 0)
        sleep(1);

    if (pid == 0) /* Shell Process */
    {
        /*
         * Set back to original user, just in case
         */
        md_normaluser();
        execl(sh == NULL ? "/bin/sh" : sh, "shell", "-i", NULL);
        perror("No shelly");
        _exit(-1);
    }
    else /* Application */
    {
	myend = signal(SIGINT, SIG_IGN);
#ifdef SIGQUIT
        myquit = signal(SIGQUIT, SIG_IGN);
#endif  
        while (wait(&ret_status) != pid)
            continue;
	    
        signal(SIGINT, myquit);
#ifdef SIGQUIT
        signal(SIGQUIT, myend);
#endif
    }
    return(ret_status);
#elif defined(HAVE__SPAWNL) 
    return((int)_spawnl(_P_WAIT,md_getshell(),"shell",NULL,0));
#elif defined(HAVE_SPAWNL)
    return ( spawnl(P_WAIT,md_getshell(),"shell",NULL,0) );
#else
	return(0);
#endif
}

int
directory_exists(char *dirname)
{
    struct stat sb;

    if (stat(dirname, &sb) == 0) /* path exists */
#if !defined(_WIN32)
        return(S_ISDIR(sb.st_mode));
#else
        return(sb.st_mode & S_IFMT);
#endif
    return(0);
}

char *
md_getrealname(uid_t uid)
{
    static char uidstr[20];
#if !defined(_WIN32) && !defined(DJGPP)
    struct passwd *pp;

	if ((pp = getpwuid(uid)) == NULL)
    {
        sprintf(uidstr,"%u", (unsigned int) uid);
        return(uidstr);
    }
	else
	    return(pp->pw_name);
#else
   sprintf(uidstr,"%d", uid);
   return(uidstr);
#endif
}

char *
md_getpass(const char *prompt)
{
#ifndef HAVE_GETPASS
    static char password_buffer[9];
    char *p = password_buffer;
    int c, count = 0;
    int max_length = 9;

    fflush(stdout);
    /* If we can't prompt, abort */
    if (fputs(prompt, stderr) < 0)
    {
        *p = '\0';
        return NULL;
    }

    for(;;)
    {
        /* Get a character with no echo */
        c = _getch();

        /* Exit on interrupt (^c or ^break) */
        if (c == '\003' || c == 0x100)
            exit(1);

        /* Terminate on end of line or file (^j, ^m, ^d, ^z) */
        if (c == '\r' || c == '\n' || c == '\004' || c == '\032')
            break;

        /* Back up on backspace */
        if (c == '\b')
        {
            if (count)
                count--;
            else if (p > password_buffer)
                p--;
            continue;
        }

        /* Ignore DOS extended characters */
        if ((c & 0xff) != c)
            continue;

        /* Add to password if it isn't full */
        if (p < password_buffer + max_length - 1)
            *p++ = (char) c;
        else
            count++;
    }
   *p = '\0';

   fputc('\n', stderr);

   return password_buffer;
#else
    char *password_buffer = getpass(prompt);
#if defined(__CYGWIN__)
    fputc('\r', stderr);
#endif
    return password_buffer;
#endif
}

int
md_erasechar(void)
{
#ifdef HAVE_ERASECHAR
    return( erasechar() ); /* process erase character */
#elif defined(VERASE)
    return(_tty.c_cc[VERASE]); /* process erase character */
#else
    return(_tty.sg_erase); /* process erase character */
#endif
}

int
md_killchar(void)
{
#ifdef HAVE_KILLCHAR
    return( killchar() );
#elif defined(VKILL)
    return(_tty.c_cc[VKILL]);
#else
    return(_tty.sg_kill);
#endif
}

int
md_dsuspchar(void)
{
#if defined(VDSUSP)			/* POSIX has priority */
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr);
    return( attr.c_cc[VDSUSP] );
#elif defined(TIOCGLTC)
    struct ltchars ltc;
    ioctl(1, TIOCGLTC, &ltc);
    return(ltc.t_dsuspc);
#elif defined(_POSIX_VDISABLE)
    return(_POSIX_VDISABLE);
#else
    return(0);
#endif
}

int
md_setdsuspchar(int c)
{
#if defined(VDSUSP)			/* POSIX has priority */
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr);
    attr.c_cc[VDSUSP] = c;
    tcgetattr(STDIN_FILENO, &attr);
#elif defined(TIOCSLTC)
    struct ltchars ltc;
    ioctl(1, TIOCGLTC, &ltc);
    ltc.t_dsuspc = c;
    ioctl(1, TIOCSLTC, &ltc);
#else
    NOOP(c);
#endif
    return(0);
}

int
md_suspchar(void)
{
#if defined(VSUSP)			/* POSIX has priority */
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr);
    return( attr.c_cc[VSUSP] );
#elif defined(TIOCGLTC)
    struct ltchars ltc;
    ioctl(1, TIOCGLTC, &ltc);
    return(ltc.t_suspc);
#elif defined(_POSIX_VDISABLE)
    return(_POSIX_VDISABLE);
#else
    return(0);
#endif
}

int
md_setsuspchar(int c)
{
#if defined(VSUSP)			/* POSIX has priority */
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr);
    attr.c_cc[VSUSP] = c;
    tcgetattr(STDIN_FILENO, &attr);
#elif defined(TIOCSLTC)
    struct ltchars ltc;
    ioctl(1, TIOCGLTC, &ltc);
    ltc.t_suspc = c;
    ioctl(1, TIOCSLTC, &ltc);
#else
    NOOP(c);
#endif

    return(0);
}

/*
    Cursor/Keypad Support

    Sadly Cursor/Keypad support is less straightforward than it should be.

    The various terminal emulators/consoles choose to differentiate the
    cursor and keypad keys (with modifiers) in different ways (if at all!).
    Furthermore they use different code set sequences for each key only
    a subset of which the various curses libraries recognize. Partly due
    to incomplete termcap/terminfo entries and partly due to inherent
    limitations of those terminal capability databases.

    I give curses first crack at decoding the sequences. If it fails to decode
    it we check for common ESC-prefixed sequences.

    All cursor/keypad results are translated into standard rogue movement
    commands.

    Unmodified keys are translated to walk commands: hjklyubn
    Modified (shift,control,alt) are translated to run commands: HJKLYUBN

    Console and supported (differentiated) keys
    Interix:  Cursor Keys, Keypad, Ctl-Keypad
    Cygwin:   Cursor Keys, Keypad, Alt-Cursor Keys
    MSYS:     Cursor Keys, Keypad, Ctl-Cursor Keys, Ctl-Keypad
    Win32:    Cursor Keys, Keypad, Ctl/Shift/Alt-Cursor Keys, Ctl/Alt-Keypad
    DJGPP:    Cursor Keys, Keypad, Ctl/Shift/Alt-Cursor Keys, Ctl/Alt-Keypad

    Interix Console (raw, ncurses)
    ==============================
    normal      shift           ctrl        alt
    ESC [D,     ESC F^,         ESC [D,     ESC [D          /# Left         #/
    ESC [C,     ESC F$,         ESC [C,     ESC [C          /# Right        #/
    ESC [A,     ESC F-,         local win,  ESC [A          /# Up           #/
    ESC [B,     ESC F+,         local win,  ESC [B          /# Down         #/
    ESC [H,     ESC [H,         ESC [H,     ESC [H          /# Home         #/
    ESC [S,     local win,      ESC [S,     ESC [S          /# Page Up      #/
    ESC [T,     local win,      ESC [T,     ESC [T          /# Page Down    #/
    ESC [U,     ESC [U,         ESC [U,     ESC [U          /# End          #/
    ESC [D,     ESC F^,         ESC [D,     O               /# Keypad Left  #/
    ESC [C,     ESC F$,         ESC [C,     O               /# Keypad Right #/
    ESC [A,     ESC [A,         ESC [-1,    O               /# Keypad Up    #/
    ESC [B,     ESC [B,         ESC [-2,    O               /# Keypad Down  #/
    ESC [H,     ESC [H,         ESC [-263,  O               /# Keypad Home  #/
    ESC [S,     ESC [S,         ESC [-19,   O               /# Keypad PgUp  #/
    ESC [T,     ESC [T,         ESC [-20,   O               /# Keypad PgDn  #/
    ESC [U,     ESC [U,         ESC [-21,   O               /# Keypad End   #/
    nothing,    nothing,        nothing,    O               /# Kaypad 5     #/

    Interix Console (term=interix, ncurses)
    ==============================
    KEY_LEFT,   ESC F^,         KEY_LEFT,   KEY_LEFT        /# Left         #/
    KEY_RIGHT,  ESC F$,         KEY_RIGHT,  KEY_RIGHT       /# Right        #/
    KEY_UP,     0x146,          local win,  KEY_UP          /# Up           #/
    KEY_DOWN,   0x145,          local win,  KEY_DOWN        /# Down         #/
    ESC [H,     ESC [H,         ESC [H,     ESC [H          /# Home         #/
    KEY_PPAGE,  local win,      KEY_PPAGE,  KEY_PPAGE       /# Page Up      #/
    KEY_NPAGE,  local win,      KEY_NPAGE,  KEY_NPAGE       /# Page Down    #/
    KEY_LL,     KEY_LL,         KEY_LL,     KEY_LL          /# End          #/
    KEY_LEFT,   ESC F^,         ESC [-4,    O               /# Keypad Left  #/
    KEY_RIGHT,  ESC F$,         ESC [-3,    O               /# Keypad Right #/
    KEY_UP,     KEY_UP,         ESC [-1,    O               /# Keypad Up    #/
    KEY_DOWN,   KEY_DOWN,       ESC [-2,    O               /# Keypad Down  #/
    ESC [H,     ESC [H,         ESC [-263,  O               /# Keypad Home  #/
    KEY_PPAGE,  KEY_PPAGE,      ESC [-19,   O               /# Keypad PgUp  #/
    KEY_NPAGE,  KEY_NPAGE,      ESC [-20,   O               /# Keypad PgDn  #/
    KEY_LL,     KEY_LL,         ESC [-21,   O               /# Keypad End   #/
    nothing,    nothing,        nothing,    O               /# Keypad 5     #/

    Cygwin Console (raw, ncurses)
    ==============================
    normal      shift           ctrl        alt
    ESC [D,     ESC [D,         ESC [D,     ESC ESC [D      /# Left         #/
    ESC [C,     ESC [C,         ESC [C,     ESC ESC [C      /# Rght         #/
    ESC [A,     ESC [A,         ESC [A,     ESC ESC [A      /# Up           #/
    ESC [B,     ESC [B,         ESC [B,     ESC ESC [B      /# Down         #/
    ESC [1~,    ESC [1~,        ESC [1~,    ESC ESC [1~     /# Home         #/
    ESC [5~,    ESC [5~,        ESC [5~,    ESC ESC [5~     /# Page Up      #/
    ESC [6~,    ESC [6~,        ESC [6~,    ESC ESC [6~     /# Page Down    #/
    ESC [4~,    ESC [4~,        ESC [4~,    ESC ESC [4~     /# End          #/
    ESC [D,     ESC [D,         ESC [D,     ESC ESC [D,O    /# Keypad Left  #/
    ESC [C,     ESC [C,         ESC [C,     ESC ESC [C,O    /# Keypad Right #/
    ESC [A,     ESC [A,         ESC [A,     ESC ESC [A,O    /# Keypad Up    #/
    ESC [B,     ESC [B,         ESC [B,     ESC ESC [B,O    /# Keypad Down  #/
    ESC [1~,    ESC [1~,        ESC [1~,    ESC ESC [1~,O   /# Keypad Home  #/
    ESC [5~,    ESC [5~,        ESC [5~,    ESC ESC [5~,O   /# Keypad PgUp  #/
    ESC [6~,    ESC [6~,        ESC [6~,    ESC ESC [6~,O   /# Keypad PgDn  #/
    ESC [4~,    ESC [4~,        ESC [4~,    ESC ESC [4~,O   /# Keypad End   #/
    ESC [-71,   nothing,        nothing,    O               /# Keypad 5     #/

    Cygwin Console (term=cygwin, ncurses)
    ==============================
    KEY_LEFT,   KEY_LEFT,       KEY_LEFT,   ESC-260         /# Left         #/
    KEY_RIGHT,  KEY_RIGHT,      KEY_RIGHT,  ESC-261         /# Rght         #/
    KEY_UP,     KEY_UP,         KEY_UP,     ESC-259         /# Up           #/
    KEY_DOWN,   KEY_DOWN,       KEY_DOWN,   ESC-258         /# Down         #/
    KEY_HOME,   KEY_HOME,       KEY_HOME,   ESC-262         /# Home         #/
    KEY_PPAGE,  KEY_PPAGE,      KEY_PPAGE,  ESC-339         /# Page Up      #/
    KEY_NPAGE,  KEY_NPAGE,      KEY_NPAGE,  ESC-338         /# Page Down    #/
    KEY_END,    KEY_END,        KEY_END,    ESC-360         /# End          #/
    KEY_LEFT,   KEY_LEFT,       KEY_LEFT,   ESC-260,O       /# Keypad Left  #/
    KEY_RIGHT,  KEY_RIGHT,      KEY_RIGHT,  ESC-261,O       /# Keypad Right #/
    KEY_UP,     KEY_UP,         KEY_UP,     ESC-259,O       /# Keypad Up    #/
    KEY_DOWN,   KEY_DOWN,       KEY_DOWN,   ESC-258,O       /# Keypad Down  #/
    KEY_HOME,   KEY_HOME,       KEY_HOME,   ESC-262,O       /# Keypad Home  #/
    KEY_PPAGE,  KEY_PPAGE,      KEY_PPAGE,  ESC-339,O       /# Keypad PgUp  #/
    KEY_NPAGE,  KEY_NPAGE,      KEY_NPAGE,  ESC-338,O       /# Keypad PgDn  #/
    KEY_END,    KEY_END,        KEY_END,    ESC-360,O       /# Keypad End   #/
    ESC [G,     nothing,        nothing,    O               /# Keypad 5     #/

    MSYS Console (raw, ncurses)
    ==============================
    normal      shift           ctrl        alt
    ESC OD,     ESC [d,         ESC Od      nothing         /# Left         #/
    ESC OE,     ESC [e,         ESC Oe,     nothing         /# Right        #/
    ESC OA,     ESC [a,         ESC Oa,     nothing         /# Up           #/
    ESC OB,     ESC [b,         ESC Ob,     nothing         /# Down         #/
    ESC [7~,    ESC [7$,        ESC [7^,    nothing         /# Home         #/
    ESC [5~,    local window,   ESC [5^,    nothing         /# Page Up      #/
    ESC [6~,    local window,   ESC [6^,    nothing         /# Page Down    #/
    ESC [8~,    ESC [8$,        ESC [8^,    nothing         /# End          #/
    ESC OD,     ESC [d,         ESC Od      O               /# Keypad Left  #/
    ESC OE,     ESC [c,         ESC Oc,     O               /# Keypad Right #/
    ESC OA,     ESC [a,         ESC Oa,     O               /# Keypad Up    #/
    ESC OB,     ESC [b,         ESC Ob,     O               /# Keypad Down  #/
    ESC [7~,    ESC [7$,        ESC [7^,    O               /# Keypad Home  #/
    ESC [5~,    local window,   ESC [5^,    O               /# Keypad PgUp  #/
    ESC [6~,    local window,   ESC [6^,    O               /# Keypad PgDn  #/
    ESC [8~,    ESC [8$,        ESC [8^,    O               /# Keypad End   #/
    11,         11,             11,         O               /# Keypad 5     #/

    MSYS Console (term=rxvt, ncurses)
    ==============================
    normal      shift           ctrl        alt
    KEY_LEFT,   KEY_SLEFT,      514         nothing         /# Left         #/
    KEY_RIGHT,  KEY_SRIGHT,     516,        nothing         /# Right        #/
    KEY_UP,     518,            519,        nothing         /# Up           #/
    KEY_DOWN,   511,            512,        nothing         /# Down         #/
    KEY_HOME,   KEY_SHOME,      ESC [7^,    nothing         /# Home         #/
    KEY_PPAGE,  local window,   ESC [5^,    nothing         /# Page Up      #/
    KEY_NPAGE,  local window,   ESC [6^,    nothing         /# Page Down    #/
    KEY_END,    KEY_SEND,       KEY_EOL,    nothing         /# End          #/
    KEY_LEFT,   KEY_SLEFT,      514         O               /# Keypad Left  #/
    KEY_RIGHT,  KEY_SRIGHT,     516,        O               /# Keypad Right #/
    KEY_UP,     518,            519,        O               /# Keypad Up    #/
    KEY_DOWN,   511,            512,        O               /# Keypad Down  #/
    KEY_HOME,   KEY_SHOME,      ESC [7^,    O               /# Keypad Home  #/
    KEY_PPAGE,  local window,   ESC [5^,    O               /# Keypad PgUp  #/
    KEY_NPAGE,  local window,   ESC [6^,    O               /# Keypad PgDn  #/
    KEY_END,    KEY_SEND,       KEY_EOL,    O               /# Keypad End   #/
    11,         11,             11,         O               /# Keypad 5     #/

    Win32 Console (raw, pdcurses)
   DJGPP Console (raw, pdcurses)
   ==============================
   normal      shift           ctrl        alt
   260,        391,            443,        493             /# Left         #/
   261,        400,            444,        492             /# Right        #/
   259,        547,            480,        490             /# Up           #/
   258,        548,            481,        491             /# Down         #/
   262,        388,            447,        524             /# Home         #/
   339,        396,            445,        526             /# Page Up      #/
   338,        394,            446,        520             /# Page Down    #/
   358,        384,            448,        518             /# End          #/
   452,        52('4'),        511,        521             /# Keypad Left  #/
   454,        54('6'),        513,        523             /# Keypad Right #/
   450,        56('8'),        515,        525             /# Keypad Up    #/
   456,        50('2'),        509,        519             /# Keypad Down  #/
   449,        55('7'),        514,        524             /# Keypad Home  #/
   451,        57('9'),        516,        526             /# Keypad PgUp  #/
   457,        51('3'),        510,        520             /# Keypad PgDn  #/
   455,        49('1'),        508,        518             /# Keypad End   #/
   453,        53('5'),        512,        522             /# Keypad 5     #/

   Win32 Console (pdcurses, MSVC/MingW32)
   DJGPP Console (pdcurses)
   ==============================
   normal      shift           ctrl        alt
   KEY_LEFT,   KEY_SLEFT,      CTL_LEFT,   ALT_LEFT        /# Left         #/
   KEY_RIGHT,  KEY_SRIGHT,     CTL_RIGHT,  ALT_RIGHT       /# Right        #/
   KEY_UP,     KEY_SUP,        CTL_UP,     ALT_UP          /# Up           #/
   KEY_DOWN,   KEY_SDOWN,      CTL_DOWN,   ALT_DOWN        /# Down         #/
   KEY_HOME,   KEY_SHOME,      CTL_HOME,   ALT_HOME        /# Home         #/
   KEY_PPAGE,  KEY_SPREVIOUS,  CTL_PGUP,   ALT_PGUP        /# Page Up      #/
   KEY_NPAGE,  KEY_SNEXTE,     CTL_PGDN,   ALT_PGDN        /# Page Down    #/
   KEY_END,    KEY_SEND,       CTL_END,    ALT_END         /# End          #/
   KEY_B1,     52('4'),        CTL_PAD4,   ALT_PAD4        /# Keypad Left  #/
   KEY_B3,     54('6'),        CTL_PAD6,   ALT_PAD6        /# Keypad Right #/
   KEY_A2,     56('8'),        CTL_PAD8,   ALT_PAD8        /# Keypad Up    #/
   KEY_C2,     50('2'),        CTL_PAD2,   ALT_PAD2        /# Keypad Down  #/
   KEY_A1,     55('7'),        CTL_PAD7,   ALT_PAD7        /# Keypad Home  #/
   KEY_A3,     57('9'),        CTL_PAD9,   ALT_PAD9        /# Keypad PgUp  #/
   KEY_C3,     51('3'),        CTL_PAD3,   ALT_PAD3        /# Keypad PgDn  #/
   KEY_C1,     49('1'),        CTL_PAD1,   ALT_PAD1        /# Keypad End   #/
   KEY_B2,     53('5'),        CTL_PAD5,   ALT_PAD5        /# Keypad 5     #/

   Windows Telnet (raw)
   ==============================
   normal      shift           ctrl        alt
   ESC [D,     ESC [D,         ESC [D,     ESC [D          /# Left         #/
   ESC [C,     ESC [C,         ESC [C,     ESC [C          /# Right        #/
   ESC [A,     ESC [A,         ESC [A,     ESC [A          /# Up           #/
   ESC [B,     ESC [B,         ESC [B,     ESC [B          /# Down         #/
   ESC [1~,    ESC [1~,        ESC [1~,    ESC [1~         /# Home         #/
   ESC [5~,    ESC [5~,        ESC [5~,    ESC [5~         /# Page Up      #/
   ESC [6~,    ESC [6~,        ESC [6~,    ESC [6~         /# Page Down    #/
   ESC [4~,    ESC [4~,        ESC [4~,    ESC [4~         /# End          #/
   ESC [D,     ESC [D,         ESC [D,     ESC [D          /# Keypad Left  #/
   ESC [C,     ESC [C,         ESC [C,     ESC [C          /# Keypad Right #/
   ESC [A,     ESC [A,         ESC [A,     ESC [A          /# Keypad Up    #/
   ESC [B,     ESC [B,         ESC [B,     ESC [B          /# Keypad Down  #/
   ESC [1~,    ESC [1~,        ESC [1~,    ESC [1~         /# Keypad Home  #/
   ESC [5~,    ESC [5~,        ESC [5~,    ESC [5~         /# Keypad PgUp  #/
   ESC [6~,    ESC [6~,        ESC [6~,    ESC [6~         /# Keypad PgDn  #/
   ESC [4~,    ESC [4~,        ESC [4~,    ESC [4~         /# Keypad End   #/
   nothing,    nothing,        nothing,    nothing         /# Keypad 5     #/

   Windows Telnet (term=xterm)
   ==============================
   normal      shift           ctrl        alt
   KEY_LEFT,   KEY_LEFT,       KEY_LEFT,   KEY_LEFT        /# Left         #/
   KEY_RIGHT,  KEY_RIGHT,      KEY_RIGHT,  KEY_RIGHT       /# Right        #/
   KEY_UP,     KEY_UP,         KEY_UP,     KEY_UP          /# Up           #/
   KEY_DOWN,   KEY_DOWN,       KEY_DOWN,   KEY_DOWN        /# Down         #/
   ESC [1~,    ESC [1~,        ESC [1~,    ESC [1~         /# Home         #/
   KEY_PPAGE,  KEY_PPAGE,      KEY_PPAGE,  KEY_PPAGE       /# Page Up      #/
   KEY_NPAGE,  KEY_NPAGE,      KEY_NPAGE,  KEY_NPAGE       /# Page Down    #/
   ESC [4~,    ESC [4~,        ESC [4~,    ESC [4~         /# End          #/
    KEY_LEFT,   KEY_LEFT,       KEY_LEFT,   O               /# Keypad Left  #/
    KEY_RIGHT,  KEY_RIGHT,      KEY_RIGHT,  O               /# Keypad Right #/
    KEY_UP,     KEY_UP,         KEY_UP,     O               /# Keypad Up    #/
    KEY_DOWN,   KEY_DOWN,       KEY_DOWN,   O               /# Keypad Down  #/
    ESC [1~,    ESC [1~,        ESC [1~,    ESC [1~         /# Keypad Home  #/
    KEY_PPAGE,  KEY_PPAGE,      KEY_PPAGE,  KEY_PPAGE       /# Keypad PgUp  #/
    KEY_NPAGE,  KEY_NPAGE,      KEY_NPAGE,  KEY_NPAGE       /# Keypad PgDn  #/
    ESC [4~,    ESC [4~,        ESC [4~,    O               /# Keypad End   #/
    ESC [-71,   nothing,        nothing,    O               /# Keypad 5     #/

    PuTTY
    ==============================
    normal      shift           ctrl        alt
    ESC [D,     ESC [D,         ESC OD,     ESC [D          /# Left         #/
    ESC [C,     ESC [C,         ESC OC,     ESC [C          /# Right        #/
    ESC [A,     ESC [A,         ESC OA,     ESC [A          /# Up           #/
    ESC [B,     ESC [B,         ESC OB,     ESC [B          /# Down         #/
    ESC [1~,    ESC [1~,        local win,  ESC [1~         /# Home         #/
    ESC [5~,    local win,      local win,  ESC [5~         /# Page Up      #/
    ESC [6~,    local win,      local win,  ESC [6~         /# Page Down    #/
    ESC [4~,    ESC [4~,        local win,  ESC [4~         /# End          #/
    ESC [D,     ESC [D,         ESC [D,     O               /# Keypad Left  #/
    ESC [C,     ESC [C,         ESC [C,     O               /# Keypad Right #/
    ESC [A,     ESC [A,         ESC [A,     O               /# Keypad Up    #/
    ESC [B,     ESC [B,         ESC [B,     O               /# Keypad Down  #/
    ESC [1~,    ESC [1~,        ESC [1~,    O               /# Keypad Home  #/
    ESC [5~,    ESC [5~,        ESC [5~,    O               /# Keypad PgUp  #/
    ESC [6~,    ESC [6~,        ESC [6~,    O               /# Keypad PgDn  #/
    ESC [4~,    ESC [4~,        ESC [4~,    O               /# Keypad End   #/
    nothing,    nothing,        nothing,    O               /# Keypad 5     #/

    PuTTY
    ==============================
    normal      shift           ctrl        alt
    KEY_LEFT,   KEY_LEFT,       ESC OD,     ESC KEY_LEFT    /# Left         #/
    KEY_RIGHT   KEY_RIGHT,      ESC OC,     ESC KEY_RIGHT   /# Right        #/
    KEY_UP,     KEY_UP,         ESC OA,     ESC KEY_UP      /# Up           #/
    KEY_DOWN,   KEY_DOWN,       ESC OB,     ESC KEY_DOWN    /# Down         #/
    ESC [1~,    ESC [1~,        local win,  ESC ESC [1~     /# Home         #/
    KEY_PPAGE   local win,      local win,  ESC KEY_PPAGE   /# Page Up      #/
    KEY_NPAGE   local win,      local win,  ESC KEY_NPAGE   /# Page Down    #/
    ESC [4~,    ESC [4~,        local win,  ESC ESC [4~     /# End          #/
    ESC Ot,     ESC Ot,         ESC Ot,     O               /# Keypad Left  #/
    ESC Ov,     ESC Ov,         ESC Ov,     O               /# Keypad Right #/
    ESC Ox,     ESC Ox,         ESC Ox,     O               /# Keypad Up    #/
    ESC Or,     ESC Or,         ESC Or,     O               /# Keypad Down  #/
    ESC Ow,     ESC Ow,         ESC Ow,     O               /# Keypad Home  #/
    ESC Oy,     ESC Oy,         ESC Oy,     O               /# Keypad PgUp  #/
    ESC Os,     ESC Os,         ESC Os,     O               /# Keypad PgDn  #/
    ESC Oq,     ESC Oq,         ESC Oq,     O               /# Keypad End   #/
    ESC Ou,     ESC Ou,         ESC Ou,     O               /# Keypad 5     #/
*/

#define M_NORMAL 0
#define M_ESC    1
#define M_KEYPAD 2
#define M_TRAIL  3

#ifndef CTRL
#define CTRL(ch) (ch & 0x1F)
#endif

int undo[5];
int uindex = -1;

int
reread(void)
{
    int redo;

    if (uindex < 0)
        return 0;

    redo = undo[0];
    undo[0] = undo[1];
    undo[1] = undo[2];
    undo[2] = undo[3];
    undo[3] = undo[4];
    uindex--;
    return redo;
}

void
unread(int c)
{
    if (uindex >= 4)
        abort();

    undo[++uindex] = c;
}

int
md_readchar(WINDOW *win)
{
    int ch = 0;
    int lastch = 0;
    int wch = 0;
    int mode = M_NORMAL;
    int mode2 = M_NORMAL;
    int nodelayf = 0;
    int count = 0;

    for(;;)
    {
        if (mode == M_NORMAL && uindex >= 0)
        {
	    wch = ch = reread();
            break;
        }

	wch = ch = wgetch(win);

	if (ch == ERR) /* timed out  or error */
	{
	    if (nodelayf)               /* likely timed out, switch to */
	    {                           /* normal mode and block on    */
	        mode = M_NORMAL;        /* next read                   */
	    	nodelayf = 0;
		nodelay(win,0);
	    }
	    else if (count > 10)        /* after 10 errors assume      */
	        auto_save(0);           /* input stream is broken and  */
	    else                        /* auto save and exit          */
	    	count++;

	    continue;
	}

        count = 0;			/* reset input error count     */

        if (mode == M_TRAIL)
        {
            if (ch == '^')              /* msys console  : 7,5,6,8: modified*/
		ch = CTRL( toupper(lastch) );
	    else if (ch == '~')		/* cygwin console: 1,5,6,4: normal  */
                ch = tolower(lastch);   /* windows telnet: 1,5,6,4: normal  */
                                        /* msys console  : 7,5,6,8: normal  */
	    else if (mode2 == M_ESC)		/* cygwin console: 1,5,6,4: modified*/
		ch = CTRL( toupper(ch) );
            else
            {
                mode = M_NORMAL;
                unread(ch);
                continue;
            }

            break;
        }

        if (mode == M_ESC)
        {
            if (ch == 27)
            {
                mode2 = M_ESC;
		unread(ch);
                continue;
            }

            if ((ch == 'F') || (ch == 'O') || (ch == '['))
            {
                mode = M_KEYPAD;
		unread(ch);
                continue;
            }


            switch(ch)
            {
                /* Cygwin Console   */
                /* PuTTY            */
		case KEY_LEFT :	ch = CTRL('H'); break;
		case KEY_RIGHT: ch = CTRL('L'); break;
		case KEY_UP   : ch = CTRL('K'); break;
		case KEY_DOWN : ch = CTRL('J'); break;
		case KEY_HOME : ch = CTRL('Y'); break;
		case KEY_PPAGE: ch = CTRL('U'); break;
		case KEY_NPAGE: ch = CTRL('N'); break;
		case KEY_END  : ch = CTRL('B'); break;

	        default: mode = M_NORMAL;
                         mode2 = M_NORMAL;
                         unread(ch);
                         continue;}

            break;
        }

        if (mode == M_KEYPAD)
        {
            switch(ch)
            {
                /* ESC F - Interix Console codes */
		case   '^': ch = CTRL('H'); break;	/* Shift-Left	    */
		case   '$': ch = CTRL('L'); break;	/* Shift-Right	    */

                /* ESC [ - Interix Console codes */
                case   'H': ch = 'y'; break;            /* Home             */
		case     1: ch = CTRL('K'); break;	/* Ctl-Keypad Up    */
		case     2: ch = CTRL('J'); break;	/* Ctl-Keypad Down  */
		case     3: ch = CTRL('L'); break;	/* Ctl-Keypad Right */
		case     4: ch = CTRL('H'); break;	/* Ctl-Keypad Left  */
		case   263: ch = CTRL('Y'); break;	/* Ctl-Keypad Home  */
		case    19: ch = CTRL('U'); break;	/* Ctl-Keypad PgUp  */
		case    20: ch = CTRL('N'); break;	/* Ctl-Keypad PgDn  */
		case    21: ch = CTRL('B'); break;	/* Ctl-Keypad End   */

                /* ESC [ - Cygwin Console codes */
                case   'G': ch = '.'; break;            /* Keypad 5         */
                case   '7': lastch = 'Y'; mode=M_TRAIL; break;  /* Ctl-Home */
                case   '5': lastch = 'U'; mode=M_TRAIL; break;  /* Ctl-PgUp */
                case   '6': lastch = 'N'; mode=M_TRAIL; break;  /* Ctl-PgDn */

                /* ESC [ - Win32 Telnet, PuTTY */
                case   '1': lastch = 'y'; mode=M_TRAIL; break;  /* Home     */
                case   '4': lastch = 'b'; mode=M_TRAIL; break;  /* End      */

                /* ESC O - PuTTY */
		case   'D': ch = CTRL('H'); break;
		case   'C': ch = CTRL('L'); break;
		case   'A': ch = CTRL('K'); break;
		case   'B': ch = CTRL('J'); break;
                case   't': ch = 'h'; break;
                case   'v': ch = 'l'; break;
                case   'x': ch = 'k'; break;
                case   'r': ch = 'j'; break;
                case   'w': ch = 'y'; break;
                case   'y': ch = 'u'; break;
                case   's': ch = 'n'; break;
                case   'q': ch = 'b'; break;
                case   'u': ch = '.'; break;
            }

            if (mode != M_KEYPAD)
	    {
		unread(ch);
                continue;
        }
	}

        if (ch == 27)
        {
	    nodelay(win,1);
            mode = M_ESC;
	    nodelayf = 1;
	    unread(ch);
            continue;
        }

        switch(ch)
        {
            case KEY_LEFT   : ch = 'h'; break;
            case KEY_DOWN   : ch = 'j'; break;
            case KEY_UP     : ch = 'k'; break;
            case KEY_RIGHT  : ch = 'l'; break;
            case KEY_HOME   : ch = 'y'; break;
            case KEY_PPAGE  : ch = 'u'; break;
            case KEY_END    : ch = 'b'; break;
#ifdef KEY_LL
            case KEY_LL     : ch = 'b'; break;
#endif
            case KEY_NPAGE  : ch = 'n'; break;

#ifdef KEY_B1
            case KEY_B1     : ch = 'h'; break;
            case KEY_C2     : ch = 'j'; break;
            case KEY_A2     : ch = 'k'; break;
            case KEY_B3     : ch = 'l'; break;
#endif
            case KEY_A1     : ch = 'y'; break;
            case KEY_A3     : ch = 'u'; break;
            case KEY_C1     : ch = 'b'; break;
            case KEY_C3     : ch = 'n'; break;
            /* next should be '.', but for problem with putty/linux */
            case KEY_B2     : ch = 'u'; break;

#ifdef KEY_SLEFT
	    case KEY_SRIGHT  : ch = CTRL('L'); break;
	    case KEY_SLEFT   : ch = CTRL('H'); break;
#ifdef KEY_SUP
	    case KEY_SUP     : ch = CTRL('K'); break;
	    case KEY_SDOWN   : ch = CTRL('J'); break;
#endif
	    case KEY_SHOME   : ch = CTRL('Y'); break;
	    case KEY_SPREVIOUS:ch = CTRL('U'); break;
	    case KEY_SEND    : ch = CTRL('B'); break;
	    case KEY_SNEXT   : ch = CTRL('N'); break;
#endif
	    case 0x146       : ch = CTRL('K'); break; 	/* Shift-Up	*/
	    case 0x145       : ch = CTRL('J'); break; 	/* Shift-Down	*/

#ifdef CTL_RIGHT
	    case CTL_RIGHT   : ch = CTRL('L'); break;
	    case CTL_LEFT    : ch = CTRL('H'); break;
	    case CTL_UP      : ch = CTRL('K'); break;
	    case CTL_DOWN    : ch = CTRL('J'); break;
	    case CTL_HOME    : ch = CTRL('Y'); break;
	    case CTL_PGUP    : ch = CTRL('U'); break;
	    case CTL_END     : ch = CTRL('B'); break;
	    case CTL_PGDN    : ch = CTRL('N'); break;
#endif
#ifdef KEY_EOL
	    case KEY_EOL     : ch = CTRL('B'); break;
#endif

#ifndef CTL_PAD1
            /* MSYS rxvt console */
	    case 511	     : ch = CTRL('J'); break; /* Shift Dn */
	    case 512         : ch = CTRL('J'); break; /* Ctl Down */
	    case 514	     : ch = CTRL('H'); break; /* Ctl Left */
	    case 516	     : ch = CTRL('L'); break; /* Ctl Right*/
	    case 518	     : ch = CTRL('K'); break; /* Shift Up */
	    case 519	     : ch = CTRL('K'); break; /* Ctl Up   */
#endif

#ifdef CTL_PAD1
	    case CTL_PAD1   : ch = CTRL('B'); break;
	    case CTL_PAD2   : ch = CTRL('J'); break;
	    case CTL_PAD3   : ch = CTRL('N'); break;
	    case CTL_PAD4   : ch = CTRL('H'); break;
            case CTL_PAD5   : ch = '.'; break;
	    case CTL_PAD6   : ch = CTRL('L'); break;
	    case CTL_PAD7   : ch = CTRL('Y'); break;
	    case CTL_PAD8   : ch = CTRL('K'); break;
	    case CTL_PAD9   : ch = CTRL('U'); break;
#endif

#ifdef ALT_RIGHT
	    case ALT_RIGHT  : ch = CTRL('L'); break;
	    case ALT_LEFT   : ch = CTRL('H'); break;
	    case ALT_DOWN   : ch = CTRL('J'); break;
	    case ALT_HOME   : ch = CTRL('Y'); break;
	    case ALT_PGUP   : ch = CTRL('U'); break;
	    case ALT_END    : ch = CTRL('B'); break;
	    case ALT_PGDN   : ch = CTRL('N'); break;
#endif

#ifdef ALT_PAD1
	    case ALT_PAD1   : ch = CTRL('B'); break;
	    case ALT_PAD2   : ch = CTRL('J'); break;
	    case ALT_PAD3   : ch = CTRL('N'); break;
	    case ALT_PAD4   : ch = CTRL('H'); break;
            case ALT_PAD5   : ch = '.'; break;
	    case ALT_PAD6   : ch = CTRL('L'); break;
	    case ALT_PAD7   : ch = CTRL('Y'); break;
	    case ALT_PAD8   : ch = CTRL('K'); break;
	    case ALT_PAD9   : ch = CTRL('U'); break;
#endif
#ifdef KEY_BACKSPACE /* NCURSES in Keypad mode sends this for Ctrl-H */
            case KEY_BACKSPACE: ch = CTRL('H'); break;
#endif
        }

        break;
    }


    if (nodelayf)
        nodelay(win,0);

    uindex = -1;

    if (!pass_ctrl_keypad && (ch != wch))
        switch(ch) 
        {
	    case CTRL('H'):
	    case CTRL('L'):
	    case CTRL('K'):
	    case CTRL('J'):
	    case CTRL('Y'):
	    case CTRL('U'):
	    case CTRL('N'):
	    case CTRL('B'):
                return(ch + 0x40);
        }

    return(ch & 0x7F);
}

#if defined(LOADAV) && defined(HAVE_NLIST_H) && defined(HAVE_NLIST)
/*
 * loadav:
 *	Looking up load average in core (for system where the loadav()
 *	system call isn't defined
 */

#include <nlist.h>

struct nlist avenrun = {
    "_avenrun"
};

int
md_loadav(double *avg)
{
    int kmem;

    if ((kmem = open("/dev/kmem", 0)) < 0)
	goto bad;
    nlist(NAMELIST, &avenrun);
    if (avenrun.n_type == 0)
    {
	close(kmem);
bad:
	avg[0] = 0.0;
	avg[1] = 0.0;
	avg[2] = 0.0;
	return -1;
    }

    lseek(kmem, avenrun.n_value, 0);
    read(kmem, (char *) avg, 3 * sizeof (double));
    close(kmem);
    return 0;
}
#else
int
md_loadav(double *avg)
{
#if defined(HAVE_LOADAV)
    loadav(avg);
    return(0);
#elif defined(HAVE_GETLOADAVG)
    int ret;
    ret = getloadavg(avg,3);
    return (ret == 3) ? 0 : -1;
#else
    avg[0] = avg[1] = avg[2] = 0;
    return(-1);
#endif
}
#endif

#ifndef NSIG
#define NSIG 32
#endif

void
md_ignoreallsignals(void)
{
	int i;

	for (i = 0; i < NSIG; i++)
		signal(i, SIG_IGN);
}

void
md_tstphold(void)
{
#ifdef SIGTSTP
    /*
     * If a process can be suspended, this code wouldn't work
     */
# ifdef SIG_HOLD
    signal(SIGTSTP, SIG_HOLD);
# else
    signal(SIGTSTP, SIG_IGN);
# endif
#endif
}

void
md_tstpresume(void (*tstp)(int))
{
#ifdef SIGTSTP
    signal(SIGTSTP, tstp);
#else
    (void) tstp;
#endif
}

void
md_tstpsignal(void)
{
#ifdef SIGTSTP
    kill(0, SIGTSTP);		/* send actual signal and suspend process */
#endif
}

#ifdef CHECKTIME
void
md_start_checkout_timer(int time)
{
    int  checkout();

#if defined(HAVE_ALARM) && defined(SIGALRM)
    signal(SIGALRM, checkout);
	alarm(time);
#endif
}

void
md_stop_checkout_timer(void)
{
#if defined(SIGALRM)
    signal(SIGALRM, SIG_IGN);
#endif
}
#endif

long
md_memused(void)
{
#ifdef _WIN32
    MEMORYSTATUS stat;

    GlobalMemoryStatus(&stat);

    return((long)stat.dwTotalPageFile);
#else
    return( (long)(char *)sbrk(0) );
#endif
}

int
md_ucount(void)
{
#ifdef HAVE_UTMPX_H
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
#elif HAVE_UTMP_H
    struct utmp buf;
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
#else
    return 1;
#endif
}

int
md_lockfile(FILE *fp)
{
    int fd;
    int ret;

    fflush(fp);
    rewind(fp);

#ifdef _WIN32
    fd = fileno(fp);
    ret = _locking(fd,_LK_LOCK,1);
#elif defined(F_LOCK)
    fd = fileno(fp);

    while((ret = lockf(fd, F_LOCK, 1)) == -1)
	if (errno != EINTR)
            break;
#else
    (void) fd;
    ret = 1;
#endif

    return ret;
}

int
md_unlockfile(FILE *fp)
{
    int fd;
    int ret;

    fflush(fp);
    rewind(fp);

#ifdef _WIN32
    fd = _fileno(fp);
    ret = _locking(fd,_LK_UNLCK,1);
#elif defined(F_ULOCK)
    fd = fileno(fp);
    
    while( (ret = lockf(fd, F_ULOCK, 1)) == -1)
        if (errno != EINTR)
            break;
#else
    (void) fd;
    ret = 1;
#endif

    return ret;
}

char *
md_crypt(const char *pw, const char *salt)
{
#ifndef HAVE_CRYPT
	const unsigned char IP[64] = { 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61,
		53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7, 56, 48, 40, 32, 24, 16, 8, 0, 58, 50, 42, 34,
		26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6 };
	const unsigned char FP[64] = { 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37,
		5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10,
		50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25, 32, 0, 40, 8, 48, 16, 56, 24 };
	const unsigned char PC1[56] = { 56, 48, 40, 32, 24, 16, 8, 0, 57, 49, 41, 33, 25, 17, 9, 1, 58, 50,
		42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 60,
		52, 44, 36, 28, 20, 12, 4, 27, 19, 11, 3 };
	const unsigned char PC2[48] = { 13, 16, 10, 23, 0, 4, 2, 27, 14, 5, 20, 9, 22, 18, 11, 3, 25, 7, 15,
		6, 26, 19, 12, 1, 40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47, 43, 48, 38, 55, 33, 52, 45, 41, 49, 35,
		28, 31 };
	const unsigned char shifts[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
	const unsigned char S[8][64] = { {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4,
		14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4,
		9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}, {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2,
		8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4,
		2, 11, 6, 7, 12, 0, 5, 14, 9}, {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4, 6, 10, 2,
		8, 5, 14, 12, 11, 15, 1, 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15,
		14, 3, 11, 5, 2, 12}, {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2,
		12, 1, 10, 14, 9, 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11,
		12, 7, 2, 14}, {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10,
		3, 9, 8, 6, 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10,
		4, 5, 3}, {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11,
		3, 8, 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8,
		13}, {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
		1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}, {13,
		2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 7, 11,
		4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11} };
	const unsigned char P[32] = { 15, 6, 19, 20, 28, 11, 27, 16, 0, 14, 22, 25, 4, 17, 30, 9, 1, 7, 23,
		13, 31, 26, 2, 8, 18, 12, 29, 5, 21, 10, 3, 24 };
	unsigned char E[48] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16,
		17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };
	unsigned char LR[64], CD[56], KS[16][48], tempL[32], f[32], preS[48], block[66];
	unsigned char *L = &LR[0], *R = &LR[32];
	unsigned int i, j, k, c, t;
	static char iobuf[16];
	for (i = 0; (c = *pw++) && i<66; i++)
	for (j = 0; j<7 && i<66; j++, i++)
		block[i] = (c >> (6 - j)) & 01;
	while (i<66)
		block[i++] = 0;
	for (i = 0; i<56; i++)
		CD[i] = block[PC1[i]];
	for (i = 0; i<16; i++){
		for (j = 0; j<shifts[i]; j++){
			t = CD[0];
			for (k = 0; k<27; k++)
				CD[k] = CD[k + 1];
			CD[27] = t;
			t = CD[28];
			for (k = 28; k<55; k++)
				CD[k] = CD[k + 1];
			CD[55] = t;
		}
		for (j = 0; j<48; j++)
			KS[i][j] = CD[PC2[j]];
	}
	for (i = 0; i<66; i++)
		block[i] = 0;
	for (i = 0; i<2; i++){
		c = *salt++;
		iobuf[i] = c;
		if (c>'Z')c -= 6;
		if (c>'9')c -= 7;
		c -= '.';
		for (j = 0; j<6; j++)
		if ((c >> j) & 01){
			t = E[6 * i + j];
			E[6 * i + j] = E[6 * i + j + 24];
			E[6 * i + j + 24] = t;
		}
	}
	for (i = 0; i<25; i++){
		for (j = 0; j<64; j++)
			LR[j] = block[IP[j]];
		for (j = 0; j<16; j++){
			for (k = 0; k<32; k++)
				tempL[k] = R[k];
			for (k = 0; k<48; k++)
				preS[k] = R[E[k] - 1] ^ KS[j][k];
			for (k = 0; k<8; k++){
				t = 6 * k;
				c = S[k][(preS[t + 0] << 5) + (preS[t + 1] << 3) + (preS[t + 2] << 2) +
					(preS[t + 3] << 1) + (preS[t + 4] << 0) + (preS[t + 5] << 4)];
				t = 4 * k;
				f[t + 0] = (c >> 3) & 01;
				f[t + 1] = (c >> 2) & 01;
				f[t + 2] = (c >> 1) & 01;
				f[t + 3] = (c >> 0) & 01;
			}
			for (k = 0; k<32; k++)
				R[k] = L[k] ^ f[P[k]];
			for (k = 0; k<32; k++)
				L[k] = tempL[k];
		}
		for (j = 0; j<32; j++){
			t = L[j];
			L[j] = R[j];
			R[j] = t;
		}
		for (j = 0; j<64; j++)
			block[j] = LR[FP[j]];
	}
	for (i = 0; i<11; i++){
		c = 0;
		for (j = 0; j<6; j++){
			c <<= 1;
			c |= block[6 * i + j];
		}
		c += '.';
		if (c>'9')c += 7;
		if (c>'Z')c += 6;
		iobuf[i + 2] = c;
	}
	iobuf[i + 2] = 0;
	if (iobuf[1] == 0)
		iobuf[1] = iobuf[0];
	return(iobuf);
#else
	return crypt(pw, salt);
#endif
}

void
md_flushinp(void)
{
#if !defined(NCURSES_VERSION) && !defined(__PDCURSES__)
	raw();	/* flush input */
	noraw();
#else
	flushinp();
#endif
}
