/* $XConsortium: Logo.c,v 1.27 91/10/30 13:35:31 converse Exp $ */

/*
Copyright 1988 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.
M.I.T. makes no representations about the suitability of
this software for any purpose.  It is provided "as is"
without express or implied warranty.
*/

#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/Xaw3d/XawInit.h>
#include <X11/Xaw3d/LogoP.h>
#include <X11/Xmu/Drawing.h>
#include <X11/extensions/shape.h>

static XtResource resources[] = {
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
        XtOffsetOf(LogoRec,logo.fgpixel), XtRString,
       (XtPointer) XtDefaultForeground},
    {XtNshapeWindow, XtCShapeWindow, XtRBoolean, sizeof (Boolean),
       XtOffsetOf(LogoRec,logo.shape_window), XtRImmediate, 
       (XtPointer) FALSE},
};

static void Initialize(), Realize(), Destroy(), Redisplay(), Resize();
static Boolean SetValues();

LogoClassRec logoClassRec = {
    { /* core fields */
    /* superclass		*/	(WidgetClass) &threeDClassRec,
    /* class_name		*/	"Logo",
    /* widget_size		*/	sizeof(LogoRec),
    /* class_initialize		*/	XawInitializeWidgetSet,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* resource_count		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
    },
    { /* simple fields */
    /* change_sensitive         */      XtInheritChangeSensitive
    },
    { /* threeD fields */
    /* shadowdraw               */      XtInheritXaw3dShadowDraw
    },
    { /* logo fields */
    /* ignore                   */      0
    }
};

WidgetClass logoWidgetClass = (WidgetClass) &logoClassRec;


/*****************************************************************************
 *									     *
 *			   private utility routines			     *
 *									     *
 *****************************************************************************/

static void create_gcs (w)
    LogoWidget w;
{
    XGCValues v;

    v.foreground = w->logo.fgpixel;
    w->logo.foreGC = XtGetGC ((Widget) w, GCForeground, &v);
    v.foreground = w->core.background_pixel;
    w->logo.backGC = XtGetGC ((Widget) w, GCForeground, &v);
}

static void check_shape (w)
    LogoWidget w;
{
    if (w->logo.shape_window) {
	int event_base, error_base;

	if (!XShapeQueryExtension (XtDisplay (w), &event_base, &error_base))
	  w->logo.shape_window = FALSE;
    }
}

/* ARGSUSED */
static void unset_shape (w)
    LogoWidget w;
{
    XSetWindowAttributes attr;
    unsigned long mask;
    Display *dpy = XtDisplay ((Widget) w);
    Window win = XtWindow ((Widget) w);

    if (w->core.background_pixmap != None && 
	w->core.background_pixmap != XtUnspecifiedPixmap) {
	attr.background_pixmap = w->core.background_pixmap;
	mask = CWBackPixmap;
    } else {
	attr.background_pixel = w->core.background_pixel;
	mask = CWBackPixel;
    }
    XChangeWindowAttributes (dpy, win, mask, &attr);
    XShapeCombineMask (dpy, win, ShapeBounding, 0, 0, None, ShapeSet);
    if (!w->logo.foreGC) create_gcs (w);
    w->logo.need_shaping = w->logo.shape_window;
}

static void set_shape (w)
    LogoWidget w;
{
    GC ones, zeros;
    Display *dpy = XtDisplay ((Widget) w);
    Window win = XtWindow ((Widget) w);
    unsigned int width = (unsigned int) w->core.width;
    unsigned int height = (unsigned int) w->core.height;
    Pixmap pm = XCreatePixmap (dpy, win, width, height, (unsigned int) 1);
    XGCValues v;

    v.foreground = (Pixel) 1;
    v.background = (Pixel) 0;
    ones = XCreateGC (dpy, pm, (GCForeground | GCBackground), &v);
    v.foreground = (Pixel) 0;
    v.background = (Pixel) 1;
    zeros = XCreateGC (dpy, pm, (GCForeground | GCBackground), &v);

    if (pm && ones && zeros) {
	int x = 0, y = 0;
	Widget parent;

	XmuDrawLogo (dpy, pm, ones, zeros, 0, 0, width, height);
	for (parent = (Widget) w; XtParent(parent);
	     parent = XtParent(parent)) {
	    x += parent->core.x + parent->core.border_width;
	    y += parent->core.y + parent->core.border_width;
	}
	XShapeCombineMask (dpy, XtWindow (parent), ShapeBounding,
			   x, y, pm, ShapeSet);
	w->logo.need_shaping = FALSE;
    } else {
	unset_shape (w);
    }
    if (ones) XFreeGC (dpy, ones);
    if (zeros) XFreeGC (dpy, zeros);
    if (pm) XFreePixmap (dpy, pm);
}


