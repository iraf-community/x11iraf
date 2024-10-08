/* Copyright(c) 1993 Association of Universities for Research in Astronomy Inc.
 */

/*
 * WIDGET.H -- Widget class public definitions (used to subclass widgets).
 * These definitions are intended for use only by Widget subclasses, not by
 * client applications.
 */

struct widgetPrivate {
	Widget w;
	ObmContext obm;
	ObmCallback callback;
	ObmCallback event_handler;
	Boolean widget_destroyed;
	XawTextPosition text_pos;
	XtCallbackProc response_cb;
	int text_newline;
	char translation_table_name[SZ_NAME];
	char *data;
	int datalen;
};

typedef struct widgetPrivate *WidgetPrivate;

struct widgetObject {
	struct obmObjectCore core;
	struct widgetPrivate widget;
};

typedef struct widgetObject *WidgetObject;

/* Object message context. */
struct msgContext {
	Tcl_Interp *tcl;		/* class interpreter */
	ObmObject object[MAX_LEVELS];	/* object which received last message */
	int level;
};
typedef struct msgContext *MsgContext;

extern	int WidgetEvaluate(ObmObject, const char *);
extern	void WidgetDestroy(ObmObject);
extern	void WidgetClassDestroy(ObmContext, ObjClassRec);
extern	ObmObject WidgetCreate(ObmContext, const char *, ObjClassRec,
			       const char *, ArgList, int);
extern	WidgetObject widgetToObject(ObmContext, Widget);
extern	int coordType(char *);
extern	char *widgetEventState(char *, unsigned int);
