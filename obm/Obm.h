/* Copyright(c) 1993 Association of Universities for Research in Astronomy Inc.
 */

/*
 * OBM.H -- Public definitions for the object manager.
 */

#ifndef _Obm_Defined

#ifndef Obm_Private
typedef	struct { int dummy; } *ObmContext;
#endif

typedef	int (*ObmFunc)();
typedef struct _obmCallback *ObmCallback;

ObmContext ObmOpen (XtAppContext app_context, int argc, char *argv[]);
void ObmClose (ObmContext);
void ObmInitialize (ObmContext);
void ObmActivate (ObmContext);
void ObmDeactivate (ObmContext, Boolean);
int ObmActivated (ObmContext);
int ObmStatus (ObmContext, char *, char *);
XtPointer ObmGetInterp (ObmContext);
int ObmDeliverMsg (ObmContext, const char *, const char *);
int ObmDeliverMsgFromFile (ObmContext, char *, char *);
void ObmAddCallback (ObmContext, int, ObmFunc, XtPointer);
void ObmRemoveCallback (ObmContext, ObmCallback);
void obmNewObject(ObmContext, const char *, const char *, const char *, ArgList, int);


/* Callback type flags. */
#define	OBMCB_preserve			0000001 /* preserve over ObmInit */

#define	OBMCB_connect			0000010 /* callback types */
#define	OBMCB_activate			0000020
#define	OBMCB_deactivate		0000040
#define	OBMCB_setGterm			0000100
#define	OBMCB_clientOutput		0000200

#define	OBMUI_activate			0100000	/* internal */
#define	OBMUI_deactivate		0200000	/* internal */

/* ObmStatus states. */
#define	OBM_INITIALIZED			0
#define	OBM_ACTIVE			1
#define	OBM_IDLE			2

#define _Obm_Defined
#endif
