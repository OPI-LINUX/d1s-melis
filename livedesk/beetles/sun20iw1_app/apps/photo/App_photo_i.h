/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2011, CHIPHD, China
*                                            All Rights Reserved
*
* File      : App_photo_i.h
* By        : CQQ
* Func      :
* Version   : v1.0
* ============================================================================================================
* 2011/06/07 15:43  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#ifndef __APP_PHOTO_I_H__9C37A19E_F096_4F25_A49B_C6D2BA82BB2A__
#define __APP_PHOTO_I_H__9C37A19E_F096_4F25_A49B_C6D2BA82BB2A__

#include "beetles_app.h"
#include "photo_uipara.h"
#include "photo_adapter.h"
#include "photo_main.h"
//#include "photo_menu.h"
#include "app_anole.h"
#include "photo_fileinfo.h"

//////////////////////////////////////////////////////////////////////////
extern H_WIN    g_PhotoMainWnd;

#define GetPhotoManWnd()                    (g_PhotoMainWnd)
#define PHOTO_GetWndPara(_p, _t, _hwnd)     (_p = (_t *)GUI_WinGetAttr(_hwnd))

#define SSET_TBAR_WIN_SRC_width             (800)           /*sset tbar �Ĵ�����ʾ������*/
#define SSET_TBAR_WIN_SRC_HEIGHT            (40)             /*sset tbar �Ĵ�����ʾ����߶�*/

#undef   NEED_SHOW_ANOLE_SATUT              /*��Ҫshow ״̬*/

//////////////////////////////////////////////////////////////////////////
//
#define SHOW_UI                             1
#define HIDE_UI                             0
//�Զ�����Ϣ
enum
{
    MSG_PHOTO_COMMAND           = (GUI_MSG_USER_DEF + 1),
    MSG_PHOTO_NEW_PLAY          ,

    /*para (HIDE_UI / SHOW_UI)*/
    MSG_PHOTO_SHOW_HIDE_PARA    ,

    /***********************/
    MSG_PHOTO_SCALE             ,
};

enum // for MSG_PHOTO_COMMAND
{
    ID_PHOTO_CMD_EXIT           = 0,
    ID_PHOTO_CMD_POP_MENU       ,
    ID_PHOTO_CMD_HIDE_MENU      ,
    ID_PHOTO_CMD_POP_SAVE_DIALOG,
    ID_PHOTO_CMD_BG_MUSIC       ,
    ID_PHOTO_CMD_INTERVAL       ,
    ID_PHOTO_CMD_SCALE          ,
    ID_PHOTO_CMD_TOUCH_UP       , // para = MAKELONG(x, y)
    ID_PHOTO_CMD_KILL_PHOTO_FRMWIN  ,     /*ɱ��photo ��ʵ�崰�� �ͷ���Դ*/

    /*UI �������ʾ����*/
    ID_PHOTO_CMD_PAUSE          ,
    ID_PHOTO_CMD_AUTOPLAY       ,
    ID_PHOTO_CMD_PLAYPREV       ,
    ID_PHOTO_CMD_PLAYNEXT       ,
    /************************/
    ID_MAX_PHOTO_CMD,
    ID_ANOLE_CMD_0              = 100,
    ID_ANOLE_CMD_INIT           ,
    ID_ANOLE_CMD_UNINIT         ,
    ID_ANOLE_CMD_AUTOPLAY       ,
    ID_ANOLE_CMD_PAUSE          ,
    ID_ANOLE_CMD_ZOOM           ,
    ID_ANOLE_CMD_ROTATE         ,
    ID_ANOLE_CMD_SLIDESHOW      ,
    ID_ANOLE_CMD_INTERVAL       ,
    ID_ANOLE_CMD_SCALE          ,
    ID_ANOLE_CMD_FILEINFO       ,

    ID_MAX_ANOLE_CMD
};


//����״̬
typedef struct
{
    load_bmp_res_t  bmp_pause;
    load_bmp_res_t  bmp_play_backward;
    load_bmp_res_t  bmp_play_forward;
    load_bmp_res_t  bmp_next;
    load_bmp_res_t  bmp_prev;
    load_bmp_res_t  bmp_auto_play;
} PHOTO_PLAY_STATUS_RES_T;


