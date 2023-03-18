/* $XConsortium: TextAction.c,v 1.44 92/03/18 12:03:30 rws Exp $ */

/***********************************************************
Copyright 1989 by the Massachusetts Institute of Technology,
Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/*
 * NOTE:  There are some ASCII Dependancies on '\n' and '\0' that I
 *        am not too thrilled with.  There is also the implicit assumption
 *        that the individual characters will fit inside a "char".
 *        It would be nice if we could generalize this a but more.  If
 *        anyone out there comes up with an implementation of this stuff
 *        that has no dependency on ASCII, please send the code back to us.
 *
 *						Chris D. Peterson     
 *						MIT X Consortium 
 */

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Misc.h>
#include <X11/Xaw3d/TextP.h>
#include <stdio.h>
#include <ctype.h>

#define SrcScan                XawTextSourceScan
#define FindDist               XawTextSinkFindDistance
#define FindPos                XawTextSinkFindPosition

/*
 * These are defined in TextPop.c
 */

void _XawTextInsertFileAction(), _XawTextInsertFile(), _XawTextSearch();
void _XawTextSearch(), _XawTextDoSearchAction(), _XawTextDoReplaceAction();
void _XawTextSetField(), _XawTextPopdownSearchAction();

/*
 * These are defined in Text.c
 */

char * _XawTextGetText();
void _XawTextBuildLineTable(), _XawTextAlterSelection(), _XawTextVScroll();
void _XawTextSetSelection(), _XawTextCheckResize(), _XawTextExecuteUpdate();
void _XawTextSetScrollBars(), _XawTextClearAndCenterDisplay();
void _XawTextSaltAwaySelection(), _XawTextPrepareToUpdate();
Atom * _XawTextSelectionList();
int _XawTextReplace();

static void
StartAction(ctx, event)
TextWidget ctx;
XEvent *event;
{
  _XawTextPrepareToUpdate(ctx);
  if (event != NULL) {
    switch (event->type) {
    case ButtonPress:
    case ButtonRelease:
      ctx->text.time = event->xbutton.time;
      break;
    case KeyPress:
    case KeyRelease:
      ctx->text.time = event->xkey.time;
      break;
    case MotionNotify:
      ctx->text.time = event->xmotion.time;
      break;
    case EnterNotify:
    case LeaveNotify:
      ctx->text.time = event->xcrossing.time;
    }
  }
}

static void
NotePosition(ctx, event)
TextWidget ctx;
XEvent *event;
{
  switch (event->type) {
  case ButtonPress:
  case ButtonRelease:
    ctx->text.ev_x = event->xbutton.x;
    ctx->text.ev_y = event->xbutton.y;
    break;
  case KeyPress:
  case KeyRelease:
    {
	XRectangle cursor;
	XawTextSinkGetCursorBounds(ctx->text.sink, &cursor);
	ctx->text.ev_x = cursor.x + cursor.width / 2;;
	ctx->text.ev_y = cursor.y + cursor.height / 2;;
    }
    break;
  case MotionNotify:
    ctx->text.ev_x = event->xmotion.x;
    ctx->text.ev_y = event->xmotion.y;
    break;
  case EnterNotify:
  case LeaveNotify:
    ctx->text.ev_x = event->xcrossing.x;
    ctx->text.ev_y = event->xcrossing.y;
  }
}

static void
EndAction(ctx)
TextWidget ctx;
{
  _XawTextCheckResize(ctx);
  _XawTextExecuteUpdate(ctx);
  ctx->text.mult = 1;
}


struct _SelectionList {
    String *params;
    Cardinal count;
    Time time;
};

static void GetSelection();

/* ARGSUSED */
static void 
_SelectionReceived(w, client_data, selection, type, value, length, format)
Widget w;
XtPointer client_data;
Atom *selection, *type;
XtPointer value;
unsigned long *length;
int *format;
{
  TextWidget ctx = (TextWidget)w;
  XawTextBlock text;
  
  if (*type == 0 /*XT_CONVERT_FAIL*/ || *length == 0) {
    struct _SelectionList* list = (struct _SelectionList*)client_data;
    if (list != NULL) {
      GetSelection(w, list->time, list->params, list->count);
      XtFree(client_data);
    }
    return;
  }
  
  StartAction(ctx, (XEvent *)NULL);
  text.ptr = (char*)value;
  text.firstPos = 0;
  text.length = *length;
  text.format = FMT8BIT;
  if (_XawTextReplace(ctx, ctx->text.insertPos, ctx->text.insertPos, &text)) {
    XBell(XtDisplay(ctx), 0);
    return;
  }
  ctx->text.insertPos = SrcScan(ctx->text.source, 
				(XawTextPosition) ctx->text.insertPos, 
				XawstPositions, XawsdRight, text.length, TRUE);

  EndAction(ctx);
  _XawTextSetScrollBars(ctx);
  XtFree(client_data);
  XFree(value);		/* the selection value should be freed with XFree */
}

