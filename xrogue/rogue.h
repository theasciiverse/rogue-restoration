/*
 * Rogue definitions and variable declarations
 */

/*
 * some compiler don't handle void pointers well so
 */

#define VOID void
#define reg register

#undef SCROLL   /* UNIX/370 defines SCROLL for some bizarre reason */
#ifdef BSD
#undef tolower
#define _tolower(c)     ((c)-'A'+'a')
extern char tolower();
#undef toupper
#define _toupper(c)     ((c)-'a'+'A')
extern char toupper();
#define strchr index

#define exfork vfork    /* Better way to do a fork followed by an exec */
#else
#define exfork fork     /* Standard fork with no paging available */
#endif

#if defined USG5_0 || defined BSD
#define flushinp()      flushout()
#endif

/*
 * Maximum number of different things
 */

#define MAXCOLS         80
#define MAXLINES        24
#define MAXROOMS        9
#define MAXTHINGS       9
#define MAXOBJ          9
#define MAXSTATS        74      /* max total of all stats at startup */
#define MAXPACK         27      /* max number of items in pack */
#define MAXDOUBLE       14      /* max number of times exppts is doubled */
#define MAXCONTENTS     20      /* max number of things beaker/book can hold */
#define MAXENCHANT      30      /* max number of enchantments on an item */
#define MAXTREAS        25      /* number monsters/treasure in treasure room */
#define MAXTRAPS        20      /* max number of traps that may be on level */
#define MAXTRPTRY       15      /* attempts/level allowed for setting traps */
#define MAXDOORS        4       /* maximum doors to a room */
#define MAXCHANTS       16      /* maximum number of chants for a druid */
#define MAXPRAYERS      16      /* maximum number of prayers for cleric */
#define MAXSPELLS       16      /* maximum number of spells for magician */
#define MAXQUILL        14      /* scrolls the Quill of Nagrom can write */
#define QUILLCHARGES    300     /* max num of charges in the Quill of Nagrom */
#define NUM_CNAMES      26      /* number of names per character level */
#define NUMMONST        211     /* current number of monsters */
#define NUMUNIQUE       60      /* number of UNIQUEs (minus jacaranda) */
#define NUMDINOS        30      /* number of dinosaurs (for OUTSIDE level) */
#define NLEVMONS        3       /* number of new monsters per level */
#define NUMSCORE        20      /* number of entries in score file */
#define HARDER          40      /* at this level start making things harder */
#define LINELEN         256     /* characters in a buffer */
#define JUG_EMPTY       -1      /* signifys that the alchemy jug is empty */
#define MAXPURCH        (pstats.s_charisma/3) /* num of purchases at t.post */
#define MAXATT          50      /* charactor's attribute maximum number */

/* Movement penalties */
#define BACKPENALTY 3
#define SHOTPENALTY 2           /* In line of sight of missile */
#define DOORPENALTY 1           /* Moving out of current room */

/*
 * these defines are used in calls to get_item() to signify what
 * it is we want
 */

#define ALL             -1
#define WEARABLE        -2
#define CALLABLE        -3
#define WIELDABLE       -4
#define USEABLE         -5
#define IDENTABLE       -6
#define REMOVABLE       -7
#define PROTECTABLE     -8
#define ZAPPABLE        -9
#define READABLE        -10
#define QUAFFABLE       -11

/*
 * stuff to do with encumberance
 */

#define NORMENCB        1400    /* normal encumberance */
#define F_SATIATED       0      /* player's stomach is very full */
#define F_OKAY           1      /* have plenty of food in stomach */
#define F_HUNGRY         2      /* player is hungry */
#define F_WEAK           3      /* weak from lack of food */
#define F_FAINT          4      /* fainting from lack of food */

/*
 * actions a player/monster will take
 */

#define A_MOVE          0200    /* normal movement */
#define A_FREEZE        0201    /* frozen in place */
#define A_ATTACK        0202    /* trying to hit */
#define A_SELL          0203    /* trying to sell goods */
#define A_NIL           0204    /* not doing anything */
#define A_BREATHE       0205    /* breathing */
#define A_MISSILE       0206    /* Firing magic missiles */
#define A_SONIC         0207    /* Sounding a sonic blast */
#define A_SUMMON        0210    /* Summoning help */
#define A_USERELIC      0211    /* Monster uses a relic */
#define A_SLOW          0212    /* monster slows the player */
#define A_ZAP           0213    /* monster shoots a wand */
#define A_PICKUP        0214    /* player is picking something up */
#define A_USEWAND       0215    /* monster is shooting a wand */
#define A_THROW         't'
#define C_CAST          'C'
#define C_COUNT         '*'
#define C_DIP           'D'
#define C_DROP          'd'
#define C_EAT           'e'
#define C_PRAY          'p'
#define C_CHANT         'c'
#define C_QUAFF         'q'
#define C_READ          'r'
#define C_SEARCH        's'
#define C_SETTRAP       '^'
#define C_TAKEOFF       'T'
#define C_USE           CTRL('U')
#define C_WEAR          'W'
#define C_WIELD         'w'
#define C_ZAP           'z'

/* Possible ways for the hero to move */

#define H_TELEPORT 0

/*
 * return values for get functions
 */

#define NORM    0       /* normal exit */
#define QUIT    1       /* quit option setting */
#define MINUS   2       /* back up one option */

/* 
 * The character types
 */

#define C_FIGHTER       0
#define C_RANGER        1
#define C_PALADIN       2
#define C_MAGICIAN      3
#define C_CLERIC        4
#define C_THIEF         5
#define C_ASSASSIN      6
#define C_DRUID         7
#define C_MONK          8
#define C_MONSTER       9
#define NUM_CHARTYPES   10

/*
 * define the ability types
 */

#define A_INTELLIGENCE  0
#define A_STRENGTH      1
#define A_WISDOM        2
#define A_DEXTERITY     3
#define A_CONSTITUTION  4
#define A_CHARISMA      5
#define NUMABILITIES    6

/*
 * values for games end
 */

#define UPDATE  -2
#define SCOREIT -1
#define KILLED   0
#define CHICKEN  1
#define WINNER   2

/*
 * definitions for function step_ok:
 *      MONSTOK indicates it is OK to step on a monster -- it
 *      is only OK when stepping diagonally AROUND a monster;
 *      it is also OK if the stepper is a friendly monster and
 *      is in a fighting mood.
 */

#define MONSTOK 1
#define NOMONST 2
#define FIGHTOK 3

/*
 * used for ring stuff
 */

#define LEFT_1          0
#define LEFT_2          1
#define LEFT_3          2
#define LEFT_4          3
#define RIGHT_1         4
#define RIGHT_2         5
#define RIGHT_3         6
#define RIGHT_4         7
#define NUM_FINGERS     8

/*
 * used for micellaneous magic (MM) stuff
 */

#define WEAR_BOOTS      0
#define WEAR_BRACERS    1
#define WEAR_CLOAK      2
#define WEAR_GAUNTLET   3
#define WEAR_JEWEL      4
#define WEAR_NECKLACE   5
#define NUM_MM          6

/*
 * All the fun defines
 */

#define next(ptr) (*ptr).l_next
#define prev(ptr) (*ptr).l_prev
#define ldata(ptr) (*ptr).l_data
#define inroom(rp, cp) (\
    (cp)->x <= (rp)->r_pos.x + ((rp)->r_max.x - 1) && (rp)->r_pos.x <= (cp)->x \
 && (cp)->y <= (rp)->r_pos.y + ((rp)->r_max.y - 1) && (rp)->r_pos.y <= (cp)->y)
#define winat(y, x) (mvwinch(mw, y, x)==' '?mvwinch(stdscr, y, x):winch(mw))
#define debug if (wizard) msg
#define RN (((seed = seed*11109+13849) & 0x7fff) >> 1)
#define unc(cp) (cp).y, (cp).x
#define cmov(xy) move((xy).y, (xy).x)
#define DISTANCE(y1, x1, y2, x2) ((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
#define OBJPTR(what)    (struct object *)((*what).l_data)
#define THINGPTR(what)  (struct thing *)((*what).l_data)
#define DOORPTR(what)   (coord *)((*what).l_data)
#define when break;case
#define otherwise break;default
#define until(expr) while(!(expr))
#define ce(a, b) ((a).x == (b).x && (a).y == (b).y)
#define draw(window) wrefresh(window)
#ifdef USG5_2
#define newfont(window) if (funfont) wattron(window, A_ALTCHARSET);
#define nofont(window) if (funfont) wattroff(window, A_ALTCHARSET);
#else
#define newfont(window)
#define nofont(window)
#endif
#define hero player.t_pos
#define pstats player.t_stats
#define max_stats player.maxstats
#define pack player.t_pack
#define attach(a, b) _attach(&a, b)
#define detach(a, b) _detach(&a, b)
#define o_free_list(a) _o_free_list(&a)
#define r_free_list(a) _r_free_list(&a)
#define t_free_list(a) _t_free_list(&a)
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#define on(thing, flag) \
    (((thing).t_flags[(flag >> FLAGSHIFT) & FLAGINDEX] & flag) != 0)
#define off(thing, flag) \
    (((thing).t_flags[(flag >> FLAGSHIFT) & FLAGINDEX] & flag) == 0)
