/*
*********************************************************************************************************
*                                                   ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              willow app sample
*
*                               (c) Copyright 2006-2007, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    :  ebook_uipara.c
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/

#include "ebook_uipara.h"
#define SET_TIPS_W  191
#define SET_TIPS_H  101

static ebook_uipara_t ebook_uipara;

ebook_uipara_t *get_ebook_uipara(void)
{
    ebook_uipara.tips_lyr.x = 0;
    ebook_uipara.tips_lyr.y = 0;
    ebook_uipara.tips_lyr.w = SET_TIPS_W;
    ebook_uipara.tips_lyr.h = SET_TIPS_H;
    ebook_uipara.page_num_progress_lyr.x = 0;
    ebook_uipara.page_num_progress_lyr.y = 30;
    ebook_uipara.page_num_progress_lyr.w = 24;
    ebook_uipara.page_num_progress_lyr.h = 200;
    //��ʾҳ��ͼ��
    ebook_uipara.page_lyr.x  = 50;
    ebook_uipara.page_lyr.y  = 448;//..
    ebook_uipara.page_lyr.w  = 700;//..
    ebook_uipara.page_lyr.h  = 27;
    /*ebook_uipara.page_lyr.x  = 0;//..
    ebook_uipara.page_lyr.y  = 0;//..
    ebook_uipara.page_lyr.w  = 800;//..
    ebook_uipara.page_lyr.h  = 480;//..*/
    //��ʾplay pause ͼƬλ��
    ebook_uipara.bmp_play.x = 70;//..
    ebook_uipara.bmp_play.y = 0;//..
    ebook_uipara.bmp_play.w = 25;
    ebook_uipara.bmp_play.h = 27;
    ebook_uipara.page_name_pos.x = 230;//.. //������ʾλ��
    ebook_uipara.page_name_pos.y = 0;//..
    ebook_uipara.page_name_pos.w = 260;
    ebook_uipara.page_name_pos.h = 24;
    ebook_uipara.page_no_progress_length.x = 0 ;
    ebook_uipara.page_no_progress_length.y = 0;
    ebook_uipara.page_no_progress_length.w = 12;
    ebook_uipara.page_no_progress_length.h = 196;
    ebook_uipara.page_no_head_length.x = ebook_uipara.page_no_progress_length.x + ebook_uipara.page_no_progress_length.w / 2  ;
    ebook_uipara.page_no_head_length.y = ebook_uipara.page_no_progress_length.y ;
    ebook_uipara.page_no_head_length.w = 12;
    ebook_uipara.page_no_head_length.h = 9;
    ebook_uipara.page_no_dot_length.w = 2;
    ebook_uipara.page_no_dot_length.x = ebook_uipara.page_no_progress_length.x + (ebook_uipara.page_no_progress_length.w - ebook_uipara.page_no_dot_length.w) / 2 ;
    ebook_uipara.page_no_dot_length.y =  ebook_uipara.page_no_progress_length.y + ebook_uipara.page_no_head_length.h ;
    ebook_uipara.page_no_dot_length.h = 1;
    //��ʾҳ��
    ebook_uipara.page_no_pos.x = 550;//..300
    ebook_uipara.page_no_pos.y = 0;//..
    ebook_uipara.page_no_pos.w = 140;
    ebook_uipara.page_no_pos.h = 24;
    ebook_uipara.tips_txt_pos.x = 0;    //�ϵ���ʾ������ʾλ��
    ebook_uipara.tips_txt_pos.y = 30;
    ebook_uipara.tips_txt_pos.w = 180;
    ebook_uipara.tips_txt_pos.h = 15;
    ebook_uipara.tip_yes_bmp_pos.x = 30;
    ebook_uipara.tip_yes_bmp_pos.y = 59;
    ebook_uipara.tip_yes_bmp_pos.w = 37;
    ebook_uipara.tip_yes_bmp_pos.h = 18;
    ebook_uipara.tip_no_bmp_pos.x = 118;
    ebook_uipara.tip_no_bmp_pos.y = 59;
    ebook_uipara.tip_no_bmp_pos.w = 37;
    ebook_uipara.tip_no_bmp_pos.h = 18;
    //��ʾ������ͼ//..
    ebook_uipara.bottom_bg.x = 0;
    ebook_uipara.bottom_bg.y = 0;
    ebook_uipara.bottom_bg.w = 800;
    ebook_uipara.bottom_bg.h = 480;
    //��ת����ͼ��//..
    ebook_uipara.goto_lyr.x = 0;
    ebook_uipara.goto_lyr.y = 0;
    ebook_uipara.goto_lyr.w = 800;
    ebook_uipara.goto_lyr.h = 480;
    //��ת���� ��ʾ��ת ҳ��
    ebook_uipara.goto_page.x = 286;
    ebook_uipara.goto_page.y = 99;
    ebook_uipara.goto_page.w = 250;
    ebook_uipara.goto_page.h = 50;
    return &ebook_uipara;
}

