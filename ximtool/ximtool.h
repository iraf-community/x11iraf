/*
 * XIMTOOL.H -- Global definitions for XImtool.
 */

#include "eps.h"

/* Default values, size limiting values.
 */
#define	MAX_FBCONFIG		128	/* max possible frame buf sizes	*/
#define	MAX_FRAMES		16	/* max number of frames		*/
#define	MAX_CLIENTS		8	/* max display server clients	*/
#define	MAX_COLORMAPS		256	/* max number of colormaps	*/
#define	MAX_COLORS		256	/* max size colormap		*/
#define	MAX_PRINTERS		128	/* max number of printers	*/
#define	FIRST_COLOR		10	/* first allocatable color	*/
#define	DEF_NCOLORS		201	/* default number of colors	*/
#define	DEF_COLORMAP		1	/* default colormap		*/
#define	DEF_NFRAMES		1	/* save memory; only one frame	*/
#define	DEF_FRAME_WIDTH		512	/* 512 square frame		*/
#define	DEF_FRAME_HEIGHT	512	/* 512 square frame		*/
#define	DEF_FRAME_DEPTH		8	/* 8 bits deep			*/
#define	DEF_WIN_WIDTH		512	/* default size window		*/
#define	DEF_WIN_HEIGHT		512	/* default size window		*/
#define	DEF_TILE_BORDER		3	/* border width for tileFrames	*/
#define DEF_BORDER_COLOR        "9"	/* border highlight color	*/
#define	SZ_CMAPNAME		32	/* colormap name buffer		*/
#define	SZ_NAME			80	/* object name buffer		*/
#define	SZ_LABEL		256	/* main frame label string	*/
#define	SZ_IMTITLE		128	/* image title string		*/
#define	SZ_WCTEXT		80	/* WCS box text 		*/
#define	SZ_WCSBUF		320	/* WCS text buffer size		*/
#define	SZ_MSGBUF		8192	/* message buffer size		*/
#define	SZ_COLORBAR		11	/* height of colorbar in pixels	*/
#define	SZ_FIFOBUF		4000	/* transfer size for FIFO i/o	*/

#define	SZ_FNAME		256
#define	SZ_LINE			256
#define	ERR			(-1)
#define	OK			0
#define	EOS			'\0'

#define	M_UNITARY		0	/* xim_setmapping		*/
#define	M_ASPECT		1
#define	M_FILL			2

/* Magic numbers. */
#define DEF_PORT		5137		/* default tcp/ip socket */
#define	I_DEVNAME		"/dev/imt1o"	/* pseudo device names */
#define	O_DEVNAME		"/dev/imt1i"	/* our IN is client's OUT */
#define	DEF_UNIXADDR		"/tmp/.IMT%d"	/* default unix socket */
#define DEF_ANTIALIASTYPE	"boxcar"	/* default antialiasing	*/
#define	FBCONFIG_1		".imtoolrc"
#define	FBCONFIG_2		"/usr/local/lib/imtoolrc"
#define	CMAPCONFIG		"/usr/local/lib/imtoolcmap"
#define	FBCONFIG_ENV1		"imtoolrc"
#define	FBCONFIG_ENV2		"IMTOOLRC"
#define	PRINTCONFIG		"/usr/local/lib/ximprint.cfg"

/* WCS definitions. */
#define	W_UNITARY	0
#define	W_LINEAR	1
#define	W_LOG		2
#define	W_DEFFORMAT	" %7.2f %7.2f %7.1f%c"

/* Rotation matrix defining world coordinate system (WCS) of a frame.  */
typedef struct {
	int valid;			/* has WCS been set?		*/
	float a, b;			/* x, y scale factors		*/
	float c, d;			/* x, y cross factors		*/
	float tx, ty;			/* x, y translation		*/
	float z1, z2;			/* greyscale range		*/
	int zt;				/* greyscale mapping		*/
	char format[32];		/* wcs output format		*/
	char imtitle[SZ_IMTITLE+1];	/* image title from WCS	*/
} Ctran, *CtranPtr;

