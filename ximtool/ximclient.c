#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Tcl/tcl.h>
#include <Obm.h>
#include <ObmW/Gterm.h>
#include "ximtool.h"

/*
 * XIMCLIENT.C -- The Ximtool "client" object.  This code implements an OBM
 * client and responds to messages sent to the client object by the GUI code
 * executing under the object manager.
 *
 *	     xim_clientOpen (xim)
 *	    xim_clientClose (xim)
 *	  xim_clientExecute (xim, key, cmd)
 *
 * The clientExecute callback is called by the GUI code in the object manager
 * to execute ximtool client commands.
 *
 * Client commands:
 *
 *		   setFrame frameno
 *       frameno = getFrame [raster]
 *	 raster = getRaster [frameno]
 *	  frame = getSource [raster [sx sy snx sny]]
 *		  nextFrame
 *		  prevFrame
 *		matchFrames [frames [reference_frame]]
 *	     registerFrames [frames [reference_frame]]
 *		   fitFrame
 *		 clearFrame [frame]
 *
 *		  setOption option value [args]
 *		setColormap colormap
 *	     windowColormap offset scale
 *	          windowRGB color offset scale save_flag
 *		       zoom [mag | xmag ymag [ xcen ycen ]]
 *		    zoomAbs [mag | xmag ymag [ xcen ycen ]]
 *			pan xcen ycen
 *		       flip axis [axis ...]
 *
 *           setPrintOption option value [args]
 *                    print [x0 y0 nx ny]
 *            setSaveOption option value [args]
 *                     save [x0 y0 nx ny]
 *            setLoadOption option value [args]
 *                     load
 *                     help
 *
 *       wcsstr = encodewcs sx sy sz
 *	       retCursorVal sx sy [frame [wcs [key [strval]]]]
 *
 *		 initialize
 *		      Reset
 *		       Quit
 */

typedef struct {
	XimDataPtr xim;
	Tcl_Interp *tcl;
	Tcl_Interp *server;
} XimClient, *XimClientPtr;

static int initialize(), Reset(), Quit();
static int setColormap(), windowColormap(), zoom(), pan(), getSource();
static int setFrame(), getFrame(), getRaster(), nextFrame(), prevFrame();
static int fitFrame(), matchFrames(), registerFrames(), retCursorVal();
static int encodewcs(), flip(), clearFrame(), setOption();
static int setPrintOption(), setSaveOption(), setLoadOption();
static int print(), save(), load(),    help(), windowRGB();
extern double atof();


/* xim_clientOpen -- Initialize the ximtool client code.
 */
void
xim_clientOpen (xim)
XimDataPtr xim;
{
	register XimClientPtr xc;
	register Tcl_Interp *tcl;

	xc = (XimClientPtr) XtCalloc (1, sizeof(XimClient));
	xim->clientPrivate = (int *)xc;

	xc->xim = xim;
	xc->tcl = tcl = Tcl_CreateInterp();
	ObmAddCallback (xim->obm, OBMCB_clientOutput|OBMCB_preserve,
	    xim_clientExecute, (XtPointer)xc);

	Tcl_CreateCommand (tcl,
	    "Quit", Quit, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "Reset", Reset, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "initialize", initialize, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "setFrame", setFrame, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "getFrame", getFrame, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "getRaster", getRaster, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "getSource", getSource, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "nextFrame", nextFrame, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "prevFrame", prevFrame, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "fitFrame", fitFrame, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "matchFrames", matchFrames, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "registerFrames", registerFrames, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "clearFrame", clearFrame, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "setOption", setOption, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "setColormap", setColormap, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "windowColormap", windowColormap, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "zoom", zoom, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "zoomAbs", zoom, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "pan", pan, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "flip", flip, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "retCursorVal", retCursorVal, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "encodewcs", encodewcs, (ClientData)xc, NULL);

	Tcl_CreateCommand (tcl,
	    "setPrintOption", setPrintOption, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "print", print, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "setSaveOption", setSaveOption, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "save", save, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "setLoadOption", setLoadOption, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "load", load, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "help", help, (ClientData)xc, NULL);
	Tcl_CreateCommand (tcl,
	    "windowRGB", windowRGB, (ClientData)xc, NULL);
}


/* xim_clientClose -- Shutdown the ximtool client code.
 */
void
xim_clientClose (xim)
XimDataPtr xim;
{
	register XimClientPtr xc = (XimClientPtr) xim->clientPrivate;
	Tcl_DeleteInterp (xc->tcl);
}


/* xim_clientExecute -- Called by the GUI code to send a message to the
 * "client", which from the object manager's point of view is ximtool itself.
 */
xim_clientExecute (xc, tcl, key, command)
register XimClientPtr xc;
Tcl_Interp *tcl;		/* caller's Tcl */
int key;			/* notused */
char *command;
{
	register XimDataPtr xim = xc->xim;

	xc->server = tcl;
        Tcl_Eval (xc->tcl, command);

	return (0);
}


/*
 * XIMTOOL CLIENT commands.
 * ----------------------------
 */

/* Quit -- Exit ximtool.
 *
 * Usage:	Quit
 */
static int 
Quit (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	xim_shutdown (xim);
}


/* Reset -- Reset ximtool.
 *
 * Usage:	Reset
 *
 * Reset does a full power-on reset of ximtool.
 */