static void 
GetSelection(w, time, params, num_params)
Widget w;
Time time;
String *params;			/* selections in precedence order */
Cardinal num_params;
{
    Atom selection;
    int buffer;

    selection = XInternAtom(XtDisplay(w), *params, False);
    switch (selection) {
      case XA_CUT_BUFFER0: buffer = 0; break;
      case XA_CUT_BUFFER1: buffer = 1; break;
      case XA_CUT_BUFFER2: buffer = 2; break;
      case XA_CUT_BUFFER3: buffer = 3; break;
      case XA_CUT_BUFFER4: buffer = 4; break;
      case XA_CUT_BUFFER5: buffer = 5; break;
      case XA_CUT_BUFFER6: buffer = 6; break;
      case XA_CUT_BUFFER7: buffer = 7; break;
      default:	       buffer = -1;
    }
    if (buffer >= 0) {
	int nbytes;
	unsigned long length;
	int fmt8 = 8;
	Atom type = XA_STRING;
	char *line = XFetchBuffer(XtDisplay(w), &nbytes, buffer);
	if ((length = nbytes))
	    _SelectionReceived(w, (XtPointer) NULL, &selection, &type, 
						   (caddr_t)line, &length, &fmt8);
	else if (num_params > 1)
	    GetSelection(w, time, params+1, num_params-1);
    } else {
	struct _SelectionList* list;
	if (--num_params) {
	    list = XtNew(struct _SelectionList);
	    list->params = params + 1;
	    list->count = num_params;
	    list->time = time;
	} else list = NULL;
	XtGetSelectionValue(w, selection, XA_STRING, _SelectionReceived,
			    (XtPointer)list, time);
    }
}

static void 
InsertSelection(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;		/* precedence list of selections to try */
Cardinal *num_params;
{
  StartAction((TextWidget)w, event); /* Get Time. */
  GetSelection(w, ((TextWidget)w)->text.time, params, *num_params);
  EndAction((TextWidget)w);
}

/************************************************************
 *
 * Routines for Moving Around.
 *
 ************************************************************/

static void
Move(ctx, event, dir, type, include)
TextWidget ctx;
XEvent *event;
XawTextScanDirection dir;
XawTextScanType type;
Boolean include;
{
  StartAction(ctx, event);
  ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos,
				type, dir, ctx->text.mult, include);
  EndAction(ctx);
}

/*ARGSUSED*/
static void 
MoveForwardChar(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
   Move((TextWidget) w, event, XawsdRight, XawstPositions, TRUE);
}

/*ARGSUSED*/
static void 
MoveBackwardChar(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstPositions, TRUE);
}

/*ARGSUSED*/
static void 
MoveForwardWord(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdRight, XawstWhiteSpace, FALSE);
}

/*ARGSUSED*/
static void 
MoveBackwardWord(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstWhiteSpace, FALSE);
}

/*ARGSUSED*/
static void MoveForwardParagraph(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdRight, XawstParagraph, FALSE);
}

/*ARGSUSED*/
static void MoveBackwardParagraph(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstParagraph, FALSE);
}

/*ARGSUSED*/
static void 
MoveToLineEnd(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdRight, XawstEOL, FALSE);
}

/*ARGSUSED*/
static void 
MoveToLineStart(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstEOL, FALSE);
}


static void
MoveLine(ctx, event, dir)
TextWidget ctx;
XEvent *event;
XawTextScanDirection dir;
{
  XawTextPosition new, next_line, junk;
  int from_left, garbage;

  StartAction(ctx, event);

  if (dir == XawsdLeft)
    ctx->text.mult++;

  new = SrcScan(ctx->text.source, ctx->text.insertPos,
		XawstEOL, XawsdLeft, 1, FALSE);

  FindDist(ctx->text.sink, new, ctx->text.margin.left, ctx->text.insertPos,
	   &from_left, &junk, &garbage);

  new = SrcScan(ctx->text.source, ctx->text.insertPos, XawstEOL, dir,
		ctx->text.mult, (dir == XawsdRight));

  next_line = SrcScan(ctx->text.source, new, XawstEOL, XawsdRight, 1, FALSE);

  FindPos(ctx->text.sink, new, ctx->text.margin.left, from_left, FALSE,
	  &(ctx->text.insertPos), &garbage, &garbage);
  
  if (ctx->text.insertPos > next_line)
    ctx->text.insertPos = next_line;

  EndAction(ctx);
}

/*ARGSUSED*/
static void 
MoveNextLine(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  MoveLine( (TextWidget) w, event, XawsdRight);
}

/*ARGSUSED*/
static void 
MovePreviousLine(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  MoveLine( (TextWidget) w, event, XawsdLeft);
}

/*ARGSUSED*/
static void
MoveBeginningOfFile(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdLeft, XawstAll, TRUE);
}

/*ARGSUSED*/
static void 
MoveEndOfFile(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Move((TextWidget) w, event, XawsdRight, XawstAll, TRUE);
}

static void 
Scroll(ctx, event, dir)
TextWidget ctx;
XEvent *event;
XawTextScanDirection dir;
{
  StartAction(ctx, event);

  if (dir == XawsdLeft)
    _XawTextVScroll(ctx, ctx->text.mult);
  else
    _XawTextVScroll(ctx, -ctx->text.mult);

  EndAction(ctx);
}

/*ARGSUSED*/
static void 
ScrollOneLineUp(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Scroll( (TextWidget) w, event, XawsdLeft);
}

/*ARGSUSED*/
static void 
ScrollOneLineDown(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  Scroll( (TextWidget) w, event, XawsdRight);
}

