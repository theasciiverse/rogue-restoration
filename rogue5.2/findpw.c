/*
 * print out an encrypted password on the standard output
 *
 * @(#)findpw.c	1.1 (Berkeley) 12/20/81
 */
#include <stdio.h>
#include <string.h>

extern char *xcrypt(const char *, const char *);

int
main(int argc, char *argv[])
{
    char buf[80];

    fprintf(stderr, "Password: ");
    if (fgets(buf, 80, stdin) == NULL)
        buf[0] = '\0';
    buf[strlen(buf) - 1] = '\0';
    printf("%s\n", xcrypt(buf, "mT"));
}
