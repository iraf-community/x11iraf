#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "ximtool.h"

/* LOAD.C -- Package for browsing directories and loading raster files into
 * the display.
 *
 *		   xim_initLoad (xim)
 *		  xim_loadClose (xim)
 *
 *	 status = xim_dirRescan (xim)
 *	  status = xim_loadFile (xim, fname, frame)
 *
 * xim_dirRescan scans the current directory and sends the file list to
 * the GUI "filelist" parameter.  xim_loadFile loads the named raster file
 * into the current display frame.
 */

#define True	1
#define False	0

#undef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#undef min
#define min(a,b) ((a) < (b) ? (a) : (b)) 

extern char 	*getwd(), *getenv();

static char 	**listFiles();
static int 	fileCompare(), globExpression(), fileType();
static void 	amapc(), sortGraymap(), loadstat(), strsort();

static int debug = False;


/* XIM_INITLOAD -- Initialize the file load structure.
 */
void
xim_initLoad (xim)
register XimDataPtr xim;
{
	register fileLoadPtr flp;
	char buf[SZ_LINE];

	xim->flp = flp = (fileLoadPtr) calloc (1, sizeof (fileLoad));
	if (!flp)
	    return;

	strcpy (flp->homedir, getenv("HOME"));
	if (getcwd (flp->curdir, SZ_FNAME) == NULL) 
	    strcpy (flp->curdir, flp->homedir);
	strcpy (flp->pattern, "*");
	flp->gray = 0;

	/* Update the GUI with the initial state. */
        sprintf (buf, "curdir %s\0", flp->curdir);
        xim_message (xim, "loadOptions", buf);
        sprintf (buf, "pattern %s\0", flp->pattern);
        xim_message (xim, "loadOptions", buf);
}


/* XIM_LOADCLOSE -- Close the load struct.
 */
void
xim_loadClose (xim)
XimDataPtr xim;
{
	register fileLoadPtr flp = xim->flp;
	register int i;

	if (flp->FileList)
	    for (i=0;  i < flp->nfiles;  i++)
	        free (flp->FileList[i]);

	free ((fileLoadPtr) flp);
}


/* XIM_LOADFILE -- Try loading a disk raster file into a display frame and
 * colormap.
 */
