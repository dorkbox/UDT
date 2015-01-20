/*
 * Whooho. api's and stuff
 */

#ifndef _JAVASOFT_JAWT_MD_H_
#define _JAVASOFT_JAWT_MD_H_

#include <windows.h>
#include "jawt.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Microsoft win32-specific declarations for using the AWT native interface.
 */ 
typedef struct jawt_Win32DrawingSurfaceInfo {
    /* Tada. The native window, DDB, or DIB handle */
    union {
        HWND hwnd;
        HBITMAP hbitmap;
        void* pbits;
    };
    /*
     * This HDC should be the one always used, instead of the one returned from
     * BeginPaint() or any particular calls to GetDC().
     */
    HDC hdc;
    HPALETTE hpalette;
} JAWT_Win32DrawingSurfaceInfo;

#ifdef __cplusplus
}
#endif

#endif
