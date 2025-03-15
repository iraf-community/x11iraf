#ifndef _Gterm_h
#define _Gterm_h

/* Parameters:
(this is not yet updated for xgterm - the rest of this file is)

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 background	     Background		Pixel		White
 border		     BorderColor	Pixel		XtDefaultForeground
 borderWidth	     BorderWidth	Dimension	1
 destroyCallback     Callback		Pointer		NULL
 foreground	     Foreground		Pixel		Black
 height		     Height		Dimension	240
 mappedWhenManaged   MappedWhenManaged	Boolean		True
 reverseVideo	     ReverseVideo	Boolean		False
 width		     Width		Dimension	320
 x		     Position		Position	0
 y		     Position		Position	0

*/

/*
 * Resource definitions.
 */
#define XtCInt				"Int"

#define XtNalphaFont1			"alphaFont1"
#define XtNalphaFont2			"alphaFont2"
#define XtNalphaFont3			"alphaFont3"
#define XtNalphaFont4			"alphaFont4"
#define XtNalphaFont5			"alphaFont5"
#define XtNalphaFont6			"alphaFont6"
#define XtNalphaFont7			"alphaFont7"
#define XtNalphaFont8			"alphaFont8"

#define XtNdialogFont1			"dialogFont1"
#define XtNdialogFont2			"dialogFont2"
#define XtNdialogFont3			"dialogFont3"
#define XtNdialogFont4			"dialogFont4"
#define XtNdialogFont5			"dialogFont5"
#define XtNdialogFont6			"dialogFont6"
#define XtNdialogFont7			"dialogFont7"
#define XtNdialogFont8			"dialogFont8"

#define XtNdialogBgColor		"dialogBgColor"
#define XtNdialogFgColor		"dialogFgColor"
#define XtNidleCursorBgColor		"idleCursorBgColor"
#define XtNidleCursorFgColor		"idleCursorFgColor"
#define XtNbusyCursorBgColor		"busyCursorBgColor"
#define XtNbusyCursorFgColor		"busyCursorFgColor"
#define XtNginmodeCursorBgColor		"ginmodeCursorBgColor"
#define XtNginmodeCursorFgColor		"ginmodeCursorFgColor"
#define XtNginmodeBlinkInterval		"ginmodeBlinkInterval"
#define XtNcrosshairCursorColor		"crosshairCursorColor"

#define XtNidleCursor			"idleCursor"
#define XtNbusyCursor			"busyCursor"
#define XtNginmodeCursor		"ginmodeCursor"
#define XtNwarpCursor			"warpCursor"
#define XtNraiseWindow			"raiseWindow"
#define XtNdeiconifyWindow		"deiconifyWindow"
#define XtNuseTimers			"useTimers"

#define XtNcolor0			"color0"
#define XtNcolor1			"color1"
#define XtNcolor2			"color2"
#define XtNcolor3			"color3"
#define XtNcolor4			"color4"
#define XtNcolor5			"color5"
#define XtNcolor6			"color6"
#define XtNcolor7			"color7"
#define XtNcolor8			"color8"
#define XtNcolor9			"color9"

#define XtNcmapName			"cmapName"
#define XtNuseGlobalCmap                "useGlobalCmap"
#define XtNcmapInitialize		"cmapInitialize"
#define XtNcopyOnResize			"copyOnResize"
#define XtNbasePixel			"basePixel"
#define XtNcmapUpdate			"cmapUpdate"
#define XtNcmapShadow			"cmapShadow"
#define XtNcmapInterpolate		"cmapInterpolate"
#define XtNmaxRasters			"maxRasters"
#define XtNcacheRasters			"cacheRasters"
#define XtNmaxMappings			"maxMappings"
#define XtNmaxColors			"maxColors"

