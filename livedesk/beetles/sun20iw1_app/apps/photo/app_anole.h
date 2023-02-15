/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2011, CHIPHD, China
*                                            All Rights Reserved
*
* File      : app_anole.h
* By        : CQQ
* Func      :
* Version   : v1.0
* ============================================================================================================
* 2011/07/04 16:57  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#ifndef __APP_ANOLE_H__DFB9326F_D8C1_4EC9_9369_496528D2D1FF__
#define __APP_ANOLE_H__DFB9326F_D8C1_4EC9_9369_496528D2D1FF__
//////////////////////////////////////////////////////////////////////////
#include "beetles_app.h"

//////////////////////////////////////////////////////////////////////////
typedef enum
{
    PHOTO_SPEEDFLAG_FAST,
    PHOTO_SPEEDFLAG_MIDDLE,
    PHOTO_SPEEDFLAG_SLOW,
    PHOTO_SPEEDFLAG_MANNUAL,
    PHOTO_SPEEDFLAG_MAX,
    PHOTO_SPEEDFLAG_EXIT,
} photo_speedflag_t;

//ͼƬ�ļ���Ϣ
typedef struct
{
    char               path_name[MAX_FILE_NAME_LEN]; //����ļ�·�����ļ����Ļ���
    char              *pic_name;                     //ָ���ļ���(����·��)
    ES_TIMESTAMP       time;                         //�ļ�����ʱ��
    __anole_pic_info_t pic;                          //ͼƬ��Ϣ(���ļ�·����Ϣ)
} APP_ANOLE_FILE_INFO_T;

//Ӧ�ó�ʼ��anole����
typedef struct
{
    void               *pErrFileData;               //�����֧����ʾ��ͼƬ�ļ�����
    void               *pDecodingData;              //��ʾ���ڽ���ʱ��ͼƬ�ļ�����
    //  H_WIN               hNotifyWnd;                 //�ص���������֪ͨӦ�õĴ��ھ��
    __pCBK_t            cb_err;                     //anole����ʱ�Ļص�����
    __pCBK_t            cb_pic_change;              //anole��ʾͼƬ�ı�ʱ�Ļص�����
    void               *pUserPara;                  //�û����������ڻص�������
} APP_ANOLE_INIT_T;
//////////////////////////////////////////////////////////////////////////
#define TouchMinDist()                 16
//��ʼ��
extern __s32 APP_ANOLE_Init(APP_ANOLE_INIT_T *pInitData);
//�˳�
extern void APP_ANOLE_Exit(void);
//ȡ�ļ���Ϣ
extern __s32 APP_ANOLE_GetCurPicInfo(APP_ANOLE_FILE_INFO_T *file_info);
//���ŵ�ǰͼƬ
extern __s32 APP_ANOLE_PlayCurFile(void);
//������Ч
extern __s32 APP_ANOLE_SetSwitchMode(__u32 Id);
//ȡ��ЧID
extern __s32 APP_ANOLE_GetSwitchMode(void);
//���ò����ٶ�(id_flag -- ��,��,��,�Զ�����; para -- �Զ���ʱ��Ч��Ϊ����)
extern __s32 APP_ANOLE_SetPlayInterval(__s32 id_flag, __s32 para);
//���㴥���Ŵ�
extern __s32 APP_ANOLE_TouchZoomIn(__u32 pixel_change);
//���㴥����С
extern __s32 APP_ANOLE_TouchZoomOut(__u32 pixel_change);
//�����ƶ�
extern __s32 APP_ANOLE_TouchMove(__s32 delta_x, __s32 delta_y);
/************************************************************************/
//����(�ֶ�)�л�ͼƬ
// delta_x = x_up - x_down; > TouchMinDist() play prev, < -TouchMinDist() play next, else see delta_y;
// delta_y = y_up - y_down; > TouchMinDist() play next, < TouchMinDist() play prev, else do nothing;
/************************************************************************/
//extern __s32 APP_ANOLE_TouchSwitchPic(__s32 delta_x, __s32 delta_y);

/************************************************************************/
// anole touch ����
// touch_action --- __gui_msg_touchid_t
// return value --- 0, û����; ��0���Ѷ���Ϣ�����˴���
/************************************************************************/
extern __s32 APP_ANOLE_Touch(__s16 x, __s16 y, __s32 touch_action, __s32 pixel_change);

//��ʾ�ڴ���bmpͼƬ�ļ�
__s32 APP_ANOLE_ShowBufPic(void *file_data);

//��ʾ����ͼƬ
//#define APP_ANOLE_ShowErrorPic(_buffer, _w, _h)   anole_show_pic_from_buffer(_buffer, _w, _h)
__s32 APP_ANOLE_ShowErrorPic(void);

//ȡ��ǰ����ͼƬ���
#define APP_ANOLE_GetCurPicIndex()                anole_npl_get_cur()
#define APP_ANOLE_GetPicNum()                     anole_npl_get_total_count()
#define APP_ANOLE_GetSta()                        anole_get_sta()

void APP_ANOLE_AutoPlay(void);

void APP_ANOLE_Pause(void);

#define APP_ANOLE_SetShowMode(_id)                anole_set_show_mode(_id)
#define APP_ANOLE_ShowPicByIndex(_id)             anole_play_file_by_index(_id)
//#define APP_ANOLE_Zoom(_id)                       DEBUG_CallFunc(anole_zoom_set, _id)//anole_zoom_set(_id)
#define APP_ANOLE_RotateRight()                   DEBUG_CallFunc(anole_rotate_right)//anole_rotate_right()
#define APP_ANOLE_RotateLeft()                    DEBUG_CallFunc(anole_rotate_left)//anole_rotate_left()
#define APP_ANOLE_GetPlayInterval()               anole_get_play_interval()
#define APP_ANOLE_GetZoomIndex()                  anole_zoom_get()
#define APP_ANOLE_ZoomInc()                       DEBUG_CallFunc(anole_zoom_inc)//anole_zoom_inc()
#define APP_ANOLE_ZoomDec()                       DEBUG_CallFunc(anole_zoom_dec)//anole_zoom_dec()
#define APP_ANOLE_ScaleMove(_way)                 DEBUG_CallFunc(anole_scale_move, _way)//anole_scale_move(_way)
#define APP_ANOLE_NPL_Index2File(_id, _pName)     anole_npl_index2file(_id, _pName)

//slider
//#define APP_ANOLE_SLIDER_INIT                     anole_slider_init
#define APP_ANOLE_SliderStart(_x, _y)             DEBUG_CallFunc(anole_slider_start, _x, _y)
#define APP_ANOLE_SliderMove(_x, _y)              DEBUG_CallFunc(anole_slider_move, _x, _y)
#define APP_ANOLE_SliderFinish(_x, _y)            DEBUG_CallFunc(anole_slider_finish, _x, _y)

extern __s32 g_ANOLE_EnableTouch;
#define APP_ANOLE_EnableTouch(_e)                 (g_ANOLE_EnableTouch = _e)
#define APP_ANOLE_IsEnableTouch()                 (g_ANOLE_EnableTouch)

#endif //__APP_ANOLE_H__DFB9326F_D8C1_4EC9_9369_496528D2D1FF__
//End of this file

