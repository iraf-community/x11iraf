/*
 * classnames.c : Map class names to classes. This code is totally
 *	self-contained and so can be used for other projects. A smaller
 *	executable might result from only including the classes an
 *	application needs.
 *
 * George Ferguson, ferguson@cs.rochester.edu, 4 Sep 1991.
 *
 */
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw3d/Command.h>
#include <X11/Xaw3d/Grip.h>
#include <X11/Xaw3d/Label.h>
#include <X11/Xaw3d/List.h>
#include <X11/Xaw3d/Scrollbar.h>
#include <X11/Xaw3d/StripChart.h>
#include <X11/Xaw3d/Toggle.h>
#include <X11/Xaw3d/SimpleMenu.h>
#include <X11/Xaw3d/SmeBSB.h>
#include <X11/Xaw3d/SmeLine.h>
#include <X11/Xaw3d/MenuButton.h>
#include <X11/Xaw3d/AsciiText.h>
#include <X11/Xaw3d/Box.h>
#include <X11/Xaw3d/Dialog.h>
#include <X11/Xaw3d/Form.h>
#include <X11/Xaw3d/Paned.h>
#include <X11/Xaw3d/Viewport.h>

static struct tableRec {
	char *name;
	WidgetClass *class;
	Boolean isShell;
} table[] = {
	{ "Command",	&commandWidgetClass,	False },
	{ "Grip",	&gripWidgetClass,	False },
	{ "Label",	&labelWidgetClass,	False },
	{ "List",	&listWidgetClass,	False },
	{ "Scrollbar",	&scrollbarWidgetClass,	False },
	{ "StripChart",	&stripChartWidgetClass,	False },
	{ "Toggle",	&toggleWidgetClass,	False },
	{ "SimpleMenu",	&simpleMenuWidgetClass,	True },
	{ "SmeBSB",	&smeBSBObjectClass,	False },
	{ "SmeLine",	&smeLineObjectClass,	False },
	{ "MenuButton",	&menuButtonWidgetClass,	False },
	{ "AsciiText",	&asciiTextWidgetClass,	False },
	{ "Box",	&boxWidgetClass,	False },
	{ "Dialog",	&dialogWidgetClass,	False },
	{ "Form",	&formWidgetClass,	False },
	{ "Paned",	&panedWidgetClass,	False },
	{ "Viewport",	&viewportWidgetClass,	False },
	{ "Shell", 		&shellWidgetClass,		True },
	{ "OverrideShell", 	&overrideShellWidgetClass,	True },
	{ "WMShell", 		&wmShellWidgetClass,		True },
	{ "TransientShell", 	&transientShellWidgetClass,	True },
	{ "TopLevelShell",	&topLevelShellWidgetClass,	True },
	{ "ApplicationShell",	&applicationShellWidgetClass,	True },
};

/*
 * classNameToWidgetClass : Returns the WidgetClass with the given "name".
 *	In addition, sets the isShell flag to True or False depending on
 *	whether the class is a Shell class or not.
 *	Return NULL if the name is not the name of any WidgetClass.
 */
WidgetClass
classNameToWidgetClass(name,isShellp)
char *name;
Boolean *isShellp;
{
    int i;

    for (i=0; i < XtNumber(table); i++)
	if (strcmp(name,table[i].name) == 0) {
	    *isShellp = table[i].isShell;
	    return(*(table[i].class));
	}
    return((WidgetClass)NULL);
}