#define XtNmarkerTranslations		"markerTranslations"
#define XtNdefaultMarker		"defaultMarker"
#define XtNnearEdge			"nearEdge"
#define XtNnearVertex			"nearVertex"
#define XtNmarkerLineWidth		"markerLineWidth"
#define XtNmarkerLineStyle		"markerLineStyle"
#define XtNmarkerFill			"markerFill"
#define XtNmarkerFillColor		"markerFillColor"
#define XtNmarkerFillBgColor		"markerFillBgColor"
#define XtNmarkerFillStyle		"markerFillStyle"
#define XtNxorFill			"xorFill"
#define XtNxorFillColor			"xorFillColor"
#define XtNxorFillBgColor		"xorFillBgColor"
#define XtNmarkerHighlightWidth		"markerHighlightWidth"
#define XtNmarkerHighlightColor		"markerHighlightColor"
#define XtNmarkerCursorFgColor		"markerCursorFgColor"
#define XtNmarkerCursorBgColor		"markerCursorBgColor"
#define XtNmarkerLineLineColor		"markerLineLineColor"
#define XtNmarkerLineKnotColor		"markerLineKnotColor"
#define XtNmarkerLineKnotSize		"markerLineKnotSize"
#define XtNmarkerTextLineColor		"markerTextLineColor"
#define XtNmarkerTextColor		"markerTextColor"
#define XtNmarkerTextBgColor		"markerTextBgColor"
#define XtNmarkerTextBorder		"markerTextBorder"
#define XtNmarkerTextFont		"markerTextFont"
#define XtNmarkerTextString		"markerTextString"
#define XtNmarkerRectLineColor		"markerRectLineColor"
#define XtNmarkerRectKnotColor		"markerRectKnotColor"
#define XtNmarkerRectKnotSize		"markerRectKnotSize"
#define XtNmarkerBoxLineColor		"markerBoxLineColor"
#define XtNmarkerBoxKnotColor		"markerBoxKnotColor"
#define XtNmarkerBoxKnotSize		"markerBoxKnotSize"
#define XtNmarkerCircleLineColor	"markerCircleLineColor"
#define XtNmarkerCircleKnotColor	"markerCircleKnotColor"
#define XtNmarkerCircleKnotSize		"markerCircleKnotSize"
#define XtNmarkerEllipseLineColor	"markerEllipseLineColor"
#define XtNmarkerEllipseKnotColor	"markerEllipseKnotColor"
#define XtNmarkerEllipseKnotSize	"markerEllipseKnotSize"
#define XtNmarkerPgonLineColor		"markerPgonLineColor"
#define XtNmarkerPgonKnotColor		"markerPgonKnotColor"
#define XtNmarkerPgonKnotSize		"markerPgonKnotSize"


/* Deep Frame Definitions */
#define XtNdialogBgColorStr             "dialogBgColorStr"
#define XtNdialogFgColorStr             "dialogFgColorStr"
#define XtNidleCursorBgColorStr         "idleCursorBgColorStr"
#define XtNidleCursorFgColorStr         "idleCursorFgColorStr"
#define XtNbusyCursorBgColorStr         "busyCursorBgColorStr"
#define XtNbusyCursorFgColorStr         "busyCursorFgColorStr"
#define XtNginmodeCursorBgColorStr      "ginmodeCursorBgColorStr"
#define XtNginmodeCursorFgColorStr      "ginmodeCursorFgColorStr"
#define XtNcrosshairCursorColorStr      "crosshairCursorColorStr"

#define XtNcolor0Str                    "color0Str"
#define XtNcolor1Str                    "color1Str"
#define XtNcolor2Str                    "color2Str"
#define XtNcolor3Str                    "color3Str"
#define XtNcolor4Str                    "color4Str"
#define XtNcolor5Str                    "color5Str"
#define XtNcolor6Str                    "color6Str"
#define XtNcolor7Str                    "color7Str"
#define XtNcolor8Str                    "color8Str"
#define XtNcolor9Str                    "color9Str"

#define XtNmarkerFillColorStr           "markerFillColorStr"
#define XtNmarkerFillBgColorStr         "markerFillBgColorStr"

#define XtNmarkerHighlightColorStr      "markerHighlightColorStr"
#define XtNmarkerCursorFgColorStr       "markerCursorFgColorStr"
#define XtNmarkerCursorBgColorStr       "markerCursorBgColorStr"

#define XtNmarkerLineLineColorStr       "markerLineLineColorStr"
#define XtNmarkerLineKnotColorStr       "markerLineKnotColorStr"

#define XtNmarkerTextLineColorStr       "markerTextLineColorStr"
#define XtNmarkerTextColorStr           "markerTextColorStr"
#define XtNmarkerTextBgColorStr         "markerTextBgColorStr"

#define XtNmarkerRectLineColorStr       "markerRectLineColorStr"
#define XtNmarkerRectKnotColorStr       "markerRectKnotColorStr"

#define XtNmarkerBoxLineColorStr        "markerBoxLineColorStr"
#define XtNmarkerBoxKnotColorStr        "markerBoxKnotColorStr"

#define XtNmarkerCircleLineColorStr     "markerCircleLineColorStr"
#define XtNmarkerCircleKnotColorStr     "markerCircleKnotColorStr"

