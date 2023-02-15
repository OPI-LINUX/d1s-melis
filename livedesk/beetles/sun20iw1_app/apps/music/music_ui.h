#ifndef __MUSIC_UI_H__
#define __MUSIC_UI_H__

#define MUSIC_BOTTOM_TEXT_LEN 40


typedef enum music_list_icon_id_s
{
    //for music explorer
    ID_MUSIC_PALETTE = 0,
    ID_MUSIC_BACKGROUND,
    ID_MUSIC_AUDIO_TYPE,
    ID_MUSIC_FILE_UNFOCUSED_ICON,
    ID_MUSIC_FILE_FOCUSED_ICON,
    ID_MUSIC_FOCUS_ITEM_BORDER,
    ID_MUSIC_UNFOCUS_ITEM_BORDER,
    ID_MUSIC_LONG_STR_SCROLL,

    //Headbar
    ID_MUSIC_PLAY_ALL_STATUS_ICON,
    ID_MUSIC_PLAY_ONE_STATUS_ICON,
    ID_MUSIC_PLAY_RANDOM_STATUS_ICON,
    ID_MUSIC_REPEAT_ALL_STATUS_ICON,
    ID_MUSIC_REPEAT_ONE_STATUS_ICON,

    ID_MUSIC_EQ_NORMAL_STATUS_ICON,
    ID_MUSIC_EQ_DBB_STATUS_ICON,
    ID_MUSIC_EQ_POP_STATUS_CION,
    ID_MUSIC_EQ_ROCK_STATUS_CION,
    ID_MUSIC_EQ_CLASSIC_STATUS_CION,
    ID_MUSIC_EQ_JAZZ_STATUS_CION,
    ID_MUSIC_EQ_VOCAL_STATUS_CION,
    ID_MUSIC_EQ_DANCE_STATUS_CION,
    ID_MUSIC_EQ_SOFT_STATUS_CION,

    ID_MUSIC_BL_ON_CION,
    ID_MUSIC_BL_TIME_10S_CION,
    ID_MUSIC_BL_TIME_30S_CION,
    ID_MUSIC_BL_TIME_60S_CION,
    ID_MUSIC_BL_TIME_90S_CION,

    ID_MUSIC_VOL_STATUS_ICON,

    ID_MUSIC_BATTERY_0_ICON,
    ID_MUSIC_BATTERY_1_ICON,
    ID_MUSIC_BATTERY_2_ICON,
    ID_MUSIC_BATTERY_3_ICON,
    ID_MUSIC_BATTERY_4_ICON,
    ID_MUSIC_BATTERY_5_ICON,

    ID_MUSIC_PLAY_ALL_BTN_UNFOCUSED_ICON,
    ID_MUSIC_PLAY_ALL_BTN_FOCUSED_ICON,
    ID_MUSIC_PLAY_ALL_BTN_PRESSED_ICON,

    ID_MUSIC_PLAY_ONE_BTN_UNFOCUSED_ICON,
    ID_MUSIC_PLAY_ONE_BTN_FOCUSED_ICON,
    ID_MUSIC_PLAY_ONE_BTN_PRESSED_ICON,

    ID_MUSIC_PLAY_RANDOM_BTN_UNFOCUSED_ICON,
    ID_MUSIC_PLAY_RANDOM_BTN_FOCUSED_ICON,
    ID_MUSIC_PLAY_RANDOM_BTN_PRESSED_ICON,

    ID_MUSIC_REPEAT_ALL_BTN_UNFOCUSED_ICON,
    ID_MUSIC_REPEAT_ALL_BTN_FOCUSED_ICON,
    ID_MUSIC_REPEAT_ALL_BTN_PRESSED_ICON,

    ID_MUSIC_REPEAT_ONE_BTN_UNFOCUSED_ICON,
    ID_MUSIC_REPEAT_ONE_BTN_FOCUSED_ICON,
    ID_MUSIC_REPEAT_ONE_BTN_PRESSED_ICON,

    ID_MUSIC_EQ_MODE_BTN_UNFOCUSED_ICON,
    ID_MUSIC_EQ_MODE_BTN_FOCUSED_ICON,
    ID_MUSIC_EQ_MODE_BTN_PRESSED_ICON,

    ID_MUSIC_BL_TIME_BTN_UNFOCUSED_CION,
    ID_MUSIC_BL_TIME_BTN_FOCUSED_CION,
    ID_MUSIC_BL_TIME_BTN_PRESSED_CION,

    ID_MUSIC_USB_DISK_BTN_UNFOCUSED_ICON,
    ID_MUSIC_USB_DISK_BTN_FOCUSED_ICON,
    ID_MUSIC_USB_DISK_BTN_PRESSED_ICON,

    ID_MUSIC_SD_CARD_BTN_UNFOCUSED_ICON,
    ID_MUSIC_SD_CARD_BTN_FOCUSED_ICON,
    ID_MUSIC_SD_CARD_BTN_PRESSED_ICON,

    ID_MUSIC_PLAY_PRE_BTN_UNFOCUSED_ICON,
    ID_MUSIC_PLAY_PRE_BTN_FOCUSED_ICON,
    ID_MUSIC_PLAY_PRE_BTN_PRESSED_ICON,

    ID_MUSIC_PLAY_NEXT_BTN_UNFOCUSED_ICON,
    ID_MUSIC_PLAY_NEXT_BTN_FOCUSED_ICON,
    ID_MUSIC_PLAY_NEXT_BTN_PRESSED_ICON,

    ID_MUSIC_PLAY_PAUSE_BTN_UNFOCUSED_ICON,
    ID_MUSIC_PLAY_PAUSE_BTN_FOCUSED_ICON,
    ID_MUSIC_PLAY_PAUSE_BTN_PRESSED_ICON,

    ID_MUSIC_PLAY_BACKWARD_BTN_UNFOCUSED_ICON,
    ID_MUSIC_PLAY_BACKWARD_BTN_FOCUSED_ICON,
    ID_MUSIC_PLAY_BACKWARD_BTN_PRESSED_ICON,

    ID_MUSIC_PLAY_FORWARD_BTN_UNFOCUSED_ICON,
    ID_MUSIC_PLAY_FORWARD_BTN_FOCUSED_ICON,
    ID_MUSIC_PLAY_FORWARD_BTN_PRESSED_ICON,

    ID_MUSIC_PALYING_STATUS_ICON,
    ID_MUSIC_PAUSE_STATUS_ICON,
    ID_MUSIC_PREVIOUS_STATUS_ICON,
    ID_MUSIC_NEXT_STATUS_ICON,

    ID_MUSIC_PROCESS_BAR_HEAD_ICON,
    ID_MUSIC_PROCESS_BAR_BODY_ICON,
    ID_MUSIC_PROCESS_BAR_TAIL_ICON,
    ID_MUSIC_PROCESS_BAR_POINT_ICON,
    ID_MUSIC_PERFORMER_ICON,
    ID_MUSIC_FILE_SIZE_ICON,
    ID_MUSIC_ALBUM_ICON,
    ID_MUSIC_BIT_RATE_ICON,

    //sptrc
    ID_MUSIC_PARTICLE,
    ID_MUSIC_BOUY,

    //scroll bar for explorer
    ID_MUSIC_LIST_SCROLL_BG,
    ID_MUSIC_LIST_SCROLL_HEAD,
    ID_MUSIC_LIST_SCROLL_BODY,
    ID_MUSIC_LIST_SCROLL_TAIL,

    //bg
    ID_MUSIC_BG,
    ID_MUSIC_INFO_BAR,
    ID_MUSIC_PLAYBAR,
    //flag
    ID_MUSIC_PROCESS_BAR_FLAG,

    //bottom
    ID_MUSIC_BACK0,
    ID_MUSIC_EQ_CHANGE0,
    ID_MUSIC_PLAY_MODE0,
    ID_MUSIC_EQ_LYRICS0,
    ID_MUSIC_AB0,//112350  ,
    ID_MUSIC_BACK1,
    ID_MUSIC_EQ_CHANGE1,
    ID_MUSIC_PLAY_MODE1,
    ID_MUSIC_EQ_LYRICS1,
    ID_MUSIC_AB1,//112350  ,
    ID_MUSIC_BOTTOM,
    //play pause
    ID_MUSIC_PLAY0,
    ID_MUSIC_PLAY1,

    //menu_tn   show_mode_btn
    ID_MUSIC_MENU_UNFOCUSED_ICON,
    ID_MUSIC_MENU_FOCUSED_ICON,

    ID_MUSIC_SHOW_MODE_UNFOCUSED_ICON,
    ID_MUSIC_SHOW_MODE_FOCUSED_ICON,

    ID_MUSIC_DEFAULT_ALBUM_ICON,

    ID_MUSIC_HEADBAR_BG,

    MUSIC_ICON_MAX,
} music_list_icon_id_e;                         //e:����enum,  s:����struct

