/* Generated by wbuild from "Scrollbar.w"
** (generator version $Revision: 2.0 $ of $Date: 93/07/06 16:08:04 $)
*/
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include "Arrow.h"
#include "Slider2.h"
#include <stdio.h>
#include "ScrollbarP.h"
static void Scroll(Widget,XEvent*,String*,Cardinal*);

static XtActionsRec actionsList[] = {
{"Scroll", Scroll},
};
static void _resolve_inheritance(WidgetClass);
static void initialize(Widget ,Widget,ArgList ,Cardinal *);
static void resize(Widget);
static void insert_child(Widget );
static Boolean  set_values(Widget ,Widget ,Widget,ArgList ,Cardinal *);
static void scroll_response(Widget ,XtPointer ,XtPointer );
static char  rcsid[] = "$Header: Scrollbar.w,v 1.1 92/11/02 14:08:00 bert Exp $";
static void up(Widget ,XtPointer ,XtPointer );
static void down(Widget ,XtPointer ,XtPointer );
static void thumbscroll(Widget ,XtPointer ,XtPointer );
static void copy_background(Widget,int ,XrmValue *);

/*ARGSUSED*/static void up(Widget arrow, XtPointer client_data, XtPointer call_data)
{
    Widget self = (Widget) client_data;
    XfwfScrollInfo info;

    XfwfGetThumb(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, &info);
    if (((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical) {
	info.reason = XfwfSUp;
	info.flags = XFWF_VPOS;
	info.vpos = max(0.0, info.vpos - ((XfwfScrollbarWidget)self)->xfwfScrollbar.increment);
    } else {
	info.reason = XfwfSLeft;
	info.flags = XFWF_HPOS;
	info.hpos = max(0.0, info.hpos - ((XfwfScrollbarWidget)self)->xfwfScrollbar.increment);
    }
    XtCallCallbackList(self, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollCallback, &info);
}
/*ARGSUSED*/static void down(Widget arrow, XtPointer client_data, XtPointer call_data)
{
    Widget self = (Widget) client_data;
    XfwfScrollInfo info;

    XfwfGetThumb(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, &info);
    if (((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical) {
	info.reason = XfwfSDown;
	info.flags = XFWF_VPOS;
	info.vpos = min(1.0, info.vpos + ((XfwfScrollbarWidget)self)->xfwfScrollbar.increment);
    } else {
	info.reason = XfwfSRight;
	info.flags = XFWF_HPOS;
	info.hpos = min(1.0, info.hpos + ((XfwfScrollbarWidget)self)->xfwfScrollbar.increment);
    }
    XtCallCallbackList(self, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollCallback, &info);
}
/*ARGSUSED*/static void thumbscroll(Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget self = (Widget) client_data;
    XfwfScrollInfo *info = (XfwfScrollInfo*) call_data;

    if (((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical)
	info->flags &= XFWF_VPOS;
    else
	info->flags &= XFWF_HPOS;
    XtCallCallbackList(self, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollCallback, info);
}
/*ARGSUSED*/static void copy_background(Widget self, int offset, XrmValue *value)
{
    value->addr = (XtPointer) &((XfwfScrollbarWidget)self)->core.background_pixel;
}

static XtResource resources[] = {
{XtNvertical,XtCVertical,XtRBoolean,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.vertical),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.vertical),XtRImmediate,(XtPointer)True },
{XtNscrollCallback,XtCScrollCallback,XtRCallback,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.scrollCallback),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.scrollCallback),XtRImmediate,(XtPointer)NULL },
{XtNscrollResponse,XtCScrollResponse,XtRXTCallbackProc,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.scrollResponse),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.scrollResponse),XtRImmediate,(XtPointer)scroll_response },
{XtNinitialDelay,XtCInitialDelay,XtRCardinal,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.initialDelay),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.initialDelay),XtRImmediate,(XtPointer)500 },
{XtNrepeatDelay,XtCRepeatDelay,XtRCardinal,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.repeatDelay),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.repeatDelay),XtRImmediate,(XtPointer)50 },
{XtNincrement,XtCIncrement,XtRFloat,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.increment),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.increment),XtRString,(XtPointer)"0.05"},
{XtNscrollbarForeground,XtCScrollbarForeground,XtRPixel,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.scrollbarForeground),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.scrollbarForeground),XtRCallProc,(XtPointer)copy_background },
{XtNshadow,XtCShadow,XtRDimension,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.shadow),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.shadow),XtRImmediate,(XtPointer)2 },
{XtNminsize,XtCMinsize,XtRDimension,sizeof(((XfwfScrollbarRec*)NULL)->xfwfScrollbar.minsize),XtOffsetOf(XfwfScrollbarRec,xfwfScrollbar.minsize),XtRImmediate,(XtPointer)20 },
{XtNframeWidth,XtCFrameWidth,XtRDimension,sizeof(((XfwfScrollbarRec*)NULL)->xfwfFrame.frameWidth),XtOffsetOf(XfwfScrollbarRec,xfwfFrame.frameWidth),XtRImmediate,(XtPointer)2 },
{XtNframeType,XtCFrameType,XtRFrameType,sizeof(((XfwfScrollbarRec*)NULL)->xfwfFrame.frameType),XtOffsetOf(XfwfScrollbarRec,xfwfFrame.frameType),XtRImmediate,(XtPointer)XfwfSunken },
};

