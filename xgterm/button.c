/* $XConsortium: button.c,v 1.69 94/04/02 12:41:50 gildea Exp $ */
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

/*
button.c	Handles button events in the terminal emulator.
		does cut/paste operations, change modes via menu,
		passes button events through to some applications.
				J. Gettys.
*/

#include "ptyx.h"		/* Xlib headers included here. */
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>

#include "data.h"
#include "error.h"
#include "menu.h"


extern void DoSecureKeyboard(Time time);

#define KeyState(x) (((x) & (ShiftMask|ControlMask)) + (((x) & Mod1Mask) ? 2 : 0))
    /* adds together the bits:
        shift key -> 1
        meta key  -> 2
        control key -> 4 */
  
#define TEXTMODES 4
#define NBUTS 3
#define DIRS 2
#define UP 1
#define DOWN 0
#define SHIFTS 8		/* three keys, so eight combinations */
#define	Coordinate(r,c)		((r) * (term->screen.max_col+1) + (c))

extern char *xgterm_name;

static void PointToRowCol(int y, int x, int *r, int *c);
static void SelectionReceived(Widget w, XtPointer client_data, Atom *selection, Atom *type, XtPointer value, long unsigned int *length, int *format);
static void TrackDown(XButtonEvent *event);
static void ComputeSelect(int startRow, int startCol, int endRow, int endCol, int extend);
static void EditorButton(XButtonEvent *event);
static void ExtendExtend(int row, int col);
static void ReHiliteText(int frow, int fcol, int trow, int tcol);
static void SelectSet(Widget w, XEvent *event, String *params, Cardinal num_params);
static void StartSelect(int startrow, int startcol);
static int Length(TScreen *screen, int row, int scol, int ecol);
static char *SaveText(TScreen *screen, int row, int scol, int ecol, char *lp, int *eol);
void DisownSelection(XgtermWidget termw);
void TrackText(int frow, int fcol, int trow, int tcol);

extern XgtermWidget term;

/* Selection/extension variables */

/* Raw char position where the selection started */
static int rawRow, rawCol;

/* Selected area before CHAR, WORD, LINE selectUnit processing */
static int startRRow, startRCol, endRRow, endRCol = 0;

/* Selected area after CHAR, WORD, LINE selectUnit processing */
static int startSRow, startSCol, endSRow, endSCol = 0;

/* Valid rows for selection clipping */
static int firstValidRow, lastValidRow;

/* Start, end of extension */
static int startERow, startECol, endERow, endECol;

/* Saved values of raw selection for extend to restore to */
static int saveStartRRow, saveStartRCol, saveEndRRow, saveEndRCol;

/* Multi-click handling */
static int numberOfClicks = 0;
static long int lastButtonUpTime = 0;
typedef int SelectUnit;
#define SELECTCHAR 0
#define SELECTWORD 1
#define SELECTLINE 2
#define NSELECTUNITS 3
static SelectUnit selectUnit;

/* Send emacs escape code when done selecting or extending? */
static int replyToEmacs;


Boolean SendMousePosition(Widget w, XEvent *event)
{
    TScreen *screen = &((XgtermWidget)w)->screen;
    
    if (screen->send_mouse_pos == 0) return False;

    if (event->type != ButtonPress && event->type != ButtonRelease)
	return False;

#define KeyModifiers \
    (event->xbutton.state & (ShiftMask | LockMask | ControlMask | Mod1Mask | \
			     Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask ))

#define ButtonModifiers \
    (event->xbutton.state & (ShiftMask | LockMask | ControlMask | Mod1Mask | \
			     Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask ))

    switch (screen->send_mouse_pos) {
      case 1: /* X10 compatibility sequences */

	if (KeyModifiers == 0) {
	    if (event->type == ButtonPress)
		EditorButton((XButtonEvent *)event);
	    return True;
	}
	return False;

      case 2: /* DEC vt200 compatible */

	if (KeyModifiers == 0 || KeyModifiers == ControlMask) {
	    EditorButton((XButtonEvent *)event);
	    return True;
	}
	return False;

      case 3: /* DEC vt200 hilite tracking */
	if (  event->type == ButtonPress &&
	      KeyModifiers == 0 &&
	      event->xbutton.button == Button1 ) {
	    TrackDown((XButtonEvent *)event);
	    return True;
	}
	if (KeyModifiers == 0 || KeyModifiers == ControlMask) {
	    EditorButton((XButtonEvent *)event);
	    return True;
	}
	/* fall through */

      default:
	return False;
    }
#undef KeyModifiers
}


