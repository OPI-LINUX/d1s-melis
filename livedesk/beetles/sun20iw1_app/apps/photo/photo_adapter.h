/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2011, CHIPHD, China
*                                            All Rights Reserved
*
* File      : photo_adapter.h
* By        : CQQ
* Func      :
* Version   : v1.0
* ============================================================================================================
* 2011/06/20 9:57  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#ifndef __PHOTO_ADAPTER_H__F8AD6949_0199_47E9_BB76_7202D21318BD__
#define __PHOTO_ADAPTER_H__F8AD6949_0199_47E9_BB76_7202D21318BD__

#include "beetles_app.h"

#define _PHOTO_USE_MEMDEV_       1

//////////////////////////////////////////////////////////////////////////
//�����
#define VK_PHOTO_MENU_POP                 VK_MENU
#define VK_PHOTO_MMENU_ADD_ITEM           VK_RIGHT
#define VK_PHOTO_MMENU_SUB_ITEM           VK_LEFT
#define VK_PHOTO_ESC                      VK_ESCAPE
#define VK_PHOTO_SMENU_LOOP               VK_SUBMENU
#define VK_PHOTO_SMENU_ENTER              VK_ENTER
#define VK_PHOTO_VOLUME_ADD               GUI_MSG_KEY_RIGHT
#define VK_PHOTO_VOLUME_SPEED_ADD         GUI_MSG_KEY_LONGRIGHT
#define VK_PHOTO_VOLUME_SUB               GUI_MSG_KEY_LEFT
#define VK_PHOTO_VOLUME_SPEED_SUB         GUI_MSG_KEY_LONGLEFT
//////////////////////////////////////////////////////////////////////////
#define APP_Temp_ResBmpDraw(_id, _x, _y)  do \
    { \
        load_bmp_res_t _res_bmp; \
        create_bmp_res(_id, _res_bmp); \
        GUI_BMP_RES_Draw(_res_bmp, _x, _y); \
        destroy_bmp_res(_res_bmp); \
    } while(0)
//////////////////////////////////////////////////////////////////////////
#define DEBUG_photo              __msg
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    PHOTO_BGMUSIC_OFF,
    PHOTO_BGMUSIC_ON,
    PHOTO_BGMUSIC_MAX,
} photo_bgmusic_t;
/************************************************************************/
//ȡ����ֵ
/************************************************************************/
#define get_photo_value(_d, _func)     (_d = _func())
extern __u32 photo_get_ratio(void);
extern __u32 photo_get_speedflag(void);
extern __u32 photo_get_effect(void);
extern __u32 photo_get_bgmusic(void);

/************************************************************************/
//���ñ���ֵ
/************************************************************************/
#define set_photo_value(_d, _func)     (_func(_d))
extern __u32 photo_set_ratio(__u32 ratio);
extern __u32 photo_set_speedflag(__u32 speedflag);
extern __u32 photo_set_effect(__u32 effect);
extern __u32 photo_set_bgmusic(__u32 bgmusic);

//����ϵ�
extern __s32 photo_save_play_info(rat_type_t root_type);
//////////////////////////////////////////////////////////////////////////
//�������ַ���
extern __s32 PhotoLongStringShow(H_LYR hLayer, RECT *pRt, char *String, GUI_COLOR TxtClr);
//���ٹ������ַ���ʵ��
extern void  PhotoLongStringDestroy(void);

/************************************************************************/
// ��BMPĳһ����
// pFileData --- ��Դ������õ�����(Ҳ��bmp�ļ�����)
// x��y      --- Ŀ������
// x0, y0, x1, y1 --- Դ����(��bmpͼƬĳһ����)
// return values  --- EPDK_OK : �ɹ�; EPDK_FAIL : ʧ��.
// Remarks : ����������������,Ч�ʽϵ�,TODO:�ĵ��ײ�ʵ��Ϊһ�ο���,���Ч��.
/************************************************************************/
__s32 APP_BMP_Draw(const void *pFileData, int x, int y, int x0, int y0, int x1, int y1);


typedef struct
{
    void *data;
    __s32 data_size;
} RAM_RES_BMP_T;
#define RAM_RES_BMP_DATA(_res)      ((_res)->data)
/************************************************************************/
// ɾ�����ͼƬ
/************************************************************************/
void DestroyRamBmpData(RAM_RES_BMP_T *res);

/************************************************************************/
// ��һ��ͼƬ��Դ(����)����ڴ�ͼƬ��Դ
// res_id        --- ͼƬ��Դid����
// id_count      --- ͼƬ��Դid�����С
// direction     --- ���췽��, 0 : x����; ��0 : y����
// len           --- ���췽��ĳ���
// return values --- �ڴ�ͼƬ����ָ��
// Remarks : �����м��idͼƬΪ����ͼƬ
/************************************************************************/
RAM_RES_BMP_T *CreateRamBmpData(__s32 res_id[], __s32 id_count, __s32 direction, __s32 len);

#endif //__PHOTO_ADAPTER_H__F8AD6949_0199_47E9_BB76_7202D21318BD__
//End of this file

