#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>

/*
 * Object manager test program.  Compile with
 * gcc -g zz.c obm.c Gterm.o tcl/libtcl.a -lXaw -lXmu -lXt -lXext -lX11 -o zz.e
 */

static void
Quit (w, clientData, callData)
Widget w;
XtPointer clientData, callData;
{
	exit (0);
}

gio_reset(){}
gio_hardreset(){}
HandlePopupMenu(){}


Arg labelArgs[] = {
	{ XtNx,		10 },
	{ XtNy,		10 },
	{ XtNlabel,	(XtArgVal)"Widget Server Prototype" },
};
Arg buttonArgs[] = {
	{ XtNx,		10 },
	{ XtNy,		40 },
	{ XtNlabel,	(XtArgVal)"Quit" },
};

char	UItext[32768];
extern	XtPointer ObmOpen();
static	int output();


main (argc, argv)
int argc;
char *argv[];
{
	Widget toplevel, label, button, box;
	XtAppContext app;
	XtPointer obm;
	int nchars, fd;
	char *fname;

	fname = "zz.uis";
	if ((fd = open (fname, 0)) >= 0) {
	    nchars = read (fd, UItext, 32768);
	    printf ("read %d chars from file %s\n", nchars, fname);
	    fflush (stdout);
	    close (fd);
	}

	toplevel = XtAppInitialize (&app, "Xgterm", NULL, 0, &argc, argv,
	    NULL, NULL, 0);

	box = XtCreateManagedWidget ("box", boxWidgetClass,
	    toplevel, NULL, 0);

	label = XtCreateManagedWidget ("label", labelWidgetClass,
	    box, labelArgs, XtNumber(labelArgs));

	button = XtCreateManagedWidget ("quit", commandWidgetClass,
	    box, buttonArgs, XtNumber(buttonArgs));
	XtAddCallback (button, XtNcallback, Quit, NULL);

	obm = ObmOpen (app, argc, argv);
	ObmPostClientOutputCallback (obm, output, 0);
	ObmDeliverMsg (obm, "server", UItext);
	ObmActivate (obm);

	XtRealizeWidget (toplevel);
	XtAppMainLoop (app);
}


static int
output (fd, key, strval)
int fd;
int key;
char *strval;
{
	printf ("output key=%03o(%c) strval=`%s'\n", key, key, strval);
	fflush (stdout);
	return (0);
}
