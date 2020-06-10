/*
 * GTERMIO.H -- Public definitions for the gterm i/o protocol module.
 */
struct GT_function {
	char *name;		/* callback name */
	int (*func)();		/* callback function */
	XtPointer data;		/* callback data */
};

void gt_deactivate();
void gt_activate();
int gt_flush();
int gt_tekmode();
int gt_activated();
int gt_status();
void gtermio_register();
void gtermio_open_workstation();
void gtermio_close_workstation();