#define turn_on(thing, flag) \
    ((thing).t_flags[(flag >> FLAGSHIFT) & FLAGINDEX] |= (flag & ~FLAGMASK))
#define turn_off(thing, flag) \
    ((thing).t_flags[(flag >> FLAGSHIFT) & FLAGINDEX] &= ~flag)

/* define the control character */

#ifndef BSD
#undef CTRL
#define CTRL(ch) (ch & 037)
#endif

#define ALLOC(x) malloc((unsigned int) x)
#define FREE(x) free((char *) x)
#define EQSTR(a, b, c)  (strncmp(a, b, c) == 0)
#define EQUAL(a, b)     (strcmp(a, b) == 0)
#define GOLDCALC (rnd(50 + 10 * level) + 2)
#define ISRING(h, r) (cur_ring[h] != NULL && cur_ring[h]->o_which == r)
#define ISWEARING(r)    (ISRING(LEFT_1, r)  || ISRING(LEFT_2, r)  ||\
                         ISRING(LEFT_3, r)  || ISRING(LEFT_4, r)  ||\
                         ISRING(RIGHT_1, r) || ISRING(RIGHT_2, r) ||\
			 ISRING(RIGHT_3, r) || ISRING(RIGHT_4, r))
#define newgrp() ++group
#define o_charges o_ac
#define o_kind o_ac
#define ISMULT(type) (type == FOOD)
#define isrock(ch) ((ch == WALL) || (ch == HORZWALL) || (ch == VERTWALL) || (ch == SECRETDOOR))
#define is_stealth(tp) \
    (rnd(25) < (tp)->t_stats.s_dext || (tp == &player && ISWEARING(R_STEALTH)))

#ifdef BSD
#define flushout() ioctl(0, TIOCFLUSH)
#endif
#if USG5_0
#define flushout() ioctl(_tty_ch,TCFLSH,0)
#endif

#define has_light(rp) (((rp)->r_flags & HASFIRE) || ISWEARING(R_LIGHT))

#define mi_wght mi_worth
#define mi_food mi_curse

/*
 * Ways to die
 */

#define D_PETRIFY       -1
#define D_ARROW         -2
#define D_DART          -3
#define D_POISON        -4
#define D_BOLT          -5
#define D_SUFFOCATION   -6
#define D_POTION        -7
#define D_INFESTATION   -8
#define D_DROWN         -9
#define D_ROT           -10
#define D_CONSTITUTION  -11
#define D_STRENGTH      -12
#define D_SIGNAL        -13
#define D_CHOKE         -14
#define D_STRANGLE      -15
#define D_FALL          -16
#define D_RELIC         -17
#define D_STARVATION    -18
#define D_FOOD_CHOKE    -19
#define D_SCROLL        -20
#define D_FRIGHT        -21
#define D_CRYSTAL       -22
#define D_CARD          -23
#define DEATHNUM         23      /* number of ways to die */

/*
 * Things that appear on the screens
 */

#define WALL            ' '
#define PASSAGE         '#'
#define DOOR            '+'
#define FLOOR           '.'
#ifdef MSDOS
#define HORZWALL        196
#define VERTWALL        179
#define VPLAYER         '\001'
#else
#define HORZWALL        '-'
#define VERTWALL        '|'
#define VPLAYER         '@'
#endif
#define IPLAYER         '_'
#define POST            '^'
#define TRAPDOOR        '>'
#define ARROWTRAP       '{'
#define SLEEPTRAP       '$'
#define BEARTRAP        '}'
#define TELTRAP         '~'
#define DARTTRAP        '`'
#define WORMHOLE        '<'
#define POOL            '"'
#define MAZETRAP        '\''
#define SECRETDOOR      '&'
#define STAIRS          '%'
#define GOLD            '*'
#define POTION          '!'
#define SCROLL          '?'
#define MAGIC           '$'
#define BMAGIC          '>'     /*      Blessed magic   */
#define CMAGIC          '<'     /*      Cursed  magic   */
#define FOOD            ':'
#define WEAPON          ')'
#define MISSILE         '*'     /*      Magic Missile   */
#define ARMOR           ']'
#define MM              ';'
#define RELIC           ','
#define RING            '='
#define STICK           '/'
#define FOREST          '\\'

/*
 * Various constants
 * Crypt() returns a different string on the PC for some silly reason
 */

#define PASSWD          "mT5uKwhm5WDRs"
#define FIGHTBASE       10
#define BEFORE          1
#define AFTER           2
#define ESC             27
#define BOLT_LENGTH     12
#define MARKLEN         20
#define SLEEPTIME       (roll(15, 2))
#define BEARTIME        (roll(15, 2))
#define FREEZETIME      30
#define HEALTIME        40
#define SICKTIME        40
#define MORETIME        80
#define STOMACHSIZE     2100
#define PAINTIME        (roll(15, 2))
#define CLOAK_TIME      (roll(15, 2))
#define CHILLTIME       (roll(15, 2))
#define STONETIME       (roll(15, 2))
#define SMELLTIME       (50+rnd(30))
#define DUSTTIME        (50+rnd(30))
#define STINKTIME       (50+rnd(30))
#define HASTETIME       (50+rnd(30))
#define HUHDURATION     (50+rnd(30))
#define GONETIME        (50+rnd(30)) 
#define SKILLDURATION   (50+rnd(30))
#define SEEDURATION     (150+rnd(50))
#define CLRDURATION     (150+rnd(50))
#define FLYTIME         (150+rnd(50))
#define PHASEDURATION   (150+rnd(50))
#define ALCHEMYTIME     (250+rnd(100))
#define FIRETIME        (180+roll(20, 2))
#define COLDTIME        (180+roll(20, 2))
#define BOLTTIME        (180+roll(20, 2))
#define DAYLENGTH       700
#define LEVEL           700  /* make depth of dungeon equal to DAYLENGTH */
#define WANDERTIME      (max(5, (HARDER+rnd(25))-rnd(vlevel*2)))
#define SPELLTIME       ((max(30-pstats.s_lvl,5)))
#define vlevel          (max(level, turns/LEVEL + 1))

/*
 * Save against things
 */

#define VS_POISON               00
#define VS_PARALYZATION         00
#define VS_DEATH                00
#define VS_PETRIFICATION        01
#define VS_WAND                 02
#define VS_BREATH               03
#define VS_MAGIC                04

/*
 * attributes for treasures in dungeon
 */

#define ISCURSED               01
#define ISKNOW                 02
#define ISPOST                 04       /* object is in a trading post */
#define ISMETAL               010
#define ISPROT                020       /* object is protected */
#define ISBLESSED             040
#define ISPOISON             0100
#define ISMISL             020000
#define ISMANY             040000

/*
 * Various flag bits
 */

#define ISDARK                 01
#define ISGONE                 02
#define ISTREAS                04
#define ISFOUND               010
#define ISTHIEFSET            020
#define FORCEDARK             040

/*
 * 1st set of creature flags (this might include player)
 */

#define ISBLIND         0x00000001UL
#define ISINWALL        0x00000002UL
#define ISRUN           0x00000004UL
#define ISFLEE          0x00000008UL
#define ISINVIS         0x00000010UL
#define ISMEAN          0x00000020UL
#define ISGREED         0x00000040UL
#define CANSHOOT        0x00000080UL
#define ISHELD          0x00000100UL
#define ISHUH           0x00000200UL
#define ISREGEN         0x00000400UL
#define CANHUH          0x00000800UL
#define CANSEE          0x00001000UL
#define HASFIRE         0x00002000UL
#define ISSLOW          0x00004000UL
#define ISHASTE         0x00008000UL
#define ISCLEAR         0x00010000UL
#define CANINWALL       0x00020000UL
#define ISDISGUISE      0x00040000UL
#define CANBLINK        0x00080000UL
#define CANSNORE        0x00100000UL
#define HALFDAMAGE      0x00200000UL
#define CANSUCK         0x00400000UL
#define CANRUST         0x00800000UL
#define CANPOISON       0x01000000UL
#define CANDRAIN        0x02000000UL
#define ISUNIQUE        0x04000000UL
#define STEALGOLD       0x08000000UL

/* 
 * Second set of flags 
 */

#define STEALMAGIC      0x10000001UL
#define CANDISEASE      0x10000002UL
#define HASDISEASE      0x10000004UL
#define CANSUFFOCATE    0x10000008UL
#define DIDSUFFOCATE    0x10000010UL
#define BOLTDIVIDE      0x10000020UL
#define BLOWDIVIDE      0x10000040UL
#define NOCOLD          0x10000080UL
#define TOUCHFEAR       0x10000100UL
#define BMAGICHIT       0x10000200UL
#define NOFIRE          0x10000400UL
#define NOBOLT          0x10000800UL
#define CARRYGOLD       0x10001000UL
#define CANITCH         0x10002000UL
#define HASITCH         0x10004000UL
#define DIDDRAIN        0x10008000UL
#define WASTURNED       0x10010000UL
#define CANSELL         0x10020000UL
#define CANBLIND        0x10040000UL
#define NOACID          0x10080000UL
#define NOSLOW          0x10100000UL
#define NOFEAR          0x10200000UL
#define NOSLEEP         0x10400000UL
#define NOPARALYZE      0x10800000UL
#define NOGAS           0x11000000UL
#define CANMISSILE      0x12000000UL
#define CMAGICHIT       0x14000000UL
#define CANPAIN         0x18000000UL

