/*
 * $XConsortium: sharedlib.c,v 1.1 91/06/29 12:40:18 rws Exp $
 * 
 * Copyright 1991 Massachusetts Institute of Technology
 * Copyright 1992 Kaleb Keithley
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 */

#if defined(SUNSHLIB) && !defined(SHAREDCODE)

#include <X11/IntrinsicP.h>
#include <X11/Xaw3d/AsciiSinkP.h>
#include <X11/Xaw3d/AsciiSrcP.h>
#include <X11/Xaw3d/AsciiTextP.h>
#include <X11/Xaw3d/BoxP.h>
#include <X11/Xaw3d/ClockP.h>
#include <X11/Xaw3d/CommandP.h>
#include <X11/Xaw3d/DialogP.h>
#include <X11/Xaw3d/FormP.h>
#include <X11/Xaw3d/GripP.h>
#include <X11/Xaw3d/LabelP.h>
#include <X11/Xaw3d/LayoutP.h>
#include <X11/Xaw3d/ListP.h>
#include <X11/Xaw3d/LogoP.h>
#include <X11/Xaw3d/MailboxP.h>
#include <X11/Xaw3d/MenuButtoP.h>
#include <X11/Xaw3d/PanedP.h>
#include <X11/Xaw3d/PannerP.h>
#include <X11/Xaw3d/PortholeP.h>
#include <X11/Xaw3d/RepeaterP.h>
#include <X11/Xaw3d/ScrollbarP.h>
#include <X11/Xaw3d/SimpleP.h>
#include <X11/Xaw3d/SimpleMenP.h>
#include <X11/Xaw3d/SmeP.h>
#include <X11/Xaw3d/SmeBSBP.h>
#include <X11/Xaw3d/SmeLineP.h>
#include <X11/Xaw3d/SmeThreeDP.h>
#include <X11/Xaw3d/StripCharP.h>
#include <X11/Xaw3d/TextP.h>
#include <X11/Xaw3d/TextSinkP.h>
#include <X11/Xaw3d/TextSrcP.h>
#include <X11/Xaw3d/ThreeDP.h>
#include <X11/Xaw3d/ToggleP.h>
#include <X11/Xaw3d/TreeP.h>
#include <X11/VendorP.h>
#include <X11/Xaw3d/ViewportP.h>

extern AsciiSinkClassRec asciiSinkClassRec;
WidgetClass asciiSinkObjectClass = (WidgetClass)&asciiSinkClassRec;

extern AsciiSrcClassRec asciiSrcClassRec;
WidgetClass asciiSrcObjectClass = (WidgetClass)&asciiSrcClassRec;

extern AsciiTextClassRec asciiTextClassRec;
WidgetClass asciiTextWidgetClass = (WidgetClass)&asciiTextClassRec;

#ifdef ASCII_STRING
extern AsciiStringClassRec asciiStringClassRec;
WidgetClass asciiStringWidgetClass = (WidgetClass)&asciiStringClassRec;
#endif

#ifdef ASCII_DISK
extern AsciiDiskClassRec asciiDiskClassRec;
WidgetClass asciiDiskWidgetClass = (WidgetClass)&asciiDiskClassRec;
#endif

extern BoxClassRec boxClassRec;
WidgetClass boxWidgetClass = (WidgetClass)&boxClassRec;

extern ClockClassRec clockClassRec;
WidgetClass clockWidgetClass = (WidgetClass) &clockClassRec;

extern CommandClassRec commandClassRec;
WidgetClass commandWidgetClass = (WidgetClass) &commandClassRec;

extern DialogClassRec dialogClassRec;
WidgetClass dialogWidgetClass = (WidgetClass)&dialogClassRec;

extern FormClassRec formClassRec;
WidgetClass formWidgetClass = (WidgetClass)&formClassRec;

extern GripClassRec gripClassRec;
WidgetClass gripWidgetClass = (WidgetClass) &gripClassRec;

extern LabelClassRec labelClassRec;
WidgetClass labelWidgetClass = (WidgetClass)&labelClassRec;

extern LayoutClassRec layoutClassRec;
WidgetClass layoutWidgetClass = (WidgetClass)&layoutClassRec;

extern ListClassRec listClassRec;
WidgetClass listWidgetClass = (WidgetClass)&listClassRec;

extern LogoClassRec logoClassRec;
WidgetClass logoWidgetClass = (WidgetClass) &logoClassRec;

extern MailboxClassRec mailboxClassRec;
WidgetClass mailboxWidgetClass = (WidgetClass) &mailboxClassRec;

extern MenuButtonClassRec menuButtonClassRec;
WidgetClass menuButtonWidgetClass = (WidgetClass) &menuButtonClassRec;

extern PanedClassRec panedClassRec;
WidgetClass panedWidgetClass = (WidgetClass) &panedClassRec;
WidgetClass vPanedWidgetClass = (WidgetClass) &panedClassRec;

extern PannerClassRec pannerClassRec;
WidgetClass pannerWidgetClass = (WidgetClass) &pannerClassRec;

extern PortholeClassRec portholeClassRec;
WidgetClass portholeWidgetClass = (WidgetClass) &portholeClassRec;

extern RepeaterClassRec repeaterClassRec;
WidgetClass repeaterWidgetClass = (WidgetClass) &repeaterClassRec;

extern ScrollbarClassRec scrollbarClassRec;
WidgetClass scrollbarWidgetClass = (WidgetClass)&scrollbarClassRec;

extern SimpleClassRec simpleClassRec;
WidgetClass simpleWidgetClass = (WidgetClass)&simpleClassRec;

extern SimpleMenuClassRec simpleMenuClassRec;
WidgetClass simpleMenuWidgetClass = (WidgetClass)&simpleMenuClassRec;

extern SmeClassRec smeClassRec;
WidgetClass smeObjectClass = (WidgetClass) &smeClassRec;

extern SmeBSBClassRec smeBSBClassRec;
WidgetClass smeBSBObjectClass = (WidgetClass) &smeBSBClassRec;

extern SmeLineClassRec smeLineClassRec;
WidgetClass smeLineObjectClass = (WidgetClass) &smeLineClassRec;

extern SmeThreeDClassRec smeThreeDClassRec;
WidgetClass smeThreeDObjectClass = (WidgetClass) &smeThreeDClassRec;

extern StripChartClassRec stripChartClassRec;
WidgetClass stripChartWidgetClass = (WidgetClass) &stripChartClassRec;

extern TextClassRec textClassRec;
WidgetClass textWidgetClass = (WidgetClass)&textClassRec;

unsigned long FMT8BIT = 0L;

extern TextSinkClassRec textSinkClassRec;
WidgetClass textSinkObjectClass = (WidgetClass)&textSinkClassRec;

extern TextSrcClassRec textSrcClassRec;
WidgetClass textSrcObjectClass = (WidgetClass)&textSrcClassRec;

extern ThreeDClassRec threeDClassRec;
WidgetClass threeDClass = (WidgetClass)&threeDClassRec;

extern ToggleClassRec toggleClassRec;
WidgetClass toggleWidgetClass = (WidgetClass) &toggleClassRec;

extern TreeClassRec treeClassRec;
WidgetClass treeWidgetClass = (WidgetClass) &treeClassRec;

extern VendorShellClassRec vendorShellClassRec;
WidgetClass vendorShellWidgetClass = (WidgetClass) &vendorShellClassRec;

extern ViewportClassRec viewportClassRec;
WidgetClass viewportWidgetClass = (WidgetClass)&viewportClassRec;

#endif /* SUNSHLIB */
