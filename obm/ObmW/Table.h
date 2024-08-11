/***********************************************************************
  
                             Table widget
		   Copyright by Vladimir T. Romanovski
			 All rights reserved.

This library is designed  for  free,  non-commercial  software  creation.
It is changeable and can be improved. The author would greatly appreciate
any  advice, new  components  and  patches  of  the  existing  programs.
Commercial  usage is  also  possible  with  participation of the author.

                      romsky@hp1.oea.ihep.su (Russia)
                      romsky@munin.ucsf.edu  (USA)
	
*************************************************************************/

#ifndef _XawTable_h
#define _XawTable_h

#include <X11/Xmu/Converters.h>

#include "XrawInit.h"




/*#########################################################################*/
/*#                                                                       #*/
/*#                           New Resources                               #*/
/*#                                                                       #*/
/*#########################################################################*/
#ifndef XawTextEncoding8bit
#define XawTextEncoding8bit     0
#endif
#ifndef XawTextEncodingChar2b
#define XawTextEncodingChar2b   1
#endif

#ifndef XtNliteral
#define XtNliteral		"literal"
#endif

#ifndef XtNrows
#define XtNrows      	     	"rows"
#endif

#ifndef XtNcolumns
#define XtNcolumns		"columns"
#endif


#ifndef XtNmaskNumber
#define XtNmaskNumber		"maskNumber"
#endif

#ifndef XtNrowOriented
#define XtNrowOriented     	"rowOriented"
#endif

#ifndef XtNeditForeground
#define XtNeditForeground  	"editForeground"
#endif

#ifndef XtNeditBackground
#define XtNeditBackground  	"editBackground"
#endif

#ifndef XtNcolumnForeground
#define XtNcolumnForeground  	"columnForeground"
#endif

#ifndef XtNrowForeground
#define XtNrowForeground     	"rowForeground"
#endif

#ifndef XtNvetricalScroll
#define XtNvetricalScroll     	"vetricalScroll"
#endif

#ifndef XtNhorizontalScroll
#define XtNhorizontalScroll     "horizontalScroll"
#endif

#ifndef XtNcolumnsWidth
#define XtNcolumnsWidth     	"columnsWidth"
#endif

#ifndef XtNrowHeight
#define XtNrowHeight     	"rowHeight"
#endif

#ifndef XtNdefaultWidth
#define XtNdefaultWidth     	"defaultWidth"
#endif

#ifndef XtNeditable
#define XtNeditable     	"editable"
#endif

#ifndef XtNliteralWidth
#define XtNliteralWidth     	"literalWidth"
#endif

#ifndef XtNtableMargin
#define XtNtableMargin      	"tableMargin"
#endif

#ifndef XtNrowMargin
#define XtNrowMargin      	"rowMargin"
#endif

#ifndef XtNcolumnMargin
#define XtNcolumnMargin      	"columnMargin"
#endif

#ifndef XtNlabelShadowWidth
#define XtNlabelShadowWidth 	"labelShadowWidth"
#endif

#ifndef XtNencoding
#define XtNencoding       	"encoding"
#endif






/*#########################################################################*/
/*#                                                                       #*/
/*#                           New Resource Classes                        #*/
/*#                                                                       #*/
/*#########################################################################*/
#ifndef XtCLiteral
#define XtCLiteral		"Literal"
#endif

#ifndef XtCColumns
#define XtCColumns		"Columns"
#endif

#ifndef XtCMaskNumber
#define XtCMaskNumber     	"MaskNumber"
#endif

#ifndef XtCScroll
#define XtCScroll     		"Scroll"
#endif

#ifndef XtCColumnsWidth
#define XtCColumnsWidth     	"ColumnsWidth"
#endif

#ifndef XtCRowHeight
#define XtCRowHeight     	"RowHeight"
#endif

#ifndef XtCDefaultWidth
#define XtCDefaultWidth     	"DefaultWidth"
#endif

#ifndef XtCEditable
#define XtCEditable     	"Editable"
#endif

#ifndef XtCLiteralWidth
#define XtCLiteralWidth     	"LiteralWidth"
#endif

#ifndef XtCRows
#define XtCRows           	"Rows"
#endif

#ifndef XtCTableMargin
#define XtCTableMargin      	"TableMargin"
#endif

#ifndef XtCRowMargin
#define XtCRowMargin      	"RowMargin"
#endif

#ifndef XtCColumnMargin
#define XtCColumnMargin      	"ColumnMargin"
#endif

#ifndef XtCLabelShadowWidth
#define XtCLabelShadowWidth 	"LabelShadowWidth"
#endif

