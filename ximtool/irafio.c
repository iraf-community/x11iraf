/*
 * IRAF.C -- Routines to load and save simple IRAF files.
 *
 *  ival = isIRAF (fname)
 *       loadIRAF (fname, pixels, w,h, r,g,b, ncolors, z1, z2)

 * LoadIRAF(fname, numcols)  -  loads a IRAF file
 * WriteIRAF(fp, pic, w, h, rmap, gmap, bmap, numcols)
 *
 * isIRAF    -- returns nonzero if the named file is a IRAF file.
 * loadIRAF  -- reads a IRAF file and returns the decoded pixel array and gray-
 * 	        scale 8 bit colormap.  The caller is responsible for freeing
 * 	        the pixels buffer.  
 */


#include <stdio.h>
#ifdef ULTRIX
#include <sys/types.h>
#endif
#include <unistd.h>


/* Image header parmeters. */
#define SZ_IMPIXFILE 	79
#define LEN_IMHDR 	513
#define SZ_IMHDR 	2048

/* Offsets into header (in sizeof(int) units) for various parameters */
#define	IM_PIXTYPE	4		/* datatype of the pixels */
#define	IM_NDIM		5		/* number of dimensions */
#define	IM_LEN		6		/* image dimensions */
#define	IM_PHYSLEN	13		/* physical length (as stored) */
#define	IM_PIXOFF	22		/* offset of the pixels */
#define	IM_PIXFILE	103		/* name of pixel storage file */

/* IRAF dataype codes */
#define	TY_CHAR		2
#define	TY_SHORT	3
#define	TY_INT		4
#define	TY_LONG		5
#define	TY_REAL		6
#define	TY_DOUBLE	7
#define	TY_USHORT	11
#define	TY_UCHAR	12


/* MONO returns total intensity of r,g,b components */
#define MONO(rd,gn,bl) (((rd)*11 + (gn)*16 + (bl)*5) >> 5)  /*.33R+ .5G+ .17B*/

#ifndef AIXV3
typedef unsigned char uchar;
#endif

/* Function prototypes */
#ifdef __STDC__

#include <stddef.h>
#include <stdlib.h>

#else

#endif

static char *irafReadPixels();
static void strpak(), flip();
char *index();


/* ----------------
 * Public routines.
 * ----------------*/

 

/* loadIRAF - Load a IRAF file.
 */
char *
loadIRAF  (fname, image, nx, ny, r,g,b, ncolors, z1, z2)
char    *fname;                         /* input filename       */
uchar   **image;                        /* output pixels        */
int     *nx, *ny;                       /* dimensions           */
uchar   *r, *g, *b;                     /* colormap             */
int     *ncolors;                       /* number of colors     */
float   *z1, *z2;                       /* zscale values        */
{
	FILE *hdr;
	register int i, len, px, py;
	int header[SZ_IMHDR], ptype, offset;
	char temp[SZ_IMPIXFILE], *ip;
	char path[SZ_IMPIXFILE], pixfile[SZ_IMPIXFILE];

        if ((hdr = fopen (fname, "r")) == NULL)
	    return "cannot open image";

	/* Read in the image header. */
        fread (header, sizeof (char), SZ_IMHDR, hdr);
	if (header[IM_NDIM] != 2)
	    return "Not a 2-D image";

	/* Get the interesting stuff. */
	px = header[IM_PHYSLEN];
	py = header[IM_PHYSLEN+1];
	*nx = header[IM_LEN];
	*ny = header[IM_LEN+1];
	ptype = header[IM_PIXTYPE];
	offset = (header[IM_PIXOFF] - 1) * sizeof(short);

	/* Find the pixfile and see if it exists. */
	strpak ((char *)&header[IM_PIXFILE], pixfile, SZ_IMPIXFILE);
	if (strncmp (pixfile, "HDR$", 4) == 0) {
	    /* Handle the special case if a HDR$ pixfile path, prepend the
	     * current working directory name to the pixfile.
	     */
    	    strncpy(temp, &pixfile[4], SZ_IMPIXFILE);
    	    strncpy(pixfile, fname, SZ_IMPIXFILE);

    	    /* Find the end of the pathname. */
    	    len = strlen(pixfile);
    	    while( (len > 0) && (pixfile[len-1] != '/') )
    	      len--;

    	    /* Add the image name. */
    	    pixfile[len] = '\0';
    	    strncat(pixfile, temp, SZ_IMPIXFILE);

	} else if (index (pixfile, '!') != NULL) {
	    /* Strip out the leading node! prefix from the pixfile path.
	     */
	    for (ip = pixfile; *ip != '!' ; )
		ip++;
	    strcpy (pixfile, ++ip);
	}

	if (access (pixfile, R_OK) != 0) {
	    fclose (hdr);	
	    return "cannot access pixel file";
	}
	fclose (hdr); 			/* we're done with the header */

	/* Now read the data and return a pointer to the scaled pixels. */
	irafReadPixels (pixfile, offset, ptype, image, *nx, *ny, px, py,
	    z1, z2);

	/* Set the (grayscale) colormap. */
	for (i=0; i<256; i++)
	    r[i] = g[i] = b[i] = i;
	*ncolors = 256;

	fclose (hdr);
	return NULL;
}


