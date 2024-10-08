#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <ObmW/Gterm.h>
#include "ximtool.h"
#include "logo.h"


void
xim_displayLogo (XimDataPtr xim)
{
    int i;
    unsigned char r[256], g[256], b[256];


    if (!xim || !xim->gt)
	return;

    /* Create the default colormap.
    */
    for (i=0; i < 200; i++)
	r[i] = g[i] = b[i] = (float)i / (float)(LOGO_NCOLORS) * 255;

    /* Set the logical resolution of the display.
    */
    GtSetLogRes ((GtermWidget)xim->gt, LOGO_XDIM, LOGO_YDIM);

    if (xim_writeDisplay (xim, 1, "logo", logo_data,
	LOGO_XDIM, LOGO_YDIM, r, g, b, LOGO_NCOLORS) < 0)
	    return;
}
