/* 
 *  A minimalistic constraint widget
 *
 *  Sun Sep 12 20:03:49 GMT 1993     Gustaf Neumann                 
 */

#ifndef MOTIF

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

/*
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/CharSet.h>
#include <X11/Xaw/XawInit.h>
*/

#include "DrawingAreaP.h"

/* Private Definitions */

static void ClassInitialize(void), ClassPartInitialize(WidgetClass class), Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args), Resize(Widget w);
static void ConstraintInitialize();
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args), ConstraintSetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static XtGeometryResult GeometryManager(Widget w, XtWidgetGeometry *request, XtWidgetGeometry *reply), PreferredGeometry(Widget widget, XtWidgetGeometry *request, XtWidgetGeometry *reply);
static void ChangeManaged(Widget w);

DrawingAreaClassRec drawingAreaClassRec = {
  { /* core_class fields */
    /* superclass         */    (WidgetClass) &constraintClassRec,
    /* class_name         */    "DrawingArea",
    /* widget_size        */    sizeof(DrawingAreaRec),
    /* class_initialize   */    ClassInitialize,
    /* class_part_init    */    ClassPartInitialize,
    /* class_inited       */    FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */    NULL,
    /* realize            */    XtInheritRealize,
    /* actions            */    NULL,
    /* num_actions        */    0,
    /* resources          */    NULL,
    /* num_resources      */    0,
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    TRUE,
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/    TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    Resize,
    /* expose             */    XtInheritExpose,
    /* set_values         */    SetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */    NULL,
    /* accept_focus       */    NULL,
    /* version            */    XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry     */	PreferredGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension          */	NULL
  },
  { /* composite_class fields */
    /* geometry_manager   */   GeometryManager,
    /* change_managed     */   NULL, /*ChangeManaged,*/
    /* insert_child       */   XtInheritInsertChild,
    /* delete_child       */   XtInheritDeleteChild,
    /* extension          */   NULL
  },
  { /* constraint_class fields */
    /* subresourses       */   NULL,
    /* subresource_count  */   0,
    /* constraint_size    */   0, 
    /* initialize         */   NULL, /*ConstraintInitialize,*/
    /* destroy            */   NULL,
    /* set_values         */   NULL, /*ConstraintSetValues,*/
    /* extension          */   NULL
  },
  { /* DrawingArea_class fields */
    /* dummy              */   0
  }
};

WidgetClass drawingAreaWidgetClass = (WidgetClass)&drawingAreaClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

static void ClassInitialize(void)
{
}

static void ClassPartInitialize(WidgetClass class)
{
}

/* ARGSUSED */
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args)
{
/*
    DrawingAreaWidget daw = (DrawingAreaWidget)new;
*/
}

static void Resize(Widget w)
{
/*
    DrawingAreaWidget daw = (DrawingAreaWidget)w;
    WidgetList children = daw->composite.children;
    int num_children = daw->composite.num_children;
    Widget *childP;
    Position x, y;
    Dimension width, height;
*/
}


/* ARGSUSED */
static XtGeometryResult GeometryManager(Widget w, XtWidgetGeometry *request, XtWidgetGeometry *reply)
             
                              
                            	/* RETURN */
{
/*
    Dimension old_width, old_height;
    DrawingAreaWidget daw = (DrawingAreaWidget) XtParent(w);
    DrawingAreaConstraints DrawingArea = (DrawingAreaConstraints) w->core.constraints;
    XtWidgetGeometry allowed;
    XtGeometryResult ret_val;
*/
    return(XtGeometryNo);
}


/* ARGSUSED */
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args)
{
    return( FALSE );
}


/*ARGSUSED*/
static Boolean ConstraintSetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args)
{
/*
  DrawingAreaConstraints cfc = 
      (DrawingAreaConstraints) current->core.constraints;
  DrawingAreaConstraints nfc = 
      (DrawingAreaConstraints) new->core.constraints;
 */
  return( FALSE );
}

static void ChangeManaged(Widget w)
{
/*
  DrawingAreaWidget daw = (DrawingAreaWidget)w;
  DrawingAreaConstraints DrawingArea;
  WidgetList children, childP;
  int num_children = daw->composite.num_children;
  Widget child;
*/
}


static XtGeometryResult PreferredGeometry(Widget widget, XtWidgetGeometry *request, XtWidgetGeometry *reply)
{
/*
    DrawingAreaWidget w = (DrawingAreaWidget)widget;
 */   
    return XtGeometryNo;
}



#endif