static int 
Reset (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	xim_initialize (xim, xim->fb_configno, xim->nframes, 1);
}


/* initialize -- Reinitialize ximtool.
 *
 * Usage:	initialize
 *
 * Initialize does a partial reinitialization of ximtool, preserving the
 * current frame buffers and view.
 */
static int 
initialize (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	xim_initialize (xim, xim->fb_configno, xim->nframes, 0);
}


/* setFrame -- Set the frame to be displayed.
 *
 * Usage:	setFrame <frameno>
 */
static int 
setFrame (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	if (argc == 2)
	    xim_setFrame (xc->xim, atoi(argv[1]));
	return (TCL_OK);
}


/* getFrame -- Get the frame number.
 *
 * Usage:	frameno = getFrame [raster]
 *
 * This routine has two forms.  When called with no argument getFrame returns
 * the current display frame.  When called with a raster number getFrame
 * returns the frame number with which the raster is associated.
 */
static int 
getFrame (xc, tcl, argc, argv)
XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb;
	char frameno[SZ_NAME];
	register int i;
	int raster;

	if (argc == 1)
	    sprintf (frameno, "%d", xc->xim->display_frame);
	else {
	    raster = atoi (argv[1]);
	    strcpy (frameno, "0");

	    for (i=1;  i <= xim->nframes;  i++) {
		fb = &xim->frames[i-1];
		if (fb->raster == raster || fb->zoomras == raster) {
		    sprintf (frameno, "%d", fb->frameno);
		    break;
		}
	    }
	}

	Tcl_SetResult (xc->server, frameno, TCL_VOLATILE);
	return (TCL_OK);
}


/* getRaster -- Get the image raster number of the given frame.
 *
 * Usage:	raster = getRaster [frameno]
 *
 * This routine has two forms.  When called with no argument getRaster returns
 * the raster number of the current display frame.  When called with a frame
 * number getRaster returns the raster number of the given frame.
 */
static int 
getRaster (xc, tcl, argc, argv)
XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb;
	char buf[SZ_NAME];
	register int i;
	int frameno;

	if (argc == 1)
	    sprintf (buf, "%d", xim->df_p->raster);
	else {
	    frameno = atoi (argv[1]);
	    strcpy (buf, "0");

	    for (i=1;  i < xim->nframes;  i++) {
		fb = &xim->frames[i-1];
		if (fb->frameno == frameno) {
		    sprintf (buf, "%d", fb->raster);
		    break;
		}
	    }
	}

	Tcl_SetResult (xc->server, buf, TCL_VOLATILE);
	return (TCL_OK);
}


/* getSource -- Get the source of the currently displayed image.
 *
 * Usage:	frame = getSource [raster [sx sy snx sny]]
 *
 * getSource returns the frame number of the currently displayed frame as
 * the function value, and the raster number and source rect within that
 * raster are returned as function arguments.
 */
static int 
getSource (xc, tcl, argc, argv)
XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register int i;
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	int src, st, sx, sy, snx, sny;
	int dst, dt, dx, dy, dnx, dny;
	char buf[SZ_NAME];
	int frameno, rop;

	GtGetMapping (xim->gt, fb->zoommap, &rop,
	    &src,&st,&sx,&sy,&snx,&sny, &dst,&dt,&dx,&dy,&dnx,&dny);

	if (argc > 1) {
	    sprintf (buf, "%d", fb->raster);
		Tcl_SetVar (xc->server, argv[1], buf, 0);
	}
	if (argc > 2) {
	    sprintf (buf, "%d", sx);
		Tcl_SetVar (xc->server, argv[2], buf, 0);
	    sprintf (buf, "%d", sy);
		Tcl_SetVar (xc->server, argv[3], buf, 0);
	    sprintf (buf, "%d", snx);
		Tcl_SetVar (xc->server, argv[4], buf, 0);
	    sprintf (buf, "%d", sny);
		Tcl_SetVar (xc->server, argv[5], buf, 0);
	}

	sprintf (buf, "%d", fb->frameno);
	Tcl_SetResult (xc->server, buf, TCL_VOLATILE);

	return (TCL_OK);
}


/* nextFrame -- Display the next frame in sequence.
 *
 * Usage:	nextFrame
 */
static int 
nextFrame (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	int frame;

	if (xim->display_frame < xim->nframes)
	    frame = xim->display_frame + 1;
	else
	    frame = 1;

	xim_setFrame (xc->xim, frame);
	return (TCL_OK);
}


/* prevFrame -- Display the previous frame in sequence.
 *
 * Usage:	prevFrame
 */
static int 
prevFrame (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	int frame;

	if (xim->display_frame > 1)
	    frame = xim->display_frame - 1;
	else
	    frame = xim->nframes;

	xim_setFrame (xc->xim, frame);
	return (TCL_OK);
}


/* matchFrames -- Set the enhancement of the listed frames to match that of
 * the given reference frame.  If no reference frame is given the current
 * display frame is used.  If no frames are listed all frames are matched
 * to the current display frame.
 *
 * Usage:	matchFrames [frames [reference_frame]]
 */
