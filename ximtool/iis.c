#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include "ximtool.h"
#include "iis.h"

/*
 * IIS.C -- IRAF/IIS "imtool" protocol module.  This code is responsible for
 * accepting connections from remote network clients and communicating with
 * them via the imtool/iis image display server communications prototcol.
 *
 *	   fd = xim_iisopen (xim)
 *	       xim_iisclose (xim)
 *		  xim_iisio (xim, &fd, &id)
 *
 *           xim_frameLabel (xim)
 *            xim_encodewcs (xim, sx, sy, sz, obuf)
 *         xim_retCursorVal (xim, sx, sy, frame, wcs, key, strval)
 *
 *	       xim_iisiomap (w, iomap, &iomap_len)
 *	    xim_iiscolormap (w, r, g, b, &first, &ngray, &rgb_len)
 *
 * xim_iisio is a callback procedure called by Xt when there is input to be
 * processed on the stream used to communicate with the remote client.
 */

#define	MEMORY		01		/* frame buffer i/o		*/
#define	LUT		02		/* lut i/o			*/
#define	FEEDBACK	05		/* used for frame clears	*/
#define	IMCURSOR	020		/* logical image cursor		*/
#define	WCS		021		/* used to set WCS		*/
#define	SZ_IOBUF	65536		/* max size data transfer	*/
#define IO_TIMEOUT	30		/* i/o not getting anywhere     */
#define	MAXCONN		5


#define	SZ_IMCURVAL	160
#define	PACKED		0040000
#define	COMMAND		0100000
#define	IIS_READ	0100000
#define	IMC_SAMPLE	0040000
#define	IMT_FBCONFIG	077
#define	XYMASK		077777

struct	iism70 {
	short	tid;
	short	thingct;
	short	subunit;
	short	checksum;
	short	x, y, z;
	short	t;
};

extern int errno;
static void set_fbconfig();
static int decode_frameno();
static int bswap2();
static CtranPtr wcs_update();

static IoChanPtr open_fifo(), open_inet(), open_unix();
static void xim_connectClient(), xim_disconnectClient();
static IoChanPtr get_iochan();


/* XIM_IISOPEN -- Initialize the IIS protocol module and ready the module to
 * accept client connections and begin processing client requests.  Clients
 * may connect to the server using a fifo connection or an internet or
 * UNIX domain socket connection.  All three types of server ports are
 * simultaneously ready to receive client connections.
 */
xim_iisopen (xim)
register XimDataPtr xim;
{
	int nopen = 0;

	if (open_fifo (xim))
	    nopen++;
	if (open_inet (xim))
	    nopen++;
	if (open_unix (xim))
	    nopen++;

	return (nopen);
}


/* XIM_IISCLOSE -- Close down the IIS protocol module.
 */
void
xim_iisclose (xim)
register XimDataPtr xim;
{
	register IoChanPtr chan;
	register int i;

	for (i=0, chan=NULL;  i < XtNumber(xim->chan);  i++) {
	    chan = &xim->chan[i];
	    if (chan->id) {
		xim_removeInput (xim, chan->id);
		chan->id = NULL;
	    }

	    switch (chan->type) {
	    case IO_FIFO:
		if (chan->keepalive >= 0)
		    close (chan->keepalive);
		if (chan->datain >= 0)
		    close (chan->datain);
		if (chan->dataout >= 0)
		    close (chan->dataout);
		chan->type = 0;
		break;

	    case IO_INET:
		close (chan->datain);
		chan->type = 0;
		break;

	    case IO_UNIX:
		close (chan->datain);
		unlink (chan->path);
		chan->type = 0;
		break;
	    }
	}
}


/* OPEN_FIFO -- Ooen the (x)imtool fifo port and make ready to accept client
 * connections and begin processing client requests.  There is no client
 * yet at this stage.
 */
