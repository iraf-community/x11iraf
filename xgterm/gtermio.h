/*
 * GTERMIO.H -- Public definitions for the gterm i/o protocol module.
 */
struct GT_function {
	char *name;		/* callback name */
	int (*func)();		/* callback function */
	XtPointer data;		/* callback data */
};

void gt_deactivate(void);
void gt_activate(void);
int gt_flush(void);
int gt_tekmode(int state);
int gt_activated(void);
int gt_status(void);
void gtermio_register(struct GT_function *functions, int nfunc);
void gtermio_open_workstation(void);
void gtermio_close_workstation(void);
