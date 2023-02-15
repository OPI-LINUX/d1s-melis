


#ifndef __EXPLORER_UI_H
#define __EXPLORER_UI_H

#include "beetles_app.h"
#include "Explorer.h"
#include "Explorer_List.h"


typedef enum explr_list_icon_id_s           //
{
    ID_EXP_PALETTE = 0,
    ID_EXP_SD_F,
    ID_EXP_SD_UF,
    ID_EXP_USB_F,
    ID_EXP_USB_UF,

    ID_EXP_CP_MOV,
    ID_EXP_CP_MOV_F,

    ID_EXP_BACKGROUND,

    ID_EXP_USB_ICON_FOCUSED,

    ID_EXP_SD_ICON_FOCUSED,

    ID_EXP_UNKNOWN_FILE,
    ID_EXP_UNKNOWN_FILE_ICON_UNFOCUSED,
    ID_EXP_UNKNOWN_FILE_ICON_FOCUSED,

    ID_EXP_ALL_FILE,
    ID_EXP_ALL_ICON_UNFOCUSED,
    ID_EXP_ALL_ICON_FOCUSED,


    ID_EXP_VIDEO_FILE,
    ID_EXP_VIDEO_ICON_UNFOCUSED,
    ID_EXP_VIDEO_ICON_FOCUSED,

    ID_EXP_MUSIC_FILE,
    ID_EXP_MUSIC_ICON_UNFOCUSED,
    ID_EXP_MUSIC_ICON_FOCUSED,

    ID_EXP_PHOTO_FILE,
    ID_EXP_PHOTO_ICON_UNFOCUSED,
    ID_EXP_PHOTO_ICON_FOCUSED,

    ID_EXP_EBOOK_FILE,
    ID_EXP_EBOOK_ICON_UNFOCUSED,
    ID_EXP_EBOOK_ICON_FOCUSED,

    ID_EXP_GAME_FILE,
    ID_EXP_GAME_ICON_UNFOCUSED,
    ID_EXP_GAME_ICON_FOCUSED,

    ID_EXP_LIST_ITEM_FOCUS_BG,
    ID_EXP_LIST_ITEM_UNFOCUS_BG,
    ID_EXP_LIST_ITEM_ROLL_BG,

    ID_EXP_MEDIATYPE_LIST_ITEM_FOCUS_BG,
    ID_EXP_MEDIATYPE_LIST_ITEM_UNFOCUS_BG,

    ID_EXP_SQUARE_FOCUS,

    ID_EXP_LIST_SCROLL_BG,
    ID_EXP_LIST_SCROLL_HEAD,
    ID_EXP_LIST_SCROLL_BODY,
    ID_EXP_LIST_SCROLL_TAIL,

    ID_EXP_FILE_TYPE_FOCUS_ICON_MOVIE,      //������࣬��ʾ�ļ����͵�ͼ������Ϊ:LEFT_LIST_MUSIC_F.png
    ID_EXP_FILE_TYPE_UNFOCUS_ICON_MOVIE,        //������࣬��ʾ�ļ����͵�ͼ������Ϊ:LEFT_LIST_MUSIC_F.png
    ID_EXP_FILE_TYPE_FOCUS_ICON_MUSIC,
    ID_EXP_FILE_TYPE_UNFOCUS_ICON_MUSIC,
    ID_EXP_FILE_TYPE_FOCUS_ICON_PHOTO,
    ID_EXP_FILE_TYPE_UNFOCUS_ICON_PHOTO,
    ID_EXP_FILE_TYPE_FOCUS_ICON_EBOOK,
    ID_EXP_FILE_TYPE_UNFOCUS_ICON_EBOOK,
    ID_EXP_FILE_TYPE_FOCUS_ICON_GAME,
    ID_EXP_FILE_TYPE_UNFOCUS_ICON_GAME,
    ID_EXP_FILE_TYPE_BG,

    ID_EXP_LISTBAR_MEDIA_TYPE_MOVIE,
    ID_EXP_LISTBAR_MEDIA_TYPE_MUSIC,
    ID_EXP_LISTBAR_MEDIA_TYPE_PHOTO,
    ID_EXP_LISTBAR_MEDIA_TYPE_EBOOK,
    ID_EXP_LISTBAR_MEDIA_TYPE_GAME,
    ID_EXP_LISTBAR_FOLDER_ICON,

    ID_FILE_MAGAGER_BIG_ICON,
    ID_FILE_MAGAGER_LITTLE_ICON,
    ID_EXP_RETURN_FOCUS,
    ID_EXP_RETURN_UNFOCUS,
    ID_EXP_DELETE_FILE_UF_ICON,
    ID_EXP_DELETE_FILE_F_ICON,
    ID_EXP_INFO_BG,
    ID_EXP_ERROR_BMP,
    BROWSER_ICON_MAX,
} explr_list_icon_id_e;                         //e:����enum,  s:����struct

