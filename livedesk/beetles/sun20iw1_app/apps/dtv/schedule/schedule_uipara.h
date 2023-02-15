/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2010, ANDY, China
*                                            All Rights Reserved
*
* File      : schedules_uipara.h
* By        : lantian.lin
* Func  : ui parameter
* Version   : v1.0
* ============================================================================================================
**************************************************************************************************************
*/

#ifndef __SCHEDULE_UIPARA_H_
#define  __SCHEDULE_UIPARA_H_
#include "schedule_i.h"

#if 0
typedef struct schedule_uipara_s
{
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
    tips_uipara_t  schedule_tips_uipara;    //��ʾ������λ��
} schedule_uipara_t;

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
schedule_uipara_t *schedule_get_uipara(void);
//tips_uipara_t* schedule_get_tips_uipara(void);

#endif
#endif      /*__SCHEDULE_UIPARA_H_*/

