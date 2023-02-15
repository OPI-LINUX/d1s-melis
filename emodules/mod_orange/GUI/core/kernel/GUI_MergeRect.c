/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, Steven.ZGJ.China
*                                             All Rights Reserved
*
* Moudle  : lemon
* File    : GUI_MergeRect.c
* Purpose : Implementation of GUI_MergeRect
*
* By      : Miracle
* Version : v1.0
* Date    : 2009-2-19 10:58:42
**********************************************************************************************************************
*/

#include "gui_core_kernel_private.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define Min(v0,v1) ((v0>v1) ? v1 : v0)
#define Max(v0,v1) ((v0>v1) ? v0 : v1)

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_MergeRect
*
* Purpose:
*   Calc smalles rectangles containing both rects.
*/
void GUI_MergeRect(GUI_RECT *pDest, const GUI_RECT *pr0, const GUI_RECT *pr1)
{
    if (pDest)
    {
        if (pr0 && pr1)
        {
            pDest->x0 = Min(pr0->x0, pr1->x0);
            pDest->y0 = Min(pr0->y0, pr1->y0);
            pDest->x1 = Max(pr0->x1, pr1->x1);
            pDest->y1 = Max(pr0->y1, pr1->y1);
            return;
        }

        *pDest = *(pr0 ? pr0 : pr1);
    }
}

/*************************** End of file ****************************/