XfwfScrollbarClassRec xfwfScrollbarClassRec = {
{ /* core_class part */
(WidgetClass) &xfwfBoardClassRec,
"Scrollbar2",
sizeof(XfwfScrollbarRec),
NULL,
_resolve_inheritance,
FALSE,
initialize,
NULL,
XtInheritRealize,
actionsList,
1,
resources,
11,
NULLQUARK,
False ,
FALSE ,
False ,
False ,
NULL,
resize,
XtInheritExpose,
set_values,
NULL,
XtInheritSetValuesAlmost,
NULL,
XtInheritAcceptFocus,
XtVersion,
NULL,
NULL,
XtInheritQueryGeometry,
XtInheritDisplayAccelerator,
NULL 
},
{ /* composite_class part */
XtInheritGeometryManager,
XtInheritChangeManaged,
insert_child,
XtInheritDeleteChild,
NULL
},
{ /* XfwfCommon_class part */
XtInherit_compute_inside,
XtInherit_highlight_border,
XtInherit_unhighlight_border,
XtInherit_would_accept_focus,
XtInherit_traverse,
XtInherit_choose_color,
XtInherit_lighter_color,
XtInherit_darker_color,
NULL ,
},
{ /* XfwfFrame_class part */
0
},
{ /* XfwfBoard_class part */
XtInherit_set_abs_location,
},
{ /* XfwfScrollbar_class part */
scroll_response,
},
};
WidgetClass xfwfScrollbarWidgetClass = (WidgetClass) &xfwfScrollbarClassRec;
/*ARGSUSED*/
static void Scroll(Widget self, XEvent *event, String *params, Cardinal *num_params)
{
    XfwfScrollInfo info;

    XfwfGetThumb(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, &info);
    info.reason = XfwfCvtStringToScrollReason(params[0]);
    switch (info.reason) {
    case XfwfSUp: 
	info.flags = XFWF_VPOS;
	info.vpos = max(0.0, info.vpos - ((XfwfScrollbarWidget)self)->xfwfScrollbar.increment);
	break;
    case XfwfSDown:
	info.flags = XFWF_VPOS;
	info.vpos = min(1.0, info.vpos + ((XfwfScrollbarWidget)self)->xfwfScrollbar.increment);
	break;
    case XfwfSLeft:
	info.flags = XFWF_HPOS;
	info.hpos = max(0.0, info.hpos - ((XfwfScrollbarWidget)self)->xfwfScrollbar.increment);
	break;
    case XfwfSRight:
	info.flags = XFWF_HPOS;
	info.hpos = min(1.0, info.hpos + ((XfwfScrollbarWidget)self)->xfwfScrollbar.increment);
	break;
    case XfwfSPageUp:
	info.flags = XFWF_VPOS;
	info.vpos = max(0.0, info.vpos - info.vsize);
	break;
    case XfwfSPageDown:
	info.flags = XFWF_VPOS;
	info.vpos = min(1.0, info.vpos + info.vsize);
	break;
    case XfwfSPageLeft:
	info.flags = XFWF_HPOS;
	info.hpos = max(0.0, info.hpos - info.hsize);
	break;
    case XfwfSPageRight:
	info.flags = XFWF_HPOS;
	info.hpos = min(1.0, info.hpos + info.hsize);
	break;
    case XfwfSTop:
	info.flags = XFWF_VPOS;
	info.vpos = 0.0;
	break;
    case XfwfSBottom:
	info.flags = XFWF_VPOS;
	info.vpos = 1.0;
	break;
    case XfwfSLeftSide:
	info.flags = XFWF_HPOS;
	info.hpos = 0.0;
	break;
    case XfwfSRightSide:
	info.flags = XFWF_HPOS;
	info.hpos = 1.0;
	break;
    default: break;				/* Not understood */
    }
    XtCallCallbackList(self, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollCallback, &info);
}