#define XtNmarkerEllipseLineColorStr    "markerEllipseLineColorStr"
#define XtNmarkerEllipseKnotColorStr    "markerEllipseKnotColorStr"

#define XtNmarkerPgonLineColorStr       "markerPgonLineColorStr"
#define XtNmarkerPgonKnotColorStr       "markerPgonKnotColorStr"

#define XtNmarkerPointLineColorStr      "markerPointLineColorStr"
#define XtNmarkerPointKnotColorStr      "markerPointKnotColorStr"
/* Deep Frame Definitions */



/*
 * Gterm widget flags.
 */
#define GtSet			1		/* drawing mode */
#define GtClear			2
#define GtInvert		3

#define GtOutline		1		/* line styles */
#define GtPoint			2
#define GtSolid			3
#define GtDashed		4
#define GtDotted		5
#define GtDashDot		6
#define GtDash3Dot		7

#define GtNoCursor		0		/* cursor types */
#define GtGinmodeCursor		1
#define GtBusyCursor		2
#define GtIdleCursor		3

#define GtDefault		0		/* raster types */
#define GtClient		1
#define GtServer		2

#define GtPixel			0		/* coordinate types */
#define GtNDC			1

#define	GtMap			0		/* mapping direction */
#define GtUnmap			1

#define GtWindow		0		/* drawable types */
#define GtWidget		1

#define R_OpcodeMask		0000017		/* rasterop definitions */
#define R_Transient		0000020
#define R_RefreshAll		0000040
#define R_RefreshNone		0000100
#define	R_MFMask		0777000

#define	MF_NEAREST		0001000		/* antialiasing functions */
#define	MF_BILINEAR		0002000
#define	MF_AREA			0004000
#define	MF_BLKAVG		0010000
#define	MF_BOXCAR		0020000
#define	MF_LOWPASS		0040000
#define	MF_GAUSSIAN		0100000

#define	GmText			"text"		/* graphics marker types */
#define	GmLine			"line"
#define	GmPolyline		"polyline"
#define	GmRectangle		"rectangle"
#define	GmBox			"box"
#define	GmCircle		"circle"
#define	GmEllipse		"ellipse"
#define	GmPolygon		"polygon"

#define	Gm_Text			1		/* integer codes for above */
#define	Gm_Line			2
#define	Gm_Polyline		3
#define	Gm_Rectangle		4
#define	Gm_Box			5
#define	Gm_Circle		6
#define	Gm_Ellipse		7
#define	Gm_Polygon		8
#define	Gm_NTypes		8

#define	GmType			"type"		/* marker attributes */
#define	GmActivated		"activated"
#define	GmVisible		"visible"
#define	GmSensitive		"sensitive"
#define	GmAutoRedraw		"autoRedraw"
#define	GmTranslations		"translations"
#define	GmX			"x"
#define	GmY			"y"
#define	GmWidth			"width"
#define	GmHeight		"height"
#define	GmRotangle		"rotangle"
#define	GmHighlightColor	"highlightColor"
#define	GmLineColor		"lineColor"
#define	GmLineWidth		"lineWidth"
#define	GmLineStyle		"lineStyle"
#define	GmKnotColor		"knotColor"
#define	GmKnotSize		"knotSize"
#define	GmFill			"fill"
#define	GmFillColor		"fillColor"
#define	GmFillBgColor		"fillBgColor"
#define	GmFillPattern		"fillPattern"
#define	GmFillStyle		"fillStyle"
#define	GmTextColor		"textColor"
#define	GmTextBgColor		"textBgColor"
#define	GmTextBorder		"textBorder"
#define	GmImageText		"imageText"
#define	GmFont			"font"
#define	GmRotIndicator		"rotIndicator"			/* MF020 */

#define	GmEvNotify		00001		/* marker callback events */
#define	GmEvMoveResize		00002
#define	GmEvModify		00004
#define	GmEvRedraw		00010
#define	GmEvDestroy		00020
#define	GmEvInput		00040
#define	GmEvFocusIn		00100
#define	GmEvFocusOut		00200
#define	GmEvConstraint		00400

/* Double version of XPoint. */
struct dPoint {
	double x;
	double y;
};
typedef struct dPoint DPoint;

typedef struct _GtermRec *GtermWidget;
typedef struct _GtermClassRec *GtermWidgetClass;
typedef struct marker *Marker;

extern WidgetClass gtermWidgetClass;

/*
 * Gterm function declarations
 */