static void 
MovePage(ctx, event, dir)
TextWidget ctx;
XEvent *event;
XawTextScanDirection dir;
{
  int scroll_val = Max(1, ctx->text.lt.lines - 2);

  if (dir == XawsdLeft)
    scroll_val = -scroll_val;

  StartAction(ctx, event);
  _XawTextVScroll(ctx, scroll_val);
  ctx->text.insertPos = ctx->text.lt.top;
  EndAction(ctx);
}

/*ARGSUSED*/
static void 
MoveNextPage(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  MovePage((TextWidget) w, event, XawsdRight);
}

/*ARGSUSED*/
static void 
MovePreviousPage(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  MovePage((TextWidget) w, event, XawsdLeft);
}

/************************************************************
 *
 * Delete Routines.
 *
 ************************************************************/

static void 
_DeleteOrKill(ctx, from, to, kill)
TextWidget ctx;
XawTextPosition from, to;
Boolean	kill;
{
  XawTextBlock text;
  char *ptr;
  
  if (kill && from < to) {
    ptr = _XawTextGetText(ctx, from, to);
    XStoreBuffer(XtDisplay(ctx), ptr, strlen(ptr), 1);
    XtFree(ptr);
  }
  text.length = 0;
  text.firstPos = 0;
  if (_XawTextReplace(ctx, from, to, &text)) {
    XBell(XtDisplay(ctx), 50);
    return;
  }
  ctx->text.insertPos = from;
  ctx->text.showposition = TRUE; 
}

static void
DeleteOrKill(ctx, event, dir, type, include, kill)
TextWidget	   ctx;
XEvent *event;
XawTextScanDirection dir;
XawTextScanType type;
Boolean	   include, kill;
{
  XawTextPosition from, to;
  
  StartAction(ctx, event);
  to = SrcScan(ctx->text.source, ctx->text.insertPos,
	       type, dir, ctx->text.mult, include);

/*
 * If no movement actually happened, then bump the count and try again. 
 * This causes the character position at the very beginning and end of 
 * a boundry to act correctly. 
 */

  if (to == ctx->text.insertPos)
      to = SrcScan(ctx->text.source, ctx->text.insertPos,
		   type, dir, ctx->text.mult + 1, include);

  if (dir == XawsdLeft) {
    from = to;
    to = ctx->text.insertPos;
  }
  else 
    from = ctx->text.insertPos;

  _DeleteOrKill(ctx, from, to, kill);
  _XawTextSetScrollBars(ctx);
  EndAction(ctx);
}

/*ARGSUSED*/
static void 
DeleteForwardChar(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  DeleteOrKill((TextWidget) w, event, XawsdRight, XawstPositions, TRUE, FALSE);
}

/*ARGSUSED*/
static void
DeleteBackwardChar(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  DeleteOrKill((TextWidget) w, event, XawsdLeft, XawstPositions, TRUE, FALSE);
}

/*ARGSUSED*/
static void 
DeleteForwardWord(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  DeleteOrKill((TextWidget) w, event,
	       XawsdRight, XawstWhiteSpace, FALSE, FALSE);
}

/*ARGSUSED*/
static void 
DeleteBackwardWord(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  DeleteOrKill((TextWidget) w, event,
	       XawsdLeft, XawstWhiteSpace, FALSE, FALSE);
}

/*ARGSUSED*/
static void 
KillForwardWord(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  DeleteOrKill((TextWidget) w, event, 
	       XawsdRight, XawstWhiteSpace, FALSE, TRUE);
}

/*ARGSUSED*/
static void 
KillBackwardWord(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  DeleteOrKill((TextWidget) w, event,
	       XawsdLeft, XawstWhiteSpace, FALSE, TRUE);
}

/*ARGSUSED*/
static void
KillToEndOfLine(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  TextWidget ctx = (TextWidget) w;
  XawTextPosition end_of_line;

  StartAction(ctx, event);
  end_of_line = SrcScan(ctx->text.source, ctx->text.insertPos, XawstEOL, 
			XawsdRight, ctx->text.mult, FALSE);
  if (end_of_line == ctx->text.insertPos)
    end_of_line = SrcScan(ctx->text.source, ctx->text.insertPos, XawstEOL, 
			  XawsdRight, ctx->text.mult, TRUE);

  _DeleteOrKill(ctx, ctx->text.insertPos, end_of_line, TRUE);
  EndAction(ctx);
  _XawTextSetScrollBars(ctx);
}

/*ARGSUSED*/
static void 
KillToEndOfParagraph(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  DeleteOrKill((TextWidget) w, event, XawsdRight, XawstParagraph, FALSE, TRUE);
}

void 
_XawTextZapSelection(ctx, event, kill)
TextWidget ctx;
XEvent *event;
Boolean kill;
{
   StartAction(ctx, event);
   _DeleteOrKill(ctx, ctx->text.s.left, ctx->text.s.right, kill);
   EndAction(ctx);
  _XawTextSetScrollBars(ctx);
}

/*ARGSUSED*/
static void 
KillCurrentSelection(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  _XawTextZapSelection( (TextWidget) w, event, TRUE);
}