int
xim_loadFile (xim, fname, frame)
register XimDataPtr xim;
char *fname;
int frame;
{
	register int i, new_config=-1;
	register char *ip;
	register fileLoadPtr flp = xim->flp;
	register FrameBufPtr fb;
	register FbConfigPtr cf;
	float	 z1=0.0, z2=0.0;
	int status = 0, has_private_cmap = 0, pixtype, w, h, ncolors;
	unsigned char *pix=NULL, r[256], g[256], b[256];
	char *mapname, *err, buf[SZ_LINE];

	extern char *loadSunRas();
	extern char *loadFITS();
	extern char *loadGIF();
	extern char *loadIRAF();

        /* Make sure the file exists. */
	if (access(fname, R_OK) != 0) {
	    sprintf (buf, "warning %s\0", "Error: File not found.");
	    xim_message (xim, "loadOptions", buf);
	    return (-1);
	}

        switch (fileType(fname)) {
        case XIM_RAS: 					/* Sun rasterfile. */
	    loadstat (xim, "Reading rasterfile...");
	    if (err = loadSunRas(fname,&pix,&pixtype,&w,&h,r,g,b,&ncolors,
		flp->gray)) {
		    pix = NULL;
		    xim_alert (xim, err, NULL, NULL);
	    }
	    if (pixtype > 8) {
		xim_alert (xim, "Error: 24 bit Sun rasterfile.", NULL, NULL);
		free ((char *) pix);
		pix = NULL;
	    }
	    has_private_cmap =  1;
	    break;

        case XIM_FITS: 					/* FITS file. */
	    loadstat (xim, "Reading FITS file...");
	    if (err = loadFITS(fname, &pix, &w, &h, r,g,b, &ncolors, &z1, &z2)){
		pix = NULL;
		xim_alert (xim, err, NULL, NULL);
	    }
	    has_private_cmap = 0;
	    break;

        case XIM_GIF:
	    loadstat (xim, "Reading GIF fle...");
	    if (err = loadGIF(fname,&pix,&w,&h,r,g,b,&ncolors, flp->gray)) {
		pix = NULL;
		xim_alert (xim, err, NULL, NULL);
	    }
	    has_private_cmap =  1;
	    break;

        case XIM_OIF:
	    loadstat (xim, "Reading IRAF image...");
	    if (err = loadIRAF(fname, &pix, &w, &h, r,g,b, &ncolors, &z1, &z2)){
		pix = NULL;
		xim_alert (xim, err, NULL, NULL);
	    }
	    has_private_cmap = 0;
	    break;

        case XIM_TIFF:
        case XIM_JPEG:
        case XIM_PNM:
        case XIM_X11:
        case XIM_RAW:
	    /* fall through */

        default:
            /* We don't know what this is so give up and notify the GUI. */
	    sprintf (buf, "warning %s\0", "Error: Unknown raster file type.");
	    xim_message (xim, "loadOptions", buf);
	    return (-1);
        }

	if (debug) {
	    fprintf (stderr,
		"Load: pix=%d w=%d h=%d xnc=%d nc=%d pcm=%d z1=%g z2=%g\n",
		    pix, w, h, xim->ncolors, ncolors, has_private_cmap, z1, z2);
	    fflush (stderr);
	}

	/* If the new image has more colors that we're allowed to display,
	 * quantize it to the available number before display.
	 */
	if (ncolors > xim->ncolors) {
	    if ( has_private_cmap && ! flp->gray) {
	        /* Quantize the colormap to the number of display colors. */
	        loadstat (xim, "Quantizing colors...");
	        ppmquant (pix, r, g, b, w, h, ncolors, xim->ncolors);
	    }

	    if (! has_private_cmap || (has_private_cmap && flp->gray)) {
		/* It's a grayscale map, so sort the colors into something
	 	 * that can be adjusted when displayed.
		 */
	        loadstat (xim, "Sorting colormap...");
	        sortGraymap (pix, pix, w*h, r, g, b, xim->ncolors);

		/* Force a grayscale colormap.  */
		for (i=0; i<xim->ncolors; i++)
		   r[i] = g[i] = b[i] = (float)i / (float)(xim->ncolors) * 255;
	    }
	    ncolors = xim->ncolors;
	    xim->psim->cmap.ncolors = xim->ncolors;
	}

	/* Use filename as the colormap name. */
	for (ip=mapname=fname;  *ip;  ip++)
	    if (*ip == '/')
	        mapname = ip + 1;

	/* Now check the dimensions against the current frame buffer config-
	 * uration.  Change the fbconfig if needed.
	 */
	if (w > xim->width || h > xim->height) {

	    register int width, height, tmin = 100000, edges;

	    loadstat (xim, "Initializing frame buffer...");

	    /* First look for a frame buffer at least as large as the image. */
	    for (i=0; i < MAX_FBCONFIG; i++) {
		width = xim->fb_config[i].width;
		height = xim->fb_config[i].height;
                if (width == w && height == h) {
                    /* Get an exact match first. */
                    new_config = i + 1;
		    break;
                } else if (width >= w && height >= h) {
                    /* Look for match with smallest padding. */
                    edges = width - w + height - h;
                    if (edges < tmin) {
                        tmin = edges;
                        new_config = i + 1;
                    }
                }
	    }

	    if (debug)
	        fprintf (stderr, "Load: new_config=%d w=%d h=%d\n", new_config,
		    xim->fb_config[new_config-1].width,
		    xim->fb_config[new_config-1].height);


	    /* We didn't find a large enough buffer, so create a new one with
	     * the right size as one of the last available configs. This will 
	     * overwrite anything there but they shouldn't normally be used 
	     * and we'll use the frame as an offset for each display frame.
	     */
	    if (new_config < 0) {
		new_config = MAX_FBCONFIG - frame + 1;
	        cf = &xim->fb_config[new_config-1];
                cf->nframes = xim->nframes;
                cf->width   = w;
                cf->height  = h;
	    } else {
                cf = &xim->fb_config[new_config-1];
                cf->nframes = (cf->nframes < frame ? frame : cf->nframes);
                cf->nframes = (cf->nframes < xim->nframes ? xim->nframes :
		    cf->nframes);
		xim->fb_configno = new_config;
            }

            /* Change the frame buffer configuration. */
            sprintf (buf, "%d %d %d\0", cf->width, cf->height, 8);
            xim_message (xim, "frameSize", buf);

            /* Create the frame. */
	    for (i=1; i <= cf->nframes; i++)
	        xim_initFrame (xim, i, cf->nframes, cf, xim->memModel);

            xim->width = cf->width;
            xim->height = cf->height;
            GtSetLogRes (xim->gt, cf->width, cf->height);

	    /* Fit the frame.  */
            xim_message (xim, "frameFit", "init");
	}

	/* Take the pixels and colormap and write them to the display. */
	if ((pix == NULL) || (xim_writeDisplay (xim,
		frame, mapname, pix, w, h, r, g, b, ncolors) < 0))
	    status = -1;

	/* Reset the image title string. */
	loadstat (xim, "");
        xim_message (xim, "frameTitle", mapname);
	eps_setLabel (xim->psim, mapname);

	/* Load a WCS for the frame. */
	fb = &xim->frames[frame-1];
	fb->ctran.z1 = z1; 			
	fb->ctran.z2 = z2;
	fb->ctran.zt = W_LINEAR;
	fb->ctran.a  = 1.;
	fb->ctran.b  = 0.;
	fb->ctran.c  = 0.;
	fb->ctran.d  = -1.;
	fb->ctran.tx = ((float)w / 2.) - ((float) xim->width / 2.) + 1;
	fb->ctran.ty = ((float)xim->height / 2.) + ((float) h / 2.);
	fb->ctran.valid = 1;
	strcpy (fb->ctran.imtitle, mapname);
	if (debug)
	    fprintf (stderr,"%s -\n%g %g %g %g %g %g %g %g %d\n", mapname,
		fb->ctran.a, fb->ctran.b, fb->ctran.c, fb->ctran.d,
		fb->ctran.tx, fb->ctran.ty, fb->ctran.z1, fb->ctran.z2,
		fb->ctran.zt);


	if (pix != NULL)
	    free ((char *) pix);
	return (status);
}