/*****************************************************************************
 *									     *
 *				 class methods				     *
 *									     *
 *****************************************************************************/

/* ARGSUSED */
static void Initialize (request, new, args, num_args)
    Widget request, new;
    ArgList args;
    Cardinal *num_args;
{
    LogoWidget w = (LogoWidget)new;

    if (w->core.width < 1) w->core.width = 100;
    if (w->core.height < 1) w->core.height = 100;

    w->logo.foreGC = (GC) NULL;
    w->logo.backGC = (GC) NULL;
    check_shape (w);
    w->logo.need_shaping = w->logo.shape_window;
}

static void Destroy (gw)
    Widget gw;
{
    LogoWidget w = (LogoWidget) gw;

    if (w->logo.foreGC) {
	XtReleaseGC (gw, w->logo.foreGC);
	w->logo.foreGC = (GC) NULL;
    }
    if (w->logo.backGC) {
	XtReleaseGC (gw, w->logo.backGC);
	w->logo.backGC = (GC) NULL;
    }
}

static void Realize (gw, valuemaskp, attr)
    Widget gw;
    XtValueMask *valuemaskp;
    XSetWindowAttributes *attr;
{
    LogoWidget w = (LogoWidget) gw;

    if (w->logo.shape_window) {
	attr->background_pixel = w->logo.fgpixel;  /* going to shape */
	*valuemaskp |= CWBackPixel;
    } else
      create_gcs (w);
    (*logoWidgetClass->core_class.superclass->core_class.realize)
	(gw, valuemaskp, attr);
}

static void Resize (gw)
    Widget gw;
{
    LogoWidget w = (LogoWidget) gw;

    if (w->logo.shape_window && XtIsRealized(gw)) set_shape (w);
}

/* ARGSUSED */
static void Redisplay (gw, event, region)
    Widget gw;
    XEvent *event;		/* unused */
    Region region;		/* unused */
{
    LogoWidget w = (LogoWidget) gw;
    LogoWidgetClass lwclass = (LogoWidgetClass) XtClass (gw);

    if (w->logo.shape_window) {
	if (w->logo.need_shaping) set_shape (w);  /* may change shape flag */
    } else {
	(*lwclass->threeD_class.shadowdraw) (gw, event, region, TRUE);
	XmuDrawLogo(XtDisplay(w), XtWindow(w), w->logo.foreGC, w->logo.backGC,
		  w->threeD.shadow_width, w->threeD.shadow_width, 
		  (unsigned int) w->core.width - 2 * w->threeD.shadow_width,
		  (unsigned int) w->core.height - 2 * w->threeD.shadow_width);
    }
}

/* ARGSUSED */
static Boolean SetValues (gcurrent, grequest, gnew, args, num_args)
    Widget gcurrent, grequest, gnew;
    ArgList args;
    Cardinal *num_args;
{
    LogoWidget current = (LogoWidget) gcurrent;
    LogoWidget new = (LogoWidget) gnew;
    Boolean redisplay = FALSE;

    if (new->logo.shape_window &&
	new->logo.shape_window != current->logo.shape_window)
	check_shape (new);	/* validate shape_window */

    if ((new->logo.fgpixel != current->logo.fgpixel) ||
	(new->core.background_pixel != current->core.background_pixel)) {
	Destroy (gnew);
	if (!new->logo.shape_window) create_gcs (new);
	redisplay = TRUE;
    }
   
    if (new->logo.shape_window != current->logo.shape_window) {
	if (new->logo.shape_window) {
	    Destroy (gnew);
	    if (XtIsRealized(gnew))
		set_shape (new);
	    else
		new->logo.need_shaping = True;
	    redisplay = FALSE;
	} else {
	    if (XtIsRealized(gnew))
		unset_shape (new);		/* creates new GCs */
	    redisplay = TRUE;
	}
    }

    return (redisplay);
}