static void _resolve_inheritance(WidgetClass class)
{
  XfwfScrollbarWidgetClass c = (XfwfScrollbarWidgetClass) class;
  XfwfScrollbarWidgetClass super;
  static CompositeClassExtensionRec extension_rec = {
    NULL, NULLQUARK, XtCompositeExtensionVersion,
    sizeof(CompositeClassExtensionRec), True};
  CompositeClassExtensionRec *ext;
  ext = (XtPointer)XtMalloc(sizeof(*ext));
  *ext = extension_rec;
  ext->next_extension = c->composite_class.extension;
  c->composite_class.extension = ext;
  if (class == xfwfScrollbarWidgetClass) return;
  super = (XfwfScrollbarWidgetClass)class->core_class.superclass;
  if (c->xfwfScrollbar_class.scroll_response == XtInherit_scroll_response)
    c->xfwfScrollbar_class.scroll_response = super->xfwfScrollbar_class.scroll_response;
}
/*ARGSUSED*/static void initialize(Widget request, Widget self, ArgList args, Cardinal *num_args)
{
    Position x, y, xa2, xslider, ya2, yslider;
    Dimension w, h, wa, ha, wslider, hslider;

    ((XfwfScrollbarWidget)self)->xfwfScrollbar.initializing = True;
    ((XfwfScrollbarWidgetClass)self->core.widget_class)->xfwfCommon_class.compute_inside(self, &x, &y, &w, &h);
    if (((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical) {
	ha = wa = wslider = w;
	xa2 = xslider = x;
	hslider = (h - 2*ha > 0) ? h - 2*ha : 10;
	yslider = y + ha;
	ya2 = yslider + hslider;
    } else {
	wa = ha = hslider = h;
	ya2 = yslider = y;
	wslider = (w - 2*wa > 0) ? w - 2*wa : 10;
	xslider = x + wa;
	xa2 = xslider + wslider;
    }
    ((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow1 = XtVaCreateManagedWidget
	("_arrow1", xfwfArrowWidgetClass, self,
	 XtNx, x,
	 XtNy, y,
	 XtNwidth, wa,
	 XtNheight, ha,
	 XtNframeWidth, 0,
	 XtNforeground, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollbarForeground,
	 XtNinitialDelay, ((XfwfScrollbarWidget)self)->xfwfScrollbar.initialDelay,
	 XtNrepeatDelay, ((XfwfScrollbarWidget)self)->xfwfScrollbar.repeatDelay,
	 XtNtraversalOn, False,
	 XtNhighlightThickness, 0,
	 XtNdirection, ((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical?XfwfTop:XfwfLeft,
	 XtNouterOffset, 0,
	 XtNborderWidth, 0,
	 NULL);
    XtAddCallback(((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow1, XtNcallback, up, self);
    ((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow2 = XtVaCreateManagedWidget
	("_arrow2", xfwfArrowWidgetClass, self,
	 XtNx, xa2,
	 XtNy, ya2,
	 XtNwidth, wa,
	 XtNheight, ha,
	 XtNframeWidth, 0,
	 XtNforeground, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollbarForeground,
	 XtNinitialDelay, ((XfwfScrollbarWidget)self)->xfwfScrollbar.initialDelay,
	 XtNrepeatDelay, ((XfwfScrollbarWidget)self)->xfwfScrollbar.repeatDelay,
	 XtNtraversalOn, False,
	 XtNhighlightThickness, 0,
	 XtNdirection, ((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical?XfwfBottom:XfwfRight,
	 XtNouterOffset, 0,
	 XtNborderWidth, 0,
	 NULL);
    XtAddCallback(((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow2, XtNcallback, down, self);
    ((XfwfScrollbarWidget)self)->xfwfScrollbar.slider = XtVaCreateManagedWidget
	("_slider", xfwfSlider2WidgetClass, self,
	 XtNx, xslider,
	 XtNy, yslider,
	 XtNwidth, wslider,
	 XtNheight, hslider,
	 XtNthumbColor, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollbarForeground,
	 XtNframeWidth, 0,
	 XtNinitialDelay, ((XfwfScrollbarWidget)self)->xfwfScrollbar.initialDelay,
	 XtNrepeatDelay, ((XfwfScrollbarWidget)self)->xfwfScrollbar.repeatDelay,
	 XtNtraversalOn, False,
	 XtNhighlightThickness, 0,
	 XtNouterOffset, 0,
	 XtNborderWidth, 0,
	 NULL);
    XtAddCallback(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, XtNscrollCallback, thumbscroll, self);
    XtVaGetValues(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, XtNscrollResponse, &((XfwfScrollbarWidget)self)->xfwfScrollbar.slider_scroll, NULL);
    ((XfwfScrollbarWidget)self)->xfwfScrollbar.initializing = False;
}
/*ARGSUSED*/static void resize(Widget self)
{
    Position x, y, xa2, xslider, ya2, yslider;
    Dimension w, h, wa, ha, wslider, hslider;

    ((XfwfScrollbarWidgetClass)self->core.widget_class)->xfwfCommon_class.compute_inside(self, &x, &y, &w, &h);
    if (((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical) {
	wa = wslider = w;
	xa2 = xslider = x;
	ha = 2 * (wa + 1)/3;
	hslider = (h - 2*ha > 0) ? h - 2*ha : 10;
	yslider = y + ha;
	ya2 = yslider + hslider;
    } else {
	ha = hslider = h;
	ya2 = yslider = y;
	wa = 2 * (ha + 1)/3;
	wslider = (w - 2*wa > 0) ? w - 2*wa : 10;
	xslider = x + wa;
	xa2 = xslider + wslider;
    }
    XtConfigureWidget(((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow1, x, y, wa, ha, 0);
    XtConfigureWidget(((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow2, xa2, ya2, wa, ha, 0);
    XtConfigureWidget(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, xslider, yslider, wslider, hslider, 0);
}
/*ARGSUSED*/static void insert_child(Widget child)
{ Widget self = XtParent(child); {
    if (((XfwfScrollbarWidget)self)->xfwfScrollbar.initializing)
	xfwfBoardClassRec.composite_class.insert_child(child);
    else {
	char s[500];
	(void)sprintf(s, "Cannot add children to a scrollbar (\"%s\"->\"%s\")",
		      XtName(child), XtName(self));
	XtWarning(s);
    }
}
}
/*ARGSUSED*/static Boolean  set_values(Widget old, Widget request, Widget self, ArgList args, Cardinal *num_args)
{
    if (((XfwfScrollbarWidget)old)->xfwfScrollbar.vertical != ((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical) {
	XtWarning("Cannot change the \"vertical\" resource of a scrollbar\n");
	((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical = ((XfwfScrollbarWidget)old)->xfwfScrollbar.vertical;
    }
    if (((XfwfScrollbarWidget)old)->xfwfScrollbar.scrollbarForeground != ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollbarForeground) {
	XtVaSetValues(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, XtNthumbColor, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollbarForeground, NULL);
	XtVaSetValues(((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow1, XtNforeground, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollbarForeground, NULL);
	XtVaSetValues(((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow2, XtNforeground, ((XfwfScrollbarWidget)self)->xfwfScrollbar.scrollbarForeground, NULL);
    }
    if (((XfwfScrollbarWidget)old)->xfwfScrollbar.shadow != ((XfwfScrollbarWidget)self)->xfwfScrollbar.shadow) {
	XtVaSetValues(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, XtNthumbFrameWidth, ((XfwfScrollbarWidget)self)->xfwfScrollbar.shadow, NULL);
	XtVaSetValues(((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow1, XtNarrowShadow, ((XfwfScrollbarWidget)self)->xfwfScrollbar.shadow, NULL);
	XtVaSetValues(((XfwfScrollbarWidget)self)->xfwfScrollbar.arrow2, XtNarrowShadow, ((XfwfScrollbarWidget)self)->xfwfScrollbar.shadow, NULL);
    }
    if (((XfwfScrollbarWidget)old)->xfwfScrollbar.minsize != ((XfwfScrollbarWidget)self)->xfwfScrollbar.minsize) {
	XtVaSetValues(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, XtNminsize, ((XfwfScrollbarWidget)self)->xfwfScrollbar.minsize, NULL);
    }
    return False;
}
/*ARGSUSED*/static void scroll_response(Widget wdg, XtPointer client_data, XtPointer call_data)
{
    Widget self = (Widget) client_data;

    ((XfwfScrollbarWidget)self)->xfwfScrollbar.slider_scroll(wdg, ((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, call_data);
}
/*ARGSUSED*/void XfwfSetScrollbar(Widget self, double pos, double size)
{
    if (! XtIsSubclass(self, xfwfScrollbarWidgetClass))
	XtError("XfwfSetScrollbar called with incorrect widget type");
    if (pos < 0.0 || pos > 1.0 || size < 0.0 || size > 1.0)
	XtError("XfwfSetScrollbar called with incorrect arguments");
    if (((XfwfScrollbarWidget)self)->xfwfScrollbar.vertical) {
	XfwfResizeThumb(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, 1.0, size);
	XfwfMoveThumb(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, 0.0, pos);
    } else {
	XfwfResizeThumb(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, size, 1.0);
	XfwfMoveThumb(((XfwfScrollbarWidget)self)->xfwfScrollbar.slider, pos, 0.0);
    }
}