/* IsIRAF -- Test a file to see if it is a IRAF file.
 */
int 
isIRAF (fname)
char	*fname;				/* input filename */
{
	register FILE *fp;
	int value = 0;
	char magic[24];

        if (fp = fopen (fname, "r")) {
            fread (magic, sizeof (char), 12, fp);
	    strpak (magic, magic, 5);
	    if (strncmp(magic, "imhdr", 5) == 0)
	        value = 1;
	    fclose (fp);
	}

	return value;
}


/* ------------------
 * Private routines.
 * ------------------
 */

#undef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#undef min
#define min(a,b) ((a) < (b) ? (a) : (b))



/* IRAFREADPIXELS -- Read the pixel file and scale the data.
 */

static char *
irafReadPixels (pixfile, pix_offset, pixtype, image, nx, ny, px, py, z1, z2)
char *pixfile;				/* pixfile pathname      */
int pix_offset;				/* offset to pixel data	 */
int pixtype;				/* pixel types		 */
uchar **image;				/* output image pixels   */
int nx, ny;				/* image dimensions      */
int px, py;				/* physical storage dims */
float *z1, *z2;
{
	FILE *fd;
	register int i, j, npix;
	register float scale;
	uchar  *line;
	unsigned short  *uspix, *usline;
	short  *spix, *sline;
	int    *ipix, *iline;
	float  *fpix, *fline;
	double *dpix, *dline;

	/* Open the pixel file and seek to the beginning of the data. */
        fd = fopen (pixfile, "r");
	lseek (fileno(fd), (off_t)pix_offset, SEEK_SET);

	/* Allocat the pointer for the pixels. */
	npix = nx * ny;
	*image = (uchar *) malloc (npix * sizeof (uchar));

	switch (pixtype) {
	case TY_USHORT:
	    uspix = (unsigned short *) malloc (npix * sizeof (unsigned short));
	    usline = (unsigned short *) malloc (px * sizeof (unsigned short));
	    for (i=0; i<ny; i++) {
                fread (usline, sizeof (unsigned short), px, fd);
		for (j=0; j<nx; j++)
		    uspix[i*nx+j] = usline[j];
	    }
	    free ((unsigned short *)usline);

	    /* Do the zscale of the image. */
	    zscale ((char *)uspix, nx, ny, 16, z1, z2, 0.25, 600, 120);

            /* rescale and convert */
            scale = (*z2 == *z1) ? 0. : 255. / (*z2 -*z1);
            for (i = 0; i < npix; i++)
                (*image)[i] = max (0, min (255,
                    (int)(scale * (float)((int)uspix[i] - *z1)) ));

	    free ((unsigned short *)uspix);
	    break;

	case TY_SHORT:
	    spix = (short *) malloc (npix * sizeof (short));
	    sline = (short *) malloc (px * sizeof (short));
	    for (i=0; i<ny; i++) {
                fread (sline, sizeof (short), px, fd);
		for (j=0; j<nx; j++)
		    spix[i*nx+j] = sline[j];
	    }
	    free ((short *)sline);

	    /* Do the zscale of the image. */
	    zscale ((char *)spix, nx, ny, 16, z1, z2, 0.25, 600, 120);

            /* rescale and convert */
            scale = (*z2 == *z1) ? 0. : 255. / (*z2 -*z1);
            for (i = 0; i < npix; i++)
                (*image)[i] = max (0, min (255,
                    (int)(scale * (float)((int)spix[i] - *z1)) ));

	    free ((short *)spix);
	    break;

	case TY_INT:
	case TY_LONG:
	    ipix = (int *) malloc (npix * sizeof (int));
	    iline = (int *) malloc (px * sizeof (int));
	    for (i=0; i<ny; i++) {
                fread (iline, sizeof (int), px, fd);
		for (j=0; j<nx; j++)
		    ipix[i*nx+j] = iline[j];
	    }
	    free ((int *)iline);

	    /* Do the zscale of the image. */
	    zscale ((char *)ipix, nx, ny, 32, z1, z2, 0.25, 600, 120);

            /* rescale and convert */
            scale = (*z2 == *z1) ? 0. : 255. / (*z2 -*z1);
            for (i = 0; i < npix; i++)
                (*image)[i] = max (0, min (255,
                    (int)(scale * (float)((int)ipix[i] - *z1)) ));

	    free ((int *)ipix);
	    break;

	case TY_REAL:
	    fpix = (float *) malloc (npix * sizeof (float));
	    fline = (float *) malloc (px * sizeof (float));
	    for (i=0; i<ny; i++) {
                fread (fline, sizeof (float), px, fd);
		for (j=0; j<nx; j++)
		    fpix[i*nx+j] = fline[j];
	    }
	    free ((float *)fline);

	    /* Do the zscale of the image. */
	    zscale ((char *)fpix, nx, ny, -32, z1, z2, 0.25, 600, 120);

            /* rescale and convert */
            scale = (*z2 == *z1) ? 0. : 255. / (*z2 -*z1);
            for (i = 0; i < npix; i++)
                (*image)[i] = max (0, min (255,
                    (int)(scale * (float)(fpix[i] - *z1)) ));

	    free ((float *)fpix);
	    break;

	case TY_DOUBLE:
	    dpix = (double *) malloc (npix * sizeof (double));
	    dline = (double *) malloc (px * sizeof (double));
	    for (i=0; i<ny; i++) {
                fread (dline, sizeof (double), px, fd);
		for (j=0; j<nx; j++)
		    dpix[i*nx+j] = dline[j];
	    }
	    free ((double *)dline);

	    /* Do the zscale of the image. */
	    zscale ((char *)dpix, nx, ny, -64, z1, z2, 0.25, 600, 120);

            /* rescale and convert */
            scale = (*z2 == *z1) ? 0. : 255. / (*z2 -*z1);
            for (i = 0; i < npix; i++)
                (*image)[i] = max (0, min (255,
                    (int)(scale * (float)(dpix[i] - *z1)) ));

	    free ((double *)dpix);
	    break;

	default:
	    fclose (fd);
	    return "invalid pixel type in image";
	}

	/* Flip the image so it's displayed correctly. */
	flip (*image, nx, ny);

	fclose (fd);
	return NULL;
}


/* STRPAK -- Convert ASCII string from SPP char per short to C char per byte
 */

static void 
strpak (in, out, len)
char	*in, *out;
int	len;
{
	int	i, j;

	/* adaptive byte selection (whichever byte order) chars alternate 
	 * with \0 
	 */
	if ( in[0] == '\0' )
	    j = 1;
	else
	    j = 0;
	for ( i = 0; i < len; i++, j += 2 )
	    out[i] = in[j];
	out[i] = '\0';
}


/* FLIP -- Reverse order of lines in image.  We do this as a separate step
 * rather than when reading so the sampling grid for zscale is the same as
 * for DISPLAY, even though the pixels are stored "flipped" in the pixfile.
 */
static void 
flip (buffer, nx, ny)
uchar *buffer;
int  nx, ny;
{
        int     i;
        register int    j, v;
        register uchar *buff1, *buff2;

        for (i = 0; i < ny / 2; i++) {
            buff1 = &buffer[i*nx];
            buff2 = &buffer[(ny-1-i)*nx];
            for (j = 0; j < nx; j++) {
                v = *buff1;
                *(buff1++) = *buff2;
                *(buff2++) = v;
            }
        }
}    
