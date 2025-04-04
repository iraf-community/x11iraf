/****************************************************************************
 * NCSA Mosaic for the X Window System                                      *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 * mosaic@ncsa.uiuc.edu                                                     *
 *                                                                          *
 * Copyright (C) 1993, Board of Trustees of the University of Illinois      *
 *                                                                          *
 * NCSA Mosaic software, both binary and source (hereafter, Software) is    *
 * copyrighted by The Board of Trustees of the University of Illinois       *
 * (UI), and ownership remains with the UI.                                 *
 *                                                                          *
 * The UI grants you (hereafter, Licensee) a license to use the Software    *
 * for academic, research and internal business purposes only, without a    *
 * fee.  Licensee may distribute the binary and source code (if released)   *
 * to third parties provided that the copyright notice and this statement   *
 * appears on all copies and that no charge is associated with such         *
 * copies.                                                                  *
 *                                                                          *
 * Licensee may make derivative works.  However, if Licensee distributes    *
 * any derivative work based on or derived from the Software, then          *
 * Licensee will (1) notify NCSA regarding its distribution of the          *
 * derivative work, and (2) clearly notify users that such derivative       *
 * work is a modified version and not the original NCSA Mosaic              *
 * distributed by the UI.                                                   *
 *                                                                          *
 * Any Licensee wishing to make commercial use of the Software should       *
 * contact the UI, c/o NCSA, to negotiate an appropriate license for such   *
 * commercial use.  Commercial use includes (1) integration of all or       *
 * part of the source code into a product for sale or license by or on      *
 * behalf of Licensee to third parties, or (2) distribution of the binary   *
 * code or source code to third parties that need it to utilize a           *
 * commercial product sold or licensed by or on behalf of Licensee.         *
 *                                                                          *
 * UI MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE FOR   *
 * ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED          *
 * WARRANTY.  THE UI SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY THE    *
 * USERS OF THIS SOFTWARE.                                                  *
 *                                                                          *
 * By using or copying this Software, Licensee agrees to abide by the       *
 * copyright law and all other applicable laws of the U.S. including, but   *
 * not limited to, export control laws, and the terms of this license.      *
 * UI shall have the right to terminate this license immediately by         *
 * written notice upon Licensee's breach of, or non-compliance with, any    *
 * of its terms.  Licensee may be held legally responsible for any          *
 * copyright infringement that is caused or encouraged by Licensee's        *
 * failure to abide by the terms of this license.                           *
 *                                                                          *
 * Comments and questions are welcome and can be sent to                    *
 * mosaic-x@ncsa.uiuc.edu.                                                  *
 ****************************************************************************/
/* #ifdef MOTIF */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "inkstore.h"
#include "HTMLP.h"

#include <sys/types.h>
#include <netinet/in.h>

typedef struct stroke_rec {
	Boolean draw;
	int x, y;
	struct stroke_rec *next;
} Stroke;

typedef struct jot_rec {
	Widget w;
	int width, height;
	Pixmap pix;
	Boolean drawing;
	int last_x, last_y;
	int min_x, min_y;
	int max_x, max_y;
	int stroke_cnt;
	Stroke *strokes;
	Stroke *last_stroke;
	struct jot_rec *next;
} JotInfo;


static JotInfo *JotList = NULL;
static JotInfo *JotCurrent = NULL;



void
NewJot(Widget w, int width, int height)
{
	if (JotCurrent == NULL)
	{
		JotList = (JotInfo *)malloc(sizeof(JotInfo));
		JotCurrent = JotList;
		JotCurrent->w = w;
		JotCurrent->width = width;
		JotCurrent->height = height;
		JotCurrent->pix = 0;
		JotCurrent->drawing = False;
		JotCurrent->strokes = NULL;
		JotCurrent->last_stroke = NULL;
		JotCurrent->stroke_cnt = 0;
		JotCurrent->min_x = width;
		JotCurrent->max_x = 0;
		JotCurrent->min_y = height;
		JotCurrent->max_y = 0;
		JotCurrent->next = NULL;
	}
	else
	{
		JotCurrent->next = (JotInfo *)malloc(sizeof(JotInfo));
		JotCurrent = JotCurrent->next;
		JotCurrent->w = w;
		JotCurrent->width = width;
		JotCurrent->height = height;
		JotCurrent->pix = 0;
		JotCurrent->drawing = False;
		JotCurrent->strokes = NULL;
		JotCurrent->last_stroke = NULL;
		JotCurrent->stroke_cnt = 0;
		JotCurrent->min_x = width;
		JotCurrent->max_x = 0;
		JotCurrent->min_y = height;
		JotCurrent->max_y = 0;
		JotCurrent->next = NULL;
	}
}