static int 
matchFrames (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	int *frames, frame_list[32], reference_frame;
	int nitems, i;
	char **items;

	/* Get reference frame. */
	if (argc > 2)
	    reference_frame = atoi (argv[2]);
	else
	    reference_frame = xim->display_frame;

	/* Get frame list. */
	if (argc > 1) {
	    if (Tcl_SplitList (tcl, argv[1], &nitems, &items) != TCL_OK)
		goto nolist;
	    for (i=0, frames=frame_list;  i < nitems;  i++)
		frames[i] = atoi (items[i]);
	    frames[i] = (int) NULL;
	    free ((char *)items);
	} else
nolist:	    frames = NULL;

	xim_matchFrames (xc->xim, frames, reference_frame);
	return (TCL_OK);
}


/* registerFrames -- Register the listed frames with the given reference
 * frame.  If no reference frame is given the current display frame is used.
 * If no frames are listed all frames are registered with the current
 * display frame.
 *
 * Usage:	registerFrames [frames [reference_frame]]
 */
static int 
registerFrames (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	int *frames, frame_list[32], reference_frame;
	int nitems, i;
	char **items;

	/* Get reference frame. */
	if (argc > 2)
	    reference_frame = atoi (argv[2]);
	else
	    reference_frame = xim->display_frame;

	/* Get frame list. */
	if (argc > 1) {
	    if (Tcl_SplitList (tcl, argv[1], &nitems, &items) != TCL_OK)
		goto nolist;
	    for (i=0, frames=frame_list;  i < nitems;  i++)
		frames[i] = atoi (items[i]);
	    frames[i] = (int) NULL;
	    free ((char *)items);
	} else
nolist:	    frames = NULL;

	xim_registerFrames (xc->xim, frames, reference_frame);
	return (TCL_OK);
}


/* clearFrame -- Clear the given frame, or the current frame in no frame
 * number is given.
 *
 * Usage:	clearFrame [frame]
 */
static int 
clearFrame (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	int frame;

	if (argc > 1)
	    frame = atoi (argv[1]);
	else
	    frame = xim->display_frame;

	xim_eraseFrame (xc->xim, frame);
	return (TCL_OK);
}


/* fitFrame -- Attempt to make the display window the same size as the frame
 * buffer.
 *
 * Usage:	fitFrame
 */
static int 
fitFrame (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;

	xim_fitFrame (xc->xim);
	return (TCL_OK);
}


/* setOption -- Set an ximtool client option.
 *
 * Usage:	setOption option value [args]
 *
 * Options:
 *	autoscale	true|false
 *	antialias	true|false [type]
 *	tileFrames	true|false [frames]
 */
static int 
setOption (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	char *option, *strval;
	char buf[SZ_LINE];
	int ch, value;

	if (argc < 3)
	    return (TCL_ERROR);
	else {
	    option = argv[1];
	    strval = argv[2];

	    ch = strval[0];
	    if (isdigit (ch))
		value = atoi (strval);
	    else if (ch == 'T' || ch == 't')
		value = 1;
	    else if (ch == 'F' || ch == 'f')
		value = 0;
	}

	if (strcmp (option, "autoscale") == 0) {
	    if (xim->autoscale != value) {
		xim->autoscale = value;
		xim_resize (xim, xim->gt);
		sprintf (buf, "%s", value ? "True" : "False");
		xim_message (xim, "autoscale", buf);
	    }
	} else if (strcmp (option, "antialias") == 0) {
	    if (xim->antialias != value) {
		xim->antialias = value;
		if (value) {
		    if (argc > 3)
			xim->rop = xim_getAntialias (xim, argv[3]);
		    else
			xim->rop = xim_getAntialias (xim, xim->antialiasType);
		} else
		    xim->rop = 0;
		xim_setRop (xim, fb, xim->rop);
		sprintf (buf, "%s", value ? "True" : "False");
		xim_message (xim, "antialias", buf);
	    }
	} else if (strcmp (option, "tileFrames") == 0) {
	    if (xim->tileFrames != value) {
		int frame_list=0, i;

		/* Get list of frames to be tiled. */
		if (argc > 3) {
		    int nitems;
		    char **items;

		    if (Tcl_SplitList (tcl, argv[3], &nitems, &items) != TCL_OK)
			goto nolist;
		    for (i=0;  i < nitems;  i++)
			frame_list |= (1 << (atoi(items[i]) - 1));
		    free ((char *)items);
		} else {
nolist:		    for (i=0;  i < xim->nframes;  i++)
			frame_list |= (1 << i);
		}

		/* Set or clear tile frame mode. */
		xim_tileFrames (xim, value ? frame_list : 0);
	    }
	}

	return (TCL_OK);
}


/* setColormap -- Set the colormap for the current display frame.
 *
 * Usage:	setColormap <colormap>
 *
 * The colormap may be specified either by number or by name.
 */
static int 
setColormap (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	unsigned short m_red[MAX_COLORS];
	unsigned short m_green[MAX_COLORS];
	unsigned short m_blue[MAX_COLORS];
	char buf[SZ_LINE];
	ColorMapPtr cm;
	int i;

	if (argc == 2) {
	    if (isdigit (*argv[1]))
		i = atoi(argv[1]);
	    else {
		for (i=1;  i <= ncolormaps;  i++)
		    if (strcmp (colormaps[i-1].name, argv[1]) == 0)
			break;
	    }

	    if (i >= 1 && i <= ncolormaps) {
		cm = &colormaps[i-1];
		if (strncmp (cm->name, "Random", 6) == 0) {
		    xim_setColormap (cm->name, NULL,
			m_red, m_green, m_blue, xim->ncolors);
		    GtWriteColormap (xim->gt, cm->mapno,
			first_color, xim->ncolors, m_red, m_green, m_blue);
		}

		fb->colormap = i;
		GtLoadColormap (xim->gt, cm->mapno, fb->offset, fb->scale);
		xim_enhancement (xim, fb);
	    }
	}

	return (TCL_OK);
}


