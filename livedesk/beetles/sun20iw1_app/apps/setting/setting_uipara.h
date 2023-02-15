/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2010, ANDY, China
*                                            All Rights Reserved
*
* File      : settings_uipara.h
* By        : lantian.lin
* Func  : ui parameter
* Version   : v1.0
* ============================================================================================================
**************************************************************************************************************
*/

#ifndef __SETTING_UIPARA_H_
#define  __SETTING_UIPARA_H_
#include "apps.h"

typedef struct res_pos_s
{
    __s32 x;
    __s32 y;
    __s32 w;
    __s32 h;
} res_pos_t;

typedef struct tips_uipara
{
    res_pos_t   tips_lyr ;
    res_pos_t       sys_info_bmp_pos ;  //ϵͳ��ϢͼƬλ��
    res_pos_t       ok_bmp_pos;         // OK ͼƬλ��
    res_pos_t   tip_tsize_pos;      // �������ַ�λ��
    res_pos_t       tip_fsize_pos;      //ʣ�������ַ�λ��

} tips_uipara_t;

typedef struct setting_uipara_s
{
    res_pos_t   item_general_setting;       //ͨ������
    res_pos_t   item_power_manager;     //��Դ����

    res_pos_t   item_alarm1_setting;        //ͨ������
    res_pos_t   item_alarm2_setting;        //��Դ����
    res_pos_t   item_alarm3_setting;        //��Դ����

    res_pos_t   lyr_win;
    res_pos_t   lyr_scn;
    res_pos_t   item;       //ÿ��item��λ������ʹ�С
    res_pos_t   left_button;    //������������εİ�ť
    res_pos_t   right_button;   //���ҷ���������ΰ�ť
    OFFSET txt_pos;     //ͨ������    ����Դ�����ַ�����
    res_pos_t   string_detail_pos_de;   //ÿ��ѡ���Ӧ����ϸ��Ϣ˵��������������
    res_pos_t   string_detail_pos_ex;   //ÿ��ѡ���Ӧ����ϸ��Ϣ˵��������������
    res_pos_t   item_title_pos;
    res_pos_t   item_content_pos;
    res_pos_t   return_bmp;         //����ͼƬ����λ��
    res_pos_t   long_string_area ;  // long string scroll area
    tips_uipara_t  setting_tips_uipara; //��ʾ������λ��

    /*scroll bar param*/
    RECT scroll_bg_rect;
    RECT scroll_rect;
    __u32 scroll_head_height;
    __u32 scroll_body_height;
    __s32 scroll_tail_height;

} setting_uipara_t;

/*
typedef struct adjust_uipara
{
    RECT    adjust_finish;//У׼�����ʾ��
    OFFSET  cross_pos_1;
    OFFSET  cross_pos_2;
    OFFSET  cross_pos_3;
    OFFSET  cross_pos_4;
}adjust_uipara_t;
*/
setting_uipara_t *setting_get_uipara(void);
//tips_uipara_t* setting_get_tips_uipara(void);
#endif      /*__SETTING_UIPARA_H_*/