/* XIM_DIRRESCAN -- Rescan the current directory and send the resulting file
 * list to the GUI.
 */
void
xim_dirRescan (xim)
register XimDataPtr xim;
{
        register fileLoadPtr flp = xim->flp;
	register char *ip, *op, *flist;
	register int i;

	if (flp->FileList)
	    for (i=0;  i < flp->nfiles;  i++)
	        free (flp->FileList[i]);

	flp->FileList = listFiles (flp->curdir, flp->pattern, &flp->nfiles);

        /* Turn the directory listing into something the GUI wants. */
        if (!(flist = (char *) malloc (SZ_NAME * flp->nfiles)))
	    return;

	strcpy (flist, "setValue {");
        for (i=0, op = flist+10; i < flp->nfiles;  i++) {
            *op++ = '"';
            for (ip = flp->FileList[i];  *op = *ip++;  op++)
                ;
            *op++ = '"';
            *op++ = '\n';
        }
        *op++ = '}';
        *op = '\0';

        /* Send the file list to the GUI.  */
        ObmDeliverMsg (xim->obm, "filelist", flist);
        free ((char *)flist);
}


/*
 * Internal routines.
 * ------------------
 */

/* fileType -- Given a filename return what type of file it is.
 */
static int
fileType (fname)
char *fname;
{
	int format;

	if (isSunRas (fname))
	    format = XIM_RAS;
	else if (isFITS (fname))
	    format = XIM_FITS;
	else if (isGIF (fname))
	    format = XIM_GIF;
	else if (isIRAF (fname))
	    format = XIM_OIF;
	else {
	    /*
	     *  FILL IN WITH REMAINDER OF FORMATS LATER.
	     */
	    format = -1;
	}

	return (format);
}


/* listFiles -- reads the directory specified and returns a list of
 * filenames matching the given pattern contained in the directory 
 * sorted in ascending alphabetic order.
 *
 * Adapted from the ImageMagick package originally developed by John Christy.
 */
static char **
listFiles (directory, pattern, number_entries)
char *directory; 			/* directory to be listed */
char *pattern;				/* pattern to be matched */
int *number_entries;			/* number of filenames in the list */
{
	char **filelist;
	DIR *current_directory;
	struct dirent *entry;
	struct stat file_info;
	unsigned int max_entries;
	int status;

	/* Open directory.  */
	*number_entries = 0;
	status = chdir (directory);
	if (status != 0)
	    return((char **) NULL);
	(void) getcwd (directory, SZ_FNAME);
	current_directory = opendir (directory);
	if (current_directory == (DIR * ) NULL)
	    return((char **) NULL);

	/* Allocate filelist.  */
	max_entries = 4096;
	filelist = (char **) malloc (max_entries * sizeof(char *));
	if (filelist == (char **) NULL) {
	    (void) closedir (current_directory);
	    return((char **) NULL);
	}

	/* Save the current and change to the new directory.  */
	(void) chdir (directory);
	entry = readdir (current_directory);
	while (entry != (struct dirent *) NULL) {
	    if (*entry->d_name == '.') {
	    	entry = readdir (current_directory);
	    	continue;
	    }
	    (void) stat (entry->d_name, &file_info);

	    if (S_ISDIR(file_info.st_mode) ||
		    globExpression(entry->d_name, pattern)) {

	    	if (*number_entries >= max_entries) {
	    	    max_entries <<= 1;
	    	    filelist = (char **)
	    	        realloc((char *)filelist, max_entries * sizeof(char *));
	    	    if (filelist == (char **) NULL) {
	    	    	(void) closedir(current_directory);
	    	    	return((char **) NULL);
	    	    }
	    	}
	    	filelist[*number_entries] = 
		    (char *) malloc (strlen(entry->d_name) + 2);
	    	if (filelist[*number_entries] == (char *) NULL)
	    	    break;
	    	(void) strcpy (filelist[*number_entries], entry->d_name);
	    	if (S_ISDIR(file_info.st_mode))
	    	    strcat (filelist[*number_entries],"/");
	    	(*number_entries)++;
	    }
	    entry = readdir (current_directory);
	}
	(void) closedir (current_directory);

	/* Sort filelist in ascending order. */
	(void) strsort(filelist, *number_entries);

	return (filelist);
}