/* 
 * Third set of flags 
 */

#define CANSLOW         0x20000001UL
#define CANTUNNEL       0x20000002UL
#define TAKEWISDOM      0x20000004UL
#define NOMETAL         0x20000008UL
#define MAGICHIT        0x20000010UL
#define CANINFEST       0x20000020UL
#define HASINFEST       0x20000040UL
#define NOMOVE          0x20000080UL
#define CANSHRIEK       0x20000100UL
#define CANDRAW         0x20000200UL
#define CANSMELL        0x20000400UL
#define CANPARALYZE     0x20000800UL
#define CANROT          0x20001000UL
#define ISSCAVENGE      0x20002000UL
#define DOROT           0x20004000UL
#define CANSTINK        0x20008000UL
#define HASSTINK        0x20010000UL
#define ISSHADOW        0x20020000UL
#define CANCHILL        0x20040000UL
#define CANHUG          0x20080000UL
#define CANSURPRISE     0x20100000UL
#define CANFRIGHTEN     0x20200000UL
#define CANSUMMON       0x20400000UL
#define TOUCHSTONE      0x20800000UL
#define LOOKSTONE       0x21000000UL
#define CANHOLD         0x22000000UL
#define DIDHOLD         0x24000000UL
#define DOUBLEDRAIN     0x28000000UL

/* 
 * Fourth set of flags 
 */

#define CANBRANDOM      0x30000001UL      /* Types of breath */
#define CANBACID        0x30000002UL      /* acid */
#define CANBFIRE        0x30000004UL      /* Fire */
#define CANBCGAS        0x30000008UL      /* confusion gas */
#define CANBBOLT        0x30000010UL      /* lightning bolt */
#define CANBGAS         0x30000020UL      /* chlorine gas */
#define CANBICE         0x30000040UL      /* ice */
#define CANBFGAS        0x30000080UL      /* Fear gas */
#define CANBPGAS        0x30000100UL      /* Paralyze gas */
#define CANBSGAS        0x30000200UL      /* Sleeping gas */
#define CANBSLGAS       0x30000400UL      /* Slow gas */
#define CANBREATHE      0x300007ffUL      /* Can it breathe at all? */

/*
 * Fifth set of flags
 */

#define ISUNDEAD        0x40000001UL
#define CANSONIC        0x40000002UL
#define TURNABLE        0x40000004UL
#define TAKEINTEL       0x40000008UL
#define NOSTAB          0x40000010UL
#define CANDISSOLVE     0x40000020UL
#define ISFLY           0x40000040UL      /* creature can fly */
#define CANTELEPORT     0x40000080UL      /* creature can teleport */
#define CANEXPLODE      0x40000100UL      /* creature explodes when hit */
#define CANDANCE        0x40000200UL      /* creature can make hero "dance" */
#define ISDANCE         0x40000400UL      /* creature (hero) is dancing */
#define CARRYFOOD       0x40000800UL
#define CARRYSCROLL     0x40001000UL
#define CARRYPOTION     0x40002000UL
#define CARRYRING       0x40004000UL
#define CARRYSTICK      0x40008000UL
#define CARRYMISC       0x40010000UL
#define CARRYMDAGGER    0x40020000UL      /* Dagger of Musty */
#define CARRYCLOAK      0x40040000UL      /* Cloak of Emori */
#define CARRYANKH       0x40080000UL      /* Ankh of Heil */
#define CARRYSTAFF      0x40100000UL      /* Staff of Ming */
#define CARRYWAND       0x40200000UL      /* Wand of Orcus */
#define CARRYROD        0x40400000UL      /* Rod of Asmodeus */
#define CARRYYAMULET    0x40800000UL      /* Amulet of Yendor */
#define CARRYMANDOLIN   0x41000000UL      /* Mandolin of Brian */
#define MISSEDDISP      0x42000000UL      /* Missed Cloak of Displacement */
#define CANBSTAB        0x44000000UL      /* Can backstab */
#define ISGUARDIAN      0x48000000UL      /* Guardian of a treasure room */

/*
 * Sixth set of flags
 */

#define CARRYHORN       0x50000001UL      /* Horn of Geryon */
#define CARRYMSTAR      0x50000002UL      /* Morning Star of Hruggek */
#define CARRYFLAIL      0x50000004UL      /* Flail of Yeenoghu */
#define CARRYWEAPON     0x50000008UL      /* A generic weapon */
#define CANAGE          0x50000010UL      /* can age you */
#define CARRYDAGGER     0x50000020UL      /* carry's a dumb old dagger */
#define AREMANY         0x50000040UL      /* they come in droves */
#define CARRYEYE        0x50000080UL      /* has the eye of Vecna */
#define HASSUMMONED     0x50000100UL      /* has already summoned */
#define ISSTONE         0x50000200UL      /* has been turned to stone */
#define NODETECT        0x50000400UL      /* detect monster will not show him */
#define NOSTONE         0x50000800UL      /* creature made its save vrs stone */
#define CARRYQUILL      0x50001000UL      /* has the quill of Nagrom */
#define CARRYAXE        0x50002000UL      /* has the axe of Aklad */
#define TOUCHSLOW       0x50004000UL      /* touch will slow hero */
#define WASDISRUPTED    0x50008000UL      /* creature was disrupted by player */
#define CARRYARMOR      0x50010000UL      /* creature will pick up armor */
#define CARRYBAMULET    0x50020000UL      /* amulet of skoraus stonebones */
#define CARRYSURTURRING 0x50040000UL      /* ring of Surtur */
#define CARRYCARD       0x50080000UL      /* carry the card of Alteran */
#define ISCHARMED       0x50100000UL      /* is the monster charmed? */
#define ISFRIENDLY      0x50100000UL      /* monster friendly for any reason? */

#define NEEDSTOACT      0x60000001      /* monster ready to act this turn n */
#define ISDEAD          0x60000002      /* monster is dead                  */
#define ISELSEWHERE     0x60000004      /* monster has been whisked away    */

/* Masks for choosing the right flag */

#define FLAGMASK     0xf0000000UL
#define FLAGINDEX    0x0000000fUL
#define FLAGSHIFT    28
#define MAXFLAGS     25                 /* max initial flags per creature */

/* 
 * Mask for cancelling special abilities 
 * The flags listed here will be the ones left on after the
 * cancellation takes place
 */

#define CANC0MASK (     ISBLIND         | ISINWALL      | ISRUN         | \
                        ISFLEE          | ISMEAN        | ISGREED       | \
                        CANSHOOT        | ISHELD        | ISHUH         | \
                        ISSLOW          | ISHASTE       | ISCLEAR       | \
                        ISUNIQUE )
#define CANC1MASK (     HASDISEASE      | DIDSUFFOCATE  | CARRYGOLD     | \
                        HASITCH         | CANSELL       | DIDDRAIN      | \
                        WASTURNED )
#define CANC2MASK (     HASINFEST       | NOMOVE        | ISSCAVENGE    | \
                        DOROT           | HASSTINK      | DIDHOLD )
#define CANC3MASK (     CANBREATHE )
#define CANC4MASK (     ISUNDEAD        | CANSONIC      | NOSTAB        | \
                        ISFLY           | CARRYFOOD     | CANEXPLODE    | \
                        ISDANCE         | CARRYSCROLL   | CARRYPOTION   | \
                        CARRYRING       | CARRYSTICK    | CARRYMISC     | \
                        CARRYMDAGGER    | CARRYCLOAK    | CARRYANKH     | \
                        CARRYSTAFF      | CARRYWAND     | CARRYROD      | \
                        CARRYYAMULET    | CARRYMANDOLIN | ISGUARDIAN )
#define CANC5MASK (     CARRYHORN       | CARRYMSTAR    | CARRYFLAIL    | \
                        CARRYEYE        | CARRYDAGGER   | HASSUMMONED   | \
                        AREMANY         | CARRYWEAPON   | NOSTONE       | \
                        CARRYQUILL      | CARRYAXE      | WASDISRUPTED  | \
                        CARRYARMOR      | CARRYBAMULET  | CARRYSURTURRING )
#define CANC6MASK ( 	CARRYCARD )
#define CANC7MASK ( 0 )
#define CANC8MASK ( 0 )
#define CANC9MASK ( 0 )
#define CANCAMASK ( 0 )
#define CANCBMASK ( 0 )
#define CANCCMASK ( 0 )
#define CANCDMASK ( 0 )
#define CANCEMASK ( 0 )
#define CANCFMASK ( 0 )

/* types of things */

#define TYP_POTION      0
#define TYP_SCROLL      1
#define TYP_FOOD        2
#define TYP_WEAPON      3
#define TYP_ARMOR       4
#define TYP_RING        5
#define TYP_STICK       6
#define TYP_MM          7
#define TYP_RELIC       8
#define NUMTHINGS       9

/*
 * food types
 */

