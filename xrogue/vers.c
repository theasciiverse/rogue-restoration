/*
 * version number.  Whenever a new version number is desired, use
 * sccs to get vers.c.  Environ and encstr are declared here to
 * force them to be loaded before the version number, and therefore
 * not to be written in saved games.
 */

unsigned char encstr[] = "\0\0\0";
char version[] = "@(#)vers.c    8.0.4 - 01/04/2023";
char *release = "8.0.4";