/* The frame buffers. */
typedef struct {
	int frameno;			/* frame number			*/
	int raster;			/* frame buffer raster		*/
	int zoomras;			/* zoom/pan raster		*/
	int zoommap;			/* zoom/pan mapping		*/
	int dispmap;			/* mapping used for display	*/
	int colormap;			/* greyscale transformation	*/
	float offset, scale;		/* transfer function 		*/
	float xscale, yscale;		/* scaling at [xy]mag==1.0	*/
	float xmag, ymag;		/* zoom/dezoom factors		*/ 
	float xcen, ycen; 		/* center of zoomed region	*/
	int xflip, yflip;		/* flip in X or Y?		*/
	char label[SZ_LABEL+1];		/* frame label string		*/
	Ctran ctran;			/* world coordinate system	*/
	char wcsbuf[SZ_WCSBUF];		/* wcs info string		*/
} FrameBuf, *FrameBufPtr;

/* Possible frame buffer sizes. */
typedef struct {
	int nframes;			/* number of frames		*/
	int width;			/* frame buffer width		*/
	int height;			/* frame buffer height		*/
} FbConfig, *FbConfigPtr;


/* Predefined colormaps. */
typedef struct {
	int mapno;			/* widget colormap number */
	char name[SZ_CMAPNAME+1];	/* colormap name */
} ColorMap, *ColorMapPtr;

/* Predefined lookup tables. */
typedef struct {
	float red, green, blue;
} Triplet, *TripletPtr;

typedef struct {
	int	lutlen;
	Triplet hue[MAX_COLORS];
} Lut, *LutPtr;


/* Client I/O channel. */
typedef struct {
	XtPointer xim;			/* backpointer to xim descriptor */
	XtPointer id;			/* input callback id */
	int type;			/* channel type */
	int datain;			/* input channel */
	int dataout;			/* output channel */
	int keepalive;			/* used to keep input fifo ready */
	char path[SZ_FNAME+1];		/* for unix sockets */
	int reference_frame;		/* reference (cmd i/o) frame */
	FrameBufPtr rf_p;		/* reference frame descriptor */
} IoChan, *IoChanPtr;

#define	IO_FIFO		1
#define	IO_INET		2
#define	IO_UNIX		3


/* Printer list. */
typedef struct {
	char printerName[SZ_FNAME+1];	/* printer name */
	char printCmd[SZ_FNAME+1];	/* printer dispose command */
} Printer, *PrinterPtr;


/* Printer configuration struct. */
typedef struct {
	int  printno;			/* printer number */
	int  seqno;			/* sequence number */
	int  diskfile;			/* print to diskfile? */
	char printFile[SZ_FNAME+1];	/* disk filename template */
	char printCmd[SZ_FNAME+1];	/* dispose command */
} PrintCfg, *PrintCfgPtr;

/* File save definitions and structure. */
typedef struct {
	int  seqno;			/* sequence number */
	int  format;			/* save format */
	int  colorType;			/* save color type */
	int  w, h, d;			/* dimensions of last file saved */
	char fname[SZ_FNAME+1];		/* save filename */
} fileSave, *fileSavePtr;

#define	XIM_GRAYSCALE	0		/* save color options */
#define	XIM_PSEUDOCOLOR	1
#define	XIM_RGB		2

#define	XIM_RAS		0		/* save format options */
#define	XIM_GIF		1
#define	XIM_TIFF	2
#define	XIM_JPEG	3
#define	XIM_X11		4
#define	XIM_FITS	5
#define	XIM_RAW		6
#define	XIM_PNM		7
#define	XIM_OIF		8


/* File load struct. */
typedef struct {
	int  nfiles;			/* number of files in directory */
	char **FileList;		/* list of directory contents */
	char curdir[SZ_FNAME+1];	/* current directory */
	char homedir[SZ_FNAME+1];	/* home directory */
	char pattern[SZ_NAME+1];	/* file pattern to match */
	int  gray;			/* load as a grayscale image? */
} fileLoad, *fileLoadPtr;

/*
 * Application resources and runtime descriptor.
 * ----------------------------------------------
 */
