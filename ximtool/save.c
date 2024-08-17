#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include "ximtool.h"

/*
 * SAVE.C -- Routines for saving image rasters to disk files.
 *
 *	           xim_initSave (xim)
 *	          xim_saveClose (xim)
 *
 *	  status = xim_saveFile (xim, fname, format, x0,y0, nx,ny)
 *
 *		    xims_rename (xim, old, new)		# save alert action
 *		    xims_cancel (xim, fname)		# save alert action
 *
 * xim_saveFile saves the current display frame to the named file using
 * the format specified.  If nx or ny is zero the entire frame is saved,
 * otherwise the indicated region is saved.
 */

static int xims_write(XimDataPtr xim, FILE *fp, int fileformat, unsigned char *pixels, int w, int h, int d, unsigned char *r, unsigned char *g, unsigned char *b, int ncolors);
static void savestat(XimDataPtr xim, char *message), savetext(XimDataPtr xim, int seqno, char *fullname, int fileformat, int filesize, int w, int h, int d);


/* XIM_INITSAVE -- Initialize the file save structure.
 */
void
xim_initSave (XimDataPtr xim)
{
	fileSavePtr fsp;
	extern char *getcwd(char *, size_t), *getenv(const char *);
	char buf[SZ_LINE];

	if (!(xim->fsp = fsp = (fileSavePtr) calloc (1, sizeof (fileSave))))
	    return;

	fsp->seqno = 0;
	fsp->format = XIM_RAS;
	fsp->colorType = XIM_PSEUDOCOLOR;
	strcpy (fsp->fname, "frame%d.ras");

	/* Now update the GUI. */
        sprintf (buf, "format ras");
        xim_message (xim, "saveOptions", buf);
        sprintf (buf, "color pseudocolor");
        xim_message (xim, "saveOptions", buf);
        sprintf (buf, "fname frame%%d.ras");
        xim_message (xim, "saveOptions", buf);
}


/* XIM_SAVECLOSE -- Close the save structure.
 */
void
xim_saveClose (XimDataPtr xim)
{
	fileSavePtr fsp = xim->fsp;

	if (fsp)
	    free ((char *) fsp);
}


/* XIM_SAVEFILE - Save the current display frame to a disk file.  If nx or
 * ny is zero the entire frame is saved, otherwise the given region is saved.
 */