typedef unsigned char	uchar;
typedef void (*GtCallbackProc)();
typedef int  (*GmIMethod)();

extern void GtActivate(GtermWidget w);
extern void GtDeactivate(GtermWidget w);
extern void GtReset(GtermWidget w);
extern void GtFlush(GtermWidget w);
extern int GtReady(GtermWidget w);
extern void GtTimerInhibit(GtermWidget w, Boolean state);
extern void GtSetDataLevel(GtermWidget w, int ival);
extern void GtDeleteInputProc(GtermWidget w, GtCallbackProc userfcn, XtPointer client_data);
extern void GtDeleteResizeProc(GtermWidget w, GtCallbackProc userfcn, XtPointer client_data);
extern void GtDeleteResetProc(GtermWidget w, GtCallbackProc userfcn, XtPointer client_data);
extern void GtPostInputProc(GtermWidget w, GtCallbackProc userfcn, XtPointer client_data);
extern void GtPostResetProc(GtermWidget w, GtCallbackProc userfcn, XtPointer client_data);
extern void GtPostResizeProc(GtermWidget w, GtCallbackProc userfcn, XtPointer client_data);
extern int GtQueryRaster(GtermWidget w, int raster, int *type,
			 int *width, int *height, int *depth);
extern void GtSetRaster(GtermWidget w, int raster);
extern int GtGetRaster(GtermWidget w);
extern void GtSetCursorPos(GtermWidget w, int x, int y);
extern void GtGetCursorPos(GtermWidget w, int *x, int *y);
extern void GtSetCursorType(GtermWidget w, int type);
extern void GtBell(GtermWidget w);
extern void GtSetLogRes(GtermWidget w, int width, int height);
extern void GtGetLogRes(GtermWidget w, int *width, int *height);
extern void GtGetPhysRes(GtermWidget w, int raster, int *width, int *height);
extern void GtSetPhysRes(GtermWidget w, int raster, int width, int height);
extern void GtSetLineWidth(GtermWidget w, int ival);
extern void GtSetLineStyle(GtermWidget w, int ival);
extern void GtSetColorIndex(GtermWidget w, int ival);
extern void GtSetFillType(GtermWidget w, int ival);
extern void GtClearScreen(GtermWidget w);
extern void GtDrawPolyline(GtermWidget w, XPoint *pv, int npts);
extern void GtDrawPolymarker(GtermWidget w, XPoint *pv, int npts);
extern void GtDrawPolygon(GtermWidget w, XPoint *pv, int npts);
extern void GtDrawAlphaText(GtermWidget w, int x, int y, char *text);
extern void GtGetAlphaTextSize(GtermWidget w, char *string, int *width, int *height, int *base);
extern void GtStartDialog(GtermWidget w);
extern void GtEndDialog(GtermWidget w);
extern void GtEraseDialog(GtermWidget w);
extern void GtDrawDialogText(GtermWidget w, int x, int y, char *text);
extern void GtGetDialogTextSize(GtermWidget w, char *string, int *width, int *height, int *base);
extern void GtRasterInit(GtermWidget w);
extern void initialize_shadow_pixmap (GtermWidget w, int dst);
extern int GtAssignRaster(GtermWidget w, int raster, XtPointer drawable, int type);
extern int GtCreateRaster(GtermWidget w, int raster, int type,
			  int width, int height, int depth);
extern void GtDestroyRaster(GtermWidget w, int raster);
extern int GtNextRaster(GtermWidget w);
extern int GtNRasters(GtermWidget w);
extern int GtWritePixels(GtermWidget w, int raster, uchar *pixels, int nbits,
			 int x1, int y1, int nx, int ny);
extern int GtReadPixels(GtermWidget w, int raster, uchar *pixels, int nbits,
			int x1, int y1, int nx, int ny);
extern void GtRefreshPixels(GtermWidget w, int raster, int ct,
			    int x1, int y1, int nx, int ny);
extern int GtGetClientPixel(GtermWidget w, int pixel);
extern int GtSetPixels(GtermWidget w, int raster, int ct, int x1, int y1,
		       int nx, int ny, int color, int rop);
extern Pixmap GtExtractPixmap(GtermWidget w, int src, int ctype,
			      int x, int y, int width, int height);
extern int GtInsertPixmap(GtermWidget w, Pixmap pixmap, int dst, int ctype,
			  int x, int y, int width, int height);
extern int GtNextColormap(GtermWidget w);
extern void GtFreeColormap(GtermWidget w, int colormap);
extern int GtWriteColormap(GtermWidget w, int map, int first, int nelem,
			   unsigned short *r, unsigned short *g, unsigned short *b);