typedef enum music_touch_icon
{
    MUSIC_USB_DISK_TOUCH,
    MUSIC_SD_CARD_TOUCH,
    MUSIC_PLAY_TOUCH,
    MUSIC_BACKWARD_TOUCH,
    MUSIC_FORWARD_TOUCH,
    MUSIC_PREV_TOUCH,
    MUSIC_NEXT_TOUCH,

    MUSIC_BACK_TOUCH,
    MUSIC_EQ_CHANGE_TOUCH,
    MUSIC_PLAY_MODE_TOUCH,
    MUSIC_EQ_LYRICS_TOUCH,
    MUSIC_AB_TOUCH,

    MUSIC_PLAYING_BAR_TOUCH,

    MUSIC_TOUCH_ICON_MAX
} music_touch_icon;

typedef struct music_icon_data_s
{
    __u32 res_id;
    HTHEME res_handle;
    void *res_buffer;
} music_icon_data_t;                                //icon data struct

typedef struct music_layer_ui_s
{
    __pixel_rgbfmt_t fb_fmt;        //ͼ���ʽ
    SIZE fb_size;                   //framebuffer��С
    RECT src_rect;                  //srcwin����
    RECT scn_rect;                  //scnwin����
    __u32 reserved[32];             //Ԥ��
} music_layer_ui_t;