typedef struct explorer_layer_ui_param_s
{
    __pixel_rgbfmt_t fb_fmt;        //ͼ���ʽ
    SIZE fb_size;                   //framebuffer��С
    RECT src_rect;                  //srcwin����
    RECT scn_rect;                  //scnwin����
    __u32 reserved[32];             //Ԥ��
} explorer_layer_ui_param_t;


//ID_MEDIATYPE_MAX ȷ�����listbar����Ŀ
enum
{
    ID_MEDIATYPE_ALL_ITEM = 0,
    ID_MEDIATYPE_MUSIC_ITEM,
    ID_MEDIATYPE_MOVIE_ITEM,
    ID_MEDIATYPE_EBOOK_ITEM,
    ID_MEDIATYPE_PHOTO_ITEM,
#if GAME_SW
    ID_MEDIATYPE_GAME_ITEM,
#endif
    ID_MEDIATYPE_MAX,

};



typedef struct explorer_mediatype_list_item_ui_param_s
{
    RECT ListBarArea;
    RECT item_rect;
    RECT icon_rect;
    RECT text_rect;
} explorer_mediatype_list_item_ui_param_t;


typedef struct explorer_list_item_ui_param_s
{
    RECT ListBarArea;                       //list display area in list mode
    RECT item_rect;                                                     //list item  UI ����

    RECT item_top_line;
    RECT item_bottom_line;
    RECT item_dividing_line;

    RECT icon_rect;
    RECT miniature_rect;

    RECT text_rect;

    RECT focus_icon_rect;
    RECT media_type_rect;           //ý������ͼ��

    RECT title_rect;
    RECT file_size_rect;            //�ļ���С
    RECT time_rect;
    RECT number_rect;
    RECT file_info_area_rect ;      //�ļ���Ϣ����
} explorer_list_item_ui_param_t;

typedef struct explorer_square_item_ui_param_s
{
    RECT ListBarArea;                       //list display area in square mode
    RECT item_rect;                         //��Ŀ��
    RECT focus_rect;                        //�����
    RECT icon_rect;                         //ͼ��
    RECT miniature_rect;                    //����ͼ���
    RECT title_rect;
} explorer_square_item_ui_param_t;


typedef struct explorer_icon_data_s
{
    __u32 res_id;
    HTHEME res_handle;
    void *res_buffer;
} explorer_icon_data_t;         //icon data struct

typedef enum
{
    FILE_TYPE_MUSIC = 0,
    FILE_TYPE_MOVIE,
    FILE_TYPE_PHOTO,
    FILE_TYPE_EBOOK,
    FILE_TYPE_GAME,

    FILE_TYPE_MAX,
} file_type_e ;

typedef struct file_typ_s
{
    RECT  file_type_music_rect ;
    RECT  file_type_movie_rect ;
    RECT  file_type_photo_rect ;
    RECT  file_type_ebook_rect ;
    RECT  file_type_game_rect ;
    RECT  file_type_bg_rect ;
    RECT  file_type_big_bmp_rect;   //�����file_type_bg_rect ����
    RECT  return_bmp_rect;      //�����file_type_bg_rect ����
} file_type_pos_t ;
typedef struct explorer_left_file_type_s   //�����ʾmovie,music,photo,ebook��ý������ͼ��
{
    RECT     file_type_rect ;       //�����ʾ�ļ����͵���������
    file_type_pos_t    file_type_pos[FILE_TYPE_MAX];

    RECT    file_manage_icon_rect;  //�ļ�����ͼ��
    RECT    file_type_txt_rect ;            //��ʾ�ļ������ַ�
    RECT    delete_file_icon_rect ;     //ɾ���ļ���ťλ��
    RECT    return_bmp_rect;    //��¼��ȥͼ������λ��
    RECT    pic_miniature_bmp_rect ;    //ͼƬ��Ӧ������ͼ����
} explorer_left_file_type_t;

