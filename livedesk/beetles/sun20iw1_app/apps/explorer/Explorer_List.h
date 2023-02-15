

#ifndef __EXPLORER_LIST_H__
#define __EXPLORER_LIST_H__

#include "FileList.h"
#define   EXPLORER_SUPPORT_MINIATURE    (0)
typedef enum exp_icon_status_s
{
    UNFOCUSED = 0,
    FOCUSED = 1,
    PRESSED = 2,
} exp_icon_status_e;


//����rat �ļ��������ݽṹ
typedef struct tag_rat_ctrl
{
    HRAT  handle;               //�ļ��������
    __s32 index;                //��ǰ�ļ�������
    __s32 total;                //�ļ�����
    //rat_media_type_t media_type;//�����ļ�������
} rat_ctrl_t;

typedef struct file_type_res_s
{
    HTHEME  file_type_bg_bmp;
    HTHEME  file_type_big_movie_bmp, file_type_big_music_bmp, file_type_big_photo_bmp, file_type_big_ebook_bmp, file_type_big_game_bmp;
    HTHEME  file_type_ebook_f_bmp, file_type_ebook_uf_bmp;
    HTHEME  file_type_movie_f_bmp, file_type_movie_uf_bmp;
    HTHEME  file_type_music_f_bmp, file_type_music_uf_bmp;
    HTHEME  file_type_photo_f_bmp, file_type_photo_uf_bmp;
    HTHEME  file_type_game_f_bmp, file_type_game_uf_bmp;
    HTHEME  fiile_manager_icon_bmp;             //�ļ�����ͼ��
    HTHEME  file_manager_big_bmp;           //�ļ������ͼ��
} file_type_res_t;
typedef struct tag_explr_list_para
{
    H_WIN               list_win;           //���ھ��
    GUI_FONT            *explr_list_font;
    H_LYR               BG_layer;           //����ͼ��
    H_LYR               list_layer;             //listbar ͼ��
    void                *core;
    __s32               root_type;
    __u32               last_start_id;      //�ϴ������ʼID
    __u32               last_focused_id;    //�ϴ�ѡ���ļ�id
    char                *last_filename;     //�ϴ�����ļ� ��

    GUI_FONT            *font;

    char                search_path[RAT_MAX_FULL_PATH_LEN];             //����·��
    H_LBAR              listbar_handle;
    H_LBAR              mediatype_listbar_handle;

    __hdle              long_str_handle;        //���ַ����������

    rat_ctrl_t          rat;                    //�ļ��������ݽṹ
    rat_media_type_t    media_type;             //�����ļ�������
    explorer_view_mode_e view_mode;             //��ʾģʽ,List or square

    file_list_t        *top_file_list;          //top filelist
    file_list_t        *cur_file_list;          //current filelist
    file_item_t        *file_item;              //��icon��file_item;

    H_WIN               h_dialog;
    H_WIN               h_dialog_msg;
    __bool              del_dlg_open;           //ɾ���Ի����Ƿ�open
    H_WIN               h_fw_update_dlg;        //�̼s�i���Ի���

    char                fw_update_file[RAT_MAX_FULL_PATH_LEN];
    root_para_t         *root_para  ;       //��¼��root �������Ĳ���
    file_type_res_t     h_file_type_res;
    HTHEME              h_item_fcs_bmp, h_item_unfcs_bmp, h_bg_bmp, h_return_bmp;
    __bool              enter_sd_usb_flag ; // 1 ��ʾ�Ѿ�����usb / sd ·����
#if (EXPLORER_SUPPORT_MINIATURE == 1)
    void               *listbar_miniature_buf ; //ÿ��item ��ʾ��buf
    __u32               listbar_miniature_size ;    // buffer ��С
    void               *big_miniature_buf ;     // ����ļ�����ʾ�Ĵ�ͼƬ��buf
    __u32               big_miniature_size ;
    __s32               big_miniature_index ;
    __u8                draw_big_miniature_thread ; //��������ͼ�߳�
    __krnl_event_t     *willow_miniature_sem ;  // ��ȡ��С����ͼʱʹ�õ��ź���
    __krnl_event_t     *get_miniature_sem ;     //��ȡ����ͼʱ���ź���
    __u32               draw_miniature_delay_time ;
#endif
    __s32               last_focus_index;//��0��ʼ��
    __s32               mediatype_index;
    __s32               cp_mov_dialog;
    __s32               listbar_left_right;//���������²�����Ӧ������߻����ұߵ�listbar
    char                copy_name_back[RAT_MAX_FULL_PATH_LEN];//�����ļ�������ʱ����
    __bool              filelist_update;

    __u8                handle_thread_id;

} explr_list_para_t;



#define EXPLR_LIST_ENTER            0
#define EXPLR_LIST_RETURN           1

#define MSG_EXPLR_UPDATE            (GUI_MSG_USER_DEF+0)


H_WIN explorer_list_win_create(H_WIN h_parent, explr_list_para_t *para);
__s32 explorer_list_win_delete(H_WIN list_win);
extern __s32  explorer_get_disk_total_num(root_para_t  *para)  ;
extern __s32  explorer_check_disk_exist(__s32 root_type);
extern __s32 GetListItemFileFullPath(explr_list_para_t *list_para, __s32 ItemIndex, char *FilePath);
void  explorer_cmd2parent(H_WIN hwin, __s32 id, __s32 data1, __s32 data2);
#if (EXPLORER_SUPPORT_MINIATURE == 1)
extern __s32 explorer_draw_pic_miniature_bmp(explr_list_para_t *list_para, __u32 item_index);
#endif
extern __s32 rat_get_pic_info_ex(rat_miniature_para_t *para, rat_pic_info_t *pic_info, explr_list_para_t *list_para);

#endif







