/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_NUMERIC_TEXT               2       /* control type: string, callback function: (none) */
#define  PANEL_NUMERICGAUGE_RPM           3       /* control type: scale, callback function: (none) */
#define  PANEL_COMMANDBUTTON_START        4       /* control type: command, callback function: COMMAND_START */
#define  PANEL_NUMERICGAUGE_SPEED         5       /* control type: scale, callback function: (none) */
#define  PANEL_NUMERICSLIDE_LOAD          6       /* control type: scale, callback function: (none) */
#define  PANEL_NUMERICSLIDE_THROTTLE      7       /* control type: scale, callback function: (none) */
#define  PANEL_NUMERICMETER_ENGINEC       8       /* control type: scale, callback function: (none) */
#define  PANEL_NUMERICTHERM_INTAKE        9       /* control type: scale, callback function: (none) */
#define  PANEL_NUMERICDIAL_FLOW           10      /* control type: scale, callback function: (none) */
#define  PANEL_COLORNUM_MINIA             11      /* control type: scale, callback function: (none) */
#define  PANEL_TEXTMSG                    12      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  13      /* control type: textMsg, callback function: (none) */
#define  PANEL_ENGINE_COOLANT             14      /* control type: picture, callback function: (none) */
#define  PANEL_DECORATION                 15      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_2               16      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_3               17      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_4               18      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_5               19      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_6               20      /* control type: deco, callback function: (none) */
#define  PANEL_PICTURE                    21      /* control type: picture, callback function: (none) */
#define  PANEL_NUMERICTANK_ERROR          22      /* control type: numeric, callback function: (none) */
#define  PANEL_PICTURE_2                  23      /* control type: picture, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK COMMAND_START(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
