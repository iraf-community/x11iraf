/*
 * IRAF.C -- Routines to load and save simple IRAF files.
 *
 *  ival = isIRAF (fname)
 *       loadIRAF (fname, pixels, w,h, r,g,b, ncolors, z1, z2)
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


#define VERSION_1       1
#define VERSION_2       2

/* Image header parmeters. */
#define SZ_V1PIXFILE 	79
#define SZ_V1HDR 	2048

/* Offsets into header (in sizeof(int) units) for various parameters */
#define	IM_V1PIXTYPE	4		/* datatype of the pixels */
#define	IM_V1NDIM	5		/* number of dimensions */
#define	IM_V1LEN	6		/* image dimensions */
#define	IM_V1PHYSLEN	13		/* physical length (as stored) */
#define	IM_V1PIXOFF	22		/* offset of the pixels */
#define	IM_V1PIXFILE	103		/* name of pixel storage file */

/* Image header parmeters. */
#define SZ_V2PIXFILE    255
#define SZ_V2HDR        513

/* Offsets into header (in sizeof(int) units) for various parameters */
#define IM_V2PIXTYPE    10              /* datatype of the pixels */
#define IM_V2SWAPPED    14              /* number of dimensions */
#define IM_V2NDIM       18              /* number of dimensions */
#define IM_V2LEN        22              /* image dimensions */
#define IM_V2PHYSLEN    50              /* physical length (as stored) */
#define IM_V2PIXOFF     86              /* offset of the pixels */
#define IM_V2PIXFILE    126             /* name of pixel storage file */
         
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
#ifndef OSF1
typedef unsigned char uchar;
#endif
#endif

/* Function prototypes */
#ifdef __STDC__

#include <stddef.h>
#include <stdlib.h>

#else

#endif

static char *irafReadPixels();
static void strpak(), flip(), bswap2(), bswap4();
static int is_swapped_machine();
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
	FILE 	*hdr, *pf;
	int 	i, len, px, py, version, swapped;
	int  	ptype, offset;
	char 	temp[SZ_V1PIXFILE], *ip;
	char 	path[SZ_V1PIXFILE];


        /* Get the format version. */
        version = isIRAF (fname);
 
        if ((hdr = fopen (fname, "r")) == NULL)
	    return "cannot open image";

        if (version == VERSION_1) {
            int         header_v1[SZ_V1HDR];
            char        pixfile_v1[SZ_V1PIXFILE];

            /* Read in the image header. */
            fread (header_v1, sizeof (char), SZ_V1HDR, hdr);

            /* Get the interesting stuff. */
            px = header_v1[IM_V1PHYSLEN];
            py = header_v1[IM_V1PHYSLEN+1];
            *nx = header_v1[IM_V1LEN];
            *ny = header_v1[IM_V1LEN+1];
            ptype = header_v1[IM_V1PIXTYPE];
            offset = (header_v1[IM_V1PIXOFF] - 1) * sizeof(short);

            /* Find the pixfile and see if it exists. */
            strpak ((char *)&header_v1[IM_V1PIXFILE], pixfile_v1, SZ_V1PIXFILE);
            irafGetPixfile (fname, pixfile_v1);
            if (access (pixfile_v1, R_OK) != 0) {
                fclose (hdr);
                return "Cannot access pixel file";
            }

            /* Open the pixel file and seek to the beginning of the data. */
            if ((pf = fopen (pixfile_v1, "r")) == NULL)
                return "Cannot open pixel file.";

	    /* Now read the data and return a pointer to the scaled pixels. */
	    irafReadPixels (pixfile_v1, 0, offset, ptype, image,
		*nx, *ny, px, py, z1, z2);

        }  else if (version == VERSION_2) {
            char        header_v2[SZ_V2HDR];
            char        pixfile_v2[SZ_V2PIXFILE];

            /* Read in the image header. */
            fread (header_v2, sizeof (char), SZ_V2HDR, hdr);

            /* Get the interesting stuff. */
            bcopy ((char *)&header_v2[IM_V2SWAPPED], &swapped, sizeof(int));
            if (!swapped &&  is_swapped_machine() ||
                 swapped && !is_swapped_machine()) {
                     bswap4 (&header_v2[IM_V2PHYSLEN], 1, &px, 1, sizeof(int));
                     bswap4 (&header_v2[IM_V2PHYSLEN+sizeof(int)], 1, &py, 1,
                         sizeof(int));
                     bswap4 (&header_v2[IM_V2LEN], 1, nx, 1, sizeof(int));
                     bswap4 (&header_v2[IM_V2LEN+sizeof(int)], 1, ny, 1,
                         sizeof(int));
                     bswap4 (&header_v2[IM_V2PIXTYPE], 1, &ptype, 1,
                         sizeof(int));
                     bswap4 (&header_v2[IM_V2PIXOFF], 1, &offset, 1,
                         sizeof(int));
		     swapped = 1;
            } else {
                bcopy ((char *)&header_v2[IM_V2PHYSLEN], &px, sizeof(int));
                bcopy ((char *)&header_v2[IM_V2PHYSLEN+sizeof(int)], &py,
                    sizeof(int));
                bcopy ((char *)&header_v2[IM_V2LEN], nx, sizeof(int));
                bcopy ((char *)&header_v2[IM_V2LEN+sizeof(int)], ny,
                    sizeof(int));
                bcopy ((char *)&header_v2[IM_V2PIXTYPE], &ptype, sizeof(int));
                bcopy ((char *)&header_v2[IM_V2PIXOFF], &offset, sizeof(int));
		swapped = 0;
            }
            offset = (offset - 1) * sizeof(short);

            /* Find the pixfile and see if it exists. */
            bcopy ((char *)&header_v2[IM_V2PIXFILE], pixfile_v2, SZ_V2PIXFILE);
            irafGetPixfile (fname, pixfile_v2);
            if (access (pixfile_v2, R_OK) != 0) {
                fclose (hdr);  
                return "Cannot access pixel file";
            }

            /* Open the pixel file and seek to the beginning of the data. */
            if ((pf = fopen (pixfile_v2, "r")) == NULL)
                return "Cannot open pixel file.";

	    /* Now read the data and return a pointer to the scaled pixels. */
	    irafReadPixels (pixfile_v2, swapped, offset, ptype, image,
		*nx, *ny, px, py, z1, z2);
        }         

	fclose (hdr); 			/* we're done with the header */

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
            fclose (fp);

            /* See if this is a valid OIF header file. */
            if (strncmp(magic, "imhv2", 5) == 0)
                value = 2;
            else {
                strpak (magic, magic, 5);
                if (strncmp(magic, "imhdr", 5) == 0)
                    value = 1;
            }
            return value;
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



