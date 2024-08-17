#ifndef _Converters_h
#define _Converters_h

Boolean XfwfCvtLongToString(
    Display *display,
    XrmValuePtr args,
    Cardinal *num_args,
    XrmValuePtr from,
    XrmValuePtr to,
    XtPointer *converter_data
);

Boolean cvtStringToIcon(
    Display *dpy,
    XrmValue *args,
    Cardinal *num_args,
    XrmValue *from,
    XrmValue *to,
    XtPointer *converter_data
);

#ifndef NO_XPM

#include <X11/xpm.h>

/* The |Icon| type is a convenient combination of a pixmap, a mask and
the pixmaps's attributes. Not all attributes are stored, only width
and height. */

typedef struct _Icon {
    Pixmap pixmap;
    Pixmap mask;
    XpmAttributes attributes;
} Icon;

#endif

#endif /* _Converters_h */