void
DiredButton(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
                                /* must be XButtonEvent */
                                /* selections */
                     
{       /* ^XM-G<line+' '><col+' '> */
        TScreen *screen = &term->screen;
        int pty = screen->respond;
        char Line[ 6 ];
        unsigned line, col;

    if (event->type != ButtonPress && event->type != ButtonRelease)
        return;
    strcpy( Line, "\030\033G  " );

        line = ( event->xbutton.y - screen->border ) / FontHeight( screen );
	if (term->misc.sb_right)
            col = (event->xbutton.x - screen->border) / FontWidth(screen);
	else
            col = (event->xbutton.x - screen->border - screen->scrollbar) / 
		FontWidth(screen);
        Line[3] = ' ' + col;
        Line[4] = ' ' + line;
        v_write(pty, Line, 5 );
}

void
ViButton(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
                                /* must be XButtonEvent */
                                /* selections */
                     
{       /* ^XM-G<line+' '><col+' '> */
        TScreen *screen = &term->screen;
        int pty = screen->respond;
        char Line[ 6 ];
        int line, col;

    if (event->type != ButtonPress && event->type != ButtonRelease)
        return;

        line = screen->cur_row -
                (( event->xbutton.y - screen->border ) / FontHeight( screen));
/* fprintf( stderr, "xtdb line=%d\n", line ); */
        if ( ! line ) return;
        Line[ 1 ] = 0;
        Line[ 0 ] = 27;
        v_write(pty, Line, 1 );

        Line[ 0 ] = 'p' & 0x1f;

        if ( line < 0 )
        {       line = -line;
                Line[ 0 ] = 'n' & 0x1f;
        }
        while ( --line >= 0 ) v_write(pty, Line, 1 );
}

/*ARGSUSED*/
void HandleSelectExtend(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
              			/* must be XMotionEvent */
               			/* unused */
                     		/* unused */
{
	TScreen *screen = &((XgtermWidget)w)->screen;
	int row, col;

	screen->selection_time = event->xmotion.time;
	switch (eventMode) {
		case LEFTEXTENSION :
		case RIGHTEXTENSION :
			PointToRowCol (event->xmotion.y, event->xmotion.x, 
				       &row, &col);
			ExtendExtend (row, col);
			break;
		case NORMAL :
			/* will get here if send_mouse_pos != 0 */
		        break;
	}
}

static void EndExtend(Widget w, XEvent *event, String *params, Cardinal num_params, int use_cursor_loc);

static void do_select_end (Widget w, XEvent *event, String *params, Cardinal *num_params, int use_cursor_loc)
         
              			/* must be XButtonEvent */
               			/* selections */
                     
                    
{
	((XgtermWidget)w)->screen.selection_time = event->xbutton.time;
	switch (eventMode) {
		case NORMAL :
		    (void) SendMousePosition(w, event);
		    break;
		case LEFTEXTENSION :
		case RIGHTEXTENSION :
		    EndExtend(w, event, params, *num_params, use_cursor_loc);
		    break;
	}
}


void HandleSelectEnd(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
              			/* must be XButtonEvent */
               			/* selections */
                     
{
	do_select_end (w, event, params, num_params, False);
}


void HandleKeyboardSelectEnd(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
              			/* must be XButtonEvent */
               			/* selections */
                     
{
	do_select_end (w, event, params, num_params, True);
}




struct _SelectionList {
    String *params;
    Cardinal count;
    Time time;
};


static void _GetSelection(Widget w, Time time, String *params, Cardinal num_params)
         
          
               			/* selections in precedence order */
                    
{
    Atom selection;
    int cutbuffer;

    XmuInternStrings(XtDisplay(w), params, (Cardinal)1, &selection);
    switch (selection) {
      case XA_CUT_BUFFER0: cutbuffer = 0; break;
      case XA_CUT_BUFFER1: cutbuffer = 1; break;
      case XA_CUT_BUFFER2: cutbuffer = 2; break;
      case XA_CUT_BUFFER3: cutbuffer = 3; break;
      case XA_CUT_BUFFER4: cutbuffer = 4; break;
      case XA_CUT_BUFFER5: cutbuffer = 5; break;
      case XA_CUT_BUFFER6: cutbuffer = 6; break;
      case XA_CUT_BUFFER7: cutbuffer = 7; break;
      default:	       cutbuffer = -1;
    }
    if (cutbuffer >= 0) {
	TScreen *screen = &((XgtermWidget)w)->screen;
	int inbytes;
	unsigned long nbytes;
	int fmt8 = 8;
	Atom type = XA_STRING;
	char *line = XFetchBuffer(screen->display, &inbytes, cutbuffer);
	nbytes = (unsigned long) inbytes;
	if (nbytes > 0)
	    SelectionReceived(w, NULL, &selection, &type, (XtPointer)line,
			      &nbytes, &fmt8);
	else if (num_params > 1)
	    _GetSelection(w, time, params+1, num_params-1);
    } else {
	struct _SelectionList* list;
	if (--num_params) {
	    list = XtNew(struct _SelectionList);
	    list->params = params + 1;
	    list->count = num_params; /* decremented above */
	    list->time = time;
	} else list = NULL;
	XtGetSelectionValue(w, selection, XA_STRING, SelectionReceived,
			    (XtPointer)list, time);
    }
}

/* SelectionReceived: stuff received selection text into pty */

/* ARGSUSED */
static void SelectionReceived(
  Widget w,
  XtPointer client_data,
  Atom *selection,
  Atom *type,
  XtPointer value,
  unsigned long *length,
  int *format)
{
    int pty = ((XgtermWidget)w)->screen.respond;	/* file descriptor of pty */
    char *lag, *cp, *end;
    char *line = (char*)value;
				  
    if (*type == 0 /*XT_CONVERT_FAIL*/ || *length == 0 || value == NULL) {
	/* could not get this selection, so see if there are more to try */
	struct _SelectionList* list = (struct _SelectionList*)client_data;
	if (list != NULL) {
	    _GetSelection(w, list->time, list->params, list->count);
	    XtFree(client_data);
	}
	return;
    }

    /* Write data to pty a line at a time. */
    /* Doing this one line at a time may no longer be necessary
       because v_write has been re-written. */

    end = &line[*length];
    lag = line;
    for (cp = line; cp != end; cp++)
	{
	    if (*cp != '\n') continue;
	    *cp = '\r';
	    v_write(pty, lag, cp - lag + 1);
	    lag = cp + 1;
	}
    if (lag != end)
	v_write(pty, lag, end - lag);

    XtFree(client_data);
    XtFree(value);
}


void
HandleInsertSelection(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
              			/* assumed to be XButtonEvent* */
               			/* selections in precedence order */
                     
{
    if (SendMousePosition(w, event)) return;
    _GetSelection(w, event->xbutton.time, params, *num_params);
}


static void
SetSelectUnit(Time buttonDownTime, SelectUnit defaultUnit)
{
/* Do arithmetic as integers, but compare as unsigned solves clock wraparound */
	if ((long unsigned)((long int)buttonDownTime - lastButtonUpTime)
	 > term->screen.multiClickTime) {
		numberOfClicks = 1;
		selectUnit = defaultUnit;
	} else {
		++numberOfClicks;
		selectUnit = ((selectUnit + 1) % NSELECTUNITS);
	}
}

static void do_select_start (Widget w, XEvent *event, int startrow, int startcol)
         
              			/* must be XButtonEvent* */
                       
{
	if (SendMousePosition(w, event)) return;
	SetSelectUnit(event->xbutton.time, SELECTCHAR);
	replyToEmacs = FALSE;
	StartSelect(startrow, startcol);
}

/* ARGSUSED */
void
HandleSelectStart(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
              			/* must be XButtonEvent* */
               			/* unused */
                     		/* unused */
{
	TScreen *screen = &((XgtermWidget)w)->screen;
	int startrow, startcol;

	firstValidRow = 0;
	lastValidRow  = screen->max_row;
	PointToRowCol(event->xbutton.y, event->xbutton.x, &startrow, &startcol);
	do_select_start (w, event, startrow, startcol);
}


/* ARGSUSED */
void
HandleKeyboardSelectStart(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
              			/* must be XButtonEvent* */
               			/* unused */
                     		/* unused */
{
	TScreen *screen = &((XgtermWidget)w)->screen;

	do_select_start (w, event, screen->cursor_row, screen->cursor_col);
}


static void
TrackDown(XButtonEvent *event)
{
	int startrow, startcol;

	SetSelectUnit(event->time, SELECTCHAR);
	if (numberOfClicks > 1 ) {
		PointToRowCol(event->y, event->x, &startrow, &startcol);
		replyToEmacs = TRUE;
		StartSelect(startrow, startcol);
	} else {
		waitingForTrackInfo = 1;
		EditorButton(event);
	}
}


#define boundsCheck(x)	if (x < 0) \
			    x = 0; \
			else if (x >= screen->max_row) \
			    x = screen->max_row;

void
TrackMouse(int func, int startrow, int startcol, int firstrow, int lastrow)
{
	TScreen *screen = &term->screen;

	if (!waitingForTrackInfo) {	/* Timed out, so ignore */
		return;
	}
	waitingForTrackInfo = 0;
	if (func == 0) return;
	boundsCheck (startrow)
	boundsCheck (firstrow)
	boundsCheck (lastrow)
	firstValidRow = firstrow;
	lastValidRow  = lastrow;
	replyToEmacs = TRUE;
	StartSelect(startrow, startcol);
}

static void
StartSelect(int startrow, int startcol)
{
	TScreen *screen = &term->screen;

	if (screen->cursor_state)
	    HideCursor ();
	if (numberOfClicks == 1) {
		/* set start of selection */
		rawRow = startrow;
		rawCol = startcol;
		
	} /* else use old values in rawRow, Col */

	saveStartRRow = startERow = rawRow;
	saveStartRCol = startECol = rawCol;
	saveEndRRow   = endERow   = rawRow;
	saveEndRCol   = endECol   = rawCol;
	if (Coordinate(startrow, startcol) < Coordinate(rawRow, rawCol)) {
		eventMode = LEFTEXTENSION;
		startERow = startrow;
		startECol = startcol;
	} else {
		eventMode = RIGHTEXTENSION;
		endERow = startrow;
		endECol = startcol;
	}
	ComputeSelect(startERow, startECol, endERow, endECol, False);

}

static void
EndExtend(Widget w, XEvent *event, String *params, Cardinal num_params, int use_cursor_loc)
             
                  			/* must be XButtonEvent */
                   			/* selections */
                        
                        
{
	int	row, col;
	TScreen *screen = &term->screen;
	char line[9];

	if (use_cursor_loc) {
	    row = screen->cursor_row;
	    col = screen->cursor_col;
	} else {
	    PointToRowCol(event->xbutton.y, event->xbutton.x, &row, &col);
	}
	ExtendExtend (row, col);
	lastButtonUpTime = event->xbutton.time;
	if (startSRow != endSRow || startSCol != endSCol) {
		if (replyToEmacs) {
			if (rawRow == startSRow && rawCol == startSCol 
			    && row == endSRow && col == endSCol) {
			 	/* Use short-form emacs select */
				strcpy(line, "\033[t");
				line[3] = ' ' + endSCol + 1;
				line[4] = ' ' + endSRow + 1;
				v_write(screen->respond, line, 5);
			} else {
				/* long-form, specify everything */
				strcpy(line, "\033[T");
				line[3] = ' ' + startSCol + 1;
				line[4] = ' ' + startSRow + 1;
				line[5] = ' ' + endSCol + 1;
				line[6] = ' ' + endSRow + 1;
				line[7] = ' ' + col + 1;
				line[8] = ' ' + row + 1;
				v_write(screen->respond, line, 9);
			}
			TrackText(0, 0, 0, 0);
		}
	}
	SelectSet(w, event, params, num_params);
	eventMode = NORMAL;
}

void
HandleSelectSet(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
	SelectSet (w, event, params, *num_params);
}

static void SaltTextAway(int crow, int ccol, int row, int col, String *params, Cardinal num_params);

/* ARGSUSED */
static void
SelectSet (Widget w, XEvent *event, String *params, Cardinal num_params)
{
	/* Only do select stuff if non-null select */
	if (startSRow != endSRow || startSCol != endSCol) {
		SaltTextAway(startSRow, startSCol, endSRow, endSCol,
			     params, num_params);
	} else
		DisownSelection(term);
}

#define Abs(x)		((x) < 0 ? -(x) : (x))

/* ARGSUSED */
static void do_start_extend (Widget w, XEvent *event, String *params, Cardinal *num_params, int use_cursor_loc)
         
              			/* must be XButtonEvent* */
               			/* unused */
                     		/* unused */
                    
{
	TScreen *screen = &((XgtermWidget)w)->screen;
	int row, col, coord;

	if (SendMousePosition(w, event)) return;
	firstValidRow = 0;
	lastValidRow  = screen->max_row;
	SetSelectUnit(event->xbutton.time, selectUnit);
	replyToEmacs = FALSE;

	if (numberOfClicks == 1) {
		/* Save existing selection so we can reestablish it if the guy
		   extends past the other end of the selection */
		saveStartRRow = startERow = startRRow;
		saveStartRCol = startECol = startRCol;
		saveEndRRow   = endERow   = endRRow;
		saveEndRCol   = endECol   = endRCol;
	} else {
		/* He just needed the selection mode changed, use old values. */
		startERow = startRRow = saveStartRRow;
		startECol = startRCol = saveStartRCol;
		endERow   = endRRow   = saveEndRRow;
		endECol   = endRCol   = saveEndRCol;

	}
	if (use_cursor_loc) {
	    row = screen->cursor_row;
	    col = screen->cursor_col;
	} else {
	    PointToRowCol(event->xbutton.y, event->xbutton.x, &row, &col);
	}
	coord = Coordinate(row, col);

	if (Abs(coord - Coordinate(startSRow, startSCol))
	     < Abs(coord - Coordinate(endSRow, endSCol))
	    || coord < Coordinate(startSRow, startSCol)) {
	 	/* point is close to left side of selection */
		eventMode = LEFTEXTENSION;
		startERow = row;
		startECol = col;
	} else {
	 	/* point is close to left side of selection */
		eventMode = RIGHTEXTENSION;
		endERow = row;
		endECol = col;
	}
	ComputeSelect(startERow, startECol, endERow, endECol, True);
}

static void
ExtendExtend (int row, int col)
{
	int coord = Coordinate(row, col);
	
	if (eventMode == LEFTEXTENSION 
	 && (coord + (selectUnit!=SELECTCHAR)) > Coordinate(endSRow, endSCol)) {
		/* Whoops, he's changed his mind.  Do RIGHTEXTENSION */
		eventMode = RIGHTEXTENSION;
		startERow = saveStartRRow;
		startECol = saveStartRCol;
	} else if (eventMode == RIGHTEXTENSION
	 && coord < Coordinate(startSRow, startSCol)) {
	 	/* Whoops, he's changed his mind.  Do LEFTEXTENSION */
		eventMode = LEFTEXTENSION;
		endERow   = saveEndRRow;
		endECol   = saveEndRCol;
	}
	if (eventMode == LEFTEXTENSION) {
		startERow = row;
		startECol = col;
	} else {
		endERow = row;
		endECol = col;
	}
	ComputeSelect(startERow, startECol, endERow, endECol, False);
}


void HandleStartExtend(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
              			/* must be XButtonEvent* */
               			/* unused */
                     		/* unused */
{
    do_start_extend (w, event, params, num_params, False);
}

void HandleKeyboardStartExtend(Widget w, XEvent *event, String *params, Cardinal *num_params)
         
              			/* must be XButtonEvent* */
               			/* unused */
                     		/* unused */
{
    do_start_extend (w, event, params, num_params, True);
}

void
ScrollSelection(TScreen *screen, int amount)
{
    int minrow = -screen->savedlines - screen->topline;
    int maxrow = screen->max_row - screen->topline;
    int maxcol = screen->max_col;

#define scroll_update_one(row, col) \
    	row += amount; \
	if (row < minrow) { \
	    row = minrow; \
	    col = 0; \
	} \
	if (row > maxrow) { \
	    row = maxrow; \
	    col = maxcol; \
	}

    scroll_update_one(startRRow, startRCol);
    scroll_update_one(endRRow, endRCol);
    scroll_update_one(startSRow, startSCol);
    scroll_update_one(endSRow, endSCol);

    scroll_update_one(rawRow, rawCol);

    scroll_update_one(screen->startHRow, screen->startHCol);
    scroll_update_one(screen->endHRow, screen->endHCol);

    screen->startHCoord = Coordinate (screen->startHRow, screen->startHCol);
    screen->endHCoord = Coordinate (screen->endHRow, screen->endHCol);
}


/*ARGSUSED*/
void
ResizeSelection (TScreen *screen, int rows, int cols)
{
    rows--;				/* decr to get 0-max */
    cols--;

    if (startRRow > rows) startRRow = rows;
    if (startSRow > rows) startSRow = rows;
    if (endRRow > rows) endRRow = rows;
    if (endSRow > rows) endSRow = rows;
    if (rawRow > rows) rawRow = rows;

    if (startRCol > cols) startRCol = cols;
    if (startSCol > cols) startSCol = cols;
    if (endRCol > cols) endRCol = cols;
    if (endSCol > cols) endSCol = cols;
    if (rawCol > cols) rawCol = cols;
}

static void
PointToRowCol(int y, int x, int *r, int *c)
             
               
/* Convert pixel coordinates to character coordinates.
   Rows are clipped between firstValidRow and lastValidRow.
   Columns are clipped between to be 0 or greater, but are not clipped to some
       maximum value. */
{
	TScreen *screen = &term->screen;
	int row, col;

	row = (y - screen->border) / FontHeight(screen);
	if(row < firstValidRow)
		row = firstValidRow;
	else if(row > lastValidRow)
		row = lastValidRow;
	if (term->misc.sb_right)
            col = (x - screen->border) / FontWidth(screen);
	else
            col = (x - screen->border - screen->scrollbar) / FontWidth(screen);
	if(col < 0)
		col = 0;
	else if(col > screen->max_col+1) {
		col = screen->max_col+1;
	}
	*r = row;
	*c = col;
}

static int
LastTextCol(int row)
{
	TScreen *screen =  &term->screen;
	int i;
	Char *ch;

	for ( i = screen->max_col,
	        ch = screen->buf[4 * (row + screen->topline) + 1] + i ;
	      i >= 0 && !(*ch & CHARDRAWN) ;
	      ch--, i--)
	    ;
	return(i);
}	

/*
** double click table for cut and paste in 8 bits
**
** This table is divided in four parts :
**
**	- control characters	[0,0x1f] U [0x80,0x9f]
**	- separators		[0x20,0x3f] U [0xa0,0xb9]
**	- binding characters	[0x40,0x7f] U [0xc0,0xff]
**  	- execeptions
*/
static int charClass[256] = {
/* NUL  SOH  STX  ETX  EOT  ENQ  ACK  BEL */
    32,   1,   1,   1,   1,   1,   1,   1,
/*  BS   HT   NL   VT   NP   CR   SO   SI */
     1,  32,   1,   1,   1,   1,   1,   1,
/* DLE  DC1  DC2  DC3  DC4  NAK  SYN  ETB */
     1,   1,   1,   1,   1,   1,   1,   1,
/* CAN   EM  SUB  ESC   FS   GS   RS   US */
     1,   1,   1,   1,   1,   1,   1,   1,
/*  SP    !    "    #    $    %    &    ' */
    32,  33,  34,  35,  36,  37,  38,  39,
/*   (    )    *    +    ,    -    .    / */
    40,  41,  42,  43,  44,  45,  46,  47,
/*   0    1    2    3    4    5    6    7 */
    48,  48,  48,  48,  48,  48,  48,  48,
/*   8    9    :    ;    <    =    >    ? */
    48,  48,  58,  59,  60,  61,  62,  63,
/*   @    A    B    C    D    E    F    G */
    64,  48,  48,  48,  48,  48,  48,  48,
/*   H    I    J    K    L    M    N    O */
    48,  48,  48,  48,  48,  48,  48,  48,
/*   P    Q    R    S    T    U    V    W */ 
    48,  48,  48,  48,  48,  48,  48,  48,
/*   X    Y    Z    [    \    ]    ^    _ */
    48,  48,  48,  91,  92,  93,  94,  48,
/*   `    a    b    c    d    e    f    g */
    96,  48,  48,  48,  48,  48,  48,  48,
/*   h    i    j    k    l    m    n    o */
    48,  48,  48,  48,  48,  48,  48,  48,
/*   p    q    r    s    t    u    v    w */
    48,  48,  48,  48,  48,  48,  48,  48,
/*   x    y    z    {    |    }    ~  DEL */
    48,  48,  48, 123, 124, 125, 126,   1,
/* x80  x81  x82  x83  IND  NEL  SSA  ESA */
     1,   1,   1,   1,   1,   1,   1,   1,
/* HTS  HTJ  VTS  PLD  PLU   RI  SS2  SS3 */
     1,   1,   1,   1,   1,   1,   1,   1,
/* DCS  PU1  PU2  STS  CCH   MW  SPA  EPA */
     1,   1,   1,   1,   1,   1,   1,   1,
/* x98  x99  x9A  CSI   ST  OSC   PM  APC */
     1,   1,   1,   1,   1,   1,   1,   1,
/*   -    i   c/    L   ox   Y-    |   So */
   160, 161, 162, 163, 164, 165, 166, 167,
/*  ..   c0   ip   <<    _        R0    - */
   168, 169, 170, 171, 172, 173, 174, 175,
/*   o   +-    2    3    '    u   q|    . */
   176, 177, 178, 179, 180, 181, 182, 183,
/*   ,    1    2   >>  1/4  1/2  3/4    ? */
   184, 185, 186, 187, 188, 189, 190, 191,
/*  A`   A'   A^   A~   A:   Ao   AE   C, */
    48,  48,  48,  48,  48,  48,  48,  48,
/*  E`   E'   E^   E:   I`   I'   I^   I: */
    48,  48,  48,  48,  48,  48,  48,  48,
/*  D-   N~   O`   O'   O^   O~   O:    X */ 
    48,  48,  48,  48,  48,  48,  48, 216,
/*  O/   U`   U'   U^   U:   Y'    P    B */
    48,  48,  48,  48,  48,  48,  48,  48,
/*  a`   a'   a^   a~   a:   ao   ae   c, */
    48,  48,  48,  48,  48,  48,  48,  48,
/*  e`   e'   e^   e:    i`  i'   i^   i: */
    48,  48,  48,  48,  48,  48,  48,  48,
/*   d   n~   o`   o'   o^   o~   o:   -: */
    48,  48,  48,  48,  48,  48,  48,  248,
/*  o/   u`   u'   u^   u:   y'    P   y: */
    48,  48,  48,  48,  48,  48,  48,  48};

int SetCharacterClassRange (int low, int high, int value)
                  		/* in range of [0..255] */
              			/* arbitrary */
{

    if (low < 0 || high > 255 || high < low) return (-1);

    for (; low <= high; low++) charClass[low] = value;

    return (0);
}

/*
 * sets startSRow startSCol endSRow endSCol
 * ensuring that they have legal values
 */

static void
ComputeSelect(int startRow, int startCol, int endRow, int endCol, int extend)
{
	TScreen *screen = &term->screen;
	Char *ptr;
	int length;
	int class;
	int osc = startSCol;

	if (Coordinate(startRow, startCol) <= Coordinate(endRow, endCol)) {
		startSRow = startRRow = startRow;
		startSCol = startRCol = startCol;
		endSRow   = endRRow   = endRow;
		endSCol   = endRCol   = endCol;
	} else {	/* Swap them */
		startSRow = startRRow = endRow;
		startSCol = startRCol = endCol;
		endSRow   = endRRow   = startRow;
		endSCol   = endRCol   = startCol;
	}	

	switch (selectUnit) {
		case SELECTCHAR :
			if (startSCol > (LastTextCol(startSRow) + 1)) {
				startSCol = 0;
				startSRow++;
			}
			if (endSCol > (LastTextCol(endSRow) + 1)) {
				endSCol = 0;
				endSRow++;
			}
			break;
		case SELECTWORD :
			if (startSCol > (LastTextCol(startSRow) + 1)) {
				startSCol = 0;
				startSRow++;
			} else {
				ptr = screen->buf[4*(startSRow+screen->topline)]
				 + startSCol;
				class = charClass[*ptr];
				do {
					--startSCol;
					--ptr;
				} while (startSCol >= 0
				 && charClass[*ptr] == class);
				++startSCol;
			}
			if (endSCol > (LastTextCol(endSRow) + 1)) {
				endSCol = 0;
				endSRow++;
			} else {
				length = LastTextCol(endSRow);
				ptr = screen->buf[4*(endSRow+screen->topline)]
				 + endSCol;
				class = charClass[*ptr];
				do {
					++endSCol;
					++ptr;
				} while (endSCol <= length
				 && charClass[*ptr] == class);
				/* Word select selects if pointing to any char
				   in "word", especially in that it includes
				   the last character in a word.  So no --endSCol
				   and do special eol handling */
				if (endSCol > length+1) {
					endSCol = 0;
					++endSRow;
				}
			}
			break;
		case SELECTLINE :
			if (term->screen.cutToBeginningOfLine) {
			    startSCol = 0;
			} else if (!extend) {
			    startSCol = osc;
			}
			if (term->screen.cutNewline) {
			    endSCol = 0;
			    ++endSRow;
			} else {
			    endSCol = LastTextCol(endSRow) + 1;
			}
			break;
	}

	TrackText(startSRow, startSCol, endSRow, endSCol);
	return;
}
void
TrackText(int frow, int fcol, int trow, int tcol)
                               
    /* Guaranteed (frow, fcol) <= (trow, tcol) */
{
	int from, to;
	TScreen *screen = &term->screen;
	int old_startrow, old_startcol, old_endrow, old_endcol;

	old_startrow = screen->startHRow;
	old_startcol = screen->startHCol;
	old_endrow = screen->endHRow;
	old_endcol = screen->endHCol;
	if (frow == old_startrow && fcol == old_startcol &&
	    trow == old_endrow   && tcol == old_endcol) return;
	screen->startHRow = frow;
	screen->startHCol = fcol;
	screen->endHRow   = trow;
	screen->endHCol   = tcol;
	from = Coordinate(frow, fcol);
	to = Coordinate(trow, tcol);
	if (to <= screen->startHCoord || from > screen->endHCoord) {
	    /* No overlap whatsoever between old and new hilite */
	    ReHiliteText(old_startrow, old_startcol, old_endrow, old_endcol);
	    ReHiliteText(frow, fcol, trow, tcol);
	} else {
	    if (from < screen->startHCoord) {
		    /* Extend left end */
		    ReHiliteText(frow, fcol, old_startrow, old_startcol);
	    } else if (from > screen->startHCoord) {
		    /* Shorten left end */
		    ReHiliteText(old_startrow, old_startcol, frow, fcol);
	    }
	    if (to > screen->endHCoord) {
		    /* Extend right end */
		    ReHiliteText(old_endrow, old_endcol, trow, tcol);
	    } else if (to < screen->endHCoord) {
		    /* Shorten right end */
		    ReHiliteText(trow, tcol, old_endrow, old_endcol);
	    }
	}
	screen->startHCoord = from;
	screen->endHCoord = to;
}

static void
ReHiliteText(int frow, int fcol, int trow, int tcol)
                               
    /* Guaranteed that (frow, fcol) <= (trow, tcol) */
{
	TScreen *screen = &term->screen;
	int i;

	if (frow < 0)
	    frow = fcol = 0;
	else if (frow > screen->max_row)
	    return;		/* nothing to do, since trow >= frow */

	if (trow < 0)
	    return;		/* nothing to do, since frow <= trow */
	else if (trow > screen->max_row) {
	    trow = screen->max_row;
	    tcol = screen->max_col+1;
	}
	if (frow == trow && fcol == tcol)
		return;

	if(frow != trow) {	/* do multiple rows */
		if((i = screen->max_col - fcol + 1) > 0) {     /* first row */
		    ScrnRefresh(screen, frow, fcol, 1, i, True);
		}
		if((i = trow - frow - 1) > 0) {		       /* middle rows*/
		    ScrnRefresh(screen, frow+1, 0,i, screen->max_col+1, True);
		}
		if(tcol > 0 && trow <= screen->max_row) {      /* last row */
		    ScrnRefresh(screen, trow, 0, 1, tcol, True);
		}
	} else {		/* do single row */
		ScrnRefresh(screen, frow, fcol, 1, tcol - fcol, True);
	}
}

static void _OwnSelection(XgtermWidget termw, String *selections, Cardinal count);

static void
SaltTextAway(int crow, int ccol, int row, int col, String *params, Cardinal num_params)
    /*register*/                          
                   			/* selections */
                        
    /* Guaranteed that (crow, ccol) <= (row, col), and that both points are valid
       (may have row = screen->max_row+1, col = 0) */
{
	TScreen *screen = &term->screen;
	int i, j = 0;
	int eol;
	char *line, *lp;

	if (crow == row && ccol > col) {
	    int tmp = ccol;
	    ccol = col;
	    col = tmp;
	}

	--col;
	/* first we need to know how long the string is before we can save it*/

	if ( row == crow ) j = Length(screen, crow, ccol, col);
	else {	/* two cases, cut is on same line, cut spans multiple lines */
		j += Length(screen, crow, ccol, screen->max_col) + 1;
		for(i = crow + 1; i < row; i++) 
			j += Length(screen, i, 0, screen->max_col) + 1;
		if (col >= 0)
			j += Length(screen, row, 0, col);
	}
	
	/* now get some memory to save it in */

	if (screen->selection_size <= j) {
	    if((line = malloc((unsigned) j + 1)) == (char *)NULL)
		SysError(ERROR_BMALLOC2);
	    XtFree(screen->selection);
	    screen->selection = line;
	    screen->selection_size = j + 1;
	} else line = screen->selection;
	if (!line || j < 0) return;

	line[j] = '\0';		/* make sure it is null terminated */
	lp = line;		/* lp points to where to save the text */
	if ( row == crow ) lp = SaveText(screen, row, ccol, col, lp, &eol);
	else {
		lp = SaveText(screen, crow, ccol, screen->max_col, lp, &eol);
		if (eol)
			*lp ++ = '\n';	/* put in newline at end of line */
		for(i = crow +1; i < row; i++) {
			lp = SaveText(screen, i, 0, screen->max_col, lp, &eol);
			if (eol)
				*lp ++ = '\n';
			}
		if (col >= 0)
			lp = SaveText(screen, row, 0, col, lp, &eol);
	}
	*lp = '\0';		/* make sure we have end marked */
	
	screen->selection_length = (lp - line);
	_OwnSelection(term, params, num_params);
}

static Boolean ConvertSelection(
    Widget w,
    Atom *selection,
    Atom *target,
    Atom *type,
    XtPointer *value,
    unsigned long *length,
    int *format)
{
    Display* d = XtDisplay(w);
    XgtermWidget xgterm = (XgtermWidget)w;

    if (xgterm->screen.selection == NULL) return False; /* can this happen? */

    if (*target == XA_TARGETS(d)) {
	Atom* targetP;
	Atom* std_targets;
	unsigned long std_length;
	XmuConvertStandardSelection(
		    w, xgterm->screen.selection_time, selection,
		    target, type, (char **)&std_targets, &std_length, format
		   );
	*length = std_length + 5;
	*value = (XtPointer)XtMalloc(sizeof(Atom)*(*length));
	targetP = *(Atom**)value;
	*targetP++ = XA_STRING;
	*targetP++ = XA_TEXT(d);
	*targetP++ = XA_COMPOUND_TEXT(d);
	*targetP++ = XA_LENGTH(d);
	*targetP++ = XA_LIST_LENGTH(d);
	memmove( (char*)targetP, (char*)std_targets, sizeof(Atom)*std_length);
	XtFree((char*)std_targets);
	*type = XA_ATOM;
	*format = 32;
	return True;
    }

    if (*target == XA_STRING ||
	*target == XA_TEXT(d) ||
	*target == XA_COMPOUND_TEXT(d)) {
	if (*target == XA_COMPOUND_TEXT(d))
	    *type = *target;
	else
	    *type = XA_STRING;
	*value = xgterm->screen.selection;
	*length = xgterm->screen.selection_length;
	*format = 8;
	return True;
    }
    if (*target == XA_LIST_LENGTH(d)) {
	*value = XtMalloc(4);
	if (sizeof(long) == 4)
	    *(long*)*value = 1;
	else {
	    long temp = 1;
	    memmove( (char*)*value, ((char*)&temp)+sizeof(long)-4, 4);
	}
	*type = XA_INTEGER;
	*length = 1;
	*format = 32;
	return True;
    }
    if (*target == XA_LENGTH(d)) {
	*value = XtMalloc(4);
	if (sizeof(long) == 4)
	    *(long*)*value = xgterm->screen.selection_length;
	else {
	    long temp = xgterm->screen.selection_length;
	    memmove( (char*)*value, ((char*)&temp)+sizeof(long)-4, 4);
	}
	*type = XA_INTEGER;
	*length = 1;
	*format = 32;
	return True;
    }
    if (XmuConvertStandardSelection(w, xgterm->screen.selection_time, selection,
				    target, type,
				    (char **)value, length, format))
	return True;

    /* else */
    return False;

}


static void LoseSelection(Widget w, Atom *selection)
{
    TScreen* screen = &((XgtermWidget)w)->screen;
    Atom* atomP;
    int i;
    for (i = 0, atomP = screen->selection_atoms;
	 i < screen->selection_count; i++, atomP++)
    {
	if (*selection == *atomP) *atomP = (Atom)0;
	switch (*atomP) {
	  case XA_CUT_BUFFER0:
	  case XA_CUT_BUFFER1:
	  case XA_CUT_BUFFER2:
	  case XA_CUT_BUFFER3:
	  case XA_CUT_BUFFER4:
	  case XA_CUT_BUFFER5:
	  case XA_CUT_BUFFER6:
	  case XA_CUT_BUFFER7:	*atomP = (Atom)0;
	}
    }

    for (i = screen->selection_count; i; i--) {
	if (screen->selection_atoms[i-1] != 0) break;
    }
    screen->selection_count = i;

    for (i = 0, atomP = screen->selection_atoms;
	 i < screen->selection_count; i++, atomP++)
    {
	if (*atomP == (Atom)0) {
	    *atomP = screen->selection_atoms[--screen->selection_count];
	}
    }

    if (screen->selection_count == 0)
	TrackText(0, 0, 0, 0);
}


/* ARGSUSED */
static void SelectionDone(Widget w, Atom *selection, Atom *target)
{
    /* empty proc so Intrinsics know we want to keep storage */
}


static void _OwnSelection(XgtermWidget termw, String *selections, Cardinal count)
{
    Atom* atoms = termw->screen.selection_atoms;
    int i;
    Boolean have_selection = False;

    if (termw->screen.selection_length < 0) return;

    if (count > termw->screen.sel_atoms_size) {
	XtFree((char*)atoms);
	atoms = (Atom*)XtMalloc(count*sizeof(Atom));
	termw->screen.selection_atoms = atoms;
	termw->screen.sel_atoms_size = count;
    }
    XmuInternStrings( XtDisplay((Widget)termw), selections, count, atoms );
    for (i = 0; i < count; i++) {
	int cutbuffer;
	switch (atoms[i]) {
	  case XA_CUT_BUFFER0: cutbuffer = 0; break;
	  case XA_CUT_BUFFER1: cutbuffer = 1; break;
	  case XA_CUT_BUFFER2: cutbuffer = 2; break;
	  case XA_CUT_BUFFER3: cutbuffer = 3; break;
	  case XA_CUT_BUFFER4: cutbuffer = 4; break;
	  case XA_CUT_BUFFER5: cutbuffer = 5; break;
	  case XA_CUT_BUFFER6: cutbuffer = 6; break;
	  case XA_CUT_BUFFER7: cutbuffer = 7; break;
	  default:	       cutbuffer = -1;
	}
	if (cutbuffer >= 0)
	    if ( termw->screen.selection_length >
		 4*XMaxRequestSize(XtDisplay((Widget)termw))-32)
		fprintf(stderr,
			"%s: selection too big (%d bytes), not storing in CUT_BUFFER%d\n",
			xgterm_name, termw->screen.selection_length, cutbuffer);
	    else
		XStoreBuffer( XtDisplay((Widget)termw), termw->screen.selection,
			      termw->screen.selection_length, cutbuffer );
	else if (!replyToEmacs) {
	    have_selection |=
		XtOwnSelection( (Widget)termw, atoms[i],
			    termw->screen.selection_time,
			    ConvertSelection, LoseSelection, SelectionDone );
	}
    }
    if (!replyToEmacs)
	termw->screen.selection_count = count;
    if (!have_selection)
	TrackText(0, 0, 0, 0);
}

void
DisownSelection(XgtermWidget termw)
{
    Atom* atoms = termw->screen.selection_atoms;
    Cardinal count = termw->screen.selection_count;
    int i;

    for (i = 0; i < count; i++) {
	int cutbuffer;
	switch (atoms[i]) {
	  case XA_CUT_BUFFER0: cutbuffer = 0; break;
	  case XA_CUT_BUFFER1: cutbuffer = 1; break;
	  case XA_CUT_BUFFER2: cutbuffer = 2; break;
	  case XA_CUT_BUFFER3: cutbuffer = 3; break;
	  case XA_CUT_BUFFER4: cutbuffer = 4; break;
	  case XA_CUT_BUFFER5: cutbuffer = 5; break;
	  case XA_CUT_BUFFER6: cutbuffer = 6; break;
	  case XA_CUT_BUFFER7: cutbuffer = 7; break;
	  default:	       cutbuffer = -1;
	}
	if (cutbuffer < 0)
	    XtDisownSelection( (Widget)termw, atoms[i],
			       termw->screen.selection_time );
    }
    termw->screen.selection_count = 0;
    termw->screen.startHRow = termw->screen.startHCol = 0;
    termw->screen.endHRow = termw->screen.endHCol = 0;
}


/* returns number of chars in line from scol to ecol out */
/* ARGSUSED */
static int
Length(TScreen *screen, int row, int scol, int ecol)
{
        int lastcol = LastTextCol(row);

	if (ecol > lastcol)
	    ecol = lastcol;
	return (ecol - scol + 1);
}

/* copies text into line, preallocated */
static char *
SaveText(TScreen *screen, int row, int scol, int ecol, char *lp, int *eol)
            
                   
                    
             		/* pointer to where to put the text */
             
{
	int i = 0;
	Char *ch = screen->buf[4 * (row + screen->topline)];
	Char attr;
	int c;

	*eol = 0;
	i = Length(screen, row, scol, ecol);
	ecol = scol + i;
	if (*eol == 0) {
		if(ScrnGetAttributes(screen, row + screen->topline, 0, &attr, 1) == 1) {
			*eol = (attr & LINEWRAPPED) ? 0 : 1;
		} else {
			/* If we can't get the attributes, assume no wrap */
			/* CANTHAPPEN */
			(void)fprintf(stderr, "%s: no attributes for %d, %d\n",
				xgterm_name, row, ecol - 1);
			*eol = 1;
		}
	}
	for (i = scol; i < ecol; i++) {
	        c = ch[i];
		if (c == 0)
			c = ' ';
		else if(c < ' ') {
			if(c == '\036')
				c = '#'; /* char on screen is pound sterling */
			else
				c += 0x5f; /* char is from DEC drawing set */
		} else if(c == 0x7f)
			c = 0x5f;
		*lp++ = c;
	}
	return(lp);
}

static void
EditorButton(XButtonEvent *event)
{
	TScreen *screen = &term->screen;
	int pty = screen->respond;
	char line[6];
	unsigned row, col;
	int button; 

	button = event->button - 1; 

	row = (event->y - screen->border) 
	 / FontHeight(screen);
	if (term->misc.sb_right)
            col = (event->x - screen->border) / FontWidth(screen);
	else
            col = (event->x - screen->border - screen->scrollbar) / FontWidth(screen);
	(void) strcpy(line, "\033[M");
	if (screen->send_mouse_pos == 1) {
		line[3] = ' ' + button;
	} else {
		line[3] = ' ' + (KeyState(event->state) << 2) + 
			((event->type == ButtonPress)? button:3);
	}
	line[4] = ' ' + col + 1;
	line[5] = ' ' + row + 1;
	v_write(pty, line, 6);
}


/* ARGSUSED */
void HandleSecure(Widget w, XEvent *event, String *params, Cardinal *param_count)
             
                  		/* unused */
                   		/* [0] = volume */
                          	/* 0 or 1 */
{
    Time time = CurrentTime;

    if ((event->xany.type == KeyPress) ||
	(event->xany.type == KeyRelease))
	time = event->xkey.time;
    else if ((event->xany.type == ButtonPress) ||
	     (event->xany.type == ButtonRelease))
      time = event->xbutton.time;
    DoSecureKeyboard (time);
}
