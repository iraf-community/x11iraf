/***********************************************************************
  
                             Table widget
		   Copyright by Vladimir T. Romanovski
			 All rights reserved.

This library is designed  for  free,  non-commercial  software  creation.
It is changeable and can be improved. The author would greatly appreciate
any  advice, new  components  and  patches  of  the  existing  programs.
Commercial  usage is  also  possible  with  participation of the author.

                       romsky@hp1.oea.ihep.su  (Russia)
                       romsky@munin.ucsf.edu   (USA)
	 
*************************************************************************/

#ifndef _table_h_
#define _table_h_

#include <stdio.h>
#include <X11/Intrinsic.h>
#include "Table.h"

typedef struct _XawTableNodeRec {              /* Node of table grid */
  struct _XawTableNodeRec *l;
  struct _XawTableNodeRec *r;
  struct _XawTableNodeRec *t;
  struct _XawTableNodeRec *b;
}XawTableNodeRec, *XawTableNode;


extern XtPointer create_table 	    (int rows,
				     int columns, 
				     int node_size);

extern Boolean row_insert_after     (XtPointer d,
				     int node_size);
     
extern Boolean row_insert_before    (XtPointer f,
				     int node_size);
     
extern Boolean column_insert_after  (XtPointer d,
				     int node_size);
     
extern Boolean column_insert_before (XtPointer f,
				     int node_size);
     
extern XtPointer get_table 	    (XtPointer f);
     
extern XtPointer get_cell 	    (XtPointer p,
				     int i,
				     int j);
     
extern void get_table_size 	    (XtPointer p,
				     int *i,
				     int *j);
     
extern void get_cell_positions 	    (XtPointer p,
				     int *i,
				     int *j);

extern void row_delete 		    (XtPointer p);
     
extern void column_delete 	    (XtPointer p);
     
extern void delete_table 	    (XtPointer p);

extern Boolean go_table 	    (XtPointer w, 
				     XawTableProc proc, 
				     XtPointer table, 
				     int begin_row,
				     int end_row,
				     int begin_column, 
				     int end_column,
				     int direction,
				     int *row,
				     int *column,
				     XtPointer client_data);

#ifdef EBUG_XRAW_MALLOC
extern void _check_table            (XtPointer table, 
				     int rows, 
				     int columns);
#endif

#endif /* _table_h_ */
