#ifndef __MUSIC_PLAYER_H__
#define __MUSIC_PLAYER_H__

#include "beetles_app.h"

#include "music_list.h"

#include "spectrum.h"

//#include "VolumeBar.h"

#include "music_lyrics.h"

typedef enum music_spectrum_cmd_s
{
    MUSIC_CLEAR_SPECTRUM = 0X88,
    MUSIC_REBUILD_SPECTRUM,

} music_spectrum_cmd_t;

typedef enum music_btn_status_s
{
    UNFOCUSED = 0,
    FOCUSED = 1,
    PRESSED = 2,
} music_btn_status_e;

typedef enum music_play_status_s
{
    MUSIC_PLAYING = 0,
    MUSIC_PAUSE   = 1,
    MUSIC_STOPED,
} music_play_status_e;
//Play mode

typedef enum music_menu_mode_s
{
    MUSIC_EXPLORER_MODE,
    MUSIC_SET_PLAYER_MODE,
} music_menu_mode_e;

typedef enum music_menu_ID_s
{
    SET_PLAY_MODE,
    SET_EQ_MODE,
    SET_BL_TIME,
    SET_PLAY_PREVIOUS,
    SET_PLAY_NEXT,
    SET_PLAY_PAUSE,
    SET_PLAY_BACKWARD,
    SET_PLAY_FORWARD,
} music_menu_ID_e;

typedef enum music_menu_touch_ID_s
{
    SET_PLAY_PAUSE_TOUCH = 0,
    SET_PLAY_BACKWARD_TOUCH,
    SET_PLAY_FORWARD_TOUCH,
    SET_PLAY_PREVIOUS_TOUCH,
    SET_PLAY_NEXT_TOUCH,
} music_menu_touch_ID_e;

typedef enum music_play_mode_s
{
    MUSIC_PALY_ALL = 0x0,               //���������ļ��������ظ�
    MUSIC_PLAY_ONE,
    MUSIC_PLAY_RANDOM,
    MUSIC_REPEAT_ALL,
    MUSIC_REPEAT_ONE,                   //�ظ�����һ��
} music_play_mode_e;

//EQ mode
typedef enum music_EQ_mode_s
{
    MUSIC_EQ_NORMAL = 0,
    MUSIC_EQ_DBB,
    MUSIC_EQ_POP,
    MUSIC_EQ_ROCK,
    MUSIC_EQ_CLASSIC,
    MUSIC_EQ_JAZZ,
    MUSIC_EQ_VOCAL,
    MUSIC_EQ_DANCE,
    MUSIC_EQ_SOFT,

} music_EQ_mode_e;

typedef enum music_AB_mode_s
{
    MUSIC_AB = 0,
    MUSIC_A,
    MUSIC_B,
} music_AB_mode_e;

//background light last time
typedef enum music_BL_time_s
{
    MUSIC_BL_ON = 0,                    //Never off
    MUSIC_BL_LAST_10S,              //last 10second
    MUSIC_BL_LAST_30S,                  //last 30second
    MUSIC_BL_LAST_60S,                  //last 60second
    MUSIC_BL_LAST_90S,                  //last 90second
} music_BL_time_e;

typedef enum music_LYRICS_s
{
    MUSIC_SPECTRUM = 0,
    MUSIC_LYRICS,
    MUSIC_ALBUM,

    MUSIC_LYRICS_MAX,
} music_LYRICS_e;


typedef enum music_play_btn_s
{
    MUSIC_LIST_MENU_ICON = 0,
#ifndef MUSIC_VIEW_CHANGE_20160811
    MUSIC_MENU_ICON,
    MUSIC_AB_ICON,
    MUSIC_PLAY_BTN_ICON_MAX,
#endif
} music_play_btn_e;

typedef struct audio_file_info_s
{
    char filename[RAT_MAX_FULL_PATH_LEN];
    char artist[ROBIN_ARTIST_SIZE];
    char album[ROBIN_ALBUM_SIZE];
    __s32 filesize;
    __s32 bitrate;
    __epdk_charset_enm_e  artist_charset ;
    __epdk_charset_enm_e  album_charset ;
} audio_file_info_t;


