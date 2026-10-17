/*      @(#) clreolinln.c: 1.1 10/15/83 (1.21   2/11/83)        */
/*      Shortened and subverted for xrogue 7/1/89   */

#include "clrln.h"
#include <term.h>

extern  int     _dellines();
extern	int	_blanks();
extern  int     _pos();
extern  int     _clearhl();
extern  int     _setwind();
extern  int     _inslines();
extern  int     _outch();
extern  int     clreol();
extern  char    *tparm();
extern  int     tputs();

_clreol()
{
        register int col;
        register struct line *cb;

#ifdef DEBUG
        if(outf) fprintf(outf, "_clreol().\n");
#endif
        if (!move_standout_mode)
                _clearhl ();
        if (clr_eol) {
                tputs (clr_eol, columns-SP->phys_x, _outch);
        } else if (delete_line && insert_line && SP->phys_x == 0) {
                _dellines (1);
                _inslines (1);
        } else {
                /*
                 * Should consider using delete/insert line here, too,
                 * if the part that would have to be redrawn is short,
                 * or if we have to get rid of some cemented highlights.
                 * This might win on the DTC 382 or the Teleray 1061.
                 */
                cb = SP->cur_body[SP->phys_y+1];
                for (col=SP->phys_x; col<cb->length; col++)
                        if (cb->body[col] != ' ') {
                                _pos(SP->phys_y, col);
                                _blanks(1);
                        }
                /*
                 * Could save and restore SP->curptr position, but since we
                 * keep track of what _blanks does to it, it turns out
                 * that this is wasted.  Not backspacing over the stuff
                 * cleared out is a real win on a super dumb terminal.
                 */
        }
}

_inslines (n)
{
        register int i;

#ifdef DEBUG
        if(outf) fprintf(outf, "_inslines(%d).\n", n);
#endif
        if (!move_standout_mode && SP->phys_gr)
                _clearhl();
        if (SP->phys_y + n >= lines && clr_eos) {
                /*
                 * Really quick -- clear to end of screen.
                 */
                tputs(clr_eos, lines-SP->phys_y, _outch);
                return;
        }

        /*
         * We are about to _shove something off the bottom of the screen.
         * Terminals with extra memory keep this around, and it might
         * show up again to haunt us later if we do a delete line or
         * scroll into it, or when we exit.  So we clobber it now.  We
         * might get better performance by somehow remembering that this
         * stuff is down there and worrying about it if/when we have to.
         * In particular, having to do this extra pair of SP->curptr motions
         * is a lose.
         */
        if (memory_below) {
                int save;

                save = SP->phys_y;
                if (save_cursor && restore_cursor)
                        tputs(save_cursor, 1, _outch);
                if (clr_eos) {
                        _pos(lines-n, 0);
                        tputs(clr_eos, n, _outch);
                } else {
                        for (i=0; i<n; i++) {
                                _pos(lines-n+i, 0);
                                _clreol();
                        }
                }
                if (save_cursor && restore_cursor)
                        tputs(restore_cursor, 1, _outch);
                else
                        _pos(save, 0);
        }

        /* Do the physical line deletion */
        if (scroll_reverse && SP->phys_y == SP->des_top_mgn /* &&costSR<costAL */) {
                /*
                 * Use reverse scroll mode of the terminal, at
                 * the top of the window.  Reverse linefeed works
                 * too, since we only use it from top line of window.
                 */
                _setwind();
                for (i = n; i > 0; i--) {
                        _pos(SP->phys_y, 0);
                        tputs(scroll_reverse, 1, _outch);
                        if (SP->ml_above > 0)
                                SP->ml_above--;
                        /*
                         * If we are at the top of the screen, and the
                         * terminal retains display above, then we
                         * should try to clear to end of line.
                         * Have to use CE since we don't remember what is
                         * actually on the line.
                         */
                        if (clr_eol && memory_above)
                                tputs(clr_eol, 1, _outch);
                }
        } else if (parm_insert_line && (n>1 || *insert_line==0)) {
                tputs(tparm(parm_insert_line, n, SP->phys_y), lines-SP->phys_y, _outch);
        } else if (change_scroll_region && *insert_line==0) {
                /* vt100 change scrolling region to fake AL */
                tputs(save_cursor, 1, _outch);
                tputs(  tparm(change_scroll_region,
                        SP->phys_y, SP->des_bot_mgn), 1, _outch);
                /* change_scroll_region homes stupid cursor */
                tputs(restore_cursor, 1, _outch);
                for (i=n; i>0; i--)
                        tputs(scroll_reverse, 1, _outch);/* should do @'s */
                /* restore scrolling region */
                tputs(tparm(change_scroll_region,
                        SP->des_top_mgn, SP->des_bot_mgn), 1, _outch);
                tputs(restore_cursor, 1, _outch);/* Once again put it back */
                SP->phys_top_mgn = SP->des_top_mgn;
                SP->phys_bot_mgn = SP->des_bot_mgn;
        } else {
                tputs(insert_line, lines - SP->phys_y, _outch);
                for (i = n - 1; i > 0; i--) {
                        tputs(insert_line, lines - SP->phys_y, _outch);
                }
        }
}