typedef struct explorer_viewer_ui_s
{
    explorer_layer_ui_param_t layer;    //layer window

    RECT listview;                      //menuview(frame window)
    RECT listview_bg_rect;
    RECT listview_bg_top_rect;
    RECT listview_bg_bottom_rect;
    RECT listview_bg_left_rect;

    RECT scroll_bg_rect;
    RECT scroll_rect;
    __u32 scroll_head_height;
    __u32 scroll_body_height;
    __s32 scroll_tail_height;

    RECT sd_coor;
    RECT usb_coor;
    RECT cp_mov_dialog_coor;

    RECT ListBarArea;      //listbar content

    explorer_mediatype_list_item_ui_param_t         mediatype_list_item_ui_param;
    explorer_list_item_ui_param_t       list_item_ui_param;                         //UI in list mode
    explorer_square_item_ui_param_t     square_item_ui_param;               //UI in square mode

    explorer_icon_data_t                    icon_table[BROWSER_ICON_MAX + 1];

    explorer_left_file_type_t            file_type_uiparam;
} explorer_viewer_ui_t;



//�������explorer AP��Ƶ�UI����
extern explorer_viewer_ui_t *explorer_get_viewer_ui_param(void);

extern RECT *explorer_get_listview_ui_param(void);

//���list �� item ��Ƶ�UI����
extern explorer_list_item_ui_param_t *explorer_get_list_item_ui_param(void);


//���list �� square ��Ƶ�UI����
extern explorer_square_item_ui_param_t *explorer_get_square_item_ui_param(void);


extern void *explorer_get_listview_icon_res(__u32 index);
extern __s32 explorer_free_listview_icon_res(void);


//���Explorer Layer rectangle
__s32 explorer_get_screen_rect(RECT *screen_rect);

//��ʾtitle�ַ���
extern __s32 explorer_draw_file_manager_title(void);
//�������ͼ����(according to the viewmode of list)
#if (EXPLORER_SUPPORT_MINIATURE == 1)
extern __s32 explorer_get_item_miniature_rect(explr_list_para_t *para, RECT *miniature_rect);
#endif
extern __s32 explorer_get_item_text_rect(explr_list_para_t *para, RECT *text_rect);
extern __s32 explorer_get_item_icon_rect(explr_list_para_t *para, RECT *icon_rect);
extern __s32 explorer_get_item_focus_icon_rect(explr_list_para_t *para, RECT *focus_icon_rect);

extern __s32 explorer_clear_filesize_area(explr_list_para_t *list_para);
extern __s32 explorer_draw_FileSize(__lbar_draw_para_t *draw_param, __s32 file_size, explr_list_para_t *list_para);
extern void explorer_clean_delete_file_hint_area(void);
extern void  explorer_delete_file_draw_process(file_op_info_t *op_info);
extern void  explorer_delete_file_draw_process_ext(explr_list_para_t *list_para);
extern void  explorer_draw_delete_file_failed(void);

extern __s32 explorer_clear_file_create_time_area(explr_list_para_t *list_para);
extern __s32 explorer_draw_file_create_time(__lbar_draw_para_t *draw_param, char *file_name, explr_list_para_t *list_para);
extern __s32 explorer_draw_background(explr_list_para_t *list_para);
extern __s32 explorer_draw_FileTypeIcon(explr_list_para_t *list_para, rat_media_type_t media_type);
extern void *explorer_get_file_item_icon(explorer_view_mode_e view_mode, rat_media_type_t media_type, exp_icon_status_e status);

extern __u32 explorer_get_listbar_pagenum(explr_list_para_t *list_para);
#if (EXPLORER_SUPPORT_MINIATURE == 1)
extern __s32 explorer_draw_pic_miniature_bmp(explr_list_para_t *list_para, __u32 item_index);
#endif
extern __s32 explorer_get_mediatype_item_icon_rect(explr_list_para_t *para, RECT *icon_rect);

extern void *explorer_get_file_mediatype_item_icon(__lbar_draw_para_t *draw_param, exp_icon_status_e status);
__s32 explorer_get_mediatype_item_text_rect(explr_list_para_t *para, RECT *text_rect);

extern __s32 CFG_get_game_and_dict_flag(__s32 id);
__s32 mtime_to_date_string(ES_TIMESTAMP *m_time, char *string);


#endif


