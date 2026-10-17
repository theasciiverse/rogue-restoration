/*
    mdport.h - Machine Dependent Code for Porting Unix/Curses games

    Copyright (C) 2008 Nicholas J. Kisseberth
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-xsh.h"
#endif

#ifdef HAVE_UNISTD_H /* Windows does not */
#include <unistd.h>
#endif

#if !HAVE_DECL_ENVIRON
extern char **environ;
#endif

#include <curses.h>

#define MD_STRIP_CTRL_KEYPAD 1
#define MD_SETUP_SIGNALS 2

#ifdef __DJGPP__
#define EILSEQ 138
#endif

#ifdef max
#undef max
#endif

#ifdef CTRL
#undef CTRL
#endif

#ifndef HAVE_CRYPT
#define crypt(k,s) md_crypt(k,s)
#endif

#ifndef HAVE_GETPID
#define getpid() md_getpid()
#endif

void md_flushinp(void);
int md_setenv(const char *name, const char *value, int overwrite);
char *md_crypt(const char *key, const char *setting);
int	md_dsuspchar(void);
int	md_erasechar(void);
char *	md_gethomedir(void);
char *	md_getusername(void);
uid_t	md_getuid(void);
char *md_getpass(const char *prompt);
char *	md_getrealname(uid_t uid);
void	md_init(int options);
int	md_killchar(void);
void	md_normaluser(void);
void	md_raw_standout(void);
void	md_raw_standend(void);
int	md_readchar(WINDOW *win);
int	md_setdsuspchar(int c);
int	md_shellescape(void);
void	md_sleep(int s);
int	md_suspchar(void);
int	md_hasclreol(void);
int	md_unlink(char *file);
int	md_unlink_open_file(const char *file, int inf);
void	md_tstpsignal(void);
void	md_tstphold(void);
void	md_tstpresume(void (*tstp)(int));
void	md_ignoreallsignals(void);
void	md_onsignal_autosave(void);
void	md_onsignal_exit(void);
void	md_onsignal_default(void);
int	md_issymlink(char *sp);
int     md_loadav(double *avg);
long    md_memused(void);
int     md_ucount(void);
int     md_unlockfile(FILE *fp);
int     md_lockfile(FILE *fp);
int	md_getpid();