#define E_RATION        0
#define E_APPLE         1
#define E_BANANA        2
#define E_BLUEBERRY     3
#define E_CANDLEBERRY   4
#define E_CAPRIFIG      5
#define E_DEWBERRY      6
#define E_ELDERBERRY    7
#define E_GOOSEBERRY    8
#define E_GUANABANA     9
#define E_HAGBERRY      10
#define E_JABOTICABA    11
#define E_PEACH         12
#define E_PITANGA       13
#define E_PRICKLEY      14
#define E_RAMBUTAN      15
#define E_SAPODILLA     16
#define E_SOURSOP       17
#define E_STRAWBERRY    18
#define E_SWEETSOP      19
#define E_WHORTLEBERRY  20
#define E_SLIMEMOLD     21
#define MAXFOODS        22

/*
 * Potion types
 */

#define P_CLEAR         0
#define P_ABIL          1
#define P_SEEINVIS      2
#define P_HEALING       3
#define P_MFIND         4
#define P_TFIND         5
#define P_RAISE         6
#define P_HASTE         7
#define P_RESTORE       8
#define P_PHASE         9
#define P_INVIS         10
#define P_FLY           11
#define P_FFIND         12
#define P_SKILL         13
#define P_FIRE          14
#define P_COLD          15
#define P_LIGHTNING     16
#define P_POISON        17
#define MAXPOTIONS      18

/*
 * Scroll types
 */

#define S_CONFUSE       0
#define S_MAP           1
#define S_LIGHT         2
#define S_HOLD          3
#define S_SLEEP         4
#define S_ALLENCH       5
#define S_IDENT         6
#define S_SCARE         7
#define S_GFIND         8
#define S_TELEP         9
#define S_CREATE        10
#define S_REMOVE        11
#define S_PETRIFY       12
#define S_GENOCIDE      13
#define S_CURING        14
#define S_MAKEIT        15
#define S_PROTECT       16
#define S_FINDTRAPS     17
#define S_RUNES         18
#define S_CHARM         19
#define MAXSCROLLS      20

/*
 * Weapon types
 */

#define MACE            0               /* mace */
#define SWORD           1               /* long sword */
#define BOW             2               /* short bow */
#define ARROW           3               /* arrow */
#define DAGGER          4               /* dagger */
#define ROCK            5               /* rocks */
#define TWOSWORD        6               /* two-handed sword */
#define SLING           7               /* sling */
#define DART            8               /* darts */
#define CROSSBOW        9               /* crossbow */
#define BOLT            10              /* crossbow bolt */
#define SPEAR           11              /* spear */
#define TRIDENT         12              /* trident */
#define SPETUM          13              /* spetum */
#define BARDICHE        14              /* bardiche */
#define PIKE            15              /* pike */
#define BASWORD         16              /* bastard sword */
#define HALBERD         17              /* halberd */
#define BATTLEAXE       18              /* battle axe */
#define MAXWEAPONS      19              /* types of weapons */
#define NONE            100             /* no weapon */

/*
 * Armor types
 */
 
#define LEATHER         0
#define RING_MAIL       1
#define STUDDED_LEATHER 2
#define SCALE_MAIL      3
#define PADDED_ARMOR    4
#define CHAIN_MAIL      5
#define SPLINT_MAIL     6
#define BANDED_MAIL     7
#define PLATE_MAIL      8
#define PLATE_ARMOR     9
#define MAXARMORS       10

/*
 * Ring types
 */
 
#define R_PROTECT       0
#define R_ADDSTR        1
#define R_SUSABILITY    2
#define R_SEARCH        3
#define R_SEEINVIS      4
#define R_ALERT         5
#define R_AGGR          6
#define R_ADDHIT        7
#define R_ADDDAM        8
#define R_REGEN         9
#define R_DIGEST        10
#define R_TELEPORT      11
#define R_STEALTH       12
#define R_ADDINTEL      13
#define R_ADDWISDOM     14
#define R_HEALTH        15
#define R_CARRY         16
#define R_LIGHT         17
#define R_DELUSION      18
#define R_FEAR          19
#define R_HEROISM       20
#define R_FIRE          21
#define R_WARMTH        22
#define R_VAMPREGEN     23
#define R_FREEDOM       24
#define R_TELCONTROL    25
#define MAXRINGS        26

/*
 * Rod/Wand/Staff types
 */
 
#define WS_LIGHT        0
#define WS_HIT          1
#define WS_ELECT        2
#define WS_FIRE         3
#define WS_COLD         4
#define WS_POLYMORPH    5
#define WS_MISSILE      6
#define WS_SLOW_M       7
#define WS_DRAIN        8
#define WS_CHARGE       9
#define WS_TELMON       10
#define WS_CANCEL       11
#define WS_CONFMON      12
#define WS_DISINTEGRATE 13
#define WS_PETRIFY      14
#define WS_PARALYZE     15
#define WS_MDEG         16
#define WS_CURING       17
#define WS_WONDER       18
#define WS_FEAR         19
#define MAXSTICKS       20

/*
 * miscellaneous magic items
 */
 
#define MM_JUG          0
#define MM_BEAKER       1
#define MM_BOOK         2
#define MM_ELF_BOOTS    3
#define MM_BRACERS      4
#define MM_OPEN         5
#define MM_HUNGER       6
#define MM_DISP         7
#define MM_PROTECT      8
#define MM_DRUMS        9
#define MM_DISAPPEAR    10
#define MM_CHOKE        11
#define MM_G_DEXTERITY  12
#define MM_G_OGRE       13
#define MM_JEWEL        14
#define MM_KEOGHTOM     15
#define MM_R_POWERLESS  16
#define MM_FUMBLE       17
#define MM_ADAPTION     18
#define MM_STRANGLE     19
#define MM_DANCE        20
#define MM_SKILLS       21
#define MM_CRYSTAL      22
#define MAXMM           23

/*
 * Relic types
 */
 
#define MUSTY_DAGGER            0
#define EMORI_CLOAK             1
#define HEIL_ANKH               2
#define MING_STAFF              3
#define ORCUS_WAND              4
#define ASMO_ROD                5
#define YENDOR_AMULET           6
#define BRIAN_MANDOLIN          7
#define GERYON_HORN             8
#define HRUGGEK_MSTAR           9
#define YEENOGHU_FLAIL          10
#define EYE_VECNA               11
#define AXE_AKLAD               12
#define QUILL_NAGROM            13
#define STONEBONES_AMULET       14
#define SURTUR_RING             15
#define ALTERAN_CARD            16
#define MAXRELIC                17

/* Now define the structures and types */

struct delayed_action {
        int d_type;
        void (*d_func)(void *arg);
        VOID *d_arg;
        int d_time;
} ;

/*
 * character types
 */
 
struct character_types {
    char        name[40];       /* name of character class              */
    long        start_exp;      /* starting exp pts for 2nd level       */
    long        cap;            /* stop doubling here                   */
    int         hit_pts;        /* hit pts gained per level             */
    int         base;           /* Base to-hit value (AC 10)            */
    int         max_lvl;        /* Maximum level for changing value     */
    int         factor;         /* Amount base changes each time        */
    int         offset;         /* What to offset level                 */
    int         range;          /* Range of levels for each offset      */
};

/*
 * level types
 */
 
typedef enum {
        NORMLEV,        /* normal level */
        POSTLEV,        /* trading post level */
        MAZELEV,        /* maze level */
        OUTSIDE,        /* outside region */
        STARTLEV        /* beginning of the game */
} LEVTYPE;

/*
 * Help lists
 */
 
struct h_list {
    char h_ch;
    char h_desc[40];
};

struct item_list {
    unsigned char item_ch;
    char item_desc[40];
};

/*
 * Coordinate data type
 */
 
typedef struct {
    int x;
    int y;
} coord;

/*
 * structure for the ways to die
 */
 
struct death_type {
    int reason;
    char name[30];
};

/*
 * Linked list data type
 */
 
struct linked_list {
    struct linked_list *l_next;
    struct linked_list *l_prev;
    char *l_data;                       /* Various structure pointers */
};

/*
 * Stuff about magic items
 */
 
struct magic_item {
    char mi_name[30];
    int  mi_prob;
    int  mi_worth;
    int  mi_curse;
    int  mi_bless;
};

/*
 * Room structure
 */
 
struct room {
    coord r_pos;                        /* Upper left corner */
    coord r_max;                        /* Size of room */
    long r_flags;                       /* Info about the room */
    struct linked_list *r_fires;        /* List of fire creatures in room */
    struct linked_list *r_exit;         /* Linked list of exits */
};

/*
 * Array of all traps on this level
 */
 
struct trap {
    char tr_type;                       /* What kind of trap */
    char tr_show;                       /* Where disguised trap looks like */
    coord tr_pos;                       /* Where trap is */
    long tr_flags;                      /* Info about trap (i.e. ISFOUND) */
};

/*
 * Structure describing a fighting being
 */
 
struct stats {
    short s_str;                        /* Strength */
    short s_intel;                      /* Intelligence */
    short s_wisdom;                     /* Wisdom */
    short s_dext;                       /* Dexterity */
    short s_const;                      /* Constitution */
    short s_charisma;                   /* Charisma */
    unsigned long s_exp;                /* Experience */
    int s_lvladj;                       /* how much level is adjusted */
    int s_lvl;                          /* Level of mastery */
    int s_arm;                          /* Armor class */
    int s_hpt;                          /* Hit points */
    int s_pack;                         /* current weight of his pack */
    int s_carry;                        /* max weight he can carry */
    char s_dmg[8];                      /* String describing damage done */
};