/* Added the following subroutines 10/24/90:
 * _dellines()
 * _pos()
 * _setwind()
 * _blanks()
 * _clearhl()
 */

/* #ident "@(#)/usr/src/lib/libcurses/screen/_dellines.c   1.2 6/6/85 06:36:53 - Amdahl/UTS" */
/*      @(#) _dellines.c: 1.1 10/15/83  (1.21   2/11/83)        */

_dellines (n)
{
        register int i;

#ifdef DEBUG
        if(outf) fprintf(outf, "_dellines(%d).\n", n);
#endif
        if (lines - SP->phys_y <= n && (clr_eol && n == 1 || clr_eos)) {
                tputs(clr_eos, n, _outch);
        } else
        if (scroll_forward && SP->phys_y == SP->des_top_mgn /* &&costSF<costDL */) {
                /*
                 * Use forward scroll mode of the terminal, at
                 * the bottom of the window.  Linefeed works
                 * too, since we only use it from the bottom line.
                 */
                _setwind();
                for (i = n; i > 0; i--) {
                        _pos(SP->des_bot_mgn, 0);
                        tputs(scroll_forward, 1, _outch);
                        SP->ml_above++;
                }
                if (SP->ml_above + lines > lines_of_memory)
                        SP->ml_above = lines_of_memory - lines;
        } else if (parm_delete_line && (n>1 || *delete_line==0)) {
                tputs(tparm(parm_delete_line, n, SP->phys_y), lines-SP->phys_y, _outch);
        }
        else if (change_scroll_region && *delete_line==0) {
                /* vt100: fake delete_line by changing scrolling region */
                /* Save since change_scroll_region homes cursor */
                tputs(save_cursor, 1, _outch);
                tputs(tparm(change_scroll_region,
                        SP->phys_y, SP->des_bot_mgn), 1, _outch);
                /* go to bottom left corner.. */
                tputs(tparm(cursor_address, SP->des_bot_mgn, 0), 1, _outch);
                for (i=0; i<n; i++)     /* .. and scroll n times */
                        tputs(scroll_forward, 1, _outch);
                /* restore scrolling region */
                tputs(tparm(change_scroll_region,
                        SP->des_top_mgn, SP->des_bot_mgn), 1, _outch);
                tputs(restore_cursor, 1, _outch);       /* put SP->curptr back */
                SP->phys_top_mgn = SP->des_top_mgn;
                SP->phys_bot_mgn = SP->des_bot_mgn;
        }
        else {
                for (i = 0; i < n; i++)
                        tputs(delete_line, lines-SP->phys_y, _outch);
        }
}

/* #ident "@(#)/usr/src/lib/libcurses/screen/_pos.c        1.2 6/6/85 06:44:20 - Amdahl/UTS" */
/*      @(#) _pos.c: 1.1 10/15/83       (1.21   2/11/83)        */