static IoChanPtr
open_fifo (xim)
register XimDataPtr xim;
{
	register IoChanPtr chan;
	int datain, dataout;
	int keepalive;

	/* Setting the input fifo to "none" or the null string disables
	 * fifo support.
	 */
	if (!xim->input_fifo[0] || strcmp(xim->input_fifo,"none")==0)
	    return (NULL);

	datain = dataout = -1;

	/* Open the output fifo (which is the client's input fifo).  We have
	 * to open it ourselves first as a client to get around the fifo
	 * open-no-client error.  
	 */
	if ((datain = open (xim->input_fifo, O_RDONLY|O_NDELAY)) != -1) {
	    if ((dataout = open (xim->input_fifo, O_WRONLY|O_NDELAY)) != -1)
		fcntl (dataout, F_SETFL, O_WRONLY);
	    else
		goto done;
	    close (datain);
	} else
	    goto done;

	/* Open the input stream, a FIFO pseudodevice file used by
	 * applications to send us commands and data.
	 */
	if ((datain = open (xim->output_fifo, O_RDONLY|O_NDELAY)) == -1)
	    goto done;
	else {
	    /* Clear O_NDELAY for reading. */
	    fcntl (datain, F_SETFL, O_RDONLY);

	    /* Open the client's output fifo as a pseudo-client to make it
	     * appear that a client is connected.
	     */
	    keepalive = open (xim->output_fifo, O_WRONLY);
	}
done:
	/* Allocate and fill in i/o channel descriptor. */
	if (datain > 0 && dataout > 0 && (chan = get_iochan(xim))) {
	    chan->xim = (XtPointer) xim;
	    chan->type = IO_FIFO;
	    chan->datain = datain;
	    chan->dataout = dataout;
	    chan->keepalive = keepalive;
	    chan->reference_frame = 1;
	    chan->rf_p = &xim->frames[0];
	} else {
	    fprintf (stderr, "Warning: cannot open %s\n", xim->output_fifo);
	    chan = NULL;
	}

	/* Register input callback. */
	if (chan) {
	    chan->id = xim_addInput (xim, chan->datain, xim_iisio,
		(XtPointer)chan);
	} else {
	    if (datain > 0)
		close (datain);
	    if (dataout > 0)
		close (dataout);
	}

	return (chan);
}


/* OPEN_INET -- Set up a port to be used for incoming client connections
 * using internet domain sockets.
 */
