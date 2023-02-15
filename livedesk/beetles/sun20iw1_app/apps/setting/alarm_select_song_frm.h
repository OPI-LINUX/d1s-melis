
#ifndef __ALARM_SELECT_SONG_FRM_H__
#define __ALARM_SELECT_SONG_FRM_H__

#include "app_setting_i.h"

typedef enum
{
    BUTTON_FCS_OK = 0,
    BUTTON_FCS_LISTEN,
    BUTTON_FCS_CANCLE,

    BUTTON_FCS_MAX

} button_fcs_e;

typedef struct tag_rat_info
{
    HRAT  handle;               //�ļ��������
    __s32 index;                //��ǰ�ļ�������
    __s32 total;                //�ļ�����
} rat_info_t;

typedef struct alarm_sel_song_s
{
    H_LYR layer ;
    GUI_FONT *font ;
    H_LBAR              listbar_handle;
    __hdle              long_str_handle;        //���ַ����������
    rat_info_t          rat;                    //�ļ��������ݽṹ
    char                search_path[RAT_MAX_FULL_PATH_LEN];             //����·��
    rat_media_type_t    media_type;             //�����ļ�������
    rat_type_t          root_type ;             // rat_tf , rat_usb
    HTHEME              h_button_fc_bmp, h_button_uf_bmp, h_list_item_bg ;
    button_fcs_e        button_fcs_id ;         //��ǰ����Ϊȷ��������ȡ��
    char                FullFileName[RAT_MAX_FULL_PATH_LEN];        //���ظ������ڵĴ˴�ѡ���������
    __bool              is_in_sel_disk_scene;       // 1 ��ʾ�ڴ���ѡ�񳡾�
    __u32               current_sel_disk_index ;    //��ǰѡ��Ĵ�������
    __bool              is_listening_music;         // �Ƿ�������������
    __bool              bg_music_is_exist;          // 1 ��ʾ�������ִ���
    rat_play_mode_e     bg_music_play_mode;         // �б�������ʱ���������ֵĲ���ģʽ
    __u32               current_npl_index ;         // ����������ʱ����ǰ���ŵ���������
} alarm_sel_song_t ;


extern H_WIN app_alarm_sel_song_create(H_WIN hparent, __u16 win_id);
extern __s32  app_alarm_sel_song_delete(H_WIN win);
extern rat_type_t  app_alarm_sel_song_get_root_type(__u32 *index);

#endif