/* Position the SP->curptr to (row, column) which start at 0. */
_pos(row, column)
int     row;
int     column;
{
#ifdef DEBUG
    if(outf) fprintf(outf, "_pos from row %d, col %d => row %d, col %d\n",
            SP->phys_y, SP->phys_x, row, column);
#endif
        if( SP->phys_x == column && SP->phys_y == row )
        {
                return; /* already there */
        }
        /*
         * Many terminals can't move the cursor when in standout mode.
         * We must be careful, however, because HP's and cookie terminals
         * will drop a cookie when we do this.
         */
        if( !move_standout_mode && SP->phys_gr && magic_cookie_glitch < 0 )
        {
                if( !ceol_standout_glitch )
                {
                        _clearhl ();
                }
        }
        /* some terminals can't move in insert mode */
        if( SP->phys_irm == 1 && !move_insert_mode )
        {
                tputs(exit_insert_mode, 1, _outch);
                SP->phys_irm = 0;
        }
        /* If we try to move outside the scrolling region, widen it */
        if( row<SP->phys_top_mgn || row>SP->phys_bot_mgn )
        {
                _window(0, lines-1, 0, columns-1);
                _setwind();
        }
        mvcur(SP->phys_y, SP->phys_x, row, column);
        SP->phys_x = column;
        SP->phys_y = row;
}

/* #ident "@(#)/usr/src/lib/libcurses/screen/_blanks.c     1.2 6/6/85 06:33:14 - Amdahl/UTS" */
/*      @(#) _blanks.c: 1.1 10/15/83    (1.21   2/11/83)        */

extern  int     _sethl();
extern  int     _setmode();

/*
 * Output n blanks, or the equivalent.  This is done to erase text, and
 * also to insert blanks.  The semantics of this call do not define where
 * it leaves the cursor - it might be where it was before, or it might
 * be at the end of the blanks.  We will, of course, leave SP->phys_x
 * properly updated.
 */
_blanks (n)
{
#ifdef DEBUG
        if(outf) fprintf(outf, "_blanks(%d).\n", n);
#endif
        if (n == 0)
                return;
        _setmode ();
        _sethl ();
        if (SP->virt_irm==1 && parm_ich) {
                if (n == 1)
                        tputs(insert_character, 1, _outch);
                else
                        tputs(tparm(parm_ich, n), n, _outch);
                return;
        }
        if (erase_chars && SP->phys_irm != 1 && n > 5) {
                tputs(tparm(erase_chars, n), n, _outch);
                return;
        }
        if (repeat_char && SP->phys_irm != 1 && n > 5) {
                tputs(tparm(repeat_char, ' ', n), n, _outch);
                SP->phys_x += n;
                return;
        }
        while (--n >= 0) {
                if (SP->phys_irm == 1 && insert_character)
                        tputs (insert_character, columns - SP->phys_x, _outch);
                if (++SP->phys_x >= columns && auto_right_margin) {
                        if (SP->phys_y >= lines-1) {
                                /*
                                 * We attempted to put something in the last
                                 * position of the last line.  Since this will
                                 * cause a scroll (we only get here if the
                                 * terminal has auto_right_margin) we refuse
                                 * to put it out.
                                 */
                                SP->phys_x--;
                                return;
                        }
                        SP->phys_x = 0;
                        SP->phys_y++;
                }
                _outch (' ');
                if (SP->phys_irm == 1 && insert_padding)
                        tputs (insert_padding, 1, _outch);
        }
}

/* #ident "@(#)/usr/src/lib/libcurses/screen/_setwind.c    1.2 6/6/85 06:46:59 - Amdahl/UTS" */
/*      @(#) _setwind.c: 1.1 10/15/83   (1.21   2/11/83)        */