/*
 * Structure describing a fighting being (monster at initialization)
 */
 
struct mstats {
    short ms_str;                        /* Strength */
    short ms_dex;                        /* dexterity */
    short ms_move;                       /* movement rate */
    unsigned long ms_exp;                /* Experience */
    short ms_lvl;                        /* Level of mastery */
    short ms_arm;                        /* Armor class */
    char ms_hpt[9];                        /* Hit points */
    char ms_dmg[30];                      /* String describing damage done */
};

/*
 * Structure for monsters and player
 */
 
struct thing {
    bool t_wasshot;                     /* Was character shot last round? */
    char t_type;                        /* What it is */
    char t_disguise;                    /* What mimic looks like */
    unsigned char t_oldch;              /* Character that was where it was */
    short t_ctype;                      /* Character type */
    short t_index;                      /* Index into monster table */
    short t_no_move;                    /* How long the thing can't move */
    short t_quiet;                      /* used in healing */
    short t_movement;                   /* Base movement rate */
    short t_action;                     /* Action we're waiting to do */
    short t_artifact;                   /* base chance of using artifact */
    short t_wand;                       /* base chance of using wands */
    short t_summon;                     /* base chance of summoning */
    short t_cast;                       /* base chance of casting a spell */
    short t_breathe;                    /* base chance to swing at player */
    char  *t_name;                      /* name player gave his pet */
    coord t_doorgoal;                   /* What door are we heading to? */
    coord *t_dest;                      /* Where it is running to */
    coord t_pos;                        /* Position */
    coord t_oldpos;                     /* Last position */
    coord t_newpos;                     /* Where we want to go */
    unsigned long t_flags[16];          /* State word */
    struct linked_list *t_pack;         /* What the thing is carrying */
    struct linked_list *t_using;        /* What the thing is using */
	int t_selection;
    struct stats t_stats;               /* Physical description */
    struct stats maxstats;              /* maximum(or initial) stats */
    int    t_reserved;                  /* reserved for save/restore code */
};

/*
 * Array containing information on all the various types of monsters
 */
 
struct monster {
    char m_name[30];                    /* What to call the monster */
    short m_carry;                      /* Probability of carrying something */
    bool m_normal;                      /* Does monster exist? */
    bool m_wander;                      /* Does monster wander? */
    char m_appear;                      /* What does monster look like? */
    char m_intel[8];                    /* Intelligence range */
    long m_flags[MAXFLAGS];             /* Things about the monster */
    char m_typesum[30];                 /* type of creature can he summon */
    short m_numsum;                     /* how many creatures can he summon */
    short m_add_exp;                    /* Added experience per hit point */
    struct mstats m_stats;              /* Initial stats */
};

/*
 * Structure for a thing that the rogue can carry
 */
 
struct object {
    int o_type;                         /* What kind of object it is */
    coord o_pos;                        /* Where it lives on the screen */
    char o_launch;                      /* What you need to launch it */
    char o_damage[8];                   /* Damage if used like sword */
    char o_hurldmg[8];                  /* Damage if thrown */
    struct linked_list *contents;       /* contents of this object */
    int o_count;                        /* Count for plural objects */
    int o_which;                        /* Which object of a type it is */
    int o_hplus;                        /* Plusses to hit */
    int o_dplus;                        /* Plusses to damage */
    int o_ac;                           /* Armor class */
    long o_flags;                       /* Information about objects */
    int o_group;                        /* Group number for this object */
    int o_weight;                       /* weight of this object */
    char o_mark[MARKLEN];               /* Mark the specific object */
};

/*
 * weapon structure
 */
 
struct init_weps {
    char w_name[20];            /* name of weapon */
    char w_dam[8];              /* hit damage */
    char w_hrl[8];              /* hurl damage */
    char w_launch;              /* need to launch it */
    int  w_flags;               /* flags */
    int  w_rate;                /* rate of fire */
    int  w_wght;                /* weight of weapon */
    int  w_worth;               /* worth of this weapon */
};

/*
 * armor structure 
 */
 
struct init_armor {
        char a_name[30];        /* name of armor */
        int  a_prob;            /* chance of getting armor */
        int  a_class;           /* normal armor class */
        int  a_worth;           /* worth of armor */
        int  a_wght;            /* weight of armor */
};

struct spells {
    short s_which;              /* which scroll or potion */
    short s_cost;               /* cost of casting spell */
    short s_type;               /* scroll or potion */
    int   s_flag;               /* is the spell blessed/cursed? */
};

struct words
{
    char w_string[30];
};

/*
 * Other structures
 */
 
#ifdef CHECKTIME
int checkout();
#endif

/*
 * Now all the global variables
 */
 
extern struct trap traps[];
extern struct h_list helpstr[];
extern struct h_list wiz_help[];
extern struct item_list item_help[];
extern struct character_types char_class[];  /* character classes */
extern struct room rooms[];             /* One for each room -- A level */
extern struct room *oldrp;              /* Roomin(&oldpos) */
extern struct linked_list *mlist;       /* List of monsters on the level */
extern struct linked_list *tlist;       /* list of monsters fallen down traps */
extern struct linked_list *rlist;       /* list of monsters that have died    */
extern struct death_type deaths[];      /* all the ways to die */
extern struct thing player;             /* The rogue */
extern struct monster monsters[NUMMONST+1];       /* The initial monster states */
extern struct linked_list *lvl_obj;     /* List of objects on this level */
extern struct linked_list *monst_dead;  /* Indicates monster that got killed */
extern struct object *cur_weapon;       /* Which weapon he is weilding */
extern struct object *cur_armor;        /* What a well dresssed rogue wears */
extern struct object *cur_ring[];       /* Which rings are being worn */
extern struct object *cur_misc[];       /* which MM's are in use */
extern struct magic_item things[];      /* Chances for each type of item */
extern struct magic_item s_magic[];     /* Names and chances for scrolls */
extern struct magic_item p_magic[];     /* Names and chances for potions */
extern struct magic_item r_magic[];     /* Names and chances for rings */
extern struct magic_item ws_magic[];    /* Names and chances for sticks */
extern struct magic_item m_magic[];     /* Names and chances for MM */
extern struct magic_item rel_magic[];   /* Names and chances for relics */
extern struct magic_item foods[];       /* Names and chances for foods */
extern struct spells magic_spells[];    /* spells for magicians */
extern struct spells cleric_spells[];   /* spells for clerics */
extern struct spells druid_spells[];    /* spells for druids */
extern struct spells quill_scrolls[];   /* scrolls for quill */
extern char *cnames[NUM_CHARTYPES-1][NUM_CNAMES];      /* Character level names */
extern char *abilities[NUMABILITIES];   /* Names of the various abilities */
extern char curpurch[];                 /* name of item ready to buy */
extern char PLAYER;                     /* what the player looks like */
extern int nfloors;                     /* Number of floors in this dungeon */
extern int cols;                        /* number of columns on terminal */
extern int lines;                       /* number of lines in terminal */
extern int char_type;                   /* what type of character is player */
extern int foodlev;                     /* how fast he eats food */
extern int level;                       /* What level rogue is on */
extern int trader;                      /* number of purchases */
extern int curprice;                    /* price of an item */
extern long purse;                      /* How much gold the rogue has */
extern int mpos;                        /* Where cursor is on top line */
extern int ntraps;                      /* Number of traps on this level */
extern int inpack;                      /* Number of things in pack */
extern int total;                       /* Total dynamic memory bytes */
extern int lastscore;                   /* Score before this turn */
extern int no_food;                     /* Number of levels without food */
extern int foods_this_level;            /* num of foods this level */
extern int seed;                        /* Random number seed */
extern int count;                       /* Number of times to repeat command */
extern int max_level;                   /* Deepest player has gone */
extern int cur_max;                     /* Deepest player has gone currently */
extern int prev_max;                    /* A flag for worm hole */
extern int move_free;                   /* Free movement check */
extern int food_left;                   /* Amount of food in hero's stomach */
extern int group;                       /* Current group number */
extern int hungry_state;                /* How hungry is he */
extern int infest_dam;                  /* Damage from parasites */
extern int lost_str;                    /* Amount of strength lost */
extern int hold_count;                  /* Number of monsters holding player */
extern int trap_tries;                  /* Number of attempts to set traps */
extern int chant_time;                  /* Number of chant points/exp level */
extern int pray_time;                   /* Number of prayer points/exp level */
extern int spell_power;                 /* Spell power left at this level */
extern long turns;                      /* Number of turns player has taken */
extern int quest_item;                  /* Item hero is looking for */
extern int cur_relic[];                 /* Current relics */
extern char take;                       /* Thing the rogue is taking */
extern char prbuf[];                    /* Buffer for sprintfs */
extern char outbuf[];                   /* Output buffer for stdout */
extern char runch;                      /* Direction player is running */
extern char *s_names[];                 /* Names of the scrolls */
extern char *p_colors[];                /* Colors of the potions */
extern char *r_stones[];                /* Stone settings of the rings */
extern struct init_weps weaps[];        /* weapons and attributes */
extern struct init_armor armors[];      /* armors and attributes */
extern char *ws_made[];                 /* What sticks are made of */
extern char *release;                   /* Release number of rogue */
extern char whoami[];                   /* Name of player */
extern char fruit[];                    /* Favorite fruit */
extern char huh[];                      /* The last message printed */
extern char *s_guess[];                 /* Players guess at what scroll is */
extern char *p_guess[];                 /* Players guess at what potion is */
extern char *r_guess[];                 /* Players guess at what ring is */
extern char *ws_guess[];                /* Players guess at what wand is */
extern char *m_guess[];                 /* Players guess at what MM is */
extern char *ws_type[];                 /* Is it a wand or a staff */
extern char file_name[];                /* Save file name */
extern char score_file[];               /* Score file name */
extern char home[];                     /* User's home directory */
extern WINDOW *cw;                      /* Window that the player sees */
extern WINDOW *hw;                      /* Used for the help command */
extern WINDOW *mw;                      /* Used to store mosnters */
extern WINDOW *msgw;                    /* Message window */
extern bool pool_teleport;              /* just teleported from a pool */
extern bool inwhgt;                     /* true if from wghtchk() */
extern bool running;                    /* True if player is running */
extern bool playing;                    /* True until he quits */
extern bool wizard;                     /* True if allows wizard commands */
extern bool after;                      /* True if we want after daemons */
extern bool notify;                     /* True if player wants to know */
extern bool fight_flush;                /* True if toilet input */
extern bool terse;                      /* True if we should be short */
extern bool auto_pickup;                /* Pick up things automatically? */
extern bool menu_overlay;               /* Use overlay type menu */
extern bool door_stop;                  /* Stop running when we pass a door */
extern bool jump;                       /* Show running as series of jumps */
extern bool slow_invent;                /* Inventory one line at a time */
extern bool def_attr;                   /* True for default attributes */
extern bool firstmove;                  /* First move after setting door_stop */
extern bool waswizard;                  /* Was a wizard sometime */
extern bool askme;                      /* Ask about unidentified things */
extern bool s_know[];                   /* Does he know what a scroll does */
extern bool p_know[];                   /* Does he know what a potion does */
extern bool r_know[];                   /* Does he know what a ring does */
extern bool ws_know[];                  /* Does he know what a stick does */
extern bool m_know[];                   /* Does he know what a MM does */
extern bool in_shell;                   /* True if executing a shell */
extern bool daytime;                    /* Indicates whether it is daytime */
extern bool funfont;                    /* Is fun font available? */
extern coord oldpos;                    /* Position before last look() call */
extern coord grid[];                    /* used for random pos generation */
extern char *nothing;                   /* "Nothing seems to happen." */
extern char *spacemsg;
extern char *morestr;
extern char *retstr;
extern LEVTYPE levtype;
extern void (*add_abil[NUMABILITIES])(int change); /* Functions to change abilities */
extern void (*res_abil[NUMABILITIES])(void *arg); /* Functions to change abilities */
extern int mf_count;       /* move_free counter - see actions.c(m_act()) */
extern int mf_jmpcnt;      /* move_free counter for # of jumps 		  */
extern int killed_chance;  /* cumulative chance for goodies to loose it, fight.c */
extern coord move_nh;        /* move.c */
extern const int cNCOLORS;
extern const int cNSTONES;
extern const int cNWOOD;
extern const int cNMETAL;
extern char rainbow[][15];
extern char *sylls[];
extern char stones[][15];
extern char wood[][15];
extern char metal[][15];
extern long cleric, monk, magician, assassin, druid, thief, fighter, ranger, paladin;