extern int GtReadColormap(GtermWidget w, int map, int first, int nelem,
			  unsigned short *r, unsigned short *g, unsigned short *b);
extern int GtLoadColormap(GtermWidget w, int map, float offset, float slope);
extern int GtQueryColormap(GtermWidget w, int map, int *first, int *nelem, int *maxelem);
extern void GtSetColormapFocus(int box_size);
extern void GtInitMappings(GtermWidget w);
extern int GtNextMapping(GtermWidget w);
extern void GtFreeMapping(GtermWidget w, int mapping);
extern int GtDisableMapping(GtermWidget w, int mapping, int erase);
extern void GtLowerMapping(GtermWidget w, int mapping, int reference);
extern void GtRaiseMapping(GtermWidget w, int mapping, int reference);
extern int GtActiveMapping(GtermWidget w, int mapping);
extern int GtEnableMapping(GtermWidget w, int mapping, int refresh);
extern void GtRefreshMapping(GtermWidget w, int mapping);
extern int GtSetMapping(GtermWidget w, int mapping, int rop, int src, int st, int sx, int sy, int snx, int sny, int dst, int dt, int dx, int dy, int dnx, int dny);
extern int GtGetMapping(GtermWidget w, int mapping, int *rop, int *src, int *st, int *sx, int *sy, int *snx, int *sny, int *dst, int *dt, int *dx, int *dy, int *dnx, int *dny);
extern int GtCompareMappings(GtermWidget w, int map1, int map2);
extern int GtCopyRaster(GtermWidget w, int rop, int src, int st, int sx, int sy, int snx, int sny, int dst, int dt, int dx, int dy, int dnx, int dny);
extern int GtSelectRaster(GtermWidget w, int dras, int dt, int dx, int dy, int rt, int *rx, int *ry, int *rmap);
extern void GtSetDisplayRaster(GtermWidget gt, int raster);
extern void GtMarkerInit(GtermWidget w);
extern void GtMapVector(GtermWidget w, int mapping, int dir, DPoint *pv1, DPoint *pv2, int npts);
extern void GtPixelToNDC(GtermWidget w, int raster, DPoint *pv1, DPoint *pv2, int npts);
extern void GtNDCToPixel(GtermWidget w, int raster, DPoint *pv1, DPoint *pv2, int npts);
extern void GtOverrideTranslations(GtermWidget w, char *translations);
extern void GtWriteAlphaCursor(GtermWidget w, int x, int y);
extern void GtEraseAlphaCursor(GtermWidget w);
extern void GtSetTextRes(GtermWidget w, int optrows, int optcols);
extern void GtReadIomap(GtermWidget w, unsigned short *iomap, int first, int nelem);
extern void GtWriteIomap(GtermWidget w, unsigned short *iomap, int first, int nelem);
extern void GtReadLUT(GtermWidget w, long unsigned int *lut, int first, int nelem);

extern void GmAddCallback(struct marker *gm, int events, GmIMethod func, XtPointer client_data);
extern void GmDeleteCallback(struct marker *gm, GmIMethod func, XtPointer client_data);
extern XtPointer GmCreate(GtermWidget w, int type, int interactive);
extern int GmDestroy(struct marker *gm);
extern int GmGetAttribute(struct marker *gm, const char *attribute, XtArgVal value, char *type);
extern void GmGetAttributes(struct marker *gm, ArgList args, int nargs, char *argtype);
extern void GmGetBoundingBox(struct marker *gm, int *x, int *y, int *width, int *height);
extern int GmGetVertices(struct marker *gm, DPoint *points, int first, int maxpts);
extern int GmStrToType(char *marker_type);
extern int GmStrToEvent(char *event_type);
extern int GmStrToFunction(char *function);
extern void GmLower(struct marker *gm, struct marker *ref_gm);
extern int GmSetAttributes(struct marker *gm, ArgList args, int nargs, char *argtype);
extern int GmSetAttribute(struct marker *gm, char *attribute, XtArgVal value, char *type);
extern void GmMarkpos(struct marker *gm);
extern void GmRedraw(struct marker *gm, int func, int erase);
extern void GmNotify(struct marker *gm, int events, XEvent *event, String *params, Cardinal nparams);
extern void GmRaise(struct marker *gm, struct marker *ref_gm);
extern void GmSetVertices(struct marker *gm, DPoint *points, int first, int npts);

#endif /* _Gterm_h */