/* Force the window to be as desired */
_setwind()
{
        if (    SP->phys_top_mgn == SP->des_top_mgn &&
                SP->phys_bot_mgn == SP->des_bot_mgn) {
#ifdef DEBUG
                if(outf) fprintf(outf, "_setwind, same values %d & %d, do nothing\n",
                        SP->phys_top_mgn, SP->phys_bot_mgn);
#endif
                return;
        }
        if (set_window)
                tputs(tparm(set_window, SP->des_top_mgn,
                        SP->des_bot_mgn, 0, columns-1), 1, _outch);
        else if (change_scroll_region) {
                /* Save & Restore SP->curptr since it becomes undefined */
                tputs(save_cursor, 1, _outch);
                tputs(tparm(change_scroll_region,
                        SP->des_top_mgn, SP->des_bot_mgn), 1, _outch);
                tputs(restore_cursor, 1, _outch);       /* put SP->curptr back */
        }
#ifdef DEBUG
        if(outf) fprintf(outf, "set phys window from (%d,%d) to (%d,%d)\n",
        SP->phys_top_mgn, SP->phys_bot_mgn, SP->des_top_mgn, SP->des_bot_mgn);
#endif
        SP->phys_top_mgn = SP->des_top_mgn;
        SP->phys_bot_mgn = SP->des_bot_mgn;
}

/* #ident "@(#)/usr/src/lib/libcurses/screen/_clearhl.c    1.2 6/6/85 06:34:06 - Amdahl/UTS" */
/*      @(#) _clearhl.c: 1.1 10/15/83   (1.21   2/11/83)        */

_clearhl ()
{
#ifdef DEBUG
        if(outf) fprintf(outf, "_clearhl().\n");
#endif
        if (SP->phys_gr) {
                register oldes = SP->virt_gr;
                SP->virt_gr = 0;
                _sethl ();
                SP->virt_gr = oldes;
        }
}

/* Added the following subroutines because they are called in by the
 * five I added above 10/24/90.
 * _sethl()
 * _window()
 * _setmode()
 */

/* #ident "@(#)/usr/src/lib/libcurses/screen/_sethl.c      1.2 6/6/85 06:46:09 - Amdahl/UTS" */
/*      @(#) _sethl.c: 1.1 10/15/83     (1.21   2/11/83)        */

_sethl ()
{
#ifdef DEBUG
        if(outf) fprintf(outf, "_sethl().  SP->phys_gr=%o, SP->virt_gr %o\n", SP->phys_gr, SP->virt_gr);
#endif
#ifdef          VIDEO
        if (SP->phys_gr == SP->virt_gr)
                return;
        vidputs(SP->virt_gr, _outch);
        SP->phys_gr = SP->virt_gr;
        /* Account for the extra space the cookie takes up */
        if (magic_cookie_glitch >= 0)
                SP->phys_x += magic_cookie_glitch;
#endif       /* VIDEO */
}

/* #ident "@(#)/usr/src/lib/libcurses/screen/_window.c     1.2 6/6/85 06:49:29 - Amdahl/UTS" */
/*      @(#) _window.c: 1.1 10/15/83    (1.21   2/11/83)        */

/*
 * Set the desired window to the box with the indicated boundaries.
 * All scrolling should only affect the area inside the window.
 * We currently ignore the last 2 args since we're only using this
 * for scrolling and want to use the feature on vt100's as well as
 * on concept 100's.  left and right are for future expansion someday.
 *
 * Note that we currently assume cursor addressing within the window
 * is relative to the screen, not the window.  This will have to be
 * generalized if concept windows are to be used.
 */
/* ARGSUSED */
_window(top, bottom, left, right)
int top, bottom, left, right;
{
#ifdef DEBUG
        if(outf) fprintf(outf, "_window old top=%d, bot %d; new top=%d, bot %d\n",
                SP->des_top_mgn, SP->des_bot_mgn, top, bottom);
#endif
        if (change_scroll_region || set_window) {
                SP->des_top_mgn = top;
                SP->des_bot_mgn = bottom;
        }
#ifdef DEBUG
        else
                if(outf) fprintf(outf, "window setting ignored\n");
#endif
}

/* #ident "@(#)/usr/src/lib/libcurses/screen/_setmode.c    1.2 6/6/85 06:46:34 - Amdahl/UTS" */
/*      @(#) _setmode.c: 1.1 10/15/83   (1.21   2/11/83)        */

_setmode ()
{
#ifdef DEBUG
        if(outf) fprintf(outf, "_setmode().\n");
#endif
        if (SP->virt_irm == SP->phys_irm)
                return;
        tputs(SP->virt_irm==1 ? enter_insert_mode : exit_insert_mode, 0, _outch);
        SP->phys_irm = SP->virt_irm;
}