JotInfo *
GetJot(Widget w)
{
	JotInfo *jptr;

	jptr = JotList;
	while (jptr != NULL)
	{
		if (jptr->w == w)
		{
			break;
		}
		jptr = jptr->next;
	}
	return(jptr);
}


void
FreeStrokes(Stroke *sptr)
{
	Stroke *tptr;

	while (sptr != NULL)
	{
		tptr = sptr;
		sptr = sptr->next;
		tptr->next = NULL;
		free((char *)tptr);
	}
}


void
ClearJot(HTMLWidget hw, Widget w, int width, int height)
{
	JotInfo *jptr;

	XClearArea(XtDisplay(w), XtWindow(w),
		0, 0, width, height, False);

	jptr = GetJot(w);
	if (jptr == NULL)
	{	
		return;
	}

	if (jptr->pix != 0)
	{
                XSetForeground(XtDisplay(w), hw->html.drawGC,
			hw->core.background_pixel);
		XFillRectangle(XtDisplay(w), jptr->pix,
			hw->html.drawGC,
			0, 0, jptr->width, jptr->height);
	}
	
	FreeStrokes(jptr->strokes);
	jptr->strokes = NULL;
	jptr->last_stroke = NULL;
	jptr->stroke_cnt = 0;
	jptr->drawing = False;
	jptr->min_x = width;
	jptr->max_x = 0;
	jptr->min_y = height;
	jptr->max_y = 0;
}


void
AddStroke(JotInfo *jptr, Stroke *sptr, Boolean drawing)
{
	if (jptr->strokes == NULL)
	{
		jptr->strokes = sptr;
		jptr->last_stroke = jptr->strokes;
		jptr->last_stroke->next = NULL;
	}
	else
	{
		jptr->last_stroke->next = sptr;
		jptr->last_stroke = jptr->last_stroke->next;
		jptr->last_stroke->next = NULL;
	}
	jptr->last_x = sptr->x;
	jptr->last_y = sptr->y;
	jptr->drawing = drawing;
	if (sptr->x < jptr->min_x)
	{
		jptr->min_x = sptr->x;
	}
	if (sptr->x > jptr->max_x)
	{
		jptr->max_x = sptr->x;
	}
	if (sptr->y < jptr->min_y)
	{
		jptr->min_y = sptr->y;
	}
	if (sptr->y > jptr->max_y)
	{
		jptr->max_y = sptr->y;
	}
	jptr->stroke_cnt++;
}


void
EVJotExpose(Widget w, XtPointer data, XEvent *event)
{
	XExposeEvent *ExEvent = (XExposeEvent *)event;
	HTMLWidget hw = (HTMLWidget)data;
	JotInfo *jptr;

	jptr = GetJot(w);
	if (jptr == NULL)
	{	
		return;
	}

	if (jptr->pix == 0)
	{
		jptr->pix = XCreatePixmap(XtDisplay(w), XtWindow(w),
			jptr->width, jptr->height,
			XDefaultDepth(XtDisplay(w), XDefaultScreen(XtDisplay(w))));
		if (jptr->pix == 0)
		{
			return;
		}
                XSetForeground(XtDisplay(w), hw->html.drawGC,
			hw->core.background_pixel);
		XFillRectangle(XtDisplay(w), jptr->pix,
			hw->html.drawGC,
			0, 0, jptr->width, jptr->height);
	}

	XCopyArea(XtDisplay(w), jptr->pix, XtWindow(w),
                        hw->html.drawGC,
                        ExEvent->x, ExEvent->y,
                        ExEvent->width, ExEvent->height,
                        ExEvent->x, ExEvent->y);
}


void
EVJotPress(Widget w, XtPointer data, XEvent *event)
{
	XButtonPressedEvent *BuEvent = (XButtonPressedEvent *)event;
	HTMLWidget hw = (HTMLWidget)data;
	JotInfo *jptr;
	Stroke *sptr;

	jptr = GetJot(w);
	if (jptr == NULL)
	{	
		return;
	}

	sptr = (Stroke *)malloc(sizeof(Stroke));
	if (sptr == NULL)
	{
		return;
	}
	sptr->x = BuEvent->x;
	sptr->y = BuEvent->y;
	sptr->draw = False;
	sptr->next = NULL;

/*
 * Without motif we use our own foreground resource instead of
 * using the manager's
 */
	XSetForeground(XtDisplay(w), hw->html.drawGC,
#ifdef MOTIF
		hw->manager.foreground);
#else
		hw->html.foreground);
