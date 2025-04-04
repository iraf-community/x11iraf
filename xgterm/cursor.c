/*
 *	$XConsortium: cursor.c,v 1.14 93/09/20 17:42:23 hersh Exp $
 */

/*
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Digital Equipment
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 *
 *
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/* cursor.c */

#include "ptyx.h"		/* also gets Xlib.h */

static void _CheckSelection(TScreen *screen)
{
    extern XgtermWidget term;	/* %%% gross */

    if (screen->cur_row > screen->endHRow ||
	(screen->cur_row == screen->endHRow &&
	 screen->cur_col >= screen->endHCol)) {}
    else
	DisownSelection(term);
}



/*
 * Moves the cursor to the specified position, checking for bounds.
 * (this includes scrolling regions)
 * The origin is considered to be 0, 0 for this procedure.
 */
void
CursorSet(TScreen *screen, int row, int col, unsigned int flags)
{
	int maxr;

	col = (col < 0 ? 0 : col);
	screen->cur_col = (col <= screen->max_col ? col : screen->max_col);
	maxr = screen->max_row;
	if (flags & ORIGIN) {
		row += screen->top_marg;
		maxr = screen->bot_marg;
	}
	row = (row < 0 ? 0 : row);
	screen->cur_row = (row <= maxr ? row : maxr);
	screen->do_wrap = 0;
	_CheckSelection(screen);
}

/*
 * moves the cursor left n, no wrap around
 */
void
CursorBack(TScreen *screen, int n)
{
	int i, j, k, rev;
	extern XgtermWidget term;

	if((rev = (term->flags & (REVERSEWRAP | WRAPAROUND)) ==
	 (REVERSEWRAP | WRAPAROUND)) && screen->do_wrap)
		n--;
	if ((screen->cur_col -= n) < 0) {
		if(rev) {
			if((i = (j = screen->max_col + 1) * screen->cur_row +
			 screen->cur_col) < 0) {
				k = j * (screen->max_row + 1);
				i += ((-i) / k + 1) * k;
			}
			screen->cur_row = i / j;
			screen->cur_col = i % j;
		} else
			screen->cur_col = 0;
	}
	screen->do_wrap = 0;
	_CheckSelection(screen);
}

/*
 * moves the cursor forward n, no wraparound
 */
void
CursorForward(TScreen *screen, int n)
{
	screen->cur_col += n;
	if (screen->cur_col > screen->max_col)
		screen->cur_col = screen->max_col;
	screen->do_wrap = 0;
	_CheckSelection(screen);
}

/* 
 * moves the cursor down n, no scrolling.
 * Won't pass bottom margin or bottom of screen.
 */
void
CursorDown(TScreen *screen, int n)
{
	int max;

	max = (screen->cur_row > screen->bot_marg ?
		screen->max_row : screen->bot_marg);

	screen->cur_row += n;
	if (screen->cur_row > max)
		screen->cur_row = max;
	screen->do_wrap = 0;
	_CheckSelection(screen);
}

/* 
 * moves the cursor up n, no linestarving.
 * Won't pass top margin or top of screen.
 */
void
CursorUp(TScreen *screen, int n)
{
	int min;

	min = (screen->cur_row < screen->top_marg ?
		0 : screen->top_marg);

	screen->cur_row -= n;
	if (screen->cur_row < min)
		screen->cur_row = min;
	screen->do_wrap = 0;
	_CheckSelection(screen);
}

/* 
 * Moves cursor down amount lines, scrolls if necessary.
 * Won't leave scrolling region. No carriage return.
 */
void
Index(TScreen *screen, int amount)
{
	int j;

	/* 
	 * indexing when below scrolling region is cursor down.
	 * if cursor high enough, no scrolling necessary.
	 */
	if (screen->cur_row > screen->bot_marg
	 || screen->cur_row + amount <= screen->bot_marg) {
		CursorDown(screen, amount);
		return;
	}

	CursorDown(screen, j = screen->bot_marg - screen->cur_row);
	Scroll(screen, amount - j);
}

/*
 * Moves cursor up amount lines, reverse scrolls if necessary.
 * Won't leave scrolling region. No carriage return.
 */
void
RevIndex(TScreen *screen, int amount)
{
	/*
	 * reverse indexing when above scrolling region is cursor up.
	 * if cursor low enough, no reverse indexing needed
	 */
	if (screen->cur_row < screen->top_marg
	 || screen->cur_row-amount >= screen->top_marg) {
		CursorUp(screen, amount);
		return;
	}

	RevScroll(screen, amount - (screen->cur_row - screen->top_marg));
	CursorUp(screen, screen->cur_row - screen->top_marg);
}

/*
 * Moves Cursor To First Column In Line
 */
void
CarriageReturn(TScreen *screen)
{
	screen->cur_col = 0;
	screen->do_wrap = 0;
	_CheckSelection(screen);
}

/*
 * Save Cursor and Attributes
 */
void
CursorSave(XgtermWidget term, SavedCursor *sc)
{
	TScreen *screen = &term->screen;

	sc->row = screen->cur_row;
	sc->col = screen->cur_col;
	sc->flags = term->flags;
	sc->curgl = screen->curgl;
	sc->curgr = screen->curgr;
	memmove( sc->gsets, screen->gsets, sizeof(screen->gsets));
}

/*
 * Restore Cursor and Attributes
 */
void
CursorRestore(XgtermWidget term, SavedCursor *sc)
{
	TScreen *screen = &term->screen;

	memmove( screen->gsets, sc->gsets, sizeof(screen->gsets));
	screen->curgl = sc->curgl;
	screen->curgr = sc->curgr;
	term->flags &= ~(BOLD|INVERSE|UNDERLINE|ORIGIN);
	term->flags |= sc->flags & (BOLD|INVERSE|UNDERLINE|ORIGIN);
	CursorSet (screen, (term->flags & ORIGIN) ? sc->row - screen->top_marg
			   : sc->row, sc->col, term->flags);
}