/* windowColormap -- Set the colormap for the current display frame.
 *
 * Usage:	windowColormap <offset> <scale>
 */
static int 
windowColormap (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	register ColorMapPtr cm;
	char buf[SZ_LINE];

	if (argc > 1) {
	    cm = &colormaps[fb->colormap-1];
	    fb->offset = atof(argv[1]);
	    fb->scale = (argc > 2) ? (float)atof(argv[2]) : fb->scale;
	    GtLoadColormap (xim->gt, cm->mapno, fb->offset, fb->scale);
	    xim_enhancement (xim, fb);
	}

	return (TCL_OK);
}


/* zoom -- Set the zoom factors for the current frame to the given values.
 * A zoom factor > 1 enlarges the image, < 1 shrinks the image, 1.0 maps
 * one source pixel to one destination pixel.
 *
 * Usage:	zoom <xymag>				1 argument
 *		zoom <xmag> <ymag>			2 arguments
 *		zoom <xmag> <ymag> <xcen> <ycen>	4 arguments
 *
 * When called as "zoom" the magnification is relative to the fixed scaling,
 * if any, used to scale the frame to fit the display window at mag=1.0.
 * When called as zoomAbs" the magnification given is the actual scale factor
 * used to map raster pixels to display pixels.
 */
static int 
zoom (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	float xmag, ymag;
	float xcen, ycen;
	Boolean absolute;

	xmag = fb->xmag;
	ymag = fb->ymag;
	xcen = fb->xcen;
	ycen = fb->ycen;

	switch (argc) {
	case 5:
	    xcen = atof (argv[3]);
	    ycen = atof (argv[4]);
	    /* fall through */
	case 3:
	    xmag = atof (argv[1]);
	    ymag = atof (argv[2]);
	    break;
	case 2:
	    xmag = ymag = atof (argv[1]);
	    break;
	}

	absolute = (strcmp (argv[0], "zoomAbs") == 0);
	xim_setZoom (xim, fb, fb->frameno, fb->zoommap,
	    fb->raster, fb->zoomras, xcen, ycen, xmag, ymag, absolute);

	return (TCL_OK);
}


/* pan -- Pan the current frame, i.e., change the view center.
 *
 * Usage:	pan <xcen> <ycen>
 */
static int 
pan (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	Boolean absolute = False;
	float xmag, ymag;
	float xcen, ycen;
	double atof();

	xmag = fb->xmag;
	ymag = fb->ymag;

	if (argc == 3) {
	    xcen = atof (argv[1]);
	    ycen = atof (argv[2]);

	    xim_setZoom (xim, fb, fb->frameno, fb->zoommap,
		fb->raster, fb->zoomras, xcen, ycen, xmag, ymag, absolute);
	}

	return (TCL_OK);
}


/* flip -- Flip the current display frame in the indicated axis or axes.
 *
 * Usage:	flip [axis [axis ...]]
 */
static int 
flip (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	int flip_x = 0, flip_y = 0;
	int ch, i;

	for (i=1;  i < argc;  i++) {
	    ch = argv[i][0];
	    if (ch == 'x' || ch == 'X')
		flip_x = !flip_x;
	    else if (ch == 'y' || ch == 'Y')
		flip_y = !flip_y;
	}

	xim_setFlip (xim, fb, flip_x, flip_y);
	return (TCL_OK);
}


/* retCursorVal -- Return a cursor value to the ximtool client process.  This
 * should be executed by the GUI to terminate a cursor read.
 *
 * Usage:	retCursorVal sx sy [frame [wcs [key [strval]]]]
 */
static int 
retCursorVal (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	int frame, wcs, key;
	float sx, sy;
	char *s, *strval;

	if (argc < 3)
	    return (TCL_ERROR);

	sx = atof (argv[1]);
	sy = atof (argv[2]);
	frame = (argc > 3) ? atoi (argv[3]) : xim->display_frame;
	wcs = (argc > 4) ? atoi (argv[4]) : 1;

	if (argc > 5) {
	    s = argv[5];
	    if (s[0] == '^')
		key = s[1] - 'A' + 1;
	    else
		key = s[0];
	} else
	    key = 0;

	strval = (argc > 6) ? argv[6] : "";

	xim_retCursorVal (xim, sx, sy, frame, wcs, key, strval);

	return (TCL_OK);
}


/* encodewcs -- Convert raw screen coordinates x,y,z (z=pixel value) to
 * world coordinates using the WCS passed to ximtool by the client application
 * when the frame was loaded.  The encoded description of the current position
 * and pixel value is returned to the GUI as a string value.
 *
 * Usage:	string = encodewcs sx sy sz
 */
static int 
encodewcs (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	char buf[SZ_LINE];
	float sx, sy;
	int sz;

	if (argc < 3)
	    return (TCL_ERROR);

	sx = atof (argv[1]);
	sy = atof (argv[2]);
	sz = (argc > 3) ? atoi (argv[3]) : 0;

	xim_encodewcs (xc->xim, sx, sy, sz, buf);
	Tcl_SetResult (xc->server, buf, TCL_VOLATILE);

	return (TCL_OK);
}


