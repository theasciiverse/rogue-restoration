#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([rogue], [3.6.5], [yendor@rogueforge.net])
AC_CONFIG_SRCDIR([armor.c])
AC_CONFIG_FILES([Makefile])
AC_CANONICAL_SYSTEM
AC_ARG_PROGRAM
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

AH_BOTTOM([
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
])

# Checks for programs.
: ${CFLAGS="-O"}
AC_PROG_CC

AC_DEFUN([AS_AC_EXPAND],
[
  EXP_VAR=[$1]
  FROM_VAR=[$2]

  dnl first expand prefix and exec_prefix if necessary
  prefix_save=$prefix
  exec_prefix_save=$exec_prefix

  dnl if no prefix given, then use /usr/local, the default prefix
  if test "x$prefix" = "xNONE"; then
    prefix="$ac_default_prefix"
  fi
  dnl if no exec_prefix given, then use prefix
  if test "x$exec_prefix" = "xNONE"; then
    exec_prefix=$prefix
  fi

  full_var="$FROM_VAR"
  dnl loop until it doesn't change anymore
  while true; do
    new_full_var="`eval echo $full_var`"
    if test "x$new_full_var" = "x$full_var"; then break; fi
    full_var=$new_full_var
  done

  dnl clean up
  full_var=$new_full_var
  AC_SUBST([$1], "$full_var")

  dnl restore prefix and exec_prefix
  prefix=$prefix_save
  exec_prefix=$exec_prefix_save
])


# Options to add to configure

AC_ARG_ENABLE(setgid, [AC_HELP_STRING([--enable-setgid=GROUP],
	[install executable as setgid with group ownership of NAME @<:@default=no@:>@])],[],[])

AC_MSG_CHECKING([if using setgid execute bit])

if test "x$enable_setgid" = "xno" ; then
	GROUP=
elif test "x$enable_setgid" = "xyes" ; then
	GROUP=games
elif test "x$enable_setgid" = "x" ; then
	GROUP=
else
	GROUP=$enable_setgid
fi

if test "x$GROUP" != "x" ; then
	AC_DEFINE_UNQUOTED([GROUP],[$GROUP], [Define to group owner of setgid executable])
	AC_MSG_RESULT([$GROUP])
else
	AC_MSG_RESULT([no])
fi

AC_SUBST(GROUP)

AS_AC_EXPAND(LOCALSTATEDIR, $localstatedir)

AC_ARG_ENABLE([scorefile],[AC_HELP_STRING([--enable-scorefile=SCOREFILE],
	[enable scoreboard with filename of SCOREFILE @<:@default=yes@:>@])],[],[])

AC_MSG_CHECKING([for scoreboard file])

if test "x$enable_scorefile" = "xno" ; then
	SCOREFILE=
elif test "x$enable_scorefile" = "xyes" ;  then
	SCOREFILE=$LOCALSTATEDIR/$PACKAGE_TARNAME.scr
elif test "x$enable_scorefile" = "x" ; then
	SCOREFILE=$LOCALSTATEDIR/$PACKAGE_TARNAME.scr
else
	SCOREFILE=$LOCALSTATEDIR/$enable_scorefile
fi

if test "x$SCOREFILE" != "x" ; then
	AC_DEFINE_UNQUOTED([SCOREFILE], ["$SCOREFILE"], [Define to file to use for scoreboard])
	AC_MSG_RESULT([$SCOREFILE])
else
	AC_MSG_RESULT([disabled])
fi

AC_SUBST(SCOREFILE)

AC_ARG_ENABLE([maxload],[AC_HELP_STRING([--enable-maxload=MAXLOAD], 
	[enable maximum allowed system load average of MAXLOAD @<:@default=no@:>@])],[],[])

AC_MSG_CHECKING([runtime execution limit (maximum system load average)])

if test "x$enable_maxload" = "xyes" ; then
	AC_DEFINE([MAXLOAD], [40], [Define if maxload feature should be enabled])
	_checktime=1
	AC_MSG_RESULT([40])
elif test "x$enable_maxload" = "x" ; then
	AC_MSG_RESULT([no])
elif test "x$enable_maxload" = "xno" ; then
	AC_MSG_RESULT([no])
else
	AC_DEFINE_UNQUOTED([MAXLOAD], [$enable_maxload], [Define if maxload feature should be enabled])
	_checktime=1
	AC_MSG_RESULT([$enable_maxload])
fi

AC_ARG_ENABLE([maxusers],[AC_HELP_STRING([--enable-maxusers=MAXUSERS], 
	[enable maximum allowed online system users of MAXUSERS @<:@default=no@:>@])],[],[])

AC_MSG_CHECKING([runtime execution limit (maximum online system users)])