typedef struct {
	/* Resources. */
	Boolean autoscale;		/* is XY autoscaling enabled */
	Boolean antialias;		/* apply antialiasing when dezooming */
	Boolean tileFrames;		/* tile rather than overlay frames */
	Boolean highlightFrames;	/* highlight tiled frames */
	Boolean invert;			/* use inverted colormap */
	int def_config;			/* default FB config */
	int def_nframes;		/* default number of frames */
	int ncolors;			/* number of image pixel colors */
	int tileBorder;			/* image border when tiling frames */
	String borderColor;		/* border color for tileFrames */
	String gui;			/* GUI file name */
	String imtoolrc;		/* imtoolrc file name */
	String memModel;		/* FB memory model */
	String userCMap1;		/* user colormap file */
	String userCMap2;		/* user colormap file */
	String userCMapDir1;		/* user colormap directory */
	String userCMapDir2;		/* user colormap directory */
	String antialiasType;		/* type of antialiasing */
	String printConfig;		/* printer configuration file */
	String input_fifo;		/* client's output, e.g. /dev/imt1o */
	String output_fifo;		/* client's input, e.g. /dev/imt1i */
	String unixaddr;		/* format for unix socket path */
	int port;			/* port for INET socket */

	/* Internal state. */
	XtPointer obm;			/* object manager */
	IoChan chan[MAX_CLIENTS];	/* client i/o descriptors */
	IoChanPtr cursor_chan;		/* cursor mode channel */
	Widget toplevel;		/* dummy toplevel app shell */
	Widget gt;			/* imagewin gterm-image widget */
	Widget cb;			/* colorbar gterm-image widget */
	XtPointer gm_border;		/* border marker for tileFrames */
	int tileFramesList;		/* frames to be tiled (bitmask) */
	int nTileFrames;		/* number of frames to be tiled */
	int rop;			/* rasterop for mappings */
	int display_frame;		/* currently displayed frame */
	FrameBufPtr df_p;		/* display frame descriptor */
	FrameBuf frames[MAX_FRAMES];	/* array of frame descriptors */
	int fb_configno;		/* current config number */
	int nframes;			/* current number of frame bufs */
	int width, height;		/* current width, height */
	FbConfig fb_config[MAX_FBCONFIG];	/* fb config table */
	int *clientPrivate;		/* used by imtool client code */

	PSImagePtr psim;		/* EPS image struct pointer */
	PrintCfgPtr pcp;		/* printer config pointer */
	fileLoadPtr flp;		/* load disk file pointer */
	fileSavePtr fsp;		/* save disk file pointer */

} XimData, *XimDataPtr;


#ifdef XIMTOOL_MAIN
XimData ximtool_data;

#define	XtNdefConfig		"defConfig"
#define	XtCDefConfig		"DefConfig"
#define	XtNdefNFrames		"defNFrames"
#define	XtCDefNFrames		"DefNFrames"
#define	XtNncolors		"ncolors"
#define	XtCNcolors		"Ncolors"
#define	XtNtileBorderWidth	"tileBorderWidth"
#define	XtCTileBorderWidth	"TileBorderWidth"
#define	XtNtileBorderColor	"tileBorderColor"
#define	XtCTileBorderColor	"TileBorderColor"
#define	XtNautoscale		"autoscale"
#define	XtCAutoscale		"Autoscale"
#define	XtNantialias		"antialias"
#define	XtCAntialias		"Antialias"
#define	XtNantialiasType	"antialiasType"
#define	XtCAntialiasType	"AntialiasType"
#define	XtNinvert		"invert"
#define	XtCInvert		"Invert"
#define	XtNtileFrames		"tileFrames"
#define	XtCTileFrames		"TileFrames"
#define	XtNhighlightFrames	"highlightFrames"
#define	XtCHighlightFrames	"HighlightFrames"
#define	XtNgui			"gui"
#define	XtCGui			"Gui"
#define	XtNimtoolrc		"imtoolrc"
#define	XtCImtoolrc		"Imtoolrc"
#define	XtNmemModel		"memModel"
#define	XtCMemModel		"MemModel"
#define	XtNuserCMap1		"cmap1"
#define	XtNuserCMap2		"cmap2"
#define	XtCUserCMap		"UserCMap"
#define	XtNuserCMapDir1		"cmapDir1"
#define	XtNuserCMapDir2		"cmapDir2"
#define	XtCUserCMapDir		"UserCMapDir"
#define	XtNprintConfig		"printConfig"
#define	XtCprintConfig		"printConfig"
#define	XtNinput_fifo		"input_fifo"
#define	XtCInput_fifo		"Input_fifo"
#define	XtNoutput_fifo		"output_fifo"
#define	XtCOutput_fifo		"Output_fifo"
#define	XtNunixaddr		"unixaddr"
#define	XtCUnixaddr		"Unixaddr"
#define	XtNport			"port"
#define	XtCPort			"Port"

