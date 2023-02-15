
/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
* File      :   music.h
* By        :   Libaiao
* Func  :
* Version   :   v1.0
* ============================================================================================================
* 2010-5-3 15:24:52  Libaiao  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "beetles_app.h"


//
/*  //MUSICPLAYER_NORMAL:       読常进入音乐播放器             */
/*  //MUSICPLAYER_BACKGROUND:   音乐播放器作为后台播放               */
/*
typedef enum musicplayer_mode_s
{
    MUSICPLAYER_NORMAL = 0,
    MUSICPLAYER_BACKGROUND = 1,
}musicplayer_mode_e;
*/
typedef struct music_ctrl_s
{
    H_LYR music_layer;              //音乐界面图层
    //H_LYR background_layer;
    //H_LYR list_layer;                 //layer for listbar, 32bpp(32bpp,因为listbar缩略图为32bpp)
    //H_LYR menu_layer;
    //H_LYR spectrum_layer;
    H_WIN h_framewin;                   //
    H_WIN h_frm_alarm_coming ;          /*  //闹钟定时时间到的时候弹出的对话框的窗口句柄                       */
    H_WIN h_spectrum_win;               //for spectrum window
    H_WIN h_frm_menu;
    __s32 root_type;                    //USB Device, or  SDCard
    //explorer_view_mode_e view_mode;       //显示模式
    rat_media_type_t media_type;
    GUI_FONT *font;
    musicplayer_mode_e   mode;
    char root[4];                           /*    //记录根目录         */
    __s32  explorer_play_music;             //文約管理播放音乐
    char  *music_file_name;
    __s32   current_npl_index ;              /*  // 当前播放的npl索襥，主要是为了响应闹铃前先保存好该npl_index，以便闹铃退出后继G幼挪シ�                                         */
} music_ctrl_t;




extern H_WIN app_music_create(root_para_t  *para);
extern __s32 app_music_delete(void  *para);
extern __s32 app_music_restore(H_WIN music_man_win, root_para_t *root_para);

__s32 app_music_get_index(H_WIN music_man_win, __u32 *index);
__s32 app_music_get_root_type(H_WIN music_man_win, unsigned long *root_type);
__s32 app_music_get_mode(H_WIN music_man_win);
__s32 app_music_set_bg_mode(H_WIN music_man_win);

#endif