if test "x$enable_maxusers" = "xyes" ; then
	AC_DEFINE([MAXUSERS], [25], [Define if maxusers feature should be enabled])
	_checktime=1
	AC_MSG_RESULT([25])
elif test "x$enable_maxusers" = "x" ; then
	AC_MSG_RESULT([no])
elif test "x$enable_maxusers" = "xno" ; then
	AC_MSG_RESULT([no])
else
	AC_DEFINE_UNQUOTED([MAXUSERS], [$enable_maxusers], [Define if maxusers feature should be enabled])
	_checktime=1
	AC_MSG_RESULT([$enable_maxusers])
fi

AC_ARG_ENABLE([checktime],[AC_HELP_STRING([--enable-checktime=CHECKTIME], 
	[enable checking of runtime execution limits every CHECKTIME minutes @<:@default=no@:>@])],[],[])

AC_MSG_CHECKING([runtime execution limit (frequency of checks)])

if test "x$enable_checktime" = "xyes" ; then
	if test "x$_checktime" = "x" ; then
		AC_MSG_RESULT([no (overridden)])
	else
		AC_DEFINE([CHECKTIME], [15], [Define if checktime feature should be enabled])
		AC_MSG_RESULT([15 minutes])
	fi
elif test "x$enable_checktime" = "x" ; then
	if test "x$_checktime" = "x" ; then
		AC_MSG_RESULT([no])
	else
		AC_MSG_RESULT([on startup])
	fi
elif test "x$enable_checktime" = "xno" ; then
	if test "x$_checktime" = "x" ; then
		AC_MSG_RESULT([no])
	else
		AC_MSG_RESULT([on startup])
	fi
else
	AC_DEFINE_UNQUOTED([CHECKTIME], [$enable_maxusers], [Define if checktime feature should be enabled])
	AC_MSG_RESULT([$(CHECKTIME) minutes])
fi

AC_ARG_ENABLE(autosave,
	AS_HELP_STRING([--enable-autosave],
		[enable autosave on system errors and signals @<:@default=yes@:>@]),
		[case "${enable_autosave}" in
			yes) 	;;
			no) 	;;
			*)	AC_MSG_ERROR([bad value ${enable_autosave} for --enable-autosave]) ;;
		esac], [])

AC_MSG_CHECKING([whether to autosave on system errors and signals])

if test "x$enable_autosave" = "xno" ; then
	AC_DEFINE([DUMP], [], [Define to enable default signal handling (core dump)])
	AC_MSG_RESULT([no])
else
	AC_MSG_RESULT([yes])
fi