#ifndef XtCEncoding
#define XtCEncoding       	"Encoding"
#endif






/*#########################################################################*/
/*#                                                                       #*/
/*#                           New Resource Types                          #*/
/*#                                                                       #*/
/*#########################################################################*/
#ifndef XtRColumnsWidth
#define XtRColumnsWidth     	"ColumnsWidth"
#endif

#ifndef XtCRowOriented
#define XtCRowOriented     	"RowOriented"
#endif


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Allowance Callbacks                         #*/
/*#                                                                       #*/
/*#########################################################################*/
#ifndef XtNallowAddColumn
#define XtNallowAddColumn     	        "allowAddColumn"
#endif

#ifndef XtNallowAddRow
#define XtNallowAddRow     		"allowAddRow"
#endif

#ifndef XtNallowDeleteColumn
#define XtNallowDeleteColumn     	"allowDeleteColumn"
#endif

#ifndef XtNallowDeleteRow
#define XtNallowDeleteRow     	        "allowDeleteRow"
#endif

#ifndef XtNallowDeleteTable
#define XtNallowDeleteTable     	"allowDeleteTable"
#endif


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Information Callbacks                       #*/
/*#                                                                       #*/
/*#########################################################################*/
#ifndef XtNaddColumn
#define XtNaddColumn     	"addColumn"
#endif

#ifndef XtNaddRow
#define XtNaddRow     		"addRow"
#endif

#ifndef XtNchangedCell
#define XtNchangedCell     	"changedCell"
#endif

#ifndef XtNchangedColumnWidth
#define XtNchangedColumnWidth   "changedColumnWidth"
#endif

#ifndef XtNchangedRowHeight
#define XtNchangedRowHeight     "changedRowHeight"
#endif

#ifndef XtNcreateTable
#define XtNcreateTable     	"createTable"
#endif

#ifndef XtNdeleteColumn
#define XtNdeleteColumn     	"deleteColumn"
#endif

#ifndef XtNdeleteRow
#define XtNdeleteRow     	"deleteRow"
#endif

#ifndef XtNdeleteTable
#define XtNdeleteTable     	"deleteTable"
#endif

#ifndef XtNwhatCell
#define XtNwhatCell     	"whatCell"
#endif



/*#########################################################################*/
/*#                                                                       #*/
/*#                           XawTableCell & XawTableColumn               #*/
/*#                                                                       #*/
/*#########################################################################*/
typedef struct _XawTableCellRec   *XawTableCell;    /* opaque to outside */

typedef struct _XawTableColumnRec *XawTableColumn;  /* opaque to outside */



/*#########################################################################*/
/*#                                                                       #*/
/*#                           Bypass Routine and Direction Types          #*/
/*#                                                                       #*/
/*#########################################################################*/
typedef Boolean (*XawTableProc)     (Widget, 
				     int, 
				     int, 
				     XawTableCell, 
				     XtPointer);
enum XawTableBypassDirection{
  XawTABLE_RIGHT_DOWN,
  XawTABLE_DOWN_RIGHT
};


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Callback Reasons                            #*/
/*#                                                                       #*/
/*#########################################################################*/
enum XawTableReasons{
  XawTABLE_ALLOW_ADD_COLUMN = Xraw_TABLE,
  XawTABLE_ALLOW_ADD_ROW,
  XawTABLE_ALLOW_CREATE_TABLE,
  XawTABLE_ALLOW_DELETE_COLUMN,
  XawTABLE_ALLOW_DELETE_ROW,
  XawTABLE_ALLOW_DELETE_TABLE,

  XawTABLE_ADD_COLUMN,
  XawTABLE_ADD_ROW,
  XawTABLE_CHANGED_CELL,
  XawTABLE_CHANGED_COLUMN_WIDTH,
  XawTABLE_CHANGED_ROW_HEIGHT,
  XawTABLE_CREATE_TABLE,
  XawTABLE_DELETE_COLUMN,
  XawTABLE_DELETE_ROW,
  XawTABLE_DELETE_TABLE,
  XawTABLE_WHAT_CELL
};


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Callback Structure                          #*/
/*#                                                                       #*/
/*#########################################################################*/
typedef struct {
  int           reason;
  XEvent       *event;
  XawTableCell  old_cell;
  XawTableCell  new_cell;
  int           row;
  int           column;
  Boolean       do_it;
}XawTableCallbackStruct;


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Layout Control Routine                      #*/
/*#                                                                       #*/
/*#########################################################################*/
extern void XawTableDoLayout            (Widget w,
					 Boolean do_layout);


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Stuff Routines                              #*/
/*#                                                                       #*/
/*#########################################################################*/
extern int XawTableSetNewSize 		(Widget w, 
					 int  rows,
					 int  columns);