/*  CDL_GETPIXFILE -- Get the pixelfile pathname.
 */

irafGetPixfile (fname, pfile)
char    *fname;
char    *pfile;
{
        char    temp[SZ_V1PIXFILE], *ip;
        int     len;
        char    *index();

        if (strncmp (pfile, "HDR$", 4) == 0) {
            /* Handle the special case of a HDR$ pixfile path, prepend the
             * current working directory name to the pixfile.
             */
            ip = pfile + 4;
            (void) strncpy (temp, ip, SZ_V1PIXFILE);
            (void) strncpy (pfile, fname, SZ_V1PIXFILE);

            /* Find the end of the pathname. */
            len = strlen (pfile);
            while ( (len > 0) && (pfile[len-1] != '/') )
              len--;

            /* Add the image name. */
            pfile[len] = '\0';
            (void) strncat (pfile, temp, SZ_V1PIXFILE);

        } else if (index (pfile, '!') != NULL) {
            /* Strip out the leading node! prefix from the pixfile path.  */
            for (ip = pfile; *ip != '!' ; )
                ip++;
            (void) strcpy (pfile, ++ip);
        }
} 


/* IRAFREADPIXELS -- Read the pixel file and scale the data.
 */

static char *
irafReadPixels (pixfile, swap, pix_offset, pixtype, image, nx,ny, px,py, z1,z2)
char    *pixfile;			/* pixfile pathname      */
int	swap;				/* byte-swap pixels      */
int     pix_offset;			/* offset to pixel data	 */
int     pixtype;			/* pixel types		 */
uchar   **image;			/* output image pixels   */
int     nx, ny;				/* image dimensions      */
int     px, py;				/* physical storage dims */
float   *z1, *z2;
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

	    if (swap)
		bswap2 ((char *)uspix, (char *)uspix, nx * ny * sizeof(short));

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

	    if (swap)
		bswap2 ((char *)spix, (char *)spix, nx * ny * sizeof(short));

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

	    if (swap)
		bswap4((char *)ipix, 1, (char *)ipix, 1, nx * ny * sizeof(int));

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

	    if (swap)
		bswap4((char *)fpix, 1, (char *)fpix, 1, 
		    nx * ny * sizeof(float));

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

	    if (swap)
		bswap8((char *)dpix, 1, (char *)dpix, 1, 
		    nx * ny * sizeof(double));

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