typedef  enum
{
    PHOTO_SCALE_ITEM_ALL = 0,
    PHOTO_SCALE_ITEM_ROTATELEFT,
    PHOTO_SCALE_ITEM_MOVE_UP,
    PHOTO_SCALE_ITEM_RPTATERIGHT,
    PHOTO_SCALE_ITEM_MOVE_LEFT,
    PHOTO_SCALE_ITEM_EXIT,
    PHOTO_SCALE_ITEM_MOVE_RIGHT,
    PHOTO_SCALE_ITEM_ZOOMDEC,
    PHOTO_SCALE_ITEM_MOVE_DOWN,
    PHOTO_SCALE_ITEM_ZOOMINC,

} __photo_scale_item_draw_e;
//�����ڿ�������
typedef struct
{
    int32_t                     last_action;
    __pos_t                     last_pos;
    int32_t                     show_para; //
    __photo_scale_item_draw_e   scale_new_item;       /*��ǰ��scale �����item ���*/
    __photo_scale_item_draw_e   scale_old_item;       /*�ɽ���item ��*/
} PHOTO_MAIN_FRW_CTRL_PARA_T;

//��������Դ
typedef struct
{
    PHOTO_FILE_INFO_RES_T       file_info;
    PHOTO_PLAY_STATUS_RES_T     status;
    //  load_bmp_res_t           bmp_bg;
} PHOTO_MAIN_FRW_RES_T;

typedef struct
{
    PHOTO_MAIN_FRW_RES_T        res;
    PHOTO_MAIN_FRW_CTRL_PARA_T  ctrl_para;
} PHOTO_MAIN_FRW_T;

#define     PHOTO_SLIDE_SHOW_ID         0x300
#define     PHOTO_PLAY_SPEED_ID         0x301


//����ID
#define PHOTO_SAVE_BG_DIALOG_ID         0xfedc
#define PHOTO_BG_OK_DIALOG_ID           (PHOTO_SAVE_BG_DIALOG_ID - 1)
//////////////////////////////////////////////////////////////////////////
#define do_photo_command(_hwnd, _cmd, _d) do \
    { \
        __msg("(%x) -> do_photo_command %s (%d, %d)\n", _hwnd, # _cmd, _cmd, _d); \
        SEND_MSG(MSG_PHOTO_COMMAND, _hwnd, GetPhotoManWnd(), _cmd, _d); \
    } while(0)

#define do_anole_command(_hwnd, _cmd, _d) do \
    { \
        __wrn("(%x) -> do_anole_command %s (%d, %d)\n", _hwnd, # _cmd, _cmd, _d);\
        __msg("(%x) -> do_anole_command %s (%d, %d)\n", _hwnd, # _cmd, _cmd, _d); \
        SEND_MSG(MSG_PHOTO_COMMAND, _hwnd, GetPhotoManWnd(), _cmd, _d); \
    } while(0)

#define todo_photo_command(_hwnd, _cmd, _d) do \
    { \
        __msg("(%x) -> todo_photo_command %s (%d, %d)\n", _hwnd, # _cmd, _cmd, _d); \
        NOTIFY_MSG(MSG_PHOTO_COMMAND, _hwnd, GetPhotoManWnd(), _cmd, _d); \
    } while(0)

#define todo_anole_command(_hwnd, _cmd, _d) do \
    { \
        __msg("(%x) -> todo_anole_command %s (%d, %d)\n", _hwnd, # _cmd, _cmd, _d); \
        NOTIFY_MSG(MSG_PHOTO_COMMAND, _hwnd, GetPhotoManWnd(), _cmd, _d); \
    } while(0)
//////////////////////////////////////////////////////////////////////////
//���������ݲ���
typedef struct
{
    H_WIN                   h_main_frw;
    H_LYR                   h_main_lyr;

    H_WIN                   h_dialog;
    H_WIN                   h_frm_alarm_coming ; //���Ӷ�ʱʱ�䵽��ʱ�򵯳��ĶԻ���Ĵ��ھ��
    __bool                  need_to_open_music ; // 1 ��ʾ��Ҫ���´򿪱�Ӱ���֣���Ҫ����������֮�󣬻�ر�����

    H_WIN                   h_sset;
    __sset_create_para_t    *sset_create_para;

    GUI_FONT                *font;
    H_WIN                   root_type;

    PHOTO_MAIN_FRW_T        *photo_frm_para;               /*photo frm win ��˽������*/

} PHOTO_WND_T;

#endif //__APP_PHOTO_I_H__9C37A19E_F096_4F25_A49B_C6D2BA82BB2A__
//End of this file