/* setPrintOption -- Set an ximtool client hardcopy option.
 *
 * Usage:	setPrintOption option value [args]
 *
 * Options:
 *	autoscale	true|false
 *	autorotate	true|false
 *	maxaspect	true|false
 *	annotate	true|false
 *	compress	true|false
 *
 * 	orientation	portrait|landscape
 *	papersize	letter|legal|A4
 *	imscale		value
 *
 *	colortype	gray|pseudo|rgb
 *	printername	strval
 *	devicetype	printer|file
 *
 *	printcmd	command
 *	printfile	filename
 *
 *	corners		llx lly urx ury
 */
static int 
setPrintOption (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	register PSImagePtr psim = xim->psim;
	register PrintCfgPtr pcp = xim->pcp;
	register PrinterPtr prp;
	register int i;
	int	 llx, lly, urx, ury;
	char *option, strval[SZ_LINE];
	char buf[SZ_LINE];
	int pnum, ch, value, psflags = psim->page.flags;

	if (argc < 3)
	    return (TCL_ERROR);
	else {
	    option = argv[1];
	    strcpy (strval, argv[2]);
	    for (i=3; i < argc; i++) {
		strcat (strval, " ");
		strcat (strval, argv[i]);
	    }

	    ch = strval[0];
	    if (isdigit (ch))
		value = atoi (strval);
	    else if (ch == 'T' || ch == 't')
		value = 1;
	    else if (ch == 'F' || ch == 'f')
		value = 0;
	}

	if (strcmp (option, "autoscale") == 0) { 		/* AUTOSCALE */
		psflags = value ? psflags | EPS_AUTOSCALE :
				  psflags & ~EPS_AUTOSCALE ;
		psim->page.flags = psflags;
		sprintf (buf, "%s %s", option, value ? "True" : "False");
		xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "autorotate") == 0) {	/* AUTOROTATE */
		psflags = value ? psflags | EPS_AUTOROTATE :
				  psflags & ~EPS_AUTOROTATE ;
		psim->page.flags = psflags;
		sprintf (buf, "%s %s", option, value ? "True" : "False");
		xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "maxaspect") == 0) {		/* MAXASPECT */
		psflags = value ? psflags | EPS_MAXASPECT :
				  psflags & ~EPS_MAXASPECT ;
		psim->page.flags = psflags;
		sprintf (buf, "%s %s", option, value ? "True" : "False");
		xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "annotate") == 0) {		/* ANNOTATE */
	    if (value) {
		if (!psim->label)
		    psim->label = (char *) calloc (SZ_LINE, sizeof (char));
        	sprintf (psim->label, "[Frame %d] %s", 
		    fb->frameno, fb->ctran.imtitle);
	 	psim->annotate = 1;
	    } else {
		if (psim->label) {
		    free ((char *)psim->label);
		    psim->label = NULL;
		}
	 	psim->annotate = 0;
	    }
	    sprintf (buf, "%s %s", option, value ? "True" : "False");
	    xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "compress") == 0) {		 /* COMPRESS */
	    if (value)
		psim->compression = RLECompression;
	    else
		psim->compression = NoCompression;
	    sprintf (buf, "%s %s", option, value ? "True" : "False");
	    xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "orientation") == 0) {     /* ORIENTATION */
	    if (ch == 'P' || ch == 'p')
		psim->page.orientation = EPS_PORTRAIT;
	    else if (ch == 'L' || ch == 'l')
		psim->page.orientation = EPS_LANDSCAPE;
	    sprintf (buf, "%s %s", option, strval);
	    xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "papersize") == 0) {	       /* PAPER SIZE */
	    if (strval[2] == 'T' || strval[2] == 't')
		psim->page.orientation = EPS_LETTER;
	    else if (strval[2] == 'G' || strval[2] == 'g')
		psim->page.orientation = EPS_LEGAL;
	    else if (strval[0] == 'A' || strval[0] == 'a')
		psim->page.orientation = EPS_A4;
	    sprintf (buf, "%s %s", option, strval);
	    xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "imscale") == 0) {	      /* IMAGE SCALE */
	    if (value >= 10) {
	        if ((int)(psim->page.scale*100.0) != value)
	            psim->page.scale = (float) value / 100.0;
	        sprintf (buf, "%s %d", option, value);
	        xim_message (xim, "printOptions", buf);
	    }

	} else if (strcmp (option, "colortype") == 0) {		/* COLORTYPE */
	    if (ch == 'G' || ch == 'g')
		psim->colorClass = EPS_GRAYSCALE;
	    else if (ch == 'P' || ch == 'p')
		psim->colorClass = EPS_PSEUDOCOLOR;
	    else if (ch == 'R' || ch == 'r')
		psim->colorClass = EPS_TRUECOLOR;
	    sprintf (buf, "%s %s", option, strval);
	    xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "devicetype") == 0) {       /* DEVICETYPE */

	    if (strval[0] == 'p' || strval[0] == 'P') {
		pcp->diskfile = 0;
	        sprintf (buf, "deviceType Printer");
	        xim_message (xim, "printOptions", buf);
        	sprintf (buf, "printCmd %s", pcp->printCmd);
        	xim_message (xim, "printOptions", buf);
        	sprintf (buf, "printerName %d", pcp->printno);
        	xim_message (xim, "printOptions", buf);

	    } else if (strval[0] == 'f' || strval[0] == 'F') {
		pcp->diskfile = 1;
	        sprintf (buf, "deviceType File");
	        xim_message (xim, "printOptions", buf);
        	sprintf (buf, "printFile %s", pcp->printFile);
        	xim_message (xim, "printOptions", buf);
	    }

	} else if (strcmp (option, "printername") == 0) {    /* PRINTER NAME */
	    /* Set to printer mode if called in file mode. */
	    if (pcp->diskfile) {
		pcp->diskfile = 0;
	        sprintf (buf, "deviceType Printer");
	        xim_message (xim, "printOptions", buf);
	    }
	    pnum = xim_getPrinterInfo (xim, strval);
	    sprintf (buf, "printerName %s", strval);
	    xim_message (xim, "printOptions", buf);
	    strcpy (pcp->printCmd, printer_list[pnum].printCmd);
	    sprintf (buf, "printCmd %s", pcp->printCmd);
	    xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "printcmd") == 0) {      /* PRINT COMMAND */
	    strcpy (pcp->printCmd, strval);
            sprintf (buf, "printCmd %s", strval);
            xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "printfile") == 0) {    /* PRINT FILENAME */
	    strcpy (pcp->printFile, strval);
            sprintf (buf, "printFile %s", strval);
            xim_message (xim, "printOptions", buf);

	} else if (strcmp (option, "corners") == 0) {      /* IMAGE CORNERS */
	    /* Set the corners of the image being printed. */
	    sscanf (strval, "%d %d %d %d", &llx, &lly, &urx, &ury);
	    eps_setCorners (psim, llx, lly, urx, ury);
	}

        /* Reload the page parameters in case anything's changed. */
        eps_setPage (psim, psim->page.orientation, psim->page.page_type,
            (int)(psim->page.scale*100), psim->page.flags);

	return (TCL_OK);
}


