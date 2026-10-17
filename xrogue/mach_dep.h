/*
 * machine dependents
 */

/*
 * define/undefine that the wizard commands exist
 */

/* #undef       WIZARD  1 */
#define WIZARD  1

/*
 * where scorefile should live
 */

#ifdef MSDOS
#define SCOREFILE       "\\rogue\\score"
#endif

#ifndef SCOREFILE
#define SCOREFILE       "/usr/games/lib/xrogue_roll"
#endif

/*
 * define if you want to limit scores to one per class per userid
 */

/* #define LIMITSCORE 1*/
#undef LIMITSCORE

/* 
 * define that rogue should "nice()" itself
 */

#if !MSDOS
#undef NICE
#endif

#ifdef NICE

/* 
 * fudge factor allowed in time for saved game
 */

#define FUDGE_TIME      200
#else
#define FUDGE_TIME      200
#endif

#if defined(_WIN32)
#define fstat _fstat
#define stat _stat
#define open _open
#define popen _popen
#define pclose _pclose
#define ntoh md_ntoh
#define rand48 rand
#define srand48 srand
#define lrand48 rand
#define fileno _fileno
#define fdopen _fdopen
#define unlink _unlink
#define getpid _getpid
#define lseek _lseek
#define close _close
#define write _write
#define read _read
#if !defined(__MINGW32__)
#define PATH_MAX _MAX_PATH
#endif
#endif

#define NOOP(x) (x += 0)

/* 
 * dump core rather than catch the signal     
 */

#define DUMP            1       

/* 
 * If set, then don't check time on save file 
 */

/* #undef  NOCHECKACCESS */
#define  NOCHECKACCESS

/*
 * Variables for checking to make sure the system isn't too loaded
 * for people to play
 */

/*
 * max number of users for this game(MAXUSERS) and number processes 
 * including system processes but not including gettys(MAXPROCESSES)
 * Since UNIX commands are located in different directories (see main.c)
 * just undefine these two guys for now.
 */

#undef  MAXUSERS
#undef  MAXPROCESSES

/* #if u370 || uts
 * #undef  MAXUSERS        300
 * #undef  MAXPROCESSES    6000
 * #endif
 * 
 * #if vax || sun
 * #undef  MAXUSERS        100
 * #undef  MAXPROCESSES    2000
 * #endif
 * 
 * #if u3b || i386
 * #undef  MAXUSERS        30
 * #undef  MAXPROCESSES    1000
 * #endif
 * 
 * #ifdef  BSD
 * #undef  MAXUSERS        50
 * #undef  MAXPROCESSES    2000
 * #endif
 */ 

/* 
 * number of minutes between load checks
 * if not defined checks are only on startup.
 */

#undef  CHECKTIME

/*
 * If you do not want to allow xrogue to be played during prime shift
 * then undefine PLAYTIME.  Otherwise, game will limit playing to
 * weekends and non-primes-shift times.  (see main.c)
 */

#undef	PLAYTIME
/* #define  PLAYTIME 1 */

/* 
 * where utmp file lives 
 */

#define  UTMP    "/etc/utmp"     

/*
 * define the current author (Hactar) of the program for "special handling".
 */

#define  AUTHOR 500
#ifndef  AUTHOR
#define  AUTHOR 0
#endif