/*ARGSUSED*/
static void 
DeleteCurrentSelection(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  _XawTextZapSelection( (TextWidget) w, event, FALSE);
}

/************************************************************
 *
 * Insertion Routines.
 *
 ************************************************************/

static int 
InsertNewLineAndBackupInternal(ctx)
TextWidget ctx;
{
  int count, error = XawEditDone;
  XawTextBlock text;
  char *buf, *ptr;

  ptr = buf = XtMalloc((unsigned) sizeof(char) * ctx->text.mult);
  for (count = 0; count < ctx->text.mult; count++, ptr++)
    ptr[0] = '\n';

  text.length = ctx->text.mult;
  text.ptr = buf;
  text.firstPos = 0;
  text.format = FMT8BIT;

  if (_XawTextReplace(ctx, ctx->text.insertPos, ctx->text.insertPos, &text)) {
    XBell( XtDisplay(ctx), 50);
    error = XawEditError;
  }
  else 
    ctx->text.showposition = TRUE;

  XtFree(buf);
  return(error);
}

/*ARGSUSED*/
static void 
InsertNewLineAndBackup(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  StartAction( (TextWidget) w, event );
  (void) InsertNewLineAndBackupInternal( (TextWidget) w );
  EndAction( (TextWidget) w );
  _XawTextSetScrollBars( (TextWidget) w);
}

static int
LocalInsertNewLine(ctx, event)
TextWidget ctx;
XEvent *event;
{
  StartAction(ctx, event);
  if (InsertNewLineAndBackupInternal(ctx) == XawEditError)
    return(XawEditError);
  ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos, 
			     XawstPositions, XawsdRight, ctx->text.mult, TRUE);
  EndAction(ctx);
  _XawTextSetScrollBars(ctx);
  return(XawEditDone);
}

/*ARGSUSED*/
static void
InsertNewLine(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  (void) LocalInsertNewLine( (TextWidget) w, event);
}

/*ARGSUSED*/
static void 
InsertNewLineAndIndent(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  XawTextBlock text;
  XawTextPosition pos1;
  register char *ptr;
  register int length;
  TextWidget ctx = (TextWidget) w;

  StartAction(ctx, event);
  pos1 = SrcScan(ctx->text.source, ctx->text.insertPos, 
		 XawstEOL, XawsdLeft, 1, FALSE);

  /* Hacked because Ascii Source Object Scan method is permanently broken. */
  text.ptr = _XawTextGetText(ctx, pos1, ctx->text.insertPos);
  length = strlen(text.ptr);
  for (ptr=text.ptr; length && isspace(*ptr); ptr++, length--)
      ;
  *ptr = '\0';

  text.length = strlen(text.ptr);
  if (LocalInsertNewLine(ctx, event)) {
      XtFree(text.ptr);
      return;
  }
  text.firstPos = 0;
  if (_XawTextReplace(ctx,ctx->text.insertPos, ctx->text.insertPos, &text)) {
    XBell(XtDisplay(ctx), 50);
    XtFree(text.ptr);
    EndAction(ctx);
    return;
  }
  ctx->text.insertPos = SrcScan(ctx->text.source, ctx->text.insertPos,
				XawstPositions, XawsdRight, text.length, TRUE);
  XtFree(text.ptr);
  EndAction(ctx);
  _XawTextSetScrollBars(ctx);
}

/************************************************************
 *
 * Selection Routines.
 *
 *************************************************************/

static void 
SelectWord(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;
Cardinal *num_params;
{
  TextWidget ctx = (TextWidget) w;
  XawTextPosition l, r;

  StartAction(ctx, event);
  l = SrcScan(ctx->text.source, ctx->text.insertPos, 
	      XawstWhiteSpace, XawsdLeft, 1, FALSE);
  r = SrcScan(ctx->text.source, l, XawstWhiteSpace, XawsdRight, 1, FALSE);
  _XawTextSetSelection(ctx, l, r, params, *num_params);
  EndAction(ctx);
}

static void 
SelectAll(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;
Cardinal *num_params;
{
  TextWidget ctx = (TextWidget) w;

  StartAction(ctx, event);
  _XawTextSetSelection(ctx,zeroPosition,ctx->text.lastPos,params,*num_params);
  EndAction(ctx);
}

static void
ModifySelection(ctx, event, mode, action, params, num_params)
TextWidget ctx;
XEvent *event;
XawTextSelectionMode mode;
XawTextSelectionAction action;
String *params;		/* unused */
Cardinal *num_params;	/* unused */
{
  StartAction(ctx, event);
  NotePosition(ctx, event);
  _XawTextAlterSelection(ctx, mode, action, params, num_params);
  EndAction(ctx);
}
		
/* ARGSUSED */
static void 
SelectStart(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;		/* unused */
Cardinal *num_params;	/* unused */
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextSelect, XawactionStart, params, num_params);
}

/* ARGSUSED */
static void 
SelectAdjust(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;		/* unused */
Cardinal *num_params;	/* unused */
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextSelect, XawactionAdjust, params, num_params);
}

static void 
SelectEnd(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;
Cardinal *num_params;
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextSelect, XawactionEnd, params, num_params);
}

/* ARGSUSED */
static void 
ExtendStart(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;		/* unused */
Cardinal *num_params;	/* unused */
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextExtend, XawactionStart, params, num_params);
}