#endif /* MOTIF */
	XDrawPoint(XtDisplay(w), XtWindow(w),
			hw->html.drawGC, sptr->x, sptr->y);
	if (jptr->pix != 0)
	{
		XDrawPoint(XtDisplay(w), jptr->pix,
				hw->html.drawGC, sptr->x, sptr->y);
	}

	AddStroke(jptr, sptr, True);
}


void
EVJotMove(Widget w, XtPointer data, XEvent *event)
{
	XPointerMovedEvent *MoEvent = (XPointerMovedEvent *)event;
	HTMLWidget hw = (HTMLWidget)data;
	JotInfo *jptr;
	Stroke *sptr;

	jptr = GetJot(w);
	if (jptr == NULL)
	{	
		return;
	}

	if (jptr->drawing == False)
	{	
		return;
	}

	sptr = (Stroke *)malloc(sizeof(Stroke));
	if (sptr == NULL)
	{
		return;
	}
	sptr->x = MoEvent->x;
	sptr->y = MoEvent->y;
	sptr->draw = True;
	sptr->next = NULL;

/*
 * Without motif we use our own foreground resource instead of
 * using the manager's
 */
	XSetForeground(XtDisplay(w), hw->html.drawGC,
#ifdef MOTIF
		hw->manager.foreground);
#else
		hw->html.foreground);
#endif /* MOTIF */
	XDrawLine(XtDisplay(w), XtWindow(w), hw->html.drawGC,
		jptr->last_x, jptr->last_y, sptr->x, sptr->y);
	if (jptr->pix != 0)
	{
		XDrawLine(XtDisplay(w), jptr->pix, hw->html.drawGC,
			jptr->last_x, jptr->last_y, sptr->x, sptr->y);
	}

	AddStroke(jptr, sptr, True);
}


void
EVJotRelease(Widget w, XtPointer data, XEvent *event)
{
	XButtonReleasedEvent *BuEvent = (XButtonReleasedEvent *)event;
	HTMLWidget hw = (HTMLWidget)data;
	JotInfo *jptr;
	Stroke *sptr;

	jptr = GetJot(w);
	if (jptr == NULL)
	{	
		return;
	}

	if (jptr->drawing == False)
	{	
		return;
	}

	sptr = (Stroke *)malloc(sizeof(Stroke));
	if (sptr == NULL)
	{
		return;
	}
	sptr->x = BuEvent->x;
	sptr->y = BuEvent->y;
	sptr->draw = True;
	sptr->next = NULL;

/*
 * Without motif we use our own foreground resource instead of
 * using the manager's
 */
	XSetForeground(XtDisplay(w), hw->html.drawGC,
#ifdef MOTIF
		hw->manager.foreground);
#else
		hw->html.foreground);
#endif /* MOTIF */
	XDrawLine(XtDisplay(w), XtWindow(w), hw->html.drawGC,
		jptr->last_x, jptr->last_y, sptr->x, sptr->y);
	if (jptr->pix != 0)
	{
		XDrawLine(XtDisplay(w), jptr->pix, hw->html.drawGC,
			jptr->last_x, jptr->last_y, sptr->x, sptr->y);
	}

	AddStroke(jptr, sptr, True);
}


char *
EJB_JOTfromJot(Widget w)
{
	int i, cnt;
	int dlen, total;
	unsigned long val;
	unsigned char uchar;
	JotInfo *jptr;
	Stroke *sptr;
	unsigned char *data;
	unsigned char *dptr;
	unsigned char *buffer;
	unsigned char *bptr;

	jptr = GetJot(w);
	if (jptr == NULL)
	{	
		return(NULL);
	}

	dlen = (2 * sizeof(unsigned long) + sizeof(char)) * jptr->stroke_cnt;
	data = (unsigned char *)malloc(dlen);

	cnt = 0;
	sptr = jptr->strokes;
	dptr = data;
	while ((sptr != NULL)&&(cnt < jptr->stroke_cnt))
	{
		val = htonl((unsigned long)sptr->x);
		bcopy((char *)&val, (char *)dptr, sizeof(unsigned long));
		dptr = dptr + sizeof(unsigned long);

		val = htonl((unsigned long)sptr->y);
		bcopy((char *)&val, (char *)dptr, sizeof(unsigned long));
		dptr = dptr + sizeof(unsigned long);

		if (sptr->draw == False)
		{
			uchar = 0;
		}
		else
		{
			uchar = 1;
		}
		*dptr++ = uchar;

		cnt++;
		sptr = sptr->next;
	}
	for (i=cnt; i<jptr->stroke_cnt; i++)
	{
		val = 0;
		bcopy((char *)&val, (char *)dptr, sizeof(unsigned long));
		dptr = dptr + sizeof(unsigned long);
		val = 0;
		bcopy((char *)&val, (char *)dptr, sizeof(unsigned long));
		dptr = dptr + sizeof(unsigned long);
		uchar = 0;
		*dptr++ = uchar;
	}
fprintf(stderr, "Packaging up %d points\n", jptr->stroke_cnt);

	cnt = 0;
	dptr = data;
	for (i=0; i<dlen; i++)
	{
		if ((*dptr == 0)||(*dptr == 1))
		{
			cnt++;
		}
		dptr++;
	}

	total = dlen + cnt + 1;
	buffer = (unsigned char *)malloc(total);
	bptr = buffer;
	dptr = data;
	for (i=0; i<dlen; i++)
	{
		if (*dptr == 0)
		{
			*bptr++ = (unsigned char)'o';
			*bptr++ = (unsigned char)'O';
		}
		else if (*dptr == 'o')
		{
			*bptr++ = (unsigned char)'o';
			*bptr++ = (unsigned char)'o';
		}
		else
		{
			*bptr++ = *dptr;
		}
		dptr++;
	}
	*bptr = (unsigned char)'\0';
	free((char *)data);

	return((char *)buffer);
}



