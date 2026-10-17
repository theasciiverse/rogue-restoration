/*
 * machine dependicies
 *
 * %G% (Berkeley) %W%
 */
#include "mdport.h"

/*
 * where scorefile should live
 */
#ifndef SCOREFILE
#define SCOREFILE	"/var/games/rogue_roll"
#endif

/*
 * Variables for checking to make sure the system isn't too loaded
 * for people to play
 */

/*#undef	MAXUSERS	25	*//* max number of users for this game */
/*#undef	MAXLOAD		40	*//* 10 * max 15 minute load average */

#if MAXUSERS|MAXLOAD
#ifndef CHECKTIME
#define	CHECKTIME	15	/* number of minutes between load checks */
				/* if not defined checks are only on startup */
#endif
#endif

#ifdef MAXLOAD
#undef	LOADAV			/* defined if rogue should provide loadav() */

#ifdef LOADAV
#define	NAMELIST	"/vmunix"	/* where the system namelist lives */
#endif
#endif

#ifdef MAXUSERS
#undef	UCOUNT			/* defined if rogue should provide ucount() */

#ifdef UCOUNT
#define UTMP	"/etc/utmp"	/* where utmp file lives */
#endif
#endif