static XtResource resources[] = {
    {
	XtNdefConfig,
	XtCDefConfig,
	XtRInt,
	sizeof(int),
	XtOffsetOf(XimData, def_config),
	XtRImmediate,
	(caddr_t)1
    },
    {
	XtNdefNFrames,
	XtCDefNFrames,
	XtRInt,
	sizeof(int),
	XtOffsetOf(XimData, def_nframes),
	XtRImmediate,
	(caddr_t)0
    },
    {
	XtNncolors,
	XtCNcolors,
	XtRInt,
	sizeof(int),
	XtOffsetOf(XimData, ncolors),
	XtRImmediate,
	(caddr_t)DEF_NCOLORS
    },
    {
	XtNtileBorderWidth,
	XtCTileBorderWidth,
	XtRInt,
	sizeof(int),
	XtOffsetOf(XimData, tileBorder),
	XtRImmediate,
	(caddr_t)DEF_TILE_BORDER
    },
    {
	XtNtileBorderColor,
	XtCTileBorderColor,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, borderColor),
	XtRImmediate,
	(caddr_t)DEF_BORDER_COLOR
    },
    {
	XtNautoscale,
	XtCAutoscale,
	XtRBoolean,
	sizeof(Boolean),
	XtOffsetOf(XimData, autoscale),
	XtRImmediate,
	(caddr_t)FALSE
    },
    {
	XtNantialias,
	XtCAntialias,
	XtRBoolean,
	sizeof(Boolean),
	XtOffsetOf(XimData, antialias),
	XtRImmediate,
	(caddr_t)FALSE
    },
    {
	XtNantialiasType,
	XtCAntialiasType,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, antialiasType),
	XtRImmediate,
	(caddr_t)DEF_ANTIALIASTYPE
    },
    {
	XtNinvert,
	XtCInvert,
	XtRBoolean,
	sizeof(Boolean),
	XtOffsetOf(XimData, invert),
	XtRImmediate,
	(caddr_t)FALSE
    },
    {
	XtNtileFrames,
	XtCTileFrames,
	XtRBoolean,
	sizeof(Boolean),
	XtOffsetOf(XimData, tileFrames),
	XtRImmediate,
	(caddr_t)FALSE
    },
    {
	XtNhighlightFrames,
	XtCHighlightFrames,
	XtRBoolean,
	sizeof(Boolean),
	XtOffsetOf(XimData, highlightFrames),
	XtRImmediate,
	(caddr_t)TRUE
    },
    {
	XtNgui,
	XtCGui,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, gui),
	XtRImmediate,
	(caddr_t)"default"
    },
    {
	XtNimtoolrc,
	XtCImtoolrc,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, imtoolrc),
	XtRImmediate,
	(caddr_t)FBCONFIG_2
    },
    {
	XtNmemModel,
	XtCMemModel,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, memModel),
	XtRImmediate,
	(caddr_t)"fast"
    },
    {
	XtNuserCMap1,
	XtCUserCMap,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, userCMap1),
	XtRImmediate,
	(caddr_t)"none"
    },
    {
	XtNuserCMap2,
	XtCUserCMap,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, userCMap2),
	XtRImmediate,
	(caddr_t)"none"
    },
    {
	XtNuserCMapDir1,
	XtCUserCMapDir,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, userCMapDir1),
	XtRImmediate,
	(caddr_t)"none"
    },
    {
	XtNuserCMapDir2,
	XtCUserCMapDir,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, userCMapDir2),
	XtRImmediate,
	(caddr_t)CMAPCONFIG
    },
    {
	XtNprintConfig,
	XtCprintConfig,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, printConfig),
	XtRImmediate,
	(caddr_t)PRINTCONFIG
    },
    {
	XtNinput_fifo,
	XtCInput_fifo,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, input_fifo),
	XtRImmediate,
	(caddr_t)O_DEVNAME
    },
    {
	XtNoutput_fifo,
	XtCOutput_fifo,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, output_fifo),
	XtRImmediate,
	(caddr_t)I_DEVNAME
    },
    {
	XtNunixaddr,
	XtCUnixaddr,
	XtRString,
	sizeof(String),
	XtOffsetOf(XimData, unixaddr),
	XtRImmediate,
	(caddr_t)DEF_UNIXADDR
    },
    {
	XtNport,
	XtCPort,
	XtRInt,
	sizeof(int),
	XtOffsetOf(XimData, port),
	XtRImmediate,
	(caddr_t)DEF_PORT
    },
};

