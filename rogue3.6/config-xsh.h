#define HAVE_ALARM 1
#define HAVE_ARPA_INET_H 1
#define HAVE_CTYPE_H 1
#define HAVE_CURSES_H 1
#define HAVE_ERASECHAR 1
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_FORK 1
#ifndef _WIN32
#define HAVE_GETPASS 1
#endif
#define HAVE_GETPID 1
#define HAVE_INTTYPES_H 1
#define HAVE_KILLCHAR 1
#define HAVE_LIMITS_H 1
#define HAVE_MALLOC 1
#define HAVE_MEMORY_H 1
#define HAVE_MEMSET 1
#define HAVE_NCURSES_H 1
#define HAVE_NCURSES_TERM_H 1
#define HAVE_NLIST_H 1
#define HAVE_PROCESS_H 1
#ifndef _WIN32
#define HAVE_PWD_H 1
#endif
#define HAVE_SETENV 1
#define HAVE_SIGNAL_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRCHR 1
#define HAVE_STRERROR 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#ifndef _WIN32
#define HAVE_SYS_WAIT_H 1
#endif
#define HAVE_TERMIOS_H 1
#ifndef _WIN32
#define HAVE_TERM_H 1
#endif
#ifndef _WIN32
#define HAVE_UNISTD_H 1
#endif
#ifndef _WIN32
#define HAVE_UTMPX_H 1
#endif
#ifndef _WIN32
#define HAVE_UTMP_H 1
#endif
#define HAVE_VFORK 1
#define HAVE_VFORK_H 1
#ifndef _WIN32
#define HAVE_WORKING_FORK 1
#endif
#ifndef _WIN32
#define HAVE_WORKING_VFORK 1
#endif
#define HAVE__BOOL 1
#define PACKAGE "rogue"
#define PACKAGE_BUGREPORT "yendor@rogueforge.net"
#define PACKAGE_NAME "rogue"
#define PACKAGE_STRING "rogue 3.6"
#define PACKAGE_TARNAME "rogue"
#define PACKAGE_URL ""
#define PACKAGE_VERSION "3.6"
#define STDC_HEADERS 1
#define VERSION "3.6"

#ifdef _WIN32
typedef int ssize_t;
#endif

#ifdef _WIN32
typedef unsigned int uid_t;
#endif

#if defined(_WIN32) && !defined(__MINGW32__) && !defined(pid_t)
typedef unsigned int pid_t;
#endif

#if !defined(PATH_MAX) && defined(_MAX_PATH)
#define PATH_MAX _MAX_PATH
#endif

#if !defined(PATH_MAX) && defined(_PATH_MAX)
#define PATH_MAX _PATH_MAX
#endif

#if defined __CYGWIN32__ && !defined __CYGWIN__
   /* For backwards compatibility with Cygwin b19 and
      earlier, we define __CYGWIN__ here, so that
      we can rely on checking just for that macro. */
#  define __CYGWIN__  __CYGWIN32__
#endif

#if (defined _WIN32 && !defined __CYGWIN__) || defined(__DJGPP__)
   /* Use Windows separators on all _WIN32 defining
      environments, except Cygwin. */
#  define DIR_SEPARATOR_CHAR		'\\'
#  define DIR_SEPARATOR_STR		"\\"
#  define PATH_SEPARATOR_CHAR		';'
#  define PATH_SEPARATOR_STR		";"
#endif
#ifndef DIR_SEPARATOR_CHAR
   /* Assume that not having this is an indicator that all
      are missing. */
#  define DIR_SEPARATOR_CHAR		'/'
#  define DIR_SEPARATOR_STR		"/"
#  define PATH_SEPARATOR_CHAR		':'
#  define PATH_SEPARATOR_STR		":"
#endif /* !DIR_SEPARATOR_CHAR */

#ifdef _WIN32
#define fileno(fp) _fileno(fp)
#define fdopen(fd,mode) _fdopen(fd,mode)
#define close(fd) _close(fd)
#define chmod(filename,mode) _chmod(filename,mode)
#define getpid() _getpid()
#define setenv(n,v,o) _putenv(n "=" v)
#endif