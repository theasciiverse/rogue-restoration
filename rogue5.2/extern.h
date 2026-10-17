/*
 * Defines for things used in mach_dep.c
 *
 * @(#)extern.h	4.3 (Berkeley) 4/2/82
 */

/*
 * Don't change the constants, since they are used for sizes in many
 * places in the program.
 */

#define MAXSTR		80	/* maximum length of strings */
#define MAXLINES	32	/* maximum number of screen lines used */
#define MAXCOLS		80	/* maximum number of screen columns used */

#define RN		(((seed = seed*11109+13849) >> 16) & 0xffff)

/*
 * Now all the global variables
 */

extern bool	after, amulet, askme, door_stop, fight_flush,
		firstmove, in_shell, jump, noscore, p_know[], passgo,
		playing, r_know[], running, s_know[], save_msg,
		slow_invent, terse, wizard, ws_know[];

extern char	_flags[], _level[], *a_names[], file_name[], fruit[],
		home[], huh[], outbuf[], *p_colors[], *p_guess[],
		prbuf[], *r_guess[], *r_stones[], *release, runch,
		*s_guess[], *s_names[], take, *w_names[], whoami[],
		*ws_guess[], *ws_made[], *ws_type[];

extern int	a_chances[], a_class[], count, dnum, food_left,
		fung_hit, fd, group, hungry_state, inpack, lastscore,
		level, max_level, mpos, no_command, no_food, no_move,
		ntraps, purse, quiet, total;

extern long	seed;

extern WINDOW	*hw;

extern int cNCOLORS, cNMETAL, cNWOOD, cNSTONES;

extern char *rainbow[];
extern STONE stones[];
extern char *wood[];
extern char *metal[];

extern coord ch_ret;
extern shint countch;
extern shint direction;
extern shint newcount;
extern int   between;
extern int   num_checks;
extern char  lvl_mons[27];
extern char  wand_mons[27];
extern coord nh;
extern bool  got_genocide;

/*
 * Function types
 */

/* armor.c */
void wear(void);
void take_off(void);
int waste_time(void);
/* chase.c */
void runners(void);
int do_chase(register THING *th);
int see_monst(register THING *mp);
void runto(register coord *runner, coord *spot);
int chase(THING *tp, coord *ee);
struct room *roomin(register coord *cp);
int diag_ok(register coord *sp, register coord *ep);
int cansee(register int y, register int x);
coord *find_dest(register THING *tp);
/* command.c */
void command(void);
void illcom(int ch);
void search(void);
void help(void);
void identify(void);
void d_level(void);
void u_level(void);
void call(void);
/* daemon.c */
struct delayed_action *d_slot(void);
struct delayed_action *find_slot(register void (*func)(void));
void start_daemon(void (*func)(void), int arg, int type);
void kill_daemon(void (*func)(void));
void do_daemons(register int flag);
void fuse(void (*func)(void), int arg, int time, int type);
void lengthen(void (*func)(void), int xtime);
void extinguish(void (*func)(void));
void do_fuses(register int flag);
/* daemons.c */
void doctor(void);
void swander(void);
void rollwand(void);
void unconfuse(void);
void unsee(void);
void sight(void);
void nohaste(void);
void stomach(void);
/* extern.c */
/* fight.c */
int fight(register coord *mp, int mn, register THING *weap, int thrown);
int attack(register THING *mp);
int swing(int at_lvl, int op_arm, int wplus);
void check_level(void);
int roll_em(THING *thatt, THING *thdef, THING *weap, int hurl);
char *prname(register char *who, int upper);
void hit(register char *er, register char *ee);
void miss(register char *er, register char *ee);
int save_throw(int which, THING *tp);
int save(register int which);
int str_plus(register str_t str);
int add_dam(register str_t str);
void raise_level(void);
void thunk(register THING *weap, register char *mname);
void bounce(register THING *weap, register char *mname);
void remove_monster(register coord *mp, register THING *tp, int waskill);
int is_magic(register THING *obj);
void killed(register THING *tp, int pr);
/* init.c */
void init_player(void);
void init_things(void);
void init_colors(void);
void init_names(void);
void init_stones(void);
void init_materials(void);
void badcheck(char *name, register struct magic_item *magic, register int bound);
/* io.c */
void msg(char *fmt, ...);
void addmsg(char *fmt, ...);
void endmsg(void);
void doadd(char *fmt, va_list args);
int step_ok(char ch);
char readchar(WINDOW *win);
#if !defined(_XOPEN_CURSES) && !defined(__NCURSES_H)
char *unctrl(int ch);
#endif
void status(void);
void wait_for(WINDOW *win, int ch);
void show_win(register WINDOW *scr, char *message);
/* list.c */
void _detach(register THING **list, register THING *item);
void _attach(register THING **list, register THING *item);
void _free_list(register THING **ptr);
void discard(register THING *item);
THING *new_item(void);
/* mach_dep.c */
void init_check(void);
void open_score(void);
void setup(void);
void start_score(void);
int issymlink(char *sp);
int too_much(void);
int author(void);
void checkout(int sig);
void chmsg(char *fmt, ...);
void loadav(register double *avg);
int lock_sc(void);
void unlock_sc(void);
void flush_type(void);
/* main.c */
int main(int argc, char **argv, char **envp);
void endit(int sig);
void fatal(char *s);
int rnd(register int range);
int roll(register int number, register int sides);
void tstp(int sig);
void playit(void);
void quit(int sig);
void leave(int sig);
void shell(void);
/* mdport.c */
void md_init(void);
int md_hasclreol(void);
void md_putchar(int c);
void md_raw_standout(void);
void md_raw_standend(void);
int md_unlink_open_file(char *file, int inf);
int md_unlink(char *file);
FILE *md_fdopen(int fd, char *mode);
int md_fileno(FILE *fp);
int md_creat(char *file, int mode);
void md_normaluser(void);
int md_getuid(void);
int md_getpid(void);
char *md_getusername(void);
char *md_gethomedir(void);
void md_sleep(int s);
char *md_getshell(void);
void md_ignore_signals(void);
int md_shellescape(void);
int directory_exists(char *dirname);
char *md_getroguedir(void);
char *md_getrealname(int uid);
char *md_crypt(char *key, char *salt);
char *md_getpass(char *prompt);
unsigned long int md_ntohl(unsigned long int x);
unsigned long int md_htonl(unsigned long int x);
int md_ucount(void);
int md_getloadavg(double *avg);
long md_random(void);
void md_srandom(unsigned x);
int md_rand(void);
void md_srand(int seed);
char *md_strdup(const char *s);
long md_memused(void);
char *md_gethostname(void);
int md_erasechar(void);
int md_killchar(void);
char *md_unctrl(char ch);
void md_flushinp(void);
int md_readchar(WINDOW *win);
/* misc.c */
char *tr_name(int type);
void look(int wakeup);
THING *find_obj(register int y, register int x);
void eat(void);
void chg_str(register int amt);
void add_str(register str_t *sp, int amt);
int add_haste(int potion);
void aggravate(void);
char *vowelstr(register char *str);
int is_current(register THING *obj);
int get_dir(void);
int sign(register int nm);
int spread(register int nm);
void call_it(int know, register char **guess);
/* monsters.c */
int randmonster(int wander);
int new_monster(register THING *tp, int type, register coord *cp);
int exp_add(register THING *tp);
void wanderer(void);
THING *wake_monster(int y, int x);
void genocide(void);
void give_pack(register THING *tp);
/* move.c */
void do_run(int ch);
void do_move(int dy, int dx);
void turnref(void);
void door_open(struct room *rp);
int be_trapped(register coord *tc);
coord *rndmove(THING *who);
/* new_level.c */
void new_level(void);
int rnd_room(void);
void put_things(void);
void treas_room(void);
/* options.c */
void option(void);
int put_bool(_Bool *b);
int put_str(char *str);
int get_bool(_Bool *bp, WINDOW *win);
int get_str(register char *opt, WINDOW *win);
int get_num(short *opt, WINDOW *win);
void parse_opts(register char *str);
void strucpy(register char *s1, register char *s2, register int len);
/* pack.c */
void add_pack(register THING *obj, int silent);
int inventory(THING *list, int type);
void pick_up(int ch);
void picky_inven(void);
THING *get_item(char *purpose, int type);
char pack_char(register THING *obj);
void money(register int value);
/* passages.c */
void do_passages(void);
void conn(int r1, int r2);
void door(register struct room *rm, register coord *cp);
void add_pass(void);
void passnum(void);
void numpass(register int y, register int x);
/* potions.c */
void quaff(void);
void invis_on(void);
int turn_see(int turn_off);
/* rings.c */
void ring_on(void);
void ring_off(void);
int gethand(void);
int ring_eat(register int hand);
char *ring_num(register THING *obj);
/* rip.c */
void score(int amount, int flags, int monst);
void death(int monst);
void total_winner(void);
char *killname(int monst, int doart);
/* rooms.c */
void do_rooms(void);
void draw_room(register struct room *rp);
void vert(register struct room *rp, register int startx);
void horiz(register struct room *rp, int starty);
void rnd_pos(register struct room *rp, register coord *cp);
void enter_room(register coord *cp);
void leave_room(register coord *cp);
/* save.c */
int save_game(void);
void auto_save(int sig);
void save_file(register FILE *savef);
int restore(register char *file, char **envp);
void encwrite(register char *start, unsigned int size, register FILE *outf);
int encread(register char *start, unsigned int size, register int inf);
/* scrolls.c */
void read_scroll(void);
/* state.c */
extern int rs_save_file(FILE *savef);
extern int rs_restore_file(int inf);
/* sticks.c */
void fix_stick(register THING *cur);
void do_zap(void);
void drain(void);
void fire_bolt(coord *start, coord *dir, char *name);
char *charge_str(register THING *obj);
/* things.c */
char *inv_name(register THING *obj, int drop);
void drop(void);
int dropcheck(register THING *op);
THING *new_thing(void);
int pick_one(register struct magic_item *magic, int nitems);
void discovered(void);
void print_disc(int type);
void set_order(short *order, int numthings);
void add_line(char *fmt, ...);
void end_line(void);
char *nothing(int type);
/* vers.c */
/* weapons.c */
void missile(int ydelta, int xdelta);
void do_motion(register THING *obj, register int ydelta, register int xdelta);
void fall(register THING *obj, int pr);
void init_weapon(register THING *weap, int type);
int hit_monster(register int y, register int x, THING *obj);
char *num(register int n1, register int n2, int type);
void wield(void);
int fallpos(register coord *pos, register coord *newpos, int pass);
/* wizard.c */
void whatis(int insist);
void create_obj(void);
int teleport(void);
int passwd(void);
void show_map(void);
/* xcrypt.c */
extern char *xcrypt(const char *, const char *);