typedef struct music_menu_layer_ui_s
{
    __pixel_rgbfmt_t fb_fmt;        //ͼ���ʽ
    SIZE fb_size;                   //framebuffer��С
    RECT src_rect;                  //srcwin����
    RECT scn_rect;                  //scnwin����
    __u32 reserved[32];             //Ԥ��
} music_menu_layer_ui_t;

typedef struct music_list_ui_s
{
    RECT listbar_rect;                                  //list display area in list mode
    RECT item_rect;                                         //list item  UI ����

    RECT focus_icon_rect;                       //select border
    RECT file_icon_rect;              //�ļ�ͼ��

    RECT text_rect;
} music_list_ui_t;

typedef struct music_scrollbar_ui_s
{
    RECT BG_rect;                       //��������
    RECT scroll_rect;               //������
    __u32 head_height;
    __u32 body_height;
    __s32 tail_height;

} music_scrollbar_ui_t;



//Headbar״̬��ui����
typedef struct music_headbar_ui_s
{
    RECT BG_rect;                           //��������
    RECT play_mode_rect;                    //����ģʽ��ť
    RECT EQ_mode_rect;                      //EQģʽͼ��
    RECT BLight_rect;                       //����ر�ʱ��ͼ��
    RECT volume_icon_rect;                  //����ͼ��
    RECT volume_text_rect;                  //����ֵ��ʾ����
    RECT battery_rect;                      //���ͼ��
    RECT sys_time_rect;                         //ʱ����ʾ
} music_headbar_ui_t;


