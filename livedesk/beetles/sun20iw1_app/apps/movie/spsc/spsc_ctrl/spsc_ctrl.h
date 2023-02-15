/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2010, ANDY, China
*                                            All Rights Reserved
*
* File      : spsc_ctrl.h
* By        : Bayden.chen
* Func      :
* Version   : v1.0
* ============================================================================================================
* 2011-05-06 Bayden.chen  create this file
**************************************************************************************************************
*/

#ifndef __MOVIE_SPSC_CTRL_H__
#define  __MOVIE_SPSC_CTRL_H__

#include "beetles_app.h"

//����spsc ctrl�ӳ�����ͼ������
typedef enum
{
    //movie_spsc_ctrl_icon_prev=0,
    //movie_spsc_ctrl_icon_next,
    //movie_spsc_ctrl_icon_playpause,
    movie_spsc_ctrl_icon_infor = 0,
    movie_spsc_ctrl_icon_tvout,
    movie_spsc_ctrl_icon_num
} movie_spsc_ctrl_icon_t;

//spsc ctrl scene���������ڵ���Ϣ
typedef enum
{
    spsc_ctrl_scene_msg_prev = 0,
    spsc_ctrl_scene_msg_next,
    spsc_ctrl_scene_msg_rr,
    spsc_ctrl_scene_msg_ff,
    spsc_ctrl_scene_msg_playpause,
    spsc_ctrl_scene_msg_timeset,
    spsc_ctrl_scene_msg_timeout,
    spsc_ctrl_scene_msg_info,
    spsc_ctrl_scene_msg_tvout,
    spsc_ctrl_scene_msg_switch,
    spsc_ctrl_scene_msg_num
} movie_spsc_ctrl_scene_msg_t;

//����spsc ctrl�ӳ����ľ�̬�ؼ�����
typedef enum
{
    movie_spsc_ctrl_static_icon_playmode = 0, //ѭ��ģʽ
    movie_spsc_ctrl_static_icon_channel,      //����
    movie_spsc_ctrl_static_icon_channel_text,
    movie_spsc_ctrl_static_icon_screen_mode,   //����
    movie_spsc_ctrl_static_icon_file_info,     //�ļ���Ϣ
    movie_spsc_ctrl_static_icon_video_info,    //��Ƶ��Ϣ
    movie_spsc_ctrl_static_icon_subtitle,      //��Ļ����
    movie_spsc_ctrl_static_icon_subtitle_text,
    movie_spsc_ctrl_static_icon_filename,      //�ļ���
    movie_spsc_ctrl_static_icon_num
} movie_spsc_ctrl_static_icon_t;

//�����ڷ����ô��ڵ���Ϣ
typedef enum
{
    movie_spsc_ctrl_scene_update_fileinfo = GUI_MSG_ + 1,
    movie_spsc_ctrl_scene_draw_difuse

} movie_spsc_ctrl_scene_recv_msg_t;


typedef struct
{
    // �������
    H_WIN hparent;
    __s32 scene_id;
    H_LYR hlyr;
    __s32 sub_state;

    //�ڲ�����

} movie_spsc_ctrl_create_para_t;

void *movie_spsc_ctrl_scene_create(movie_spsc_ctrl_create_para_t *create_para);
__s32 movie_spsc_ctrl_scene_set_focus(void *handle);
__s32 movie_spsc_ctrl_scene_delete(void *handle);
H_WIN movie_spsc_ctrl_scene_get_hwnd(void *handle);

#endif