/* setSaveOption -- Set an ximtool client disk file option.
 *
 * Usage:       setSaveOption option value [args]
 *
 * Options:
 *
 *	format 	fmt
 *	color	gray|pseudo|rgb
 *	fname	strval
 */
static int
setSaveOption (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
        register XimDataPtr xim = xc->xim;
        register FrameBufPtr fb = xim->df_p;
	register fileSavePtr fsp = xim->fsp;
	register int i;
        char *option, strval[SZ_LINE];
        char buf[SZ_LINE];
        int ch, value;

        if (argc < 3)
            return (TCL_ERROR);
        else {
            option = argv[1];
	    strcpy (strval, argv[2]);
	    for (i=3; i < argc; i++) {
		strcat (strval, " ");
		strcat (strval, argv[i]);
	    }


            ch = strval[0];
            if (isdigit (ch))
                value = atoi (strval);
            else if (ch == 'T' || ch == 't')
                value = 1;
            else if (ch == 'F' || ch == 'f')
                value = 0;
        }

	if (strcmp (option, "format") == 0) {	       		/* FORMAT */
	    switch (strval[0]) {
	    case 'r':
		if (strval[2] == 's') {
		    fsp->format = XIM_RAS;
		    strcpy (fsp->fname, "frame%d.ras");
		} else if (strval[2] == 'w') {
		    fsp->format = XIM_RAW;
		    strcpy (fsp->fname, "frame%d.raw");
		}
		break;
	    case 'g': 	
		fsp->format = XIM_GIF; 	
		strcpy (fsp->fname, "frame%d.gif");
		break;
	    case 'j': 	
		fsp->format = XIM_JPEG; 
		strcpy (fsp->fname, "frame%d.jpg");
		break;
	    case 't': 	
		fsp->format = XIM_TIFF; 
		strcpy (fsp->fname, "frame%d.tiff");
		break;
	    case 'f': 	
		fsp->format = XIM_FITS; 
		strcpy (fsp->fname, "frame%d.fits");
		break;
	    case 'p': 	
		fsp->format = XIM_PNM; 	
		strcpy (fsp->fname, "frame%d.pnm");
		break;
	    case 'x': 	
		fsp->format = XIM_X11; 	
		strcpy (fsp->fname, "frame%d.xwd");
		break;
	    }
	    sprintf (buf, "%s %s", option, strval);
	    xim_message (xim, "saveOptions", buf);
            sprintf (buf, "fname %s", fsp->fname);
            xim_message (xim, "saveOptions", buf);

	} else if (strcmp (option, "color") == 0) {	      	/* COLOR */
            if (ch == 'G' || ch == 'g')
                fsp->colorType = XIM_GRAYSCALE;
            else if (ch == 'P' || ch == 'p')
                fsp->colorType = XIM_PSEUDOCOLOR;
            else if (ch == 'R' || ch == 'r')
                fsp->colorType = XIM_RGB;
	    sprintf (buf, "%s %s", option, strval);
	    xim_message (xim, "saveOptions", buf);

	} else if (strcmp (option, "fname") == 0) {	       	/* FILENAME */
	    strcpy (fsp->fname, strval);
	    sprintf (buf, "%s %s", option, strval);
	    xim_message (xim, "saveOptions", buf);
	}

	return (TCL_OK);
}


/* setLoadOption -- Set an ximtool client disk file option.
 *
 * Usage:       setLoadOption option value [args]
 *
 * Options:
 * 	up
 *	root
 *	home
 * 	rescan
 *	pattern	patstr
 *	gray    0|1
 *
 */
