/*
*********************************************************************************************************
*                                                   ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              willow app sample
*
*                               (c) Copyright 2006-2007, JOHN, China
*                                           All Rights Reserved
*
* File    : movie_uipara.h
* By      : john.fu
* Version : V1.00
*********************************************************************************************************
*/

#ifndef _DVBPLAY_UIPARA_H_
#define _DVBPLAY_UIPARA_H_

#include "apps.h"

//#include "dtv_ui_i.h"

typedef struct MOVIE_UIPARA_LAYER
{
    __pixel_rgbfmt_t    fb_fmt;                         //ͼ���ʽ
    SIZE    fb_size;                        //framebuffer��С
    RECT    src_rect;                       //srcwin����
    RECT    scn_rect;                       //scnwin����
    __u32  reserved[32];                    //Ԥ��
} __movie_uipara_layer_t;

typedef struct MOVIE_UIPARA
{
    __movie_uipara_layer_t  spsc_lyrwin;   //lyrwin
    __movie_uipara_layer_t  sub_lyrwin;     //sub layer win

    RECT                    preview;    //Ԥ����С
} __movie_uipara_t;


__movie_uipara_t *movie_get_uipara(void);

#endif //_MOVIE_UIPARA_H_