AC_ARG_ENABLE([wizardmode],[AC_HELP_STRING([--enable-wizardmode=PASSWD], 
	[enable wizard mode with a password of PASSWD (which must be crypt(3) encrypted @<:@default=mTmZcVjBEccKk@:>@])],[],[])

AC_MSG_CHECKING([for wizard mode password])

if test "x$enable_wizardmode" = "xno" ; then
	AC_MSG_RESULT([no])
	AC_DEFINE_UNQUOTED([PASSWD],[""], [Define crypt(3) wizard mode password])
elif test "x$enable_wizardmode" = "x" ; then
	AC_DEFINE_UNQUOTED([PASSWD],["mTmZcVjBEccKk"], [Define crypt(3) wizard mode password])
	AC_MSG_RESULT([mTmZcVjBEccKk])
else
	if test "x$enable_wizardmode" != "xyes" ; then
		AC_DEFINE_UNQUOTED([PASSWD],["mTmZcVjBEccKk"], [Define crypt(3) wizard mode password])
		AC_MSG_RESULT([mTmZcVjBEccKk])
	else
		AC_DEFINE_UNQUOTED([PASSWD],["$enable_wizardmode"], [Define crypt(3) wizard mode password])
		AC_MSG_RESULT([$enable_wizardmode])
	fi
fi

AC_MSG_CHECKING([whether to docdir is defined])

if test "x$docdir" = "x" ; then
	AC_MSG_RESULT([docdir undefined])
	docdir=\${datadir}/doc/\${PACKAGE_TARNAME}
	AC_SUBST(docdir)
else
	AC_MSG_RESULT([docdir defined])
fi

# determine curses library to use

mp_save_LIBS="$LIBS"
CURSES_LIB=""

AC_CACHE_CHECK([for working curses], mp_cv_curses,
	[LIBS="$LIBS -lcurses"
AC_TRY_LINK(
	[#include <curses.h>],
	[chtype a; int b=A_STANDOUT, c=KEY_LEFT; initscr(); ],
	mp_cv_curses=yes, mp_cv_curses=no)])
if test "$mp_cv_curses" = yes
then
	AC_DEFINE(HAVE_CURSES_H, 1, [Define to 1 if libcurses is requested])
	CURSES_LIB="-lcurses"
fi

if test ! "$CURSES_LIB"
then
	AC_CACHE_CHECK([for working ncurses], mp_cv_ncurses,
		[LIBS="$mp_save_LIBS -lncurses"
	AC_TRY_LINK(
		[#include <curses.h>],
		[chtype a; int b=A_STANDOUT, c=KEY_LEFT; initscr(); ],
		mp_cv_ncurses=yes, mp_cv_ncurses=no)])
	if test "$mp_cv_ncurses" = yes
	then
		AC_DEFINE(HAVE_CURSES_H, 1, [Define to 1 if libcurses is requested])
		CURSES_LIB="-lncurses"
	fi
fi

if test ! "$CURSES_LIB"
then
	AC_CACHE_CHECK([for working pdcurses], mp_cv_pdcurses,
		[LIBS="$mp_save_LIBS -lpdcurses"
	AC_TRY_LINK(
		[#include <curses.h>],
		[chtype a; int b=A_STANDOUT, c=KEY_LEFT; initscr(); ],
		mp_cv_pdcurses=yes, mp_cv_pdcurses=no)])
	if test "$mp_cv_pdcurses" = yes
	then
		AC_DEFINE(HAVE_CURSES_H, 1, [Define to 1 if libcurses is requested])
		CURSES_LIB="-lpdcurses"
	fi
fi

if test ! "$CURSES_LIB"
then
	AC_CACHE_CHECK([for working pdcur], mp_cv_pdcur,
		[LIBS="$mp_save_LIBS -lpdcur"
	AC_TRY_LINK(
		[#include <curses.h>],
		[chtype a; int b=A_STANDOUT, c=KEY_LEFT; initscr(); ],
		mp_cv_pdcur=yes, mp_cv_pdcur=no)])
	if test "$mp_cv_pdcur" = yes
	then
		AC_DEFINE(HAVE_CURSES_H, 1, [Define to 1 if libcurses is requested])
		CURSES_LIB="-lpdcur"
	fi
fi

AC_CACHE_CHECK([for ESCDELAY variable],
	[mc_cv_ncurses_escdelay],
	[AC_TRY_LINK([], [
		extern int ESCDELAY;
		ESCDELAY = 0;
	],
	[mc_cv_ncurses_escdelay=yes],
	[mc_cv_ncurses_escdelay=no])
])

if test "$mc_cv_ncurses_escdelay" = yes; then
	AC_DEFINE(HAVE_ESCDELAY, 1,
		[Define if ncurses has ESCDELAY variable])
fi

if test ! "$CURSES_LIB" ; then
	LIBS="$mp_save_LIBS"
fi


# Checks for libraries.
AC_SEARCH_LIBS([tputs], [termcap termlib terminfo], [], [
    echo "Termlib library is required for this program"
    exit -1], [])

AC_SEARCH_LIBS([crypt], [crypt], [], [], [])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h ctype.h curses.h errno.h fcntl.h limits.h nlist.h process.h pwd.h signal.h sys/wait.h ncurses/term.h term.h utmp.h utmpx.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_FUNC_FORK

AC_CHECK_DECLS(environ)

# Checks for library functions.
AC_CHECK_FUNCS([killchar erasechar alarm getpid getpass memset setenv strchr strerror])

AC_CHECK_PROG([STRIP], [strip], [strip],)
AC_CHECK_PROG([RM], [rm], [rm -f],)
AC_CHECK_PROG([NROFF], [nroff], [nroff],)
AC_CHECK_PROG([GROFF], [groff], [groff],)
AC_CHECK_PROG([PDFROFF], [pdfroff], [pdfroff],)
AC_CHECK_PROG([TAR], [tar], [tar],)
AC_CHECK_PROG([GZIP], [gzip], [gzip],)
AC_CHECK_PROG([ZIP], [zip], [zip],)
AC_CHECK_PROG([COMPRESS], [compress], [compress],)
AC_CHECK_PROG([CP], [cp], [cp],)

if test "x$GROFF" != "xgroff"; then
AC_SUBST([GROFF], ["@echo \"'groff' is missing on your system\";\#"])
fi

if test "x$PDFROFF" != "xpdfroff"; then
AC_SUBST([PDFROFF], ["@echo \"'pdfroff' is missing on your system\";\#"])
fi

if test "x$ZIP" != "xzip"; then
AC_SUBST([ZIP], ["@echo \"'zip' is missing on your system\";\#"])
fi

AC_SUBST([PLATFORM],[$target_os-$target_cpu])

AC_OUTPUT