typedef struct music_player_ctrl_s
{
    H_WIN                       h_framewin;                     //���ھ��
    GUI_FONT                        *font;
    H_LYR                           music_layer;                    //ǰ��ͼ��
    //H_LYR                             background_layer;               //����ͼ��
    //void                              *core;
    __bool              visiable;                   //�Ƿ���ʾ�����棬����ʾ��ʾ��������״̬

    __s32               root_type;                  //USB or SDCard

    rat_media_type_t    media_type;                 //�����ļ�������
    H_LBAR              listbar_handle;
    music_list_ctrl_t   *list_ctrl;                 //�ļ������
    __s32               menu_index;                 // �˵���show_mode_menu

    music_menu_mode_e   menu_mode;                  //�˵�ģʽ, Explorer mode ,or setting player mode
    music_menu_ID_e     menu_id;                    //menu id in  setting mode ,
    music_menu_touch_ID_e       menu_touch_id;                  //menu id in  setting mode ,
    __cedar_status_t    setting_status;             //FF,�������ÿ��
    //music_play_mode_e   play_mode;                    //����ģʽ
    rat_play_mode_e     play_mode;                  //����ģʽ
    music_EQ_mode_e     EQ_mode;                    //��Чģʽ
    music_AB_mode_e     AB_mode;
    music_BL_time_e     BL_time;                    //�������ʱ��
    //__s32             volume;                     //����
    //power_level_e     battery_status;             //���״̬

    //__u32                 cur_file_total_time;        //��ǰ�����ļ���ʱ��
    __u32               play_index;                 //��ǰ�����ļ�������
    audio_file_info_t   play_file_info;             //�����ļ���Ϣ

    H_LYRIC             h_lyrics;                   //��ʾ��
    music_LYRICS_e      showMode;                   //0 Ƶ��, 1 ���
    //spectrum_para_t       spectrum_para;              //Ƶ�׿���
    //H_WIN              h_spectrum;                    //spectrum�ؼ����

    __u32               AutoExitMenuCnt;            //�Զ��˳���ʱ��
    __u32               CheckFocusItemCnt;          //��⽹����Ŀ�Ƿ��뵱ǰ������Ŀһ��
    __u32               RefreshPlayTimeCnt;         //ˢ�²���ʱ��timercount
    __u32               RefreshSystimeCnt;          //ˢ��ϵͳʱ��timercount
    __u32               RefreshLyricsTimerCnt;      //ˢ�¸��ʱ��timercount
    //VolumeBar_t       *hVolumeBar;

    __bool              isTouchPlayingBar;
    __bool              isTouchMusicList;
    __bool              isTouchMusicListMove;
    __u32               curTouchPlayingBarTime;
    //music_rat_ctrl_t      rat;                        //�ļ��������ݽṹ
    //explorer_view_mode_e view_mode;                   //��ʾģʽ,List or square
    __s32   explorer_play_music;                        //�ļ�����������
    char   *music_file_name;
    __bool  alarmComing;

    //���ֵ��===================================
    __s32  ndigit_song_num; //���ֵ�����
    __u8   digit_timmer_id; //���ֵ��timmer id��
    H_LYR  hdigit_layer;    //��ʾ��ǰ���ֵ�����ֵ�ͼ��
    //=============================================

} music_player_ctrl_t; //NOTE: size <= 1024 bytes

#define     MUSIC_LIST_WIN_ID       (APP_MUSIC_ID+1)

//1����10ms
#define     MUSIC_SYSTIMER_INTERVAL (100)                           // 1 ������һ���ж�
#define     MUSIC_REFRESH_SYSTIME_CNT   (60)                            //ÿ���ӵ�����ʾʱ��һ��


#define     MUSIC_TIMER_INTERVAL            (22)
#define     MUSIC_REFRESH_PLAYTIME_CNT      (80 / MUSIC_TIMER_INTERVAL)
#define     MUSIC_REFRESH_LYRICS_CNT        (100 / MUSIC_TIMER_INTERVAL)

#define     MUSIC_SPECTRUM_TIMER_INTERVAL   (15)

#define     MUSIC_CHECK_FOCUS_TIMER_INTERVAL (100*30)           //30����û�в������򷵻ص�ʵ�ʲ��ŵ���Ŀ
#define     MUSIC_CHECK_FOCUS_TIMER_CNT       (20)              //

#define     MUSIC_FF_RR_TIMER_INTERVAL       (300)                  //���ʱ�������΢�����һЩ

//2���Զ��˳��˵�
#define     C_AUTO_EXIT_MENU_TIME   (35)



#define MUSIC_FF_SPEED      (CEDAR_FASTEST_FRSPEED/8)
#define MUSIC_RR_SPEED      (CEDAR_FASTEST_FRSPEED/4)


//#define       C_MUSIC_DEBUG

extern H_WIN music_player_win_create(H_WIN h_parent, music_player_ctrl_t *para);

extern __s32 music_player_restore(H_WIN  music_frame_win, __s32 root_type);

__s32 __music_save_breakpoint_info(__s32 index);
__s32 __music_load_breakpoint_info(__s32 index, __cedar_tag_inf_t *tag_inf);

__s32 music_player_scene_delete(H_WIN  music_frame_win);
extern __s32 music_player_delete_digit_layer(H_WIN       music_frame_win);

#endif