typedef struct my_INK_POINT {
	XY32 position;
	INK_BUTTONS buttons;
} MY_INK_POINT;


unsigned char *
JOTfromJot(Widget w, int *buffer_len)
{
	int i, cnt;
	int dlen, total;
	JotInfo *jptr;
	Stroke *sptr;
	MY_INK_POINT *dataArray;
	INK_BUNDLE_RECORD *iptr;
	INK_PENDATA_RECORD *pptr;
	INK_END_RECORD *eptr;
	unsigned char *buffer;

	jptr = GetJot(w);
	if (jptr == NULL)
	{	
		return(NULL);
	}

	dlen = sizeof(MY_INK_POINT) * jptr->stroke_cnt;
                
	dataArray = (MY_INK_POINT *)malloc(dlen);
	cnt = 0;
	sptr = jptr->strokes;
	while ((sptr != NULL)&&(cnt < jptr->stroke_cnt))
	{
		dataArray[cnt].position.x = sptr->x;
		dataArray[cnt].position.y = sptr->y;
		dataArray[cnt].buttons = inkPointDefaultButtons;
		dataArray[cnt].buttons |= flag0;
		if ((sptr->next != NULL)&&(sptr->next->draw == False))
		{
		}
		else
		{
			dataArray[cnt].buttons |= flag1;
		}
		cnt++;
		sptr = sptr->next;
	}
	for (i=cnt; i<jptr->stroke_cnt; i++)
	{
		dataArray[i].position.x = 0;
		dataArray[i].position.y = 0;
		dataArray[i].buttons = inkPointDefaultButtons;
	}

	iptr = (INK_BUNDLE_RECORD *)malloc(inkRecordBundleSize);
	iptr->header.recordType = inkRecordBundle;
	iptr->header.recordLength = inkRecordBundleSize;
	iptr->version = inkPointDefaultVersion;
	iptr->compactionType = inkNoCompression;
	iptr->flags = (inkPointDefaultBundleFlags | inkButtonDataPresent);
	iptr->penUnitsPerX = inkPointDefaultPenUnitsPerX;
	iptr->penUnitsPerY = inkPointDefaultPenUnitsPerY;

	pptr = (INK_PENDATA_RECORD *)malloc(inkRecordPenDataSize(dlen));
	pptr->header.recordType = inkRecordPenData;
	pptr->header.recordLength = inkRecordPenDataSize(dlen);
	pptr->bounds.origin.x = jptr->min_x; 
	pptr->bounds.origin.y = jptr->min_y;
	pptr->bounds.size.w = jptr->max_x - jptr->min_x + 1;
	pptr->bounds.size.h = jptr->max_y - jptr->min_y + 1;
	bcopy((char *)dataArray, (char *)pptr->inkData, dlen);
	free((char *)dataArray);
/*
	pptr->inkData = dataArray;
*/

	eptr = (INK_END_RECORD *)malloc(inkRecordEndSize);
	eptr->header.recordType = inkRecordEnd;

	total = inkRecordBundleSize + inkRecordPenDataSize(dlen) +
		inkRecordEndSize;
	buffer = (unsigned char *)malloc(total);
	bcopy((char *)iptr, buffer, inkRecordBundleSize);
	bcopy((char *)pptr, (char *)(buffer + inkRecordBundleSize),
		inkRecordPenDataSize(dlen));
	bcopy((char *)eptr, (char *)(buffer + inkRecordBundleSize +
		inkRecordPenDataSize(dlen)), inkRecordEndSize);
	free((char *)iptr);
	free((char *)pptr);
	free((char *)eptr);
	*buffer_len = total;
	return(buffer);
}
/* #endif /* MOTIF */