//���������沿��
typedef struct music_palyer_ui_s
{
    RECT contrl_BG_rect;        //���ư�ť������ʾ������Ƶ�ײ��ֹ���һ����ʾ����
    RECT btn_hint_rect;         //������ʾ������
    RECT play_mode_btn_rect;    //����ģʽ��ť
    RECT EQ_mode_btn_rect;      //EQģʽ��ť
    RECT BLight_btn_rect;           //����ʱ����ư�ť

    RECT play_usb_disk_btn_rect;        //
    RECT play_sd_card_btn_rect;     //
    RECT play_pre_btn_rect;     //��һ�װ�ť
    RECT play_next_btn_rect;    //��һ�װ�ť
    RECT play_pause_btn_rect;   //������ͣ��ť
    RECT backward_btn_rect;     //���˰�ť
    RECT forward_btn_rect;      //�����ť

    RECT play_status_rect;      //����״̬ͼ��(��ͣ�򲥷�)
    RECT play_time;                     //��ǰ����ʱ��
    RECT total_time;                    //��ʱ����ʾ����

    RECT process_bar_head_rect;     //������ͷ��
    RECT process_bar_body_rect;     //������body
    RECT process_bar_point_rect;    //��ǰ���ȵ�
    RECT process_bar_tail_rect;     //������β��
    RECT process_bar_flag_rect;     //������β��

    RECT performer_icon_rect;   //�ݳ���ͼ��
    RECT performer_name_rect; //�ݳ�������
    RECT file_size_icon_rect;   //�ļ���Сͼ��
    RECT file_size_text_rect;           //�ļ���С
    RECT album_icon_rect;       //ר������ͼ��
    RECT album_text_rect;       //ר�������ı�
    RECT bit_rate_icon_rect;    //BitRateͼ��
    RECT bit_rate_text_rect;    //�ı���ʾ

    RECT menu_icon_rect;             //�˵�
    RECT menu_text_rect;            //�˵��ı�
    RECT show_mode_icon_rect;       //��ʾ��ʽ
    RECT show_mode_text_rect;        // ��ʾ��ʽ�ı�
    RECT ab_icon_rect;       //ab����
    RECT ab_text_rect;        // ab ����

    RECT tack_text_rect;            // ��ʾ����
} music_palyer_ui_t;

//����������ʾ����
typedef struct music_volume_bar_ui_s
{
    RECT BG_rect;                       //����������������
    RECT left_speaker_rect; //�������ͼ��
    RECT vol_bar_rect;          //����������ͼ��
    RECT right_speaker_rect;//�ұ�����ͼ��
} music_volume_bar_ui_t;

typedef struct music_spectrum_ui_s
{
    RECT BG_rect;
} music_spectrum_ui_t;


typedef struct music_bg_ui_s
{
    RECT BG_rect;
    //  RECT InfoBar_rect;
    //  RECT PlayBar_rect;
} music_bg_ui_t;

typedef struct music_bottom_ui_s
{
    RECT    back_rect;
    RECT    EQ_Change_rect;
    RECT    play_mode_rect;
    RECT    EQ_lyrics_rect;
    RECT    AB_rect;
    RECT    bottom_rect;
} music_bottom_ui_t;

typedef struct music_album_ui_s
{
    RECT    albumRect;
    void   *albumBuf;
} music_album_ui_t;

typedef struct music_ui_s
{
    music_layer_ui_t            layer;              //����������ͼ���������
    music_menu_layer_ui_t       menu_layer;

    music_list_ui_t             list_ctrl;      //�����list
    music_scrollbar_ui_t    scrollbar;      //�����������

    music_headbar_ui_t      headbar;            //����״̬��
    music_palyer_ui_t     player;               //����������
    music_volume_bar_ui_t volume_bar;       //����������

    music_spectrum_ui_t     spectrum;

    music_bg_ui_t           bg;

    music_bottom_ui_t       bottom;

    music_album_ui_t        album;

    music_icon_data_t       icon_table[MUSIC_ICON_MAX + 1];
} music_ui_t;

//��ʾ������
#define     C_LAYER_CACHE_AREA_X        310
#define     C_LAYER_CACHE_AREA_Y        40
#define     C_LAYER_CACHE_AREA_W        180
#define     C_LAYER_CACHE_AREA_H        140

extern __s32   tv_out_status;

extern GUI_RECT *music_get_touch_rect(__u32 iconRectID);
extern __bool music_is_touch_icon(__u32 iconRectID, __u16 x, __u16 y);

extern music_ui_t *music_get_ui_param(void);                    //���UI����

//���Explorer ����layer size
extern __s32 music_get_screen_rect(RECT *screen_rect);

extern void *music_get_album_buf(void);
extern void *music_get_icon_res(__u32 index);
extern __s32 music_free_icon_res(void);

//������������UI����
extern music_ui_t *music_get_ui_param(void);

//������������UI����
extern music_list_ui_t *music_get_list_ui_param(void);

//��ö���HeadbarUI����
extern music_headbar_ui_t *music_get_headbar_ui_param(void);

//��ö���playerUI����
extern music_palyer_ui_t *music_get_player_ui_param(void);

//���volume barUI����
extern music_volume_bar_ui_t *music_get_volume_bar_ui_param(void);

//���listbarÿҳ������ʾ��Ŀ��
extern __u32 music_count_listbar_pagenum(void);

void music_modify_icon_res(__s32 rat_root_type);


#endif