int
xim_saveFile (XimDataPtr xim, char *template, int fileformat, int x0, int y0, int nx, int ny)
               
               			/* file name or printf format */
               			/* output raster file format/type */
                   		/* region of display to be saved */
{
	int i, j;
	fileSavePtr fsp = xim->fsp;
        int w, h, ncols;
        char text[SZ_LINE], fname[SZ_FNAME];
        unsigned char r[256], g[256], b[256];
        unsigned char *pixels = NULL;
        int status = -1;
	static int debug=0;
	extern int errno;
	FILE *fp;
	char *mktemp(char *);

	/* Generate output file name. */
        sprintf (fname, template, fsp->seqno++);

        /* Get the display pixels and colormap.  The routine allocates a 
         * pointer to the pixels we'll need to free when we're done.
         */
        pixels = xim_readDisplay (xim, x0,y0,nx,ny, &w,&h, r,g,b, &ncols);
        if (!pixels)
            goto done;

	if (debug) {
	    int cpix, val;
	    fprintf (stderr, "saveFile: %s -> %dx%d at %d colors\n", 
		fname, w, h, ncols);
	    cpix = (w/2+(h/2)*w);
	    val = pixels[cpix];
	    fprintf (stderr, "Center pixel %d: val=%d (%d,%d,%d)\n",cpix,
		val,r[val],g[val],b[val]);
	    fprintf (stderr, "User Colormap\n");
	    for (i=0; i < 256; ) {
		for (j=1; j < 4 && i < 256; j++)
		    fprintf (stderr, "    %3d(%3d,%3d,%3d)",i,r[i],g[i],b[i++]);
		fprintf (stderr, "\n");
	    }
	}

	fsp->w = w;
	fsp->h = h;
	fsp->d = 8;

	if (access (fname, F_OK) < 0) {
	    if ((fp = fopen (fname, "w"))) {
		struct stat fs;

		xims_write (xim, fp, fileformat, pixels, w,h,8, r,g,b, 256);
		fclose (fp);
		stat (fname, &fs);

		savetext (xim, fsp->seqno, fname, fileformat, (int)fs.st_size,
		    w, h, 8);
		sprintf (text, "Wrote %d bytes to %s", (int)fs.st_size, fname);
		savestat (xim, text);

	    } else {
		sprintf (text, "Error %d\nCannot open file %s", errno, fname);
		xim_alert (xim, text, NULL, NULL);
		fsp->seqno--;
	    }

	} else {
	    /* Named file already exists.  Write a temporary file and
	     * post an alert to ask the user if they want to overwrite
	     * the existing file.
	     */
	    char ok_action[SZ_LINE];
	    char cancel_action[SZ_LINE];
	    char tmpfile[SZ_FNAME];
	    char *ip, *op, *last;

	    /* Write to a temporary file in the same directory as fname. */
	    for (ip=fname, op=tmpfile, last=tmpfile;  (*op = *ip++);  op++)
		if (*op == '/')
		    last = op + 1;
	    *last = '\0';
	    strcat (tmpfile, "ximsXXXXXX");
	    if (mktemp(tmpfile) == (char *)NULL)
		goto done;

	    if (!(fp = fopen (tmpfile, "w"))) {
		sprintf (text, "Cannot open temporary file:\n%s", tmpfile);
		xim_alert (xim, text, NULL, NULL);
		goto done;
	    }
	    xims_write (xim, fp, fileformat, pixels, w,h,8, r,g,b, 256);
	    fclose (fp);

	    sprintf (text, "%s\n%s\n%s",
		"The following file already exists:", fname,
		"Do you want to overwrite this file?");

	    sprintf (ok_action, "save rename %s %s", tmpfile, fname);
	    sprintf (cancel_action, "save cancel %s", tmpfile);

	    xim_alert (xim, text, ok_action, cancel_action);
	}

	status = 0;
done:
	if (pixels)
	    free ((char *) pixels);
	return (status);
}


/* XIMS_WRITE -- Write a file in the indicated format.
 */
static int
xims_write (XimDataPtr xim, FILE *fp, int fileformat, unsigned char *pixels, int w, int h, int d, unsigned char *r, unsigned char *g, unsigned char *b, int ncolors)
{
        PSImagePtr psim = xim->psim;
        FrameBufPtr fb = xim->df_p;
        ColorMapPtr cm = &colormaps[fb->colormap-1];
	int sv_annotate = psim->annotate;
	int sv_colorClass = psim->colorClass;
	int gray=0, status=0;
	extern int writeSunRas(FILE *fp, unsigned char *pixels, int pixtype, int w, int h, unsigned char *r, unsigned char *g, unsigned char *b, int ncolors, int colorstyle);
	extern int writeFITS(FILE *fp, byte *image, int w, int h, byte *rmap, byte *gmap, byte *bmap, int numcols);
	extern int writeGIF(FILE *fp, byte *image, int w, int h, byte *rmap, byte *gmap, byte *bmap, int numcols, int gray);
	extern int writeTIFF(FILE *fa, unsigned char *data, int w, int h, int ncolors, int gray, unsigned char *r, unsigned char *g, unsigned char *b);

        switch (fileformat) {
        case XIM_RAS:
	    /* Write Sun Rasterfile.  We don't support 24-bit yet but can
	     * add it later.
	     */
	    savestat (xim, "Generating Sun rasterfile...");
	    if (xim->fsp->colorType == XIM_GRAYSCALE)
	        status = writeSunRas (fp, pixels, 8, w,h, r,g,b, ncolors, 1);
	    else
	        status = writeSunRas (fp, pixels, 8, w,h, r,g,b, ncolors, 2);

	    break;

        case XIM_FITS:
	    /* Write a simple FITS file.
	     */
	    savestat (xim, "Generating FITS file...");
	    status = writeFITS (fp, pixels, w, h, r, g, b, ncolors);
	    break;

        case XIM_GIF:
	    /* Write a GIF file.
	     */
	    savestat (xim, "Generating GIF file...");
	    gray = (xim->fsp->colorType == XIM_GRAYSCALE);
	    status = writeGIF (fp, pixels, w, h, r, g, b, ncolors, gray);
	    break;

        case XIM_TIFF:
	    /* Write a TIFF file.
	     */
            savestat (xim, "Generating TIFF file...");
            gray = (xim->fsp->colorType == XIM_GRAYSCALE);
            status = writeTIFF (fp, pixels, w, h, ncolors, gray, r, g, b);
	    break;

        case XIM_EPS:
	    /* Write an EPS file.
	     */
            savestat (xim, "Generating postscript output...");

	    /* Temporarily reset the values. */
	    psim->annotate   = 0;
	    psim->colorClass = xim->fsp->colorType;

            /* Set up some of the EPS options print to the file.  */
            eps_setCmap (psim, r, g, b, ncolors);
            eps_setTransform (psim, fb->ctran.z1, fb->ctran.z2, fb->ctran.zt,
                fb->offset, fb->scale, cm->name);
            eps_print (psim, fp, pixels, w, h, 8, 0);

	    /* Restore the saved values. */
	    psim->annotate   = sv_annotate;
	    psim->colorClass = sv_colorClass;
	    break;

        case XIM_JPEG:
        case XIM_X11:
        case XIM_RAW:
        default:
            /* We don't know what this is so give up and notify the GUI. */
	    savestat (xim, "Save file format not implemented.");
	    status = -1;
	    break;
        }

        return (status);
}