/* fileCompare -- Comparison routine needed by quicksort for listFiles.
 * Adapted from the ImageMagick package originally developed by John Christy.
 */
static int	
fileCompare (x, y)
void *x, *y;
{
	register char	*p, *q;

	p = (char *) * ((char **) x);
	q = (char *) * ((char **) y);
	while ((*p != '\0') && (*q != '\0') && (*p == *q)) {
	    p++;
	    q++;
	}
	return (*p - (*q));
}


/* globExpression -- returns True if the expression matches the given pattern.
 * Adapted from the ImageMagick package originally developed by John Christy.
 */
static int	
globExpression (expression, pattern)
char *expression; 	/* file name */
char *pattern; 		/* pattern */
{
	int done;

	if (pattern == (char *) NULL)
	    return (True);
	if (strlen (pattern) == 0)
	    return (True);
	if (strcmp (pattern, "*") == 0)
	    return (True);
	done = False;

	while ((*pattern != '\0') && !done) {
	    if (*expression == '\0')
	    	if ((*pattern != '{') && (*pattern != '*'))
	    	    break;
	    switch (*pattern) {
	    case '\\':
	    	 {
	    	    pattern++;
	    	    if (*pattern != '\0')
	    	    	pattern++;
	    	    break;
	    	}
	    case '*':
	    	 {
	    	    int status;

	    	    pattern++;
	    	    status = False;
	    	    while ((*expression != '\0') && !status)
	    	    	status = globExpression((char *) expression++, pattern);
	    	    if (status) {
	    	    	while (*expression != '\0')
	    	    	    expression++;
	    	    	while (*pattern != '\0')
	    	    	    pattern++;
	    	    }
	    	    break;
	    	}
	    case '[':
	    	 {
	    	    char c;

	    	    pattern++;
	    	    for ( ; ; ) {
	    	    	if ((*pattern == '\0') || (*pattern == ']')) {
	    	    	    done = True;
	    	    	    break;
	    	    	}
	    	    	if (*pattern == '\\') {
	    	    	    pattern++;
	    	    	    if (*pattern == '\0') {
	    	    	    	done = True;
	    	    	    	break;
	    	    	    }
	    	    	}
	    	    	if (*(pattern + 1) == '-') {
	    	    	    c = (*pattern);
	    	    	    pattern += 2;
	    	    	    if (*pattern == ']') {
	    	    	    	done = True;
	    	    	    	break;
	    	    	    }
	    	    	    if (*pattern == '\\') {
	    	    	    	pattern++;
	    	    	    	if (*pattern == '\0') {
	    	    	    	    done = True;
	    	    	    	    break;
	    	    	    	}
	    	    	    }
	    	    	    if ((*expression < c) || (*expression > *pattern)) {
	    	    	    	pattern++;
	    	    	    	continue;
	    	    	    }
	    	    	} else if (*pattern != *expression) {
	    	    	    pattern++;
	    	    	    continue;
	    	    	}
	    	    	pattern++;
	    	    	while ((*pattern != ']') && (*pattern != '\0')) {
	    	    	    if ((*pattern == '\\') && (*(pattern + 1) != '\0'))
	    	    	    	pattern++;
	    	    	    pattern++;
	    	    	}
	    	    	if (*pattern != '\0') {
	    	    	    pattern++;
	    	    	    expression++;
	    	    	}
	    	    	break;
	    	    }
	    	    break;
	    	}
	    case '?':
	    	 {
	    	    pattern++;
	    	    expression++;
	    	    break;
	    	}
	    case '{':
	    	 {
	    	    int match;
	    	    register char *p;

	    	    pattern++;
	    	    while ((*pattern != '}') && (*pattern != '\0')) {
	    	    	p = expression;
	    	    	match = True;
	    	    	while ((*p != '\0') && (*pattern != '\0') && 
	    	    	    (*pattern != ',') && (*pattern != '}') && match) {
	    	    	    if (*pattern == '\\')
	    	    	    	pattern++;
	    	    	    match = (*pattern == *p);
	    	    	    p++;
	    	    	    pattern++;
	    	    	}
	    	    	if (*pattern == '\0') {
	    	    	    match = False;
	    	    	    done = True;
	    	    	    break;
	    	    	} else if (match) {
	    	    	    expression = p;
	    	    	    while ((*pattern != '}') && (*pattern != '\0')) {
	    	    	    	pattern++;
	    	    	    	if (*pattern == '\\') {
	    	    	    	    pattern++;
	    	    	    	    if (*pattern == '}')
					pattern++;
	    	    	    	}
	    	    	    }
	    	    	} else
	    	    	 {
	    	    	    while ((*pattern != '}') && (*pattern != ',') && 
	    	    	        (*pattern != '\0')) {
	    	    	    	pattern++;
	    	    	    	if (*pattern == '\\') {
	    	    	    	    pattern++;
	    	    	    	    if ((*pattern == '}') || (*pattern == ','))
					pattern++;
	    	    	    	}
	    	    	    }
	    	    	}
	    	    	if (*pattern != '\0')
	    	    	    pattern++;
	    	    }
	    	    break;
	    	}
	    default:
	    	 {
	    	    if (*expression != *pattern)
	    	    	done = True;
	    	    else
	    	     {
	    	    	expression++;
	    	    	pattern++;
	    	    }
	    	}
	    }
	}

	while (*pattern == '*')
	    pattern++;

	return ((*expression == '\0') && (*pattern == '\0'));
}