static int
setLoadOption (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register int i;
        register XimDataPtr xim = xc->xim;
	register fileLoadPtr flp = xim->flp;
        char *ip, *op, *option, *strval;
        char buf[SZ_LINE];

        if (argc < 2)
            return (TCL_ERROR);
        else {
            option = argv[1];
            strval = (argc == 3 ? argv[2] : "" );
        }

	if (strcmp (option, "up") == 0) {	       		/* UP */
	    if (strcmp("/", flp->curdir) != 0) {
		for (i=strlen(flp->curdir); i > 1; i--) {
		    if (flp->curdir[i] == '/')
			break;
		}
		flp->curdir[i] = '\0';
	        sprintf (buf, "curdir %s", flp->curdir);
	        xim_message (xim, "loadOptions", buf);
	        xim_dirRescan (xim);
	    }

	} else if (strcmp (option, "root") == 0) {	      	/* ROOT */
	    strcpy (flp->curdir, "/");
	    sprintf (buf, "curdir %s", flp->curdir);
	    xim_message (xim, "loadOptions", buf);
	    xim_dirRescan (xim);

	} else if (strcmp (option, "home") == 0) {	      	/* HOME */
	    strcpy (flp->curdir, flp->homedir);
	    sprintf (buf, "curdir %s", flp->curdir);
	    xim_message (xim, "loadOptions", buf);
	    xim_dirRescan (xim);

	} else if (strcmp (option, "pattern") == 0) {	       	/* PATTERN */
	    if (strcmp(strval, flp->pattern) != 0) {
	        strcpy (flp->pattern, strval);
	        sprintf (buf, "pattern %s", flp->pattern);
	        xim_message (xim, "loadOptions", buf);
	        xim_dirRescan (xim);
	    }

	} else if (strcmp (option, "rescan") == 0) {	       	/* RESCAN */
	    xim_dirRescan (xim);

	} else if (strcmp (option, "gray") == 0) {	      	/* GRAY */
	    flp->gray = (strval[0] == '0' ? 0 : 1);
	    sprintf (buf, "gray %s", strval[0] == '0' ? "off" : "on");
	    xim_message (xim, "loadOptions", buf);
	}

	return (TCL_OK);
}


/* Print -- Print the current display frame to a printer or to a file (EPS).
 *
 * Usage:       print [x0 y0 nx ny]
 *
 *		print rename old new
 *		print cancel fname
 *
 * If a subregion is given the indicated region is printed, otherwise the
 * full display frame is printed.
 *
 * The forms "print rename" and "print cancel" are actions for print alerts.
 */
static int
print (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
        register XimDataPtr xim = xc->xim;
        int x0, y0, nx, ny;

        /* Handle the special cases first. */
        if (argc == 4 && strcmp (argv[1], "rename") == 0) {
	    ximp_rename (xim, argv[2], argv[3]);
	    return (TCL_OK);
        } else if (argc == 3 && strcmp (argv[1], "cancel") == 0) {
	    ximp_cancel (xim, argv[2]);
	    return (TCL_OK);
        }

	/* Normal case of a print. */
	if (argc == 5) {
	    x0 = atoi (argv[1]);
	    y0 = atoi (argv[2]);
	    nx = atoi (argv[3]);
	    ny = atoi (argv[4]);
	} else
	    x0 = y0 = nx = ny = 0;

	if (xim_print (xim, x0,y0, nx,ny) < 0)
	    return (TCL_ERROR);
	else
	    return (TCL_OK);
}


/* Save -- Save the current display frame to a disk file.
 *
 * Usage:       save [x0 y0 nx ny]
 *
 *		save rename old new
 *		save cancel fname
 *
 * If a subregion is given the indicated region is saved, otherwise the
 * full display frame is saved.
 *
 * The forms "save rename" and "save cancel" are actions for save alerts.
 */
static int
save (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
        register XimDataPtr xim = xc->xim;
	register fileSavePtr fsp = xim->fsp;
        int x0, y0, nx, ny;

        /* Handle the special cases first. */
        if (argc == 4 && strcmp (argv[1], "rename") == 0) {
	    xims_rename (xim, argv[2], argv[3]);
	    return (TCL_OK);
        } else if (argc == 3 && strcmp (argv[1], "cancel") == 0) {
	    xims_cancel (xim, argv[2]);
	    return (TCL_OK);
        }

	if (argc == 5) {
	    x0 = atoi (argv[1]);
	    y0 = atoi (argv[2]);
	    nx = atoi (argv[3]);
	    ny = atoi (argv[4]);
	} else
	    x0 = y0 = nx = ny = 0;

	/* Pass off to the file save routines. */
	if (xim_saveFile (xim, fsp->fname, fsp->format, x0,y0, nx,ny) < 0)
            return (TCL_ERROR);
	else
            return (TCL_OK);
}


/* Load -- Load a frame from a disk file.
 *
 * Usage:       load filename [frame]
 *
 * Options:	frame			display frame to be loaded
 */