extern void XawTableGetSize             (Widget w,
					 int *rows,
					 int *columns);

     
/*#########################################################################*/
/*#                                                                       #*/
/*#                           Row Routines                                #*/
/*#                                                                       #*/
/*#########################################################################*/
extern int XawTablePrependRow           (Widget w);
     
extern int XawTableAppendRow 		(Widget w);
     
extern int XawTableInsertRow            (Widget w, int row);
     
extern int XawTableDeleteRow            (Widget w, int row);
     

/*#########################################################################*/
/*#                                                                       #*/
/*#                           Column Routines                             #*/
/*#                                                                       #*/
/*#########################################################################*/
extern int XawTablePrependColumn        (Widget w, int width);
     
extern int XawTableAppendColumn         (Widget w, int width);
     
extern int XawTableInsertColumn         (Widget w, 
					 int column,
					 int width);
     
extern int XawTableDeleteColumn 	(Widget w, int column);


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Set Label Routines                          #*/
/*#                                                                       #*/
/*#########################################################################*/
extern char *XawTableGetLabelByCell 	(XawTableCell cell);
     
extern char *XawTableGetLabelByPosition (Widget w,
					 int row,
					 int column);
     
extern int XawTableSetLabel 		(Widget w,
					 int row,
					 int column,
					 const char *label);
     

/*#########################################################################*/
/*#                                                                       #*/
/*#                           Bypass Routines                             #*/
/*#                                                                       #*/
/*#########################################################################*/
extern Boolean XawTableWalk 		(Widget w,
					 XawTableProc proc,
					 int b_row,
					 int e_row,
					 int b_column,
					 int e_column,
					 int direction,
					 int *row, int *column,
					 XtPointer client_data);
     
extern Boolean XawTableSearchLabel 	(Widget w,
					 char *name,
					 int *row,
					 int *column);

     
/*#########################################################################*/
/*#                                                                       #*/
/*#                           Edit Cell Routines                          #*/
/*#                                                                       #*/
/*#########################################################################*/
extern Boolean XawTableIsEditManaged 	(Widget w);

extern void XawTableGetEditPosition 	(Widget w,
					 int *row,
					 int *column);
     
extern void XawTableUnsetEdit 		(Widget w);

extern void XawTableSetEdit 		(Widget w,
					 int row,
					 int column);


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Set Colour Routines                         #*/
/*#                                                                       #*/
/*#########################################################################*/
extern int XawTableSetCellBackground 	(Widget w,
					 int   row,
					 int   column,
					 Pixel background);
     
extern int XawTableSetCellForeground 	(Widget w,
					 int   row,
					 int   column,
					 Pixel foreground);

extern int XawTableSetCellDefaultColours 	(Widget w,
						 int   row,
						 int   column);

extern int XawTableSetCellColours 	(Widget w,
					 int   row,
					 int   column,
					 Pixel foreground,
					 Pixel background);
     

extern void XawTableGetCellColours    	(Widget w,
					 int   row,
					 int   column,
					 Pixel *foreground,
					 Pixel *background);
     
extern void XawTableGetCellColoursByCell 	(Widget w,
						 XawTableCell cell,
						 Pixel *foreground,
						 Pixel *background);
     

/*#########################################################################*/
/*#                                                                       #*/
/*#                           Column Deta                                 #*/
/*#                                                                       #*/
/*#########################################################################*/
extern void XawTableSetColumnJustify       (Widget w,
					    int   column,
					    XtJustify justify);

extern XtJustify XawTableGetColumnJustify  (Widget w,
					    int   column);

extern void XawTableSetColumnWidth         (Widget w,
					    int   column,
					    int   width);

extern int XawTableGetColumnWidth          (Widget w,
					    int   column);

extern int XawTableGetColumnPixelWidth          (Widget w,
						 int   column);

extern void XawTableSetMultiColumnWidths	(Widget w,
						 int *columns,
						 int *widths,
						 int ncols);


/*#########################################################################*/
/*#                                                                       #*/
/*#                           Widget Class Pointer                        #*/
/*#                                                                       #*/
/*#########################################################################*/
extern WidgetClass tableWidgetClass;

typedef struct _TableClassRec *XawTableWidgetClass;
typedef struct _TableRec      *XawTableWidget;


#endif /* _XawTable_h */
/* DON'T ADD STUFF AFTER THIS #endif */
