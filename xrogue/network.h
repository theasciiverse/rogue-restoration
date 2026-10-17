/*
 * Note that networking is set up for machines that can communicate
 * via some system such as uucp.  The mechanism listed here uses uux
 * and assumes that the target machine allows access to the game via
 * the uux command.  NETCOMMAND must be defined if networking is desired.
 */

/* #undef  NETCOMMAND "uux - -n '%s!%s -u' >/dev/null 2>&1" */
/* #define NETCOMMAND "usend -s -d%s -uNoLogin -!'%s -u' - 2>/dev/null" */
#undef NETCOMMAND /* "" */

/* Networking information -- should not vary among networking machines */
#define SYSLEN 9
#define LOGLEN 8
#define NUMNET 6
#if MSDOS
#undef  NUMNET
#endif
struct network {
    char *system;
    char *rogue;
};
extern struct network Network[];

/* This system's name -- should not be defined if uname() is available */

#ifdef BSD
#define SYSTEM  "PHAEDRA"
#endif
#ifdef MSDOS
#define SYSTEM "PC-DOS"
#endif
#ifdef _WIN32
#define SYSTEM "Windows"
#endif