static int
load (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register int i;
        register XimDataPtr xim = xc->xim;
	register fileLoadPtr flp = xim->flp;
        char *ip, *op, *fname;
        char *flist, buf[SZ_LINE];
	struct stat file_info;
	int frame;

        if (argc < 2)
            return (TCL_ERROR);

	fname = argv[1];
	frame = (argc >= 3) ? atoi(argv[2]) : xim->display_frame;

        /* If given a directory change the directory browser current reference
	 * directory and get a listing.
	 */
        if (fname[strlen(fname)-1] == '/') {
            fname[strlen(fname)-1] = '\0';
            strcat (flp->curdir, "/");
            strcat (flp->curdir, fname);
            sprintf (buf, "curdir %s", flp->curdir);
            xim_message (xim, "loadOptions", buf);

            xim_dirRescan (xim);

        } else {
            /* Otherwise it may be some kind of image to be loaded, but first
	     * check to see if it's not some other directory first.
	     */
	    (void) stat (fname, &file_info);
	    if (S_ISDIR(file_info.st_mode)) {
                sprintf (flp->curdir, "%s/%s", flp->curdir, fname);
                sprintf (buf, "curdir %s", flp->curdir);
                xim_message (xim, "loadOptions", buf);
                xim_dirRescan (xim);
		return (TCL_OK);
	    }

	    /* It's not a directory, so try loading the file. */
	    if (xim_loadFile (xim, fname, frame) != 0)
                return (TCL_ERROR);
        }

	return (TCL_OK);
}

/* Help -- Send the default help text (HTML) to the GUI.
 *
 * Usage:       help
 */

/* The builtin default help text. */
static char *help_text[] = {
    "setValue {",
#   include "ximtool.html.h"
    "}",
    NULL
};

static int
help (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
        register XimDataPtr xim = xc->xim;
        register char *ip, *op, *helptxt;
        register int i;

        helptxt = (char *) malloc (51200);
        for (i=0, op=helptxt;  ip = help_text[i];  i++) {
            while (*ip)
                *op++ = *ip++;
            *op++ = '\n';
        }
        *op++ = '\0';

        ObmDeliverMsg (xim->obm, "help", helptxt);
        free ((char *)helptxt);

	return (TCL_OK);
}


/* windowRGB -- Window an individual component of an RGB colormap.  We start
 * with the currently defined cmap and scale it's component by the given
 * offset and slope.  A 'save' flag is set when the button is released meaning
 * the user is done with that component and the loaded colormap is updated,
 * otherwise continue changing that color from the previous call allowing us
 * to window the color progressively.  The GUI's 'initialize' option should
 * restore the original colormap.  [This is still test code.]
 *
 * Usage:	windowRGB <color> <offset> <scale> <save>
 *
 * Options:	color		color to manipulate (1=Red,2=Green,3=Blue)
 *		offset		offset of transformation
 *		scale		slope of transformation
 *		save		save to loaded colormap when complete?
 */

static int 
windowRGB (xc, tcl, argc, argv)
register XimClientPtr xc;
Tcl_Interp *tcl;
int argc;
char **argv;
{
	register XimDataPtr xim = xc->xim;
	register FrameBufPtr fb = xim->df_p;
	register ColorMapPtr cm;
	int color, first, nelem, maxelem, save = 0;
	unsigned short r[MAX_COLORS];
	unsigned short g[MAX_COLORS];
	unsigned short b[MAX_COLORS];
	char buf[SZ_LINE];

	if (argc > 1) {
	    cm = &colormaps[fb->colormap-1];
	    color = atoi(argv[1]);
	    fb->offset = atof(argv[2]);
	    fb->scale = (argc > 2) ? (float)atof(argv[3]) : fb->scale;
	    save = atoi(argv[4]);

	    /* Query and read the current colormap. */
	    GtQueryColormap (xim->gt, cm->mapno, &first, &nelem, &maxelem);
	    GtReadColormap (xim->gt, cm->mapno, first, nelem, r,g,b);

	    /* compute the scaled colormap, scaling only the color we're
	     * interested in.
	     */
	    switch (color) {
	    case 1:
		cmapScale (r, nelem, first, fb->offset, fb->scale);
		break;
	    case 2:
		cmapScale (g, nelem, first, fb->offset, fb->scale);
		break;
	    case 3:
		cmapScale (b, nelem, first, fb->offset, fb->scale);
		break;
	    }

	    /* Lastly, write it back to the widget. */
	    GtWriteColormap (xim->gt, 0, first, nelem, r, g, b);
	    if (save)
	        GtWriteColormap (xim->gt, cm->mapno, first, nelem, r, g, b);
	}

	return (TCL_OK);
}


/* cmapScale -- Given a single-color cmap scale it with the given offset and
 * slope, the scaling is done in place.
 */

cmapScale (map, ncells, first, offset, slope)
unsigned short map[MAX_COLORS];
int ncells, first;
float offset, slope;
{
	register int i, c1, c2;
	register float x, y, z, frac;
	unsigned short val, out[MAX_COLORS];

        for (i=0;  i < ncells;  i++) {
            x = (float)i / (float)(ncells - 1);
            y = (x - offset) * slope + 0.5;

            if (y <= 0.0) {
                val = map[first];
            } else if (y >= 1.0) {
                val = map[ncells-1];
            } else {
                z = y * (ncells - 1);
                c1 = (int)z;
                c2 = min (ncells-1, c1 + 1);
                frac = z - c1;
                val = map[c1] * (1.0 - frac) + map[c2] * frac;
            }

            out[i] = val;
        }  

	for (i=0; i < MAX_COLORS; i++)
	    map[i] = out[i];
}