/* ARGSUSED */
static void 
ExtendAdjust(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;		/* unused */
Cardinal *num_params;	/* unused */
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextExtend, XawactionAdjust, params, num_params);
}

static void 
ExtendEnd(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;
Cardinal *num_params;
{
  ModifySelection((TextWidget) w, event, 
		  XawsmTextExtend, XawactionEnd, params, num_params);
}

static void
SelectSave(w, event, params, num_params)
Widget  w;
XEvent *event;
String *params;
Cardinal *num_params;
{
    int	    num_atoms;
    Atom*   sel;
    Display* dpy = XtDisplay(w);
    Atom    selections[256];

    StartAction ((TextWidget)w, event);
    num_atoms = *num_params;
    if (num_atoms > 256)
	num_atoms = 256;
    for (sel=selections; --num_atoms >= 0; sel++, params++)
	*sel = XInternAtom(dpy, *params, False);
    num_atoms = *num_params;
    _XawTextSaltAwaySelection ((TextWidget)w, selections, num_atoms);
    EndAction ((TextWidget)w);
}

/************************************************************
 *
 * Misc. Routines.
 *
 ************************************************************/

/* ARGSUSED */
static void 
RedrawDisplay(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  StartAction( (TextWidget) w, event);
  _XawTextClearAndCenterDisplay((TextWidget) w);
  EndAction( (TextWidget) w);
}

/*ARGSUSED*/
static void
TextFocusIn (w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  TextWidget ctx = (TextWidget) w;

  ctx->text.hasfocus = TRUE; 
}

/*ARGSUSED*/
static void
TextFocusOut(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  TextWidget ctx = (TextWidget) w;

  ctx->text.hasfocus = FALSE;
}

static XComposeStatus compose_status = {NULL, 0};

/*	Function Name: AutoFill
 *	Description: Breaks the line at the previous word boundry when
 *                   called inside InsertChar.
 *	Arguments: ctx - The text widget.
 *	Returns: none
 */

static void
AutoFill(ctx)
TextWidget ctx;
{
  int width, height, x, line_num, max_width;
  XawTextPosition ret_pos;
  XawTextBlock text;

  if ( !((ctx->text.auto_fill) && (ctx->text.mult == 1)) )
    return;

  for ( line_num = 0; line_num < ctx->text.lt.lines ; line_num++)
    if ( ctx->text.lt.info[line_num].position >= ctx->text.insertPos )
      break;
  line_num--;			/* backup a line. */

  max_width = Max(0, (int)(ctx->core.width - HMargins(ctx)));

  x = ctx->text.margin.left;
  XawTextSinkFindPosition( ctx->text.sink,ctx->text.lt.info[line_num].position,
			  x, max_width, TRUE, &ret_pos, &width, &height);
  
  if ( ret_pos >= ctx->text.insertPos )
    return;
  
  text.ptr = "\n";
  text.length = 1;
  text.firstPos = 0;
  text.format = FMT8BIT;

  if (_XawTextReplace(ctx, ret_pos - 1, ret_pos, &text))
    XBell(XtDisplay((Widget) ctx), 0);	/* Unable to edit, complain. */

}

/*ARGSUSED*/
static void
InsertChar(w, event, p, n)
Widget w;
XEvent *event;
String *p;
Cardinal *n;
{
  TextWidget ctx = (TextWidget) w;
  char *ptr, strbuf[BUFSIZ];
  int count, error;
  KeySym keysym;
  XawTextBlock text;

  if ( (text.length = XLookupString (&event->xkey, strbuf, BUFSIZ,
			       &keysym, &compose_status)) == 0) {
    return;
  }
  
  text.ptr = ptr = 
	XtMalloc((unsigned) sizeof(char) * text.length * ctx->text.mult);
  for (count = 0 ; count < ctx->text.mult ; count++) {
    (void) strncpy(ptr, strbuf, text.length);
    ptr += text.length;
  }

  text.length = text.length * ctx->text.mult;
  text.firstPos = 0;
  text.format = FMT8BIT;
  
  StartAction(ctx, event);
  
  error = _XawTextReplace(ctx, ctx->text.insertPos,ctx->text.insertPos, &text);

  if (error == XawEditDone) {
    ctx->text.insertPos = 
      SrcScan(ctx->text.source, ctx->text.insertPos,
	      XawstPositions, XawsdRight, text.length, TRUE);
    AutoFill(ctx);
  }
  else 
    XBell(XtDisplay(ctx), 50);

  XtFree(text.ptr);
  EndAction(ctx);
  _XawTextSetScrollBars(ctx);
}

/*ARGSUSED*/
static void 
InsertString(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;
Cardinal *num_params;
{
  TextWidget ctx = (TextWidget) w;
  XawTextBlock text;
  int	   i;

  text.firstPos = 0;
  StartAction(ctx, event);
  for (i = *num_params; i; i--, params++) {
    unsigned char hexval;
    if ((*params)[0] == '0' && (*params)[1] == 'x' && (*params)[2] != '\0') {
      char c, *p;
      hexval = 0;
      for (p = *params+2; (c = *p); p++) {
	hexval *= 16;
	if (c >= '0' && c <= '9')
	  hexval += c - '0';
	else if (c >= 'a' && c <= 'f')
	  hexval += c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
	  hexval += c - 'A' + 10;
	else break;
      }
      if (c == '\0') {
	text.ptr = (char*)&hexval;
	text.length = 1;
      } else text.length = strlen(text.ptr = *params);
    } else text.length = strlen(text.ptr = *params);
    if (text.length == 0) continue;
    if (_XawTextReplace(ctx, ctx->text.insertPos, 
			ctx->text.insertPos, &text)) {
      XBell(XtDisplay(ctx), 50);
      EndAction(ctx);
      return;
    }
    ctx->text.insertPos =
      SrcScan(ctx->text.source, ctx->text.insertPos,
	      XawstPositions, XawsdRight, text.length, TRUE);
  }
  EndAction(ctx);
}

static void 
DisplayCaret(w, event, params, num_params)
Widget w;
XEvent *event;		/* CrossingNotify special-cased */
String *params;		/* Off, False, No, On, True, Yes, etc. */
Cardinal *num_params;	/* 0, 1 or 2 */
{
  TextWidget ctx = (TextWidget)w;
  Boolean display_caret = True;

  if (event->type == EnterNotify || event->type == LeaveNotify) {
    /* for Crossing events, the default case is to check the focus
     * field and only change the caret when focus==True.  A second
     * argument of "always" will cause the focus field to be ignored.
     */
    Boolean check_focus = True;
    if (*num_params == 2 && strcmp(params[1], "always") == 0)
      check_focus = False;
    if (check_focus && !event->xcrossing.focus) return;
  }

  if (*num_params > 0) {	/* default arg is "True" */
    XrmValue from, to;
    from.size = strlen(from.addr = params[0]);
    XtConvert(w, XtRString, &from, XtRBoolean, &to);
    if (to.addr != NULL) display_caret = *(Boolean*)to.addr;
    if (ctx->text.display_caret == display_caret) return;
  }
  StartAction(ctx, event);
  ctx->text.display_caret = display_caret;
  EndAction(ctx);
}

/*	Function Name: Multiply
 *	Description: Multiplies the current action by the number passed.
 *	Arguments: w - the text widget.
 *                 event - ** NOT USED **.
 *                 params, num_params - The parameter list, see below.
 *	Returns: none.
 *
 * Parameter list;
 *  
 * The parameter list may contain either a number or the string 'Reset'.
 * 
 * A number will multiply the current multiplication factor by that number.
 * Many of the text widget actions will will perform n actions, where n is
 * the multiplication factor.
 *
 * The string reset will reset the mutiplication factor to 1.
 * 
 */

/* ARGSUSED */
static void 
Multiply(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  TextWidget ctx = (TextWidget) w;
  int mult;

  if (*num_params != 1) {
    XtAppError(XtWidgetToApplicationContext(w), 
	       "The multiply action takes exactly one argument.");
    XBell(XtDisplay(w), 0);
    return;
  }

  if ( (params[0][0] == 'r') || (params[0][0] == 'R') ) {
    XBell(XtDisplay(w), 0);
    ctx->text.mult = 1;
    return;
  }

  if ( (mult = atoi(params[0])) == 0 ) {
    char buf[BUFSIZ];
    (void) sprintf(buf, "%s %s", "Text Widget: The multiply action's argument",
	    "must be a number greater than zero, or 'Reset'.");
    XtAppError(XtWidgetToApplicationContext(w), buf);
    XBell(XtDisplay(w), 0);
    return;
  }

  ctx->text.mult *= mult;
}

/*	Function Name: StripOutOldCRs
 *	Description: strips out the old carrige returns.
 *	Arguments: ctx - the text widget.
 *                 from - starting point.
 *                 to - the ending point
 *	Returns: the new ending location (we may add some characters),
 *		 or XawTextReplaceError, which indicates failure.
 */

static XawTextPosition
StripOutOldCRs(ctx, from, to)
TextWidget ctx;
XawTextPosition from, to;
{
  XawTextPosition startPos, endPos, eop_begin, eop_end, temp;
  Widget src = ctx->text.source;
  XawTextBlock text;
  char *buf;

  text.ptr= "  ";
  text.firstPos = 0;
  text.format = FMT8BIT;
   
/*
 * Strip out CR's. 
 */

  eop_begin = eop_end = startPos = endPos = from;
  while (TRUE) {
    endPos=SrcScan(src, startPos, XawstEOL, XawsdRight, 1, FALSE);

    temp=SrcScan(src, endPos, XawstWhiteSpace, XawsdLeft, 1, FALSE);
    temp=SrcScan(src, temp, XawstWhiteSpace, XawsdRight, 1, FALSE);

    if (temp > startPos)
	endPos = temp;

    if (endPos >= to)
      break;

    if (endPos >= eop_begin) {
      startPos = eop_end;
      eop_begin = SrcScan(src, startPos, XawstParagraph, XawsdRight, 1, FALSE);
      eop_end = SrcScan(src, startPos, XawstParagraph, XawsdRight, 1, TRUE);
    }
    else {
      XawTextPosition periodPos, next_word;
      int i, len;

      periodPos= SrcScan(src, endPos, XawstPositions, XawsdLeft, 1, TRUE);
      next_word = SrcScan(src, endPos, XawstWhiteSpace, XawsdRight, 1, FALSE);

      len = next_word - periodPos;

      text.length = 1;
      buf = _XawTextGetText(ctx, periodPos, next_word);
      if ( (periodPos < endPos) && (buf[0] == '.') )
	  text.length++;	/* Put in two spaces. */

      /*
       * Remove all extra spaces. 
       */

      for (i = 1 ; i < len; i++) 
	  if ( !isspace(buf[i]) || ((periodPos + i) >= to) ) {
	      break;
	  }
      
      XtFree(buf);

      to -= (i - text.length - 1);
      startPos = SrcScan(src, periodPos, XawstPositions, XawsdRight, i, TRUE);
      if (_XawTextReplace(ctx, endPos, startPos, &text) != XawEditDone)
	  return XawReplaceError;
      startPos -= i - text.length;
    }
  }
  return(to);
}

/*	Function Name: InsertNewCRs
 *	Description: Inserts the new Carrige Returns.
 *	Arguments: ctx - the text widget.
 *                 from, to - the ends of the region.
 *	Returns: none
 */

static void
InsertNewCRs(ctx, from, to)
TextWidget ctx;
XawTextPosition from, to;
{
  XawTextPosition startPos, endPos, space, eol;
  XawTextBlock text;
  int i, width, height, len;
  char * buf;

  text.ptr = "\n";
  text.length = 1;
  text.firstPos = 0;
  text.format = FMT8BIT;

  startPos = from;
  while (TRUE) {
    XawTextSinkFindPosition( ctx->text.sink, startPos, 
			    (int) ctx->text.margin.left,
			    (int) (ctx->core.width - HMargins(ctx)), 
			    TRUE, &eol, &width, &height);
    if (eol >= to)
      break;

    eol = SrcScan(ctx->text.source, eol, XawstPositions, XawsdLeft, 1, TRUE);
    space= SrcScan(ctx->text.source, eol, XawstWhiteSpace, XawsdRight, 1,TRUE);
    
    startPos = endPos = eol;
    if (eol == space) 
      return;

    len = (int) (space - eol);
    buf = _XawTextGetText(ctx, eol, space);
    for ( i = 0 ; i < len ; i++)
      if (!isspace(buf[i]))
	break;

    to -= (i - 1);
    endPos = SrcScan(ctx->text.source, endPos,
		     XawstPositions, XawsdRight, i, TRUE);
    XtFree(buf);
    
    if (_XawTextReplace(ctx, startPos, endPos, &text))
	return;
    startPos = SrcScan(ctx->text.source, startPos,
		       XawstPositions, XawsdRight, 1, TRUE);
  }
}  
  
/*	Function Name: FormRegion
 *	Description: Forms up the region specified.
 *	Arguments: ctx - the text widget.
 *                 from, to - the ends of the region.
 *	Returns: XawEditDone if successful, or XawReplaceError.
 */

static int
FormRegion(ctx, from, to)
TextWidget ctx;
XawTextPosition from, to;
{
  if (from >= to) return XawEditDone;

  if ((to = StripOutOldCRs(ctx, from, to)) == XawReplaceError)
      return XawReplaceError;
  /* insure that the insertion point is within legal bounds */
  if (ctx->text.insertPos >
      SrcScan(ctx->text.source, (XawTextPosition)0, XawstAll, XawsdRight, 1, TRUE))
      ctx->text.insertPos = to;
  InsertNewCRs(ctx, from, to);
  _XawTextBuildLineTable(ctx, ctx->text.lt.top, TRUE);
  return XawEditDone;
}

/*	Function Name: FormParagraph.
 *	Description: reforms up the current paragraph.
 *	Arguments: w - the text widget.
 *                 event - the X event.
 *                 params, num_params *** NOT USED ***.
 *	Returns: none
 */

/* ARGSUSED */
static void 
FormParagraph(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  TextWidget ctx = (TextWidget) w;
  XawTextPosition from, to;

  StartAction(ctx, event);

  from =  SrcScan(ctx->text.source, ctx->text.insertPos,
		  XawstParagraph, XawsdLeft, 1, FALSE);
  to  =  SrcScan(ctx->text.source, from,
		 XawstParagraph, XawsdRight, 1, FALSE);

  if (FormRegion(ctx, from, to) == XawReplaceError)
      XBell(XtDisplay(w), 0);
  EndAction(ctx);
  _XawTextSetScrollBars(ctx);
}

/*	Function Name: TransposeCharacters
 *	Description: Swaps the character to the left of the mark with
 *                   the character to the right of the mark.
 *	Arguments: w - the text widget.
 *                 event - the event that cause this action.
 *                 params, num_params *** NOT USED ***.
 *	Returns: none.
 */
	     
/* ARGSUSED */
static void 
TransposeCharacters(w, event, params, num_params)
Widget w;
XEvent *event;
String * params;
Cardinal * num_params;
{
  TextWidget ctx = (TextWidget) w;
  XawTextPosition start, end;
  XawTextBlock text;
  unsigned char * buf, c;
  int i;

  StartAction(ctx, event);

/*
 * Get bounds. 
 */

  start = SrcScan(ctx->text.source, ctx->text.insertPos, XawstPositions, 
		  XawsdLeft, 1, TRUE);
  end = SrcScan(ctx->text.source, ctx->text.insertPos, XawstPositions, 
		XawsdRight, ctx->text.mult, TRUE);

  if ( (start == ctx->text.insertPos) || (end == ctx->text.insertPos) ) 
    XBell(XtDisplay(w), 0);	/* complain. */
  else {
    ctx->text.insertPos = end;

    /*
     * Retrieve text and swap the characters. 
     */
    
    buf = (unsigned char *) _XawTextGetText(ctx, start, end);
    text.length = strlen((char *)buf);
    text.firstPos = 0;
    text.format = FMT8BIT;
    
    c = buf[0];
    for (i = 1 ; i < text.length ; i++)
      buf[i - 1] = buf[i];
    buf[i - 1] = c;
    
    /* 
     * Store new text is source.
     */
    
    text.ptr = (char *) buf;
    if (_XawTextReplace (ctx, start, end, &text))
	XBell(XtDisplay(w), 0);	/* Unable to edit, complain. */
    XtFree((char *) buf);
  }
  EndAction(ctx);
}

/*	Function Name: NoOp
 *	Description: This action performs no action, and allows
 *                   the user or application programmer to unbind a 
 *                   translation.
 *	Arguments: w - the text widget.
 *                 event - *** UNUSED ***.
 *                 parms and num_params - the action parameters.
 *	Returns: none.
 *
 * Note: If the parameter list contains the string "RingBell" then
 *       this action will ring the bell.
 */

/*ARGSUSED*/
static void
NoOp(w, event, params, num_params)
Widget w;
XEvent *event;
String *params;
Cardinal *num_params;
{
    if (*num_params != 1)
	return;

    switch(params[0][0]) {
    case 'R':
    case 'r':
	XBell(XtDisplay(w), 0);
    default:			/* Fall Through */
	break;
    }
}
	
/* Action Table */

XtActionsRec _XawTextActionsTable[] = {
/* motion bindings */
  {"forward-character", 	MoveForwardChar},
  {"backward-character", 	MoveBackwardChar},
  {"forward-word", 		MoveForwardWord},
  {"backward-word", 		MoveBackwardWord},
  {"forward-paragraph", 	MoveForwardParagraph},
  {"backward-paragraph", 	MoveBackwardParagraph},
  {"beginning-of-line", 	MoveToLineStart},
  {"end-of-line", 		MoveToLineEnd},
  {"next-line", 		MoveNextLine},
  {"previous-line", 		MovePreviousLine},
  {"next-page", 		MoveNextPage},
  {"previous-page", 		MovePreviousPage},
  {"beginning-of-file", 	MoveBeginningOfFile},
  {"end-of-file", 		MoveEndOfFile},
  {"scroll-one-line-up", 	ScrollOneLineUp},
  {"scroll-one-line-down", 	ScrollOneLineDown},
/* delete bindings */
  {"delete-next-character", 	DeleteForwardChar},
  {"delete-previous-character", DeleteBackwardChar},
  {"delete-next-word", 		DeleteForwardWord},
  {"delete-previous-word", 	DeleteBackwardWord},
  {"delete-selection", 		DeleteCurrentSelection},
/* kill bindings */
  {"kill-word", 		KillForwardWord},
  {"backward-kill-word", 	KillBackwardWord},
  {"kill-selection", 		KillCurrentSelection},
  {"kill-to-end-of-line", 	KillToEndOfLine},
  {"kill-to-end-of-paragraph", 	KillToEndOfParagraph},
/* new line stuff */
  {"newline-and-indent", 	InsertNewLineAndIndent},
  {"newline-and-backup", 	InsertNewLineAndBackup},
  {"newline", 			InsertNewLine},
/* Selection stuff */
  {"select-word", 		SelectWord},
  {"select-all", 		SelectAll},
  {"select-start", 		SelectStart},
  {"select-adjust", 		SelectAdjust},
  {"select-end", 		SelectEnd},
  {"select-save",		SelectSave},
  {"extend-start", 		ExtendStart},
  {"extend-adjust", 		ExtendAdjust},
  {"extend-end", 		ExtendEnd},
  {"insert-selection",		InsertSelection},
/* Miscellaneous */
  {"redraw-display", 		RedrawDisplay},
  {"insert-file", 		_XawTextInsertFile},
  {"search",		        _XawTextSearch},
  {"insert-char", 		InsertChar},
  {"insert-string",		InsertString},
  {"focus-in", 	 	        TextFocusIn},
  {"focus-out", 		TextFocusOut},
  {"display-caret",		DisplayCaret},
  {"multiply",		        Multiply},
  {"form-paragraph",            FormParagraph},
  {"transpose-characters",      TransposeCharacters},
  {"no-op",                     NoOp},
/* Action to bind special translations for text Dialogs. */
  {"InsertFileAction",          _XawTextInsertFileAction},
  {"DoSearchAction",            _XawTextDoSearchAction},
  {"DoReplaceAction",           _XawTextDoReplaceAction},
  {"SetField",                  _XawTextSetField},
  {"PopdownSearchAction",       _XawTextPopdownSearchAction},
};

Cardinal _XawTextActionsTableCount = XtNumber(_XawTextActionsTable);