/*  AMAPC -- Vector linear transformation.  Map the range of pixel values
 *  a1,a2 from a into the range b1,b2 in b.  It is assumed that a1 < a2
 *  and b1 < b2.
 */

static void
amapc (a, b, npix, a1, a2, b1, b2)
char   	*a, *b;
int	npix, a1, a2, b1, b2;
{
	register int     i, minout, maxout, aoff, boff, pixval;
	register double  scalar;

        scalar = ((double) b2 - (double) b1) / ((double) a2 - (double) a1);

        minout = min (b1, b2);
        maxout = max (b1, b2);
        aoff = a1;
        boff = b1;

	for (i=0; i < npix; i++) {
            pixval = (a[i] - aoff) * scalar;
            b[i] = (char) max(minout, min(maxout, pixval + boff));
        }
}   


/* SORGRAYMAP -- Sort a random grayscale colormap.
 */

static void
sortGraymap (a, b, npix, red, green, blue, ncols)
unsigned char *a, *b;
unsigned char *red, *green, *blue;
int	npix, ncols;
{
	register int	i, pmin=0, pmax=255;
	register float scale;
	int ucharCompare();

	/* Reset the pixels so they are the final grayscale values. */
	scale = 255.0 / (float) (ncols - 1);
	for (i=0; i<npix; i++)
	    b[i] = (unsigned char) ((float) red[a[i]] / scale);

	/* Sort the input colormap. */
	qsort (red, ncols, sizeof (unsigned char), ucharCompare);

	/* Set the new colormap.  0-ncols are the scaled colors in the desired
	 * number of cells, everything above that we set to zero.
	 */
	for (i=0; i<ncols; i++)
	    green[i] = blue[i] = (unsigned char)min(pmax,max(pmin,(int)red[i]));
	for (i=ncols; i<256; i++)
	    red[i] = green[i] = blue[i] = 0;
}

int ucharCompare (i, j)
unsigned char *i, *j;
{
	return (*i - *j);
}


/* LOADSTAT -- Internal routine for load status messages.
 */
static void
loadstat (xim, message)
register XimDataPtr xim;
char *message;
{
        char text[SZ_LINE];
        sprintf (text, "status {%s}", message);
        xim_message (xim, "loadOptions", text);
}


/* STRSORT -- Shell sort an array of string pointers via strcmp()
 */
static void
strsort (array, array_size)
char    **array;
int     array_size;
{
      int       gap, i, j;
      char      **a, **b, *tmp;

      for (gap = 0; ++gap < array_size; )
          gap *= 2;
      while (gap /= 2) {
          for (i = gap; i < array_size; i++) {
              for (j = i - gap; ;j -= gap) {
                  a = array + j;
                  b = a + gap;
                  if (strcmp(*a, *b) <= 0)
                     break;
                  tmp = *a;
                  *a = *b;
                  *b = tmp;
                  if (j < gap)
                     break;
              }
          }
      }
}