int ncolormaps = 0;
int first_color = FIRST_COLOR;

ColorMap colormaps[MAX_COLORMAPS] = {
    {	0,  "Grayscale" },
    {	0,  "Color" },
    {	0,  "Heat" },
    {	0,  "HSV" },
    {	0,  "AIPS0" },
    {	0,  "Halley" },
    {	0,  "Ramp" },
    {	0,  "Standard" },
    {	0,  "Staircase" },
    {	0,  "Rainbow1" },
    {	0,  "Rainbow2" },
    {	0,  "Random" },
    {	0,  "Random8" },
    {	0,  "Random16" },
    {	0,  "Red" },
    {	0,  "Green" },
    {	0,  "Blue" },
};


int nprinters = 0;
Printer printer_list[MAX_PRINTERS] = {
    {	"default", "lpr" },	     /* should do more interesting examples */
    {	"clp",     "lpr -Pclp" },
    {	"lw1",     "lpr -Plw1" },
    {	"lw2",     "lpr -Plw2" },
    {	"lw3",     "lpr -Plw3" },
    {	"lw4",     "lpr -Plw4" },
    {	"lw5",     "lpr -Plw5" },
    {	"lw6",     "lpr -Plw6" },
    {	"lw7",     "lpr -Plw7" },
    {	"lw8",     "lpr -Plw8" },
    {	"lw9",     "lpr -Plw9" },
    {	"lw10",    "lpr -Plw10" },
    {	"lw11",    "lpr -Plw11" },
    {	"lw12",    "lpr -Plw12" },
    {	"lw13",    "lpr -Plw13" },
    {	"lw14",    "lpr -Plw14" },
    {	"lw15",    "lpr -Plw15" },
    {	"lw16",    "lpr -Plw16" },
    {	"lw18",    "lpr -Plw18" },
    {	"lw19",    "lpr -Plw19" },
    {	"lw20",    "lpr -Plw20" },
    {	"lw21",    "lpr -Plw21" },
    {	"lw22",    "lpr -Plw22" },
    {	"lw23",    "lpr -Plw23" },
    {	"lw24",    "lpr -Plw24" },
    {	"lw25",    "lpr -Plw25" },
    {	"lw26",    "lpr -Plw26" },
    {	"lw27",    "lpr -Plw27" },
    {	"\0",      "\0" },
};


#else

extern XtResource resources[];
extern XimData ximtool_data;
extern int ncolormaps;
extern int first_color;
extern ColorMap colormaps[];
extern int nprinters;
extern Printer printer_list[];

#endif

/* Functions.
 */
#ifndef abs
#define	abs(a)		(((a)<0)?(-(a)):(a))
#endif
#ifndef min
#define min(a,b)	((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b)	((a)<(b)?(b):(a))
#endif

void xim_initialize(), xim_reset(), xim_resize(), xim_refresh();
void xim_close(), xim_initFrame(), xim_setFrame(), xim_setRop();
void xim_setReferenceFrame(), xim_setDisplayFrame();
void xim_delFrame(), xim_setMapping(), xim_setZoom();
void xim_setCursorPos(), xim_getCursorPos(), xim_setFlip();
void xim_matchFrames(), xim_registerFrames();
void xim_readCursor(), xim_frameChanged();
void xim_message(), xim_msgi(), xim_alert();
int xim_setColormap();
int xim_getAntialias();
char *xim_frameLabel();

int xim_writeDisplay();
unsigned char *xim_readDisplay();
void xim_initSave(), xim_closeSave();
void ximp_rename(), ximp_cancel();
void xims_rename(), xims_cancel();
int xim_saveFile();

int xim_iisopen();
void xim_iisclose(), xim_iisio();
XtPointer xim_addInput();
void xim_removeInput();

void xim_clientOpen(), xim_clientClose();
int xim_clientExecute();