/* The following implement the ok and cancel actions posted by the alert in
 * xim_save above.
 */
void
xims_rename (XimDataPtr xim, char *old, char *new)
{
	fileSavePtr fsp = xim->fsp;
	char text[SZ_LINE];
	struct stat fs;

	unlink (new);
	if (rename(old,new) != 0 || stat(new,&fs) != 0) {
	    sprintf (text, "Could not write file %s", new);
	    savestat (xim, text);
	} else {
	    stat (new, &fs);
	    savetext (xim, fsp->seqno, new, fsp->format, (int)fs.st_size,
		fsp->w, fsp->h, fsp->d);
	    sprintf (text, "Wrote %d bytes to %s", (int)fs.st_size, new);
	    savestat (xim, text);
	}
}

void
xims_cancel (XimDataPtr xim, char *fname)
{
	savestat (xim, "Save cancelled.");
	unlink (fname);
}


/* SAVESTAT -- Internal routine for save status messages.
 */
static void
savestat (XimDataPtr xim, char *message)
{
	char text[SZ_LINE];
	sprintf (text, "status {%s}", message);
        xim_message (xim, "saveOptions", text);
}


/* SAVETEXT -- Write something useful the text box in the save panel.
 */
static void
savetext (XimDataPtr xim, int seqno, char *fullname, int fileformat, int filesize, int w, int h, int d)
{
	char *ip;
	char *fmt, *fname, text[SZ_LINE];

	for (ip=fname=fullname;  *ip;  ip++)
	    if (*ip == '/')
		fname = ip + 1;

	switch (fileformat) {
	case XIM_RAS:
	    fmt = "Sun rasterfile";
	    break;
	case XIM_GIF:
	    fmt = "GIF file";
	    break;
	case XIM_TIFF:
	    fmt = "TIFF file";
	    break;
	case XIM_JPEG:
	    fmt = "JPEG file";
	    break;
	case XIM_X11:
	    fmt = "X11 window dump";
	    break;
	case XIM_FITS:
	    fmt = "FITS file";
	    break;
	case XIM_RAW:
	    fmt = "Raw bytes";
	    break;
	case XIM_EPS:
	    fmt = "EPS file";
	    break;
	default:
	    fmt = "unknown format";
	    break;
	}

	sprintf (text, "text {-- Frame %d --\n%s\n%s\n%d bytes\n%dx%dx%d}",
	    seqno-1, fname, fmt, filesize, w, h, d);
        xim_message (xim, "saveOptions", text);
}