#if u370 || uts || i386
#define ENCREAD(b,n,fd) read(fd,b,n)
#define ENCWRITE(b,n,fd) write(fd,b,n)
#endif
#ifndef ENCREAD
#define ENCREAD encread
#define ENCWRITE encwrite
#endif

#define MAXDAEMONS      10
#define MAXFUSES        20
 
extern struct delayed_action d_list[MAXDAEMONS];
extern struct delayed_action f_list[MAXFUSES];

extern int demoncnt;        /* number of active daemons */
extern int fusecnt;

/* actions.c */
extern void dsrpt_monster(register struct thing *tp, bool always, bool see_him);
extern void dsrpt_player(void);
extern void reap(void);
extern void m_act(register struct thing *tp);
extern void m_breathe(register struct thing *tp);
extern void m_select(register struct thing *th, register bool flee);
extern void m_sonic(register struct thing *tp);
extern void m_spell(register struct thing *tp);
extern void m_summon(register struct thing *tp);
extern bool m_use_it(register struct thing *tp, bool flee, register struct room *rer, register struct room *ree);
extern int runners(int segments);
extern bool m_use_pack(register struct thing *monster, coord *monst_pos, coord *defend_pos, register int dist, register coord *shoot_dir);
/* bolt.c */
extern void shoot_bolt(struct thing *shooter, coord start, coord dir, bool get_points, short reason, char *name, int damage);
/* chase.c */
extern bool can_blink(register struct thing *tp);
extern int can_shoot(register coord *er, register coord *ee, register coord *can_shoot);
extern void chase(register struct thing *tp, coord *ee, register struct room *rer, register struct room *ree, bool flee);
extern void do_chase(register struct thing *th);
extern struct linked_list *get_hurl(register struct thing *tp);
extern void runto(register struct thing *runner, coord *spot);
extern bool straight_shot(register int ery, register int erx, register int eey, register int eex, register coord *shooting);
/* command.c */
extern void command(void);
extern void display(void);
extern void quit(int a);
extern void bugkill(int sig);
extern void search(register bool is_thief, register bool door_chime);
extern void d_level(void);
extern void u_level(void);
extern void shell(void);
extern void nameit(void);
extern void nameitem(struct linked_list *item, bool mark);
extern void namemonst(void);
extern void count_gold(void);
extern void do_teleport(void);
/* daemon.c */
extern struct delayed_action *d_slot(void);
extern struct delayed_action *f_slot(void);
extern struct delayed_action *find_slot(register void (*func)(void *));
extern void start_daemon(register void (*func)(void *), register void *arg, register int type);
extern void kill_daemon(register void (*func)(void *));
extern void do_daemons(register int flag);
extern void fuse(register void (*func)(void *), void *arg, register int time, register int type);
extern void lengthen(register void (*func)(void *), register int xtime);
extern void extinguish(register void (*func)(void *));
extern void do_fuses(register int flag);
extern void activity(void);
/* daemons.c */
extern void doctor(void *arg);
extern void swander(void *arg);
extern void rollwand(void *arg);
extern void trap_look(void *arg);
extern void unconfuse(void *arg);
extern void unsee(void *arg);
extern void unstink(void *arg);
extern void unclrhead(void *arg);
extern void unphase(void *arg);
extern void land(void *arg);
extern void sight(void *arg);
extern void res_strength(void *howmuch);
extern void nohaste(void *arg);
extern void noslow(void *arg);
extern void suffocate(void *arg);
extern void stomach(void *arg);
extern void cure_disease(void *arg);
extern void appear(void *arg);
extern void dust_appear(void *arg);
extern void unchoke(void *arg);
extern void alchemy(void *obj);
extern void undance(void *arg);
extern void strangle(void *arg);
extern void fumble(void *arg);
extern void ring_search(void *arg);
extern void ring_teleport(void *arg);
extern void quill_charge(void *arg);
extern void unskill(void *arg);
extern void cloak_charge(void *arg);
extern void nofire(void *arg);
extern void nocold(void *arg);
extern void nobolt(void *arg);
extern void eat_gold(void *obj);
extern void spell_recovery(void *arg);
extern void prayer_recovery(void *arg);
extern void chant_recovery(void *arg);
/* eat.c */
extern void eat(void);
/* effects.c */
extern int effect(register struct thing *att, register struct thing *def, struct object *weap, bool thrown, bool see_att, bool see_def);
/* encumb.c */
extern void updpack(int getmax, struct thing *tp);
extern int packweight(register struct thing *tp);
extern int itemweight(register struct object *wh);
extern int playenc(register struct thing *tp);
extern int totalenc(register struct thing *tp);
extern void wghtchk(void *arg);
extern int hitweight(void);
/* fight.c */
extern int player_can_hit(register struct thing *tp, register struct object *weap);
extern int fight(register coord *mp, struct object *weap, bool thrown);
extern int attack(register struct thing *mp, register struct object *weapon, bool thrown);
extern int swing(short class, int at_lvl, int op_arm, int wplus);
extern int roll_em(struct thing *att_er, struct thing *def_er, struct object *weap, bool hurl, struct object *cur_weapon, bool back_stab);
extern char *prname(register char *who, bool upper);
extern void hit(register struct object *weapon, bool see_att, bool see_def, register char *er, register char *ee, bool back_stab, bool thrown, bool short_msg);
extern void miss(register struct object *weapon, bool see_att, bool see_def, register char *er, register char *ee, bool thrown, bool short_msg);
extern int dext_plus(register int dexterity);
extern int dext_prot(register int dexterity);
extern int str_plus(register short str);
extern int add_dam(register short str);
extern int hung_dam(void);
extern int is_magic(register struct object *obj);
extern void killed(register struct linked_list *item, bool pr, bool points, bool treasure);
extern struct linked_list *wield_weap(struct object *thrown, struct thing *mp);
extern void explode(register struct thing *tp);
extern int skirmish(register struct thing *attacker, register coord *mp, struct object *weap, bool thrown);
/* help.c */
extern void ident_hero(void);
extern void help(void);
extern void identify(register unsigned char ch);
/* init.c */
extern void badcheck(char *name, register struct magic_item *magic, register int bound);
extern void init_colors(void);
extern void init_foods(void);
extern void init_materials(void);
extern void init_misc(void);
extern void init_names(void);
extern void init_player(void);
extern void init_stones(void);
extern void init_things(void);
/* io.c */
extern void msg(char *fmt, ...);
extern void addmsg(char *fmt, ...);
extern void rmmsg(void);
extern void endmsg(void);
extern void doadd(char *fmt, va_list args);
extern int step_ok(register int y, register int x, register int can_on_monst, register struct thing *flgptr);
extern int shoot_ok(int ch);
extern void status(bool display);
extern void wait_for(register char ch);
extern void over_win(WINDOW *oldwin, WINDOW *newin, int maxy, int maxx, int cursory, int cursorx, char redraw);
extern void show_win(register WINDOW *scr, char *message);
extern void dbotline(WINDOW *scr, char *message);
extern void restscr(WINDOW *scr);
/* list.c */
extern void _detach(register struct linked_list **list, register struct linked_list *item);
extern void _attach(register struct linked_list **list, register struct linked_list *item);
extern void _r_free_fire_list(register struct linked_list **ptr);
extern void _o_free_list(register struct linked_list **ptr);
extern void o_discard(register struct linked_list *item);
extern void _r_free_list(register struct linked_list **ptr);
extern void r_discard(register struct linked_list *item);
extern void _t_free_list(register struct linked_list **ptr);
extern void t_discard(register struct linked_list *item);
extern void destroy_item(register struct linked_list *item);
extern struct linked_list *new_item(int size);
extern struct linked_list *creat_item(void);
extern char *new(int size);
/* main.c */
extern int main(int argc, char **argv, char **envp);
extern void endit(int a);
extern void fatal(char *s);
extern int rnd(register int range);
extern int roll(register int number, register int sides);
extern void tstp(int a);
extern void setup(void);
extern void playit(void);
extern int too_much(void);
extern int author(void);
extern int holiday(void);
/* maze.c */
extern void do_maze(void);
extern bool maze_view(int y, int x);
/* misc.c */
extern void changeclass(void *arg);
extern void m_use_relic(register struct thing *monster);
extern void put_contents(register struct object *bag, register struct linked_list *item);
extern void take_contents(register struct object *bag, register struct linked_list *item);
extern void do_bag(register struct linked_list *item);
extern void do_panic(int who);
extern char *misc_name(register struct object *obj);
extern void use_emori(void);
extern void use_quill(register struct object *obj);
extern void use_mm(int which);
extern int usage_time(struct linked_list *item);
/* mons_def.c */
/* monsters.c */
extern void check_residue(register struct thing *tp);
extern int creat_mons(struct thing *person, short monster, bool report);
extern void genmonsters(register int least, bool treas);
extern short id_monst(register char monster);
extern void new_monster(struct linked_list *item, short type, coord *cp, bool max_monster);
extern short randmonster(register bool wander, register bool no_unique);
extern void sell(register struct thing *tp);
extern struct linked_list *wake_monster(int y, int x);
extern void wanderer(void);
/* move.c */
extern int be_trapped(register struct thing *th, register coord *tc);
extern bool blue_light(bool blessed, bool cursed);
extern void corr_move(int dy, int dx);
extern void dip_it(void);
extern void do_move(int dy, int dx);
extern void do_run(char ch);
extern bool getdelta(char match, int *dy, int *dx);
extern int isatrap(register char ch);
extern void light(coord *cp);
extern bool lit_room(register struct room *rp);
extern short movement(register struct thing *tp);
extern coord *rndmove(struct thing *who);
extern void set_trap(register struct thing *tp, register int y, register int x);
extern int show(register int y, register int x);
extern struct trap *trap_at(register int y, register int x);
extern int weap_move(register struct thing *wielder, register struct object *weap);
/* n_level.c */
extern void new_level(LEVTYPE ltype);
extern int rnd_room(void);
extern void put_things(LEVTYPE ltype);
/* options.c */
extern int get_default(void *arg, WINDOW *win);
extern int get_abil(void *arg, WINDOW *win);
extern int get_quest(void *arg, WINDOW *win);
extern int get_ro(WINDOW *win, register int oy, register int ox);
extern int get_bool(void *arg, WINDOW *win);
extern int get_str(void *arg, WINDOW *win);
extern void option(void);
extern void parse_opts(register char *str);
extern void put_abil(void *arg, WINDOW *win);
extern void put_quest(void *arg, WINDOW *win);
extern void put_bool(void *arg, WINDOW *win);
extern void put_str(void *arg, WINDOW *win);
/* outside.c */
extern void init_terrain(void);
extern void do_terrain(int basey, int basex, int deltay, int deltax, bool fresh);
extern char rnd_terrain(void);
extern char get_terrain(char one, char two, char three, char four);
/* pack.c */
extern bool add_pack(register struct linked_list *item, bool silent);
extern int inventory(register struct linked_list *list, register int type);
extern void picky_inven(void);
extern struct linked_list *get_item(register struct linked_list *list, char *purpose, int type, bool askfirst, bool showcost);
extern char pack_char(struct linked_list *list, register struct object *obj);
extern void cur_null(register struct object *op);
extern void idenpack(void);
extern int is_type(register struct object *obj, register int type);
extern void del_pack(register struct linked_list *item);
extern void carry_obj(register struct thing *mp, int chance);
extern int grab(register int y, register int x);
extern void make_sell_pack(struct thing *tp);
/* passages.c */
extern void do_passages(void);
extern void conn(int r1, int r2);
extern void door(register struct room *rm, register coord *cp);
/* player.c */
extern void affect(void);
extern void pray(void);
extern void cast(void);
extern void chant(void);
extern int const_bonus(void);
extern void give(void);
extern void fright(void);
extern void gsense(void);
extern void xsense(void);
extern void steal(void);
extern void take_with(void);
extern int pick_spell(struct spells spells[], int ability, int num_spells, int power, char *prompt, char *type);
extern void opt_player(void);
/* potions.c */
extern void add_constitution(int change);
extern void add_charisma(int change);
extern void add_dexterity(int change);
extern void add_haste(bool blessed);
extern void add_intelligence(int change);
extern void add_slow(void);
extern void add_strength(int change);
extern void add_wisdom(int change);
extern void quaff(int which, int kind, int flags, bool is_potion);
extern void res_dexterity(void *howmuch);
extern void res_intelligence(void *howmuch);
extern void res_wisdom(void *howmuch);
extern void res_constitution(void *howmuch);
extern void res_charisma(void *howmuch);
/* rings.c */
extern int ring_eat(register int hand);
extern void ring_on(register struct linked_list *item);
extern char *ring_num(register struct object *obj);
extern int ring_value(int type);
/* rip.c */
extern void byebye(int n);
extern void death(register short monst);
extern char *killname(register short monst);
extern void score(unsigned long amount, int flags, short monst);
extern void showpack(char *howso);
extern void total_winner(void);
/* rogue.c */
/* rooms.c */
extern void do_rooms(void);
extern coord *doorway(register struct room *rp, register coord *door);
extern void draw_room(register struct room *rp);
extern void horiz(register int cnt);
extern void rnd_pos(register struct room *rp, register coord *cp);
extern struct room *roomin(register coord *cp);
extern void vert(register int cnt);
/* save.c */
extern bool save_game(void);
extern void auto_save(int sig);
extern bool save_file(int savefd);
extern int restore(register char *file, char **envp);
extern long encwrite(register char *start, register unsigned long size, register int outf);
extern long encread(register char *start, register unsigned long size, int inf);
/* scrolls.c */
extern void genocide(void);
extern void read_scroll(register int which, int flag, bool is_scroll);
/* state.c */
extern void rsPrintf(char *fmt, ...);
extern void *get_list_item(struct linked_list *l, int i);
extern int rs_write(FILE *savef, void *ptr, int size);
extern int rs_read(int inf, void *ptr, int size);
extern int rs_write_uint(FILE *savef, unsigned int c);
extern int rs_write_int(FILE *savef, int c);
extern int rs_write_ulong(FILE *savef, unsigned long c);
extern int rs_write_long(FILE *savef, long c);
extern int rs_write_boolean(FILE *savef, bool c);
extern int rs_read_int(int inf, int *i);
extern int rs_read_uint(int inf, unsigned int *i);
extern int rs_read_ulong(int inf, unsigned long *i);
extern int rs_read_long(int inf, long *i);
extern int rs_read_boolean(int inf, bool *i);
extern int rs_write_ints(FILE *savef, int *c, int count);
extern int rs_write_short(FILE *savef, short c);
extern int rs_read_short(int inf, short *s);
extern int rs_write_shorts(FILE *savef, short *c, int count);
extern int rs_write_longs(FILE *savef, long *c, int count);
extern int rs_write_ulongs(FILE *savef, unsigned long *c, int count);
extern int rs_write_booleans(FILE *savef, bool *c, int count);
extern int rs_read_ints(int inf, int *i, int count);
extern int rs_read_shorts(int inf, short *i, int count);
extern int rs_read_longs(int inf, long *i, int count);
extern int rs_read_ulongs(int inf, unsigned long *i, int count);
extern int rs_read_booleans(int inf, bool *i, int count);
extern int rs_write_levtype(FILE *savef, LEVTYPE c);
extern int rs_read_levtype(int inf, LEVTYPE *l);
extern int rs_write_char(FILE *savef, char c);
extern int rs_read_char(int inf, char *c);
extern int rs_write_uchar(FILE *savef, unsigned char c);
extern int rs_read_uchar(int inf, unsigned char *c);
extern int rs_write_string(FILE *savef, char *s);
extern int rs_read_string_index(int inf, char master[][15], int maxindex, char **str);
extern int rs_write_string_index(FILE *savef, char master[][15], int maxindex, char *str);
extern int rs_read_scrolls(int inf);
extern int rs_write_scrolls(FILE *savef);
extern int rs_read_potions(int inf);
extern int rs_write_potions(FILE *savef);
extern int rs_read_rings(int inf);
extern int rs_write_rings(FILE *savef);
extern int rs_read_misc(int inf);
extern int rs_write_misc(FILE *savef);
extern int rs_write_sticks(FILE *savef);
extern int rs_read_sticks(int inf);
extern int rs_read_string(int inf, char *s, int max);
extern int rs_read_new_string(int inf, char **s);
extern int rs_write_strings(FILE *savef, char *s[], int count);
extern int rs_write_words(FILE *savef, struct words *w, int count);
extern int rs_read_words(int inf, struct words *w, int count);
extern int rs_read_new_strings(int inf, char **s, int count);
extern int rs_write_coord(FILE *savef, coord *c);
extern int rs_read_coord(int inf, coord *c);
extern int rs_write_daemons(FILE *savef, struct delayed_action *d_list, int count);
extern int rs_read_daemons(int inf, struct delayed_action *d_list, int count);
extern int rs_write_rooms(FILE *savef, struct room r[], int count);
extern int rs_read_rooms(int inf, struct room *r, int count);
extern int rs_write_object(FILE *savef, struct object *o);
extern int rs_read_object(int inf, struct object *o);
extern int rs_write_stats(FILE *savef, struct stats *s);
extern int rs_read_stats(int inf, struct stats *s);
extern int rs_write_mstats(FILE *savef, struct mstats *s);
extern int rs_read_mstats(int inf, struct mstats *s);
extern int rs_write_init_weps(FILE *savef, struct init_weps *w, int count);
extern int rs_read_init_weps(int inf, struct init_weps *w, int count);
extern int rs_write_init_armor(FILE *savef, struct init_armor *a, int count);
extern int rs_read_init_armor(int inf, struct init_armor *a, int count);
extern int rs_write_spells(FILE *savef, struct spells *s, int count);
extern int rs_read_spells(int inf, struct spells *s, int count);
extern int rs_write_item_list(FILE *savef, struct item_list *i);
extern int rs_read_item_list(int inf, struct item_list *i);
extern int rs_write_h_list(FILE *savef, struct h_list *h);
extern int rs_read_h_list(int inf, struct h_list *h);
extern int rs_write_death_types(FILE *savef, struct death_type *d, int count);
extern int rs_read_death_types(int inf, struct death_type *d, int count);
extern int rs_write_character_types(FILE *savef, struct character_types *c, int count);
extern int rs_read_character_types(int inf, struct character_types *c, int count);
extern int rs_write_traps(FILE *savef, struct trap *trap, int count);
extern int rs_read_traps(int inf, struct trap *trap, int count);
extern int rs_write_monsters(FILE *savef, struct monster *m, int count);
extern int rs_read_monsters(int inf, struct monster *m, int count);
extern int rs_write_coord_list(FILE *savef, struct linked_list *l);
extern int rs_read_coord_list(int inf, struct linked_list **list);
extern int rs_write_object_list(FILE *savef, struct linked_list *l);
extern int rs_read_object_list(int inf, struct linked_list **list);
extern int find_thing_coord(struct linked_list *monlist, coord *c);
extern int find_object_coord(struct linked_list *objlist, coord *c);
extern int rs_write_thing(FILE *savef, struct thing *t);
extern void rs_fix_thing(struct thing *t);
extern int rs_read_thing(int inf, struct thing *t);
extern int find_list_ptr(struct linked_list *l, void *ptr);
extern int list_size(struct linked_list *l);
extern int rs_write_monster_list(FILE *savef, struct linked_list *l);
extern void rs_fix_monster_list(struct linked_list *list);
extern int rs_read_monster_list(int inf, struct linked_list **list);
extern int rs_write_magic_items(FILE *savef, struct magic_item *i, int count);
extern int rs_read_magic_items(int inf, struct magic_item *mi, int count);
extern int rs_write_window(FILE *savef, WINDOW *win);
extern int rs_read_window(int inf, WINDOW *win);
extern int rs_save_file(int savefd);
extern int rs_restore_file(int savefd);
extern void rs_print_thing(FILE *outf, struct thing *thing, char *prefix, int list, int index);
extern void rs_print_game_state(FILE *outf);
extern int md_rand(register int range);
extern void md_srand(register int seed);
extern void md_flushinp(void);
extern int md_getuid(void);
extern long md_memused(void);
extern char *md_getusername(void);
extern char *md_gethomedir(void);
extern int directory_exists(char *dirname);
extern char *md_getroguedir(void);
extern char *md_getshell(void);
extern char *md_gethostname(void);
extern int md_shellescape(void);
extern int md_erasechar(void);
extern int md_killchar(void);
extern void md_init(void);
extern char *md_getpass(char *prompt);
extern void md_setup(void);
extern long md_ntohl(long netlong);
extern long md_htonl(long netlong);
/* sticks.c */
extern void do_zap(struct thing *zapper, struct object *obj, coord *direction, int which, int flags);
extern void drain(int ymin, int ymax, int xmin, int xmax);
extern void fix_stick(register struct object *cur);
extern void m_use_wand(register struct thing *monster);
extern bool need_dir(int type, int which);
extern int player_zap(int which, int flag);
/* things.c */
extern char *charge_str(register struct object *obj);
extern char *inv_name(register struct object *obj, bool drop);
extern char *weap_name(register struct object *obj);
extern int drop(struct linked_list *item);
extern int dropcheck(register struct object *op);
extern struct linked_list *new_thing(int thing_type, bool allow_curse);
extern struct linked_list *spec_item(int type, int which, int hit, int damage);
extern int pick_one(register struct magic_item *magic, int nitems);
extern char *blesscurse(int flags);
extern char *p_kind(struct object *obj);
extern int extras(void);
/* trader.c */
extern void buy_it(void);
extern void do_post(bool startup);
extern int open_market(void);
extern int price_it(void);
extern void sell_it(void);
extern void trans_line(void);
extern char *typ_name(register struct object *obj);
/* util.c */
extern int ac_compute(bool ignoremetal);
extern void aggravate(bool do_uniques, bool do_good);
extern int cansee(register int y, register int x);
extern long check_level(void);
extern void chg_str(register int amt);
extern void confus_player(void);
extern int dex_compute(void);
extern int diag_ok(register coord *sp, register coord *ep, struct thing *flgptr);
extern coord *fallpos(register coord *pos, bool be_clear, int range);
extern int findmindex(char *name);
extern struct linked_list *find_mons(register int y, register int x);
extern struct linked_list *find_obj(register int y, register int x);
extern coord get_coordinates(void);
extern bool get_dir(coord *direction);
extern long get_worth(register struct object *obj);
extern bool invisible(register struct thing *monst);
extern int is_current(register struct object *obj);
extern void look(bool wakeup, bool runend);
extern void lower_level(short who);
extern char *monster_name(register struct thing *tp);
extern bool move_hero(int why);
extern void raise_level(void);
extern int save(int which, struct thing *who, int adj);
extern int secretdoor(register int y, register int x);
extern int str_compute(void);
extern void strucpy(register char *s1, register char *s2, register size_t len);
extern size_t strxcpy(register char *s1, register char *s2, register size_t len);
extern char *tr_name(char ch);
extern char *vowelstr(register char *str);
extern void wake_room(register struct room *rp);
extern void waste_time(void);
/* vers.c */
/* weapons.c */
extern void boomerang(int ydelta, int xdelta, register struct linked_list *item, register struct thing *tp);
extern void do_motion(register struct object *obj, register int ydelta, register int xdelta, register struct thing *tp);
extern void fall(register struct linked_list *item, bool pr);
extern int hit_monster(register int y, register int x, struct object *obj, register struct thing *tp);
extern void init_weapon(register struct object *weap, char type);
extern void missile(int ydelta, int xdelta, register struct linked_list *item, register struct thing *tp);
extern char *num(register int n1, register int n2);
extern void wield(void);
/* wear.c */
extern void take_off(void);
extern void wear(void);
extern int dress_units(struct linked_list *item);
/* wizard.c */
extern void create_obj(bool prompt, int which_item, int which_type);
extern int getbless(void);
extern int getdeath(void);
extern int makemonster(bool showall, char *label, char *action);
extern bool passwd(void);
extern int teleport(void);
extern void whatis(struct linked_list *what);
extern void choose_qst(void);
/* xcrypt.c */
extern char *xcrypt(const char *key, const char *setting);