static IoChanPtr
open_inet (xim)
register XimDataPtr xim;
{
	register int s = 0;
	register IoChanPtr chan;
	struct sockaddr_in sockaddr;

	/* Setting the port to zero disables inet socket support. */
	if (xim->port <= 0)
	    return (NULL);

	if ((s = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    goto err;

	memset ((void *)&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons((short)xim->port);
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind (s, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
	    goto err;

	if (listen (s, MAXCONN) < 0)
	    goto err;

	/* Allocate and fill in i/o channel descriptor. */
	if (chan = get_iochan(xim)) {
	    chan->xim = (XtPointer) xim;
	    chan->type = IO_INET;
	    chan->datain = s;
	    chan->dataout = s;
	    chan->reference_frame = 1;
	    chan->rf_p = &xim->frames[0];

	    /* Register connectClient callback. */
	    chan->id = xim_addInput (xim,s,xim_connectClient,(XtPointer)chan);
	    return (chan);
	}
err:
	fprintf (stderr, "ximtool: cannot open socket on port %d, errno=%d\n",
	    xim->port, errno);
	if (s)
	    close (s);
	return (NULL);
}


/* OPEN_UNIX -- Set up a port to be used for incoming client connections
 * using unix domain sockets.
 */
static IoChanPtr
open_unix (xim)
register XimDataPtr xim;
{
	register int s = 0;
	register IoChanPtr chan;
	struct sockaddr_un sockaddr;
	char path[256];

	/* Setting the addr to "none" or the null string disables unix
	 * socket support.
	 */
	if (!xim->unixaddr[0] || strcmp(xim->unixaddr,"none")==0)
	    return (NULL);

	/* Get path to be used for the unix domain socket. */
	sprintf (path, xim->unixaddr, getuid());
	unlink (path);

	if ((s = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
	    goto err;

	memset ((void *)&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sun_family = AF_UNIX;
	strcpy (sockaddr.sun_path, path);
	if (bind (s, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
	    goto err;

	if (listen (s, MAXCONN) < 0)
	    goto err;

	/* Allocate and fill in i/o channel descriptor. */
	if (chan = get_iochan(xim)) {
	    chan->xim = (XtPointer) xim;
	    chan->type = IO_UNIX;
	    chan->datain = s;
	    chan->dataout = s;
	    chan->reference_frame = 1;
	    chan->rf_p = &xim->frames[0];
	    strncpy (chan->path, path, SZ_FNAME);

	    /* Register connectClient callback. */
	    chan->id = xim_addInput (xim,s,xim_connectClient,(XtPointer)chan);
	    return (chan);
	}
err:
	fprintf (stderr, "ximtool: cannot open socket on port %s, errno=%d\n",
	    path, errno);
	if (s)
	    close (s);
	return (NULL);
}


/* XIM_CONNECTCLIENT -- Called when a client has attempted a connection on
 * a socket port.  Accept the connection and set up a new i/o channel to
 * communicate with the new client.
 */
static void
xim_connectClient (chan_port, source, id)
IoChanPtr chan_port;
int *source;
XtPointer id;
{
	register XimDataPtr xim = (XimDataPtr) chan_port->xim;
	register IoChanPtr chan;
	register int s;

	/* Accept connection. */
	if ((s = accept ((int)*source, (struct sockaddr *)0, (int *)0)) < 0)
	    return;
	if (fcntl (s, F_SETFD, O_RDWR|O_NDELAY) < 0) {
	    close (s);
	    return;
	}

	/* Allocate and fill in i/o channel descriptor. */
	if (chan = get_iochan(xim)) {
	    chan->xim = (XtPointer) xim;
	    chan->type = chan_port->type;
	    chan->datain = s;
	    chan->dataout = s;
	    chan->reference_frame = 1;
	    chan->rf_p = &xim->frames[0];
	    chan->id = xim_addInput (xim, s, xim_iisio, (XtPointer)chan);
	}
}


/* XIM_DISCONNECTCLIENT -- Called to close a client connection when EOF is
 * seen on the input port.  Close the connection and free the channel
 * descriptor.
 */
static void
xim_disconnectClient (chan)
register IoChanPtr chan;
{
	switch (chan->type) {
	case IO_INET:
	case IO_UNIX:
	    close (chan->datain);
	    if (chan->id) {
		xim_removeInput (chan->xim, chan->id);
		chan->id = NULL;
	    }
	    chan->type = 0;
	    break;
	default:
	    break;
	}
}


/* GET_IOCHAN --- Get an i/o channel descriptor.
 */
static IoChanPtr
get_iochan (xim)
register XimDataPtr xim;
{
	register IoChanPtr chan;
	register int i;

	for (i=0;  i < XtNumber(xim->chan);  i++)
	    if (!xim->chan[i].type)
		return (&xim->chan[i]);

	return (NULL);
}


/* XIM_IISIO -- Xt file i/o callback procedure, called when there is input
 * pending on the data stream to the ximtool client.
 */
void
xim_iisio (chan, fd_addr, id_addr)
IoChanPtr chan;
int *fd_addr;
XtInputId *id_addr;
{
	register XimDataPtr xim = (XimDataPtr) chan->xim;
	register int sum, i;
	register short *p;
	int	datain = *fd_addr;
	int	dataout = chan->dataout;
	int	ndatabytes, nbytes, n, ntrys=0;
	struct	iism70 iis;
	char	buf[SZ_FIFOBUF];
	static	int errmsg=0, bswap=0;
	static	int iis_debug = -1;


	/* Initialize the debug output. */
	if (iis_debug == -1)
	    iis_debug = (getenv("DEBUG_IIS") != (char *)NULL);

	/* Get the IIS header. */
	if ((n = read (datain, (char *)&iis, sizeof(iis))) < sizeof(iis)) {
	    if (n <= 0)
		xim_disconnectClient (chan);
	    else
		fprintf (stderr, "imtool: command input read error\n");
	    return;
	} else if (bswap)
	    bswap2 ((char *)&iis, (char *)&iis, sizeof(iis));

	/* Verify the checksum.  If it fails swap the bytes and try again.
	 */
	for (;;) {
	    for (i=0, sum=0, p=(short *)&iis;  i < 8;  i++)
		sum += *p++;
	    if ((sum & 0177777) == 0177777)
		break;

	    if (ntrys++) {
		if (!errmsg++) {
		    fprintf (stderr, "imtool: bad data header checksum\n");
		    if (bswap)
			bswap2 ((char *)&iis, (char *)&iis, sizeof(iis));
		    fprintf (stderr, "noswap:");
		    for (i=0, p=(short *)&iis;  i < 8;  i++)
			fprintf (stderr, " %6o", p[i]);
		    fprintf (stderr, "\n");

		    bswap2 ((char *)&iis, (char *)&iis, sizeof(iis));
		    fprintf (stderr, "  swap:");
		    for (i=0, p=(short *)&iis;  i < 8;  i++)
			fprintf (stderr, " %6o", p[i]);
		    fprintf (stderr, "\n");
		}
		break;

	    } else {
		bswap2 ((char *)&iis, (char *)&iis, sizeof(iis));
		bswap = !bswap;
	    }
	}

	ndatabytes = -iis.thingct;
	if (!(iis.tid & PACKED))
	    ndatabytes *= 2;

	if (iis_debug) {
            fprintf (stderr,
               "subunit=%06o tid=%06o nbytes=%7d x=%06o y=%06o z=%06o t=%06o\n",
               iis.subunit & 077,
               iis.tid,
	       ndatabytes,
               iis.x & 0177777,
               iis.y & 0177777,
               iis.z & 0177777,
               iis.t & 0177777);
            fflush (stderr);
        }


	switch (iis.subunit & 077) {
	case FEEDBACK:
	    /* The feedback unit is used only to clear a frame.
	     */
	    xim_setReferenceFrame (chan, decode_frameno (iis.z & 07777));
	    xim_eraseFrame (xim, chan->reference_frame);
	    if (iis_debug)
  		fprintf (stderr, "erase frame %d - ref = %d\n", 
		    decode_frameno(iis.z & 07777), chan->reference_frame);
	    break;

	case LUT:
	    /* Data mode writes to the frame lookup tables are not implemented.
	     * A command mode write to the LUT subunit is used to connect
	     * image memories up to the RGB channels, i.e., to select the frame
	     * to be displayed.  We ignore any attempt to assign multiple
	     * frames to multiple color channels, and just do a simple frame
	     * select.
	     */
	    if (iis.subunit & COMMAND) {
		int	frame, z, n;
		short	x[14];

		if (read (datain, (char *)x, ndatabytes) == ndatabytes) {
		    if (bswap)
			bswap2 ((char *)x, (char *)x, ndatabytes);

		    z = x[0];
		    if (!z) z = 1;
		    for (n=0;  !(z & 1);  z >>= 1)
			n++;

		    frame = max (1, n + 1);
		    if (frame > xim->nframes) {
			if (frame < MAX_FRAMES) {
			    set_fbconfig (chan, xim->fb_configno, frame);
	    		    if (iis_debug)
                                fprintf (stderr, "set_fbconfig (%d, %d)\n",
                                    xim->fb_configno, frame);
			} else {
			    fprintf (stderr, "imtool warning: ");
			    fprintf (stderr, 
				"attempt to display nonexistent frame %d\n",
				frame);
			    return;
			}
		    }

		    xim_setDisplayFrame (xim, frame);
		    if (iis_debug)
                        fprintf (stderr, "set_frame (%d)\n", frame);
		    return;
		}
	    }

	case MEMORY:
	    /* Load data into the frame buffer.  Data is assumed to be byte
	     * packed.
	     */
	    if (iis.tid & IIS_READ) {
		/* Read from the display.
		 */
		unsigned char *ip, iobuf[SZ_IOBUF];
		int     nbytes, nleft, n, x, y;
		long    starttime;

		/* Get the frame to be read from. */
		xim_setReferenceFrame (chan, decode_frameno (iis.z & 07777));

		nbytes = ndatabytes;
		x = iis.x & XYMASK;
		y = iis.y & XYMASK;

		if (x < 0 || x >= xim->width || y < 0 || y >= xim->height) {
		    fprintf (stderr,
			"ximtool: attempted read out of bounds on framebuf\n");
		    fprintf (stderr,
			"read %d bytes at [%d,%d]\n", nbytes, x, y);
		    memset ((void *)iobuf, 0, nbytes);
		} else {
		    GtReadPixels (xim->gt, chan->rf_p->raster, iobuf, 8, x, y,
			min(xim->width-x,nbytes), max(1,nbytes/xim->width));
		    if (iis_debug)
                        fprintf (stderr,
                            "read %d bytes at [%d,%d]\n", nbytes, x, y);
		}

		/* Return the data from the frame buffer. */
		starttime = time(0);
		for (nleft=nbytes, ip=iobuf;  nleft > 0;  nleft -= n) {
		    n = (nleft < SZ_FIFOBUF) ? nleft : SZ_FIFOBUF;
		    if ((n = write (dataout, ip, n)) <= 0) {
			if (n < 0 || (time(0) - starttime > IO_TIMEOUT)) {
			    fprintf (stderr, "IMTOOL: timeout on write\n");
			    break;
			}
		    } else
			ip += n;
		}

		return;

	    } else {
		/* Write to the display.
		 */
		unsigned char *op, iobuf[SZ_IOBUF];
		int     nbytes, nleft, n, x, y;
		long    starttime;

		/* Get the frame to be written into (encoded with a bit for
		 * each frame, 01 is frame 1, 02 is frame 2, 04 is frame 3,
		 * and so on).
		 */
		xim_setReferenceFrame (chan, decode_frameno (iis.z & 07777));

		nbytes = ndatabytes;
		x = iis.x & XYMASK;
		y = iis.y & XYMASK;

		/* Read the data into the frame buffer.
		 */
		starttime = time(0);
		for (nleft=nbytes, op=iobuf;  nleft > 0;  nleft -= n) {
		    n = (nleft < SZ_FIFOBUF) ? nleft : SZ_FIFOBUF;
		    if ((n = read (datain, op, n)) <= 0) {
			if (n < 0 || (time(0) - starttime > IO_TIMEOUT))
			    break;
		    } else
			op += n;
		}

		if (x < 0 || x >= xim->width || y < 0 || y >= xim->height) {
		    fprintf (stderr,
			"ximtool: attempted write out of bounds on framebuf\n");
		    fprintf (stderr,
			"write %d bytes at [%d,%d]\n", nbytes, x, y);
		    memset ((void *)iobuf, 0, nbytes);
		} else {
		    GtWritePixels (xim->gt, chan->rf_p->raster, iobuf, 8, x, y,
			min(xim->width-x,nbytes), max(1,nbytes/xim->width));
		    if (iis_debug)
                	fprintf (stderr, "write %d bytes at x=%d, y=%d\n",
                    	    nbytes, x, y);
		}

		return;
	    }
	    break;

	case WCS:
	    /* Read or write the WCS for a frame.  The frame number to
	     * which the WCS applies is passed in Z and the frame buffer
	     * configuration in T.  The client changes the frame buffer
	     * configuration in a WCS set.  The WCS text follows the header
	     * as byte packed ASCII data.
	     */
	    if (iis.tid & IIS_READ) {
		/* Return the WCS for the referenced frame.
		 */
		char emsg[SZ_FNAME];
		char *text;
		int frame;

		frame = decode_frameno (iis.z & 07777);
		xim_setReferenceFrame (chan, frame);

		if (chan->rf_p->frameno <= 0)
		    strcpy (text=emsg, "[NOSUCHFRAME]\n");
		else
		    text = chan->rf_p->wcsbuf;

		write (dataout, text, SZ_WCSBUF);
		if (iis_debug) {
                    fprintf (stderr, "query wcs:\n");
                    write (2, text, SZ_WCSBUF);
		}

	    } else {
		/* Set the WCS for the referenced frame.
		 */
		register CtranPtr ct;
		int fb_config, frame;

		frame = decode_frameno (iis.z & 07777);
		fb_config = (iis.t & 0777) + 1;

		/* See if we need to change the frame buffer configuration,
		 * or allocate a new frame.
	
		if (fb_config == 1) {
		    if (xim->fb_config[0].width != xim->width || 
		        xim->fb_config[0].height != xim->height)
		            set_fbconfig (chan, fb_config, frame);
		} else 
		*/
		if (fb_config != xim->fb_configno)
		        set_fbconfig (chan, fb_config, frame);
		else if (frame > xim->nframes && frame < MAX_FRAMES)
		    set_fbconfig (chan, xim->fb_configno, frame);

		/* Read in and set up the WCS. */
		xim_setReferenceFrame (chan, frame);
		if (read (datain, buf, ndatabytes) == ndatabytes)
		    strncpy (chan->rf_p->wcsbuf, buf, SZ_WCSBUF);

		if (iis_debug) {
                    fprintf (stderr, "set wcs:\n");
                    write (2, buf, ndatabytes);
		}

		strcpy (chan->rf_p->ctran.format, W_DEFFORMAT);
		chan->rf_p->ctran.imtitle[0] = '\0';
		chan->rf_p->ctran.valid = 0;

		ct = wcs_update (xim, chan->rf_p);
		xim_message (xim, "frameTitle", ct->imtitle);
	    }
	    return;

	case IMCURSOR:
	    /* Read or write the logical image cursor.  This is an extension
	     * added to provide a high level cursor read facility; this is
	     * not the same as a low level access to the IIS cursor subunit.
	     * Cursor reads may be either nonblocking (immediate) or blocking,
	     * using the keyboard or mouse to terminate the read, and
	     * coordinates may be returned in either image (world) or frame
	     * buffer pixel coordinates.
	     */
	    if (iis.tid & IIS_READ) {
		/* Read the logical image cursor.  In the case of a blocking
		 * read all we do is initiate a cursor read; completion occurs
		 * when the user hits a key or button.
		 */
		if (iis_debug)
		    fprintf (stderr, "read cursor position\n");
		if (iis.tid & IMC_SAMPLE) {
		    /* Sample the cursor position and return the cursor value
		     * on the output datastream encoded in a fixed size
		     * ascii buffer.
		     */
		    int wcs = iis.z;
		    int raster, frame;
		    float sx, sy;
		    IoChanPtr sv_chan;

		    /* Save the cursor channel so that sampled cursor reads
		     * can occur on one channel without affecting any
		     * interactive cursor reads in progress on another
		     * channel.
		     */
		    sv_chan = xim->cursor_chan;
		    xim->cursor_chan = chan;
		    xim_getCursorPos (xim, &sx, &sy, &raster, &frame);
		    xim_retCursorVal (xim, sx, sy, frame, wcs, 0, "");
		    xim->cursor_chan = sv_chan;

		} else {
		    /* Initiate a user triggered cursor read. */
		    if (xim->cursor_chan) {
			int frame = xim->cursor_chan->reference_frame;
			xim_retCursorVal (xim, 0., 0., frame, 0, EOF, "");
		    }
		    xim->cursor_chan = chan;
		    xim_cursorMode (xim, 1);
		}

	    } else {
		/* Write (set) the logical image cursor position. */
		register CtranPtr ct;
		int sx = iis.x, sy = iis.y;
		float wx = sx, wy = sy;
		int wcs = iis.z;

		if (iis_debug)
		    fprintf(stderr, "write cursor position: [%d,%d]\n", sx, sy);
		if (wcs) {
		    ct = wcs_update (xim, xim->df_p);
		    if (ct->valid) {
			if (abs(ct->a) > .001)
			    sx = (wx - ct->tx) / ct->a;
			if (abs(ct->d) > .001)
			    sy = (wy - ct->ty) / ct->d;
		    }
		}

		xim_setCursorPos (xim, sx, sy);
	    }
	    return;

	default:
	    /* Ignore unsupported command input.
	     */
	    break;
	}

	/* Discard any data following the header. */
	if (!(iis.tid & IIS_READ))
	    for (nbytes = ndatabytes;  nbytes > 0;  nbytes -= n) {
		n = (nbytes < SZ_FIFOBUF) ? nbytes : SZ_FIFOBUF;
		if ((n = read (datain, buf, n)) <= 0)
		    break;
	    }
}


/* XIM_IISIOMAP, XIM_IISCOLORMAP -- Set the iomap and colormap for a Gterm
 * widget to emulate the color allocation scheme assumed by the imtool
 * protocol.
 *
 * The nominal pixel mappings are as follows.
 *
 *	Imtool		Gterm		Description
 *
 *	0		0		background
 *	1 - 200		10-209		image data
 *	201		210		cursor (white)
 *	202		0		background (black)		
 *	203		1		white
 *	204		2		red
 *	205		3		green
 *	206		4		blue
 *	207		6		yellow
 *	208		5		cyan
 *	209		7		magenta
 *	210		211		coral
 *	211		212		maroon
 *	212		213		orange
 *	213		214		khaki
 *	214		215		orchid
 *	215		216		turquoise
 *	216		217		violet
 *	217		218		wheat
 *
 * The Gterm color allocation is the standard 10 static colors, followed by
 * 200 dynamically allocated colors used to display image data, followed by
 * a few additional colors.  Only the block of 200 colors are subject to
 * modification when the display is windowed or a new colortable is loaded.
 *
 * If the gterm widget is permitted a sufficient number of dynamic colors then
 * pixels will be allocated as in the figure above.  If there are too few
 * colormap cells available to allocate all the colors then the image data
 * region of gterm colormap space will be shrunk to make up the difference.
 * The full set of static colors (imtool:201-217) are always allocated; it
 * is the range 1-200 which is reduced if there are too few colors.  The
 * iomap always makes it appear to the client that the full range of 200
 * image levels and 17 static colors are available, even if the actual number
 * of color cells available is less.
 *
 * The input arrays should be dimension of length 256.  The number of
 * iomap entries and number of RGB entries set is returned in the output
 * variables iomap_len and rgb_len.
 */
xim_iisiomap (w, iomap, iomap_len)
register XtPointer w;
unsigned short *iomap;
int *iomap_len;
{
	register int i;
	int first, nelem, maxelem;
	int delta;

	GtQueryColormap (w, 0, &first, &nelem, &maxelem);
	delta = min(maxelem,209) - 209;

	/* Set the iomap. */
	iomap[0] = 0;
	for (i=1;  i < 201;  i++)
	    iomap[i] = (200 + delta) * (i-1) / 200 + first;
	iomap[201] = first + 200 + delta;
	for (i=0;  i < 8;  i++)
	    iomap[202+i] = i;
	iomap[207] = 6;
	iomap[208] = 5;
	for (i=0;  i < 8;  i++)
	    iomap[210+i] = first + 201 + i + delta;
	*iomap_len = 218;
}


/* xim_iiscolormap -- This is the gterm colormap for the IIS emulation, set
 * before the IIS iomap is defined.  The first colormap cell corresponds to
 * first = gterm color 10.
 */
xim_iiscolormap (w, r, g, b, first, ngray, rgb_len)
register XtPointer w;
unsigned short *r, *g, *b;
int *first, *ngray, *rgb_len;
{
	register int i, j;
	int nelem, maxelem;
	int delta;

	GtQueryColormap (w, 0, first, &nelem, &maxelem);
	delta = min(maxelem,209) - 209;

	/* Set the colormap. */
	for (i=0;  i < 200 + delta;  i++)
	    r[i] = g[i] = b[i] = (i * 255 / (200 + delta));

        r[i] = 255;  g[i] = 255;  b[i] = 255;   i++;    /* 210=cursor */
        r[i] = 255;  g[i] = 127;  b[i] =   0;   i++;    /* 211=coral */
        r[i] = 142;  g[i] =  35;  b[i] = 107;   i++;    /* 212=maroon */
        r[i] = 204;  g[i] =  50;  b[i] =  50;   i++;    /* 213=orange */
        r[i] = 159;  g[i] = 159;  b[i] =  95;   i++;    /* 214=khaki */
        r[i] = 219;  g[i] = 112;  b[i] = 219;   i++;    /* 215=orchid */
        r[i] = 112;  g[i] = 219;  b[i] = 219;   i++;    /* 216=turquoise */
        r[i] = 159;  g[i] =  95;  b[i] = 159;   i++;    /* 217=violet */
        r[i] = 216;  g[i] = 216;  b[i] = 191;   i++;    /* 218=wheat */

	/* Scale colormap values to 16 bits. */
	for (j=0;  j < i;  j++) {
	    r[j] <<= 8;
	    g[j] <<= 8;
	    b[j] <<= 8;
	}

	*ngray = 200 + delta;
	*rgb_len = i;
}


/* SET_FBCONFIG -- Set the frame buffer configuration, or add additional
 * frames to the current configuration.
 */
static void
set_fbconfig (chan, config, frame)
IoChanPtr chan;
int config;
int frame;
{
	register XimDataPtr xim = (XimDataPtr) chan->xim;
	register FrameBufPtr fb = &xim->frames[frame-1];
	register int i;

	if (config != xim->fb_configno) {
	    /* Change the frame buffer configuration. */
	    xim_initialize (xim, config,
		max (xim->fb_config[config-1].nframes, frame), 1);

	} else if (frame > xim->nframes) {
	    /* Add additional frames.  */
	    for (i=1;  i <= frame;  i++) {
		fb = &xim->frames[i-1];
		if (fb->frameno != i)
		    xim_initFrame (xim, i, frame,
			&xim->fb_config[config-1], xim->memModel);
	    }
	}

	xim_setReferenceFrame (chan, frame);
}


/* DECODE_FRAMENO -- Decode encoded IIS register frame number.
 */
static
decode_frameno (z)
register int	z;
{
	register int	n;

	/* Get the frame number, encoded with a bit for each frame, 01 is
	 * frame 1, 02 is frame 2, 04 is frame 3, and so on.
	 */
	if (!z) z = 1;
	for (n=0;  !(z & 1);  z >>= 1)
	    n++;

	return (max (1, n + 1));
}


/* BSWAP2 - Move bytes from array "a" to array "b", swapping successive
 * pairs of bytes.  The two arrays may be the same but may not be offset
 * and overlapping.
 */
static
bswap2 (a, b, nbytes)
char 	*a, *b;		/* input array			*/
int	nbytes;		/* number of bytes to swap	*/
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


/* XIM_RETCURSORVAL -- Return the cursor value on the output datastream to
 * the client which requested the cursor read.
 */
xim_retCursorVal (xim, sx, sy, frame, wcs, key, strval)
register XimDataPtr xim;
float	sx, sy;			/* cursor screen coordinates */
int	frame;			/* frame number */
int	wcs;			/* nonzero if WCS coords desired */
int	key;			/* keystroke used as trigger */
char	*strval;		/* optional string value */
{
	register CtranPtr ct;
	int dataout, wcscode;
	char curval[SZ_IMCURVAL];
	char keystr[20];
	float wx, wy;

	if (xim->cursor_chan)
	    dataout = xim->cursor_chan->dataout;
	else
	    return;

	/* Compute cursor coordinates. */
	if (wcs) {
	    ct = wcs_update (xim, xim->df_p);
	    if (ct->valid) {
		/* The imtool WCS assumes that the center of the first display
		 * pixel is at (0,0) but actually it is at (0.5,0.5).
		 */
		sx -= 0.5;
		sy -= 0.5;

		if (abs(ct->a) > .001)
		    wx = ct->a * sx + ct->c * sy + ct->tx;
		if (abs(ct->d) > .001)
		    wy = ct->b * sx + ct->d * sy + ct->ty;
	    }
	} else {
	    wx = sx;
	    wy = sy;
	}

	/* Compute WCS code. */
	wcscode = frame * 100 + wcs;

	/* Encode the cursor value. */
	if (key == EOF)
	    sprintf (curval, "EOF\n");
	else {
	    if (isprint (key) && !isspace(key)) {
		keystr[0] = key;
		keystr[1] = '\0';
	    } else
		sprintf (keystr, "\\%03o", key);

	    sprintf (curval, "%10.3f %10.3f %d %s %s\n",
		wx, wy, wcscode, keystr, strval);
	}

	/* Send it to the client program and terminate cursor mode. */
	write (dataout, curval, sizeof(curval));
	xim_cursorMode (xim, 0);
	xim->cursor_chan = NULL;
}


/* XIM_ENCODEWCS -- Transform the input screen (raster) coordinates and
 * pixel value and return a string giving X, Y, and the pixel intensity in
 * world units.
 */
xim_encodewcs (xim, sx, sy, sz, obuf)
register XimDataPtr xim;
float sx, sy;			/* screen (raster) pixel coordinates */
int sz;				/* screen pixel value */
char *obuf;			/* receives encoded string */
{
	register FrameBufPtr fr = xim->df_p;
	register CtranPtr ct;
	float wx, wy, wz;
	int ch;

	ct = wcs_update (xim, fr);
        if (ct->valid) {
	    /* The imtool WCS assumes that the center of the first display
	     * pixel is at (0,0) but actually it is at (0.5,0.5).
	     */
	    sx -= 0.5;
	    sy -= 0.5;

            wx = ct->a * sx + ct->c * sy + ct->tx;
            wy = ct->b * sx + ct->d * sy + ct->ty;

            if (sz == 0)
                wz = 0.0;
            else {
                switch (ct->zt) {
                case W_LINEAR:
                    wz = ((sz - CMS_DATASTART) * (ct->z2 - ct->z1) /
                          (CMS_DATARANGE-1)) + ct->z1;
                    break;
                default:
                    wz = sz;
                    break;
                }
            }

        } else {
            wx = sx;
            wy = sy;
            wz = sz;
        }

        ch = ' ';
        if (sz && ct->valid) {
            if (ct->z1 < ct->z2) {
                if (wz < (ct->z1 + 0.01))
                    ch = '-';
                else if (wz > (ct->z2 - 0.01))
                    ch = '+';
            } else if (ct->z1 > ct->z2) {
                if (wz < (ct->z2 + 0.01))
                    ch = '-';
                else if (wz > (ct->z1 - 0.01))
                    ch = '+';
            }
        }

        sprintf (obuf, ct->format, wx + 0.005, wy + 0.005, wz, ch);
}


/* XIM_FRAMELABEL -- Return a pointer to the frame label string for the current
 * frame.
 */
char *
xim_frameLabel (xim)
register XimDataPtr xim;
{
	register FrameBufPtr df_p = xim->df_p;

	sprintf (df_p->label, "[%d] %s", df_p->frameno, df_p->ctran.imtitle);
	return (df_p->label);
}


/* WCS_UPDATE -- Load the screen WCS, if not yet validated, from the user
 * wcs file, if any.
 *
 * File format (two lines):
 *
 *	image title (imtool header label string)\n
 *	a b c d tx ty z1 z2 zt
 *
 * The WCS text is passed in via the data stream as a write to the subunit
 * WCS and left in the buffer "wcsbuf".
 */
static CtranPtr 
wcs_update (xim, fr)
register XimDataPtr xim;
FrameBufPtr fr;
{
	register CtranPtr ct = &fr->ctran;
	char buf[1024], *format;

	/* Get the new WCS. */
	if (!ct->valid) {
	    fr->label[0] = '\0';
	    ct->zt = W_UNITARY;

	    /* Attempt to read the WCS and set up a unitary transformation
	     * if the information cannot be read.
	     */
	    if (sscanf (fr->wcsbuf, "%[^\n]\n%f%f%f%f%f%f%f%f%d",
		buf, &ct->a, &ct->b, &ct->c, &ct->d, &ct->tx, &ct->ty,
		&ct->z1, &ct->z2, &ct->zt) < 7) {

		if (fr->wcsbuf[0])
		    fprintf (stderr, "imtool: error decoding WCS\n");

		strncpy (ct->imtitle, "[NO WCS]\n", SZ_IMTITLE);
		ct->a  = ct->d  = 1;
		ct->b  = ct->c  = 0;
		ct->tx = ct->ty = 0;
		ct->zt = W_UNITARY;

	    } else
		strncpy (ct->imtitle, buf, SZ_IMTITLE);

	    ct->valid++;
	}

	/* Determine best format for wcs output. */
	if (ct->valid && ct->zt == W_LINEAR) {
	    float   z1, z2, zrange;
	    z1 = ct->z1;
	    z2 = ct->z2;
	    zrange = (z1 > z2) ? z1 - z2 : z2 - z1;
	    if (zrange < 100.0 && (abs(z1) + abs(z2)) / 2.0 < 200.0)
		format = " %7.2f %7.2f %7.3f%c";
	    else if (zrange > 99999.0 || (abs(z1) + abs(z2)) / 2.0 > 99999.0)
		format = " %7.2f %7.2f %7.3g%c";
	    else
		format = W_DEFFORMAT;
	} else
	    format = " %7.2f %7.2f %7.0f%c";

	strcpy (ct->format, format);
	return (ct);
}
