/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2010, ANDY, China
*                                            All Rights Reserved
*
* File      :
* By        :
* Func      :
* Version   : v1.0
* ============================================================================================================

**************************************************************************************************************
*/

#ifndef __dtv_setting_disp_H_
#define __dtv_setting_disp_H_

#include "dtv_setting_i.h"

typedef struct tag_dtv_setting_disp_para
{
    GUI_FONT *font;

    H_LYR layer;
    __s32 focus_id;
    __s32 old_focus;

} dtv_setting_disp_para_t;

typedef    enum
{
    //DTV_DISP_SET_DENOISE,//����ʹ��
    DTV_DISP_BLIGHT,  //����
    DTV_DISP_CONTRAST,//�Աȶ�
    DTV_DISP_EDGE,//���
    //DTV_DISP_HUE,       //ɫ��
    DTV_DISP_SATURATION,//���Ͷ�
    DTV_DISP_SET_DETAIL,//������
    DTV_DISP_RADIO,    //ͼ�����


    DTV_DISP_MAX
} __disp_setting_e;

H_WIN dtv_setting_disp_win_create(H_WIN h_parent, dtv_setting_disp_para_t *list_para);
__s32 dtv_setting_disp_win_delete(H_WIN list_win);
#endif