/* BSWAP2 - Move bytes from array "a" to array "b", swapping successive
 * pairs of bytes.  The two arrays may be the same but may not be offset
 * and overlapping.
 */
static void
bswap2 (a, b, nbytes)
char    *a, *b;         	/* input array                  */
int     nbytes;         	/* number of bytes to swap      */
{
        register char *ip=a, *op=b, *otop;
        register unsigned temp;

        /* Swap successive pairs of bytes.
         */
        for (otop = op + (nbytes & ~1);  op < otop;  ) {
            temp  = *ip++;
            *op++ = *ip++;
            *op++ = temp;
        }

        /* If there is an odd byte left, move it to the output array.
         */
        if (nbytes & 1)
            *op = *ip;
}


/* BSWAP4 - Move bytes from array "a" to array "b", swapping the four bytes
 * in each successive 4 byte group, i.e., 12345678 becomes 43218765.
 * The input and output arrays may be the same but may not partially overlap.
 */
static void
bswap4 (a, aoff, b, boff, nbytes)
char	*a;			/* input array			*/
int	aoff;			/* first byte in input array	*/
char	*b;			/* output array			*/
int	boff;			/* first byte in output array	*/
int	nbytes;			/* number of bytes to swap	*/
{
	register char	*ip, *op, *tp;
	register int	n;
	static	char temp[4];

	tp = temp;
	ip = (char *)a + aoff - 1;
	op = (char *)b + boff - 1;

	/* Swap successive four byte groups.
	 */
	for (n = nbytes >> 2;  --n >= 0;  ) {
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *op++ = *--tp;
	    *op++ = *--tp;
	    *op++ = *--tp;
	    *op++ = *--tp;
	}

	/* If there are any odd bytes left, move them to the output array.
	 * Do not bother to swap as it is unclear how to swap a partial
	 * group, and really incorrect if the data is not modulus 4.
	 */
	for (n = nbytes & 03;  --n >= 0;  )
	    *op++ = *ip++;
}


/* BSWAP8 - Move bytes from array "a" to array "b", swapping the eight bytes
 * in each successive 8 byte group, i.e., 12345678 becomes 87654321.
 * The input and output arrays may be the same but may not partially overlap.
 */
bswap8 (a, aoff, b, boff, nbytes)
char	*a;			/* input array			*/
int	aoff;			/* first byte in input array	*/
char	*b;			/* output array			*/
int	boff;			/* first byte in output array	*/
int	nbytes;		/* number of bytes to swap	*/
{
	register char	*ip, *op, *tp;
	register int	n;
	static	char temp[8];

	tp = temp;
	ip = (char *)a + aoff - 1;
	op = (char *)b + boff - 1;

	/* Swap successive eight byte groups.
	 */
	for (n = nbytes >> 3;  --n >= 0;  ) {
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *tp++ = *ip++;
	    *op++ = *--tp;
	    *op++ = *--tp;
	    *op++ = *--tp;
	    *op++ = *--tp;
	    *op++ = *--tp;
	    *op++ = *--tp;
	    *op++ = *--tp;
	    *op++ = *--tp;
	}

	/* If there are any odd bytes left, move them to the output array.
	 * Do not bother to swap as it is unclear how to swap a partial
	 * group, and really incorrect if the data is not modulus 8.
	 */
	for (n = nbytes & 03;  --n >= 0;  )
	    *op++ = *ip++;
}


/* IS_SWAPPED_MACHINE -- See if this is a byte-swapped (relative to Sun)
 * machine.
 */

static int
is_swapped_machine ()
{
        union {
            char ch[4];
            int  i;
        } u;

        u.i = 1;
        return ((int) u.ch[0]);
}
