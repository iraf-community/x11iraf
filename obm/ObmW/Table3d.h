#ifndef _3d_h_
#define _3d_h_

#include <stdio.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include "XrawInit.h"

typedef enum {
  XawRAISED = Xraw_3d,
  XawSUNKEN,
  XawCHISELED,
  XawLEDGED,
  XawTACK
} XawFrameType;

#define TOP    (1)
#define BOTTOM (2)

extern GC AllocGCFromPixmap		(Widget  , Pixmap );
extern GC AllocGCFromPixel		(Widget  , Pixel );

extern void RGBtoHLS			(double  , 
					 double  ,
					 double  ,
					 double *  ,
					 double *  ,
					 double * );
     
extern void HLStoRGB			(double *  ,
					 double *  ,
					 double *  ,
					 double  ,
					 double  ,
					 double );
						
extern Boolean TopShadowColor		(Widget /* self */,
					 Pixel  /* base */,
					 Pixel* /* result */);
     
extern Boolean BottomShadowColor	(Widget /* self */,
					 Pixel  /* base */,
					 Pixel* /* result */);
     
extern Boolean ArmedColor		(Widget /* self */,
					 Pixel  /* base */,
					 Pixel* /* result */);
     
extern GC MakeTopShadowGC		(Widget  , Pixel );
extern GC MakeBottomShadowGC		(Widget  , Pixel );
extern GC MakeArmedGC			(Widget  , Pixel );
		
		   
extern GC AllocGCFromPixmap		(Widget  , Pixmap );
		
		   
extern GC AllocGCFromPixel		(Widget  , Pixel );
		
		   
extern Pixmap CreateShadowPixmap	(Widget  ,
					 Pixel  ,
					 int );
		
		   
extern Boolean AllocShadowPixel	        (Widget  ,
					 Pixel  ,
					 int  ,
					 Pixel * );
     
		   
extern GC MakeGC			(Widget  ,
					 Pixel  ,
					 int  ,
					 Boolean  ,
					 int );
     
		   
extern GC MakeTopShadowGC		(Widget  , Pixel );
		
		   
extern GC MakeBottomShadowGC		(Widget  , Pixel );
		
		   
extern GC MakeArmedGC			(Widget  , Pixel );
		
		   
extern void XawDrawFrame		(Widget  ,
					 Position  ,
					 Position  ,
					 Dimension  ,
					 Dimension  ,
					 XawFrameType  ,
					 Dimension  ,
					 GC  ,
					 GC );
		
		   
extern void RGBtoHLS			(double  ,
					 double  ,
					 double  ,
					 double *  ,
					 double *  ,
					 double * );
						
extern void HLStoRGB			(double *  ,
					 double *  ,
					 double *  ,
					 double  ,
					 double  ,
					 double );
						
extern Boolean BottomShadowColor	(Widget  ,
					 Pixel  ,
					 Pixel * );
     
		   
extern Boolean TopShadowColor		(Widget  ,
					 Pixel  ,
					 Pixel * );
		
		   
extern Boolean ArmedColor		(Widget  ,
					 Pixel  ,
					 Pixel * );
		
		   
extern void DrawRhombus		        (Widget  ,
					 short  ,
					 short  ,
					 short  ,
					 short  ,
					 GC  ,
					 GC  ,
					 GC  ,
					 Boolean );
     
extern Boolean FetchPixel               (Widget  ,
					 String name ,
					 Pixel* );

#endif /* _3d_h_ */




