/* Generated by wbuild from "Button.w"
** (generator version $Revision: 2.0 $ of $Date: 93/07/06 16:08:04 $)
*/
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include "ButtonP.h"
static void activate(Widget,XEvent*,String*,Cardinal*);

static XtActionsRec actionsList[] = {
{"activate", activate},
};

static char defaultTranslations[] = "\
<Btn1Down>: set_shadow(sunken) \n\
<Btn1Down>,<Btn1Up>: activate() set_shadow() \n\
Button1<Leave>: set_shadow() \n\
<Key>Return: set_shadow(sunken) activate() set_shadow() \n\
";
static void _resolve_inheritance(WidgetClass);

static XtResource resources[] = {
{XtNactivate,XtCActivate,XtRCallback,sizeof(((XfwfButtonRec*)NULL)->xfwfButton.activate),XtOffsetOf(XfwfButtonRec,xfwfButton.activate),XtRImmediate,(XtPointer)NULL },
{XtNframeWidth,XtCFrameWidth,XtRDimension,sizeof(((XfwfButtonRec*)NULL)->xfwfFrame.frameWidth),XtOffsetOf(XfwfButtonRec,xfwfFrame.frameWidth),XtRImmediate,(XtPointer)2 },
{XtNtraversalOn,XtCTraversalOn,XtRBoolean,sizeof(((XfwfButtonRec*)NULL)->xfwfCommon.traversalOn),XtOffsetOf(XfwfButtonRec,xfwfCommon.traversalOn),XtRImmediate,(XtPointer)True },
};

XfwfButtonClassRec xfwfButtonClassRec = {
{ /* core_class part */
(WidgetClass) &xfwfLabelClassRec,
"TextButton",
sizeof(XfwfButtonRec),
NULL,
_resolve_inheritance,
FALSE,
NULL,
NULL,
XtInheritRealize,
actionsList,
1,
resources,
3,
NULLQUARK,
False ,
FALSE ,
False ,
False ,
NULL,
XtInheritResize,
XtInheritExpose,
NULL,
NULL,
XtInheritSetValuesAlmost,
NULL,
XtInheritAcceptFocus,
XtVersion,
NULL,
defaultTranslations,
XtInheritQueryGeometry,
XtInheritDisplayAccelerator,
NULL 
},
{ /* composite_class part */
XtInheritGeometryManager,
XtInheritChangeManaged,
XtInheritInsertChild,
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
{ /* XfwfLabel_class part */
XtInherit_set_label,
},
{ /* XfwfButton_class part */
0
},
};
WidgetClass xfwfButtonWidgetClass = (WidgetClass) &xfwfButtonClassRec;
/*ARGSUSED*/
static void activate(Widget self, XEvent *event, String *params, Cardinal *num_params)
{
    XtCallCallbackList(self, ((XfwfButtonWidget)self)->xfwfButton.activate, event);
}

static void _resolve_inheritance(WidgetClass class)
{
  XfwfButtonWidgetClass c = (XfwfButtonWidgetClass) class;
  XfwfButtonWidgetClass super;
  static CompositeClassExtensionRec extension_rec = {
    NULL, NULLQUARK, XtCompositeExtensionVersion,
    sizeof(CompositeClassExtensionRec), True};
  CompositeClassExtensionRec *ext;
  ext = (CompositeClassExtensionRec *) XtMalloc(sizeof(*ext));
  *ext = extension_rec;
  ext->next_extension = c->composite_class.extension;
  c->composite_class.extension = (XtPointer) ext;
  if (class == xfwfButtonWidgetClass) return;
  super = (XfwfButtonWidgetClass)class->core_class.superclass;
}
