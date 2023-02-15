
//libaiao
#include <log.h>
#include "beetles_app.h"
#include "music_ui.h"

/*#define KARAOKE_LYRIC_USE_MALLOC 1


#if KARAOKE_LYRIC_USE_MALLOC
#define esMEMS_Balloc(x) esMEMS_Malloc(0, x)
#define esMEMS_Bfree(x, s) esMEMS_Mfree(0, x)
#endif
*/

static GUI_RECT music_touch_rect[MUSIC_TOUCH_ICON_MAX] =
{
    {263 - 38, 163, 296 - 38, 185},
    {263, 163, 296, 185},
    {263 + 38, 163, 296 + 38, 185},
    {302 + 38, 163, 330 + 38, 185},
    {334 + 38, 163, 362 + 38, 185},
    {368 + 38, 163, 403 + 38, 185},
    {406 + 38, 163, 442 + 38, 185},

    //      {16+MUSIC_BOTTOM_TEXT_LEN*0,235,61+MUSIC_BOTTOM_TEXT_LEN*0+48,271},
    //      {85+MUSIC_BOTTOM_TEXT_LEN*1,235,130+MUSIC_BOTTOM_TEXT_LEN*1+32,271},
    //      {164+MUSIC_BOTTOM_TEXT_LEN*2,235,209+MUSIC_BOTTOM_TEXT_LEN*2+48,271},
    //      {236+MUSIC_BOTTOM_TEXT_LEN*3,235,282+MUSIC_BOTTOM_TEXT_LEN*3+64,271},


    {10 + 20 * 0 - 8/*+MUSIC_BOTTOM_TEXT_LEN*0*/, 235, 10 + 20 * 0 + 72, 271}, //24
    {72 + 20 * 1 - 8/*+MUSIC_BOTTOM_TEXT_LEN*1*/, 235, 72 + 20 * 1 + 72, 271}, //93
    {139 + 20 * 2 - 8/*+MUSIC_BOTTOM_TEXT_LEN*2*/, 235, 139 + 20 * 2 + 72, 271}, //172
    {206 + 20 * 3 - 8/*+MUSIC_BOTTOM_TEXT_LEN*3*/, 235, 206 + 20 * 3 + 72, 271}, //245
    {308 + 20 * 4 - 8/*+MUSIC_BOTTOM_TEXT_LEN*4*/, 235, 308 + 20 * 4 + 72, 271}, //

    {248, 138, 462, 157},

};


static music_ui_t  music_ui_480_640 =
{
    //layer window
    {
        PIXEL_COLOR_ARGB8888,       //fmt
        { 640, 480 },               //fb size
        { 0, 0, 640, 480 },         //src rect
        { 0, 0, 640, 480 },         //scn rect
        { 0 },                      //reserved
    },

    //layer window
    {
        PIXEL_COLOR_ARGB8888,       //fmt
        { 246, 456 },               //fb size
        { 0, 0, 246, 456 },         //src rect
        { 0, 0, 246, 456 },         //scn rect
        { 0 },                      //reserved
    },
    //list window, ����ļ�
    {
        //{0, 46, 300, 424},                        //listbar area
        //{0, 0, 300, 106},                     //item rectangle
        //{0, 0, 300, 106},                     //focus icon(select border)
        //{0, 46+10, 300, 424},                     //listbar area
        {0, 46, 300, 432},                      //listbar area
        //{0, 0, 300, 106},                     //item rectangle
        //{0, 0, 300, 106},                     //focus icon(select border)
        {0, 0, 300, 54},                        //item rectangle
        {0, 0, 300, 54},                        //focus icon(select border)
        //{8,(106-42)/2,42, 42 },                       //file icon
        {8, (54 - 42) / 2, 42, 42 },                        //file icon
        //{55, 0, 300-55, 106},     //text rectangle
        {55, 0, 300 - 55, 54},      //text rectangle
    },

    //scroll bar ui
    {
        {300, 46,   10, 434},                       //scroll bar background
        {300, 46,   10, 434},                       //scroll
        6,                                      //head
        6,                                      //body
        6,                                      //tail
    },

    //head bar ui
    {
        {640 - 494, 42, 494, 88},                   //headbar background
#if BEETLES_RTC_EXIST
        {450, 210, 24, 20},                             //����ģʽ״̬ͼ��
        {450, 189, 24, 20},                             //EQģʽ״̬ͼ��
        {226, 5, 26, 16},                               //����ر�ʱ��ͼ��
        {278, 5, 26, 16},                               //����ͼ��
        {304, 3, 18, 16},                               //����text
        {326, 6, 31, 12},                               //���ͼ��
        {359, 3, 40, 16},                               //ʱ����ʾ����
#else
        {450, 210, 24, 20},                             //����ģʽ״̬ͼ��
        {450, 189, 24, 20},                             //EQģʽ״̬ͼ��
        {284, 5, 26, 16},                               //����ر�ʱ��ͼ��
        {315, 5, 26, 16},                               //����ͼ��
        {341, 4, 18, 16},                               //����text
        {364, 6, 31, 12},                               //���ͼ��
        {0, 0, 0, 16},                                  //ʱ����ʾ����
#endif
    },
    //palyer ui
    {
        {270,   40, 160,    80},                    //���ư�ť��������()��Ƶ�׹���
        {230, 120, 150, 16},                            //������ʾ�ַ�����
        {450, 210, 24, 20},                             //����ģʽ��ť
        {450, 189, 24, 20},                             //EQģʽ��ť
        {324, 68, 26, 16},                              // ����ʱ����ư�ť

#ifndef MUSIC_VIEW_CHANGE_20160811
        {713, 362, 42, 22},                         //usb disk��ť
        {713, 329, 42, 22},                         //sd card��ť
#else
        {713, 362 - 80, 42, 22},                        //usb disk��ť
        {713, 329 - 80, 42, 22},                        //sd card��ť
#endif
        {377 + 38, 164, 22, 18},                            //��һ�װ�ť
        {410 + 38, 164, 22, 18},                            //��һ�װ�ť
        {271 + 38, 164, 22, 18},                            //������ͣ��ť
        {307 + 38, 164, 22, 18},                            //���˰�ť
        {335 + 38, 164, 22, 18},                            //�����ť


        {271, 164, 22, 18},                         //����״̬ͼ��(��ͣ�򲥷�)

        {327, 105, 54 + 40, 20},                        //��ǰ����ʱ��
        {800 - 100, 105, 54 + 40, 20},                      //��ʱ����ʾ����

        //������
        {327, 106, 24, 20},                              //������head
        {327, 106, 454, 6},                              //������body
        {327, 76, 2, 9},                                //������point,��head֮����˼�5
        {327 + 454 - 24, 106, 24, 20},                           //������tail
        {327, 88, 24, 20},                              //flag

        {216,       187, 26, 16},                       //�ݳ���ͼ��
        {213 + 26,  187, 70 + 20, 16},                      //�ݳ�������
        {310 + 20,  187, 26, 16},                       //�ļ���Сͼ��
        {310 + 26 + 20, 187, 70, 16},                       //�ļ���С��ʾ

        {216,       211, 26, 16},                       //ר������ͼ��
        {213 + 26,  211, 70 + 20, 16},                      //ר������
        {310 + 20,  211, 26, 16},                       //bitrateͼ��
        {310 + 26 + 20, 211, 70, 16},                       //bitrate��С��ʾ

        //menu
        {326, 150, 93, 58},                   //menu btn
        {326, 220, 100, 30},                   //menu text
        {686, 150, 93, 58},                   // show mode btn
        {686, 220, 105, 30},                      //show mode btn text
        {326, 260, 32, 28},                   // ab btn
        {326 + 32, 260, 50, 30},                      //ab btn text

        {686, 260, 105, 30},                    //show tack text
    },

    //volume bar ui
    {
        {0, 0, 0, 0},                                   //��������������ʾ����
        {0, 0, 0, 0},                                   //���speakerͼ����ʾ��������״̬
        {0, 0, 0, 0},                                   //�м�������ʾ������
        {0, 0, 0, 0},                                   //�ұ�speakerͼ����ʾ
    },

    //spectrum
    {
        //{408,284,360,96}
        //408,284-46+35,254,98
        {400, 284 - 46 + 25, 300, 98}

    },

    //bg
    {
        {0, 0, 800, 480},
        //      {215, 186, 260, 44},
        //      {258 + 38, 162, 189, 24}
    },

    {
        {10 + 20 * 0 + 300/*+MUSIC_BOTTOM_TEXT_LEN*0*/, 238 + 40, 32, 28}, //24
        {72 + 20 * 1 + 300/*+MUSIC_BOTTOM_TEXT_LEN*1*/, 238 + 40, 32, 28}, //93
        {139 + 20 * 2 + 300/*+MUSIC_BOTTOM_TEXT_LEN*2*/, 238 + 40, 32, 28}, //172
        {206 + 20 * 3 + 300/*+MUSIC_BOTTOM_TEXT_LEN*3*/, 238 + 40, 32, 28}, //245
        {308 + 20 * 4 + 300/*+MUSIC_BOTTOM_TEXT_LEN*4*/, 238 + 40, 32, 28}, //245
        {0, 235, 480, 38},//0
    },

    //album
    {
        { 420, 145, 254, 254},
        NULL,
    },

    //icon table
    {
        //ID                                                            res handle,     res buffer
        //���������
        { ID_MUSIC_PALETTE_BMP,                     NULL,                NULL },    //
        { NULL,    NULL,                 NULL },    //
        { ID_MUSIC_FILE_UNFOCUSED_BMP,              NULL,                NULL },    //
        { ID_MUSIC_FILE_UNFOCUSED_BMP,              NULL,                NULL },    //�ļ�ͼ�꣬δ��ѡ��ʱ
        { ID_MUSIC_FILE_FOCUSED_BMP,                NULL,                NULL },    //�ļ�ͼ�꣬��ѡ��ʱ
        { ID_MUSIC_LIST_ITEM_FOCUS_BMP,             NULL,                NULL },    //����ѡ���
        { ID_MUSIC_LIST_ITEM_UNFOCUS_BMP,           NULL,                NULL },    //�ǽ���ѡ���
        { ID_MUSIC_LONG_STR_SCROLL_BMP,             NULL,                NULL },     //���ļ�������������
        //header bar
        //play mode
        { ID_MUSIC_PLAY_ALL_BMP,                    NULL,                NULL },    //  ����ȫ��,���ظ�
        { ID_MUSIC_PLAY_ONE_BMP,                    NULL,                NULL },    //����һ��
        { ID_MUSIC_PLAY_RANDOM_BMP,                 NULL,                NULL },    //�漴����
        { ID_MUSIC_REPEAT_ALL_BMP,                  NULL,                NULL },    //�ظ�����
        { ID_MUSIC_REPEAT_ONE_BMP,                  NULL,                NULL },    //�ظ�һ��

        //EQ mode
        { ID_MUSIC_EQ_NORMAL_BMP,                   NULL,                NULL },    //EQ normal
        { ID_MUSIC_EQ_DBB_BMP,                      NULL,                NULL },    //EQ classic
        { ID_MUSIC_EQ_POP_BMP,                      NULL,                NULL },    //  EQ POP
        { ID_MUSIC_EQ_ROCK_BMP,                     NULL,                NULL },    //  EQ Rock
        { ID_MUSIC_EQ_CLASSIC_BMP,                  NULL,                NULL },    //EQ classic
        { ID_MUSIC_EQ_JAZZ_BMP,                     NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_VOCAL_BMP,                    NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_DANCE_BMP,                    NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_SOFT_BMP,                     NULL,                NULL },    //  EQ JAZZ

        //BL Time
        { ID_MUSIC_BL_ON_BMP,                       NULL,                NULL },    //  Backlight never off
        { ID_MUSIC_BL_10S_BMP,                      NULL,                NULL },    //last 10 second
        { ID_MUSIC_BL_30S_BMP,                      NULL,                NULL },    //
        { ID_MUSIC_BL_60S_BMP,                      NULL,                NULL },    //
        { ID_MUSIC_BL_90S_BMP,                      NULL,                NULL },    //

        //volume
        { ID_MUSIC_VOLUME_STATUS_BMP,               NULL,                NULL },    //

        //bettery
        { ID_MUSIC_BETTERY0_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY1_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY2_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY3_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY4_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY5_BMP,                    NULL,                NULL },    //

        //������
        //play mode button
        { ID_MUSIC_PLAY_ALL_BMP,                    NULL,                NULL },        //����ȫ��, unfocused
        { ID_MUSIC_PLAY_ALL_BTN_FOCUSED_BMP,        NULL,                NULL },        //����ȫ��, focused
        { ID_MUSIC_PLAY_ALL_BTN_PRESSED_BMP,        NULL,                NULL },        //����ȫ��, pressed

        { ID_MUSIC_PLAY_ONE_BMP,                    NULL,                NULL },        //����һ��, unfocused
        { ID_MUSIC_PLAY_ONE_BTN_FOCUSED_BMP,        NULL,                NULL },        //����һ��, focused
        { ID_MUSIC_PLAY_ONE_BTN_PRESSED_BMP,        NULL,                NULL },        //����һ��, pressed

        { ID_MUSIC_PLAY_RANDOM_BMP,                 NULL,                NULL },        //�������, unfocused
        { ID_MUSIC_PLAY_RANDOM_BTN_FOCUSED_BMP,     NULL,                NULL },        //�������, focused
        { ID_MUSIC_PLAY_RANDOM_BTN_PRESSED_BMP,     NULL,                NULL },        //�������, pressed

        { ID_MUSIC_REPEAT_ALL_BMP,                  NULL,                NULL },        //�ظ�����ȫ��, unfocused
        { ID_MUSIC_REPEAT_ALL_BTN_FOCUSED_BMP,      NULL,                NULL },        //�ظ�����ȫ��, focused
        { ID_MUSIC_REPEAT_ALL_BTN_PRESSED_BMP,      NULL,                NULL },        //�ظ�����ȫ��, pressed

        { ID_MUSIC_REPEAT_ONE_BMP,                  NULL,                NULL },        //�ظ�����һ��, unfocused
        { ID_MUSIC_REPEAT_ONE_BTN_FOCUSED_BMP,      NULL,                NULL },        //�ظ�����һ��, focused
        { ID_MUSIC_REPEAT_ONE_BTN_PRESSED_BMP,      NULL,                NULL },        //�ظ�����һ��, pressed

        //EQ mode button
        { ID_MUSIC_EQ_MODE_BTN_UNFOCUSED_BMP,       NULL,                NULL },        //EQ mode button, unfocused
        { ID_MUSIC_EQ_MODE_BTN_FOCUSED_BMP,         NULL,                NULL },        //focused
        { ID_MUSIC_EQ_MODE_BTN_PRESSED_BMP,         NULL,                NULL },        //pressed

        //BL time button
        { ID_MUSIC_BL_TIME_BTN_UNFOCUSED_BMP,       NULL,                NULL },        //EQ mode button, unfocused
        { ID_MUSIC_BL_TIME_BTN_FOCUSED_BMP,         NULL,                NULL },        //focused
        { ID_MUSIC_BL_TIME_BTN_PRESSED_BMP,         NULL,                NULL },        //pressed

        { ID_MUSIC_USB_UNFOCUSED_BMP,               NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_USB_UNFOCUSED_BMP,                   NULL,                NULL },    //focused
        { ID_MUSIC_USB_FOCUSED_BMP,                 NULL,                NULL },        //pressed
        //play next button
        { ID_MUSIC_SD_UNFOCUSED_BMP,                NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_SD_UNFOCUSED_BMP,                    NULL,                NULL },    //focused
        { ID_MUSIC_SD_FOCUSED_BMP,                  NULL,                NULL },    //pressed

        //play previous button
        { ID_MUSIC_PLAY_PRE_BTN_UNFOCUSED_BMP,      NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_PLAY_PRE_BTN_FOCUSED_BMP,        NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_PRE_BTN_FOCUSED_BMP,        NULL,                NULL },        //pressed
        //play next button
        { ID_MUSIC_PLAY_NEXT_BTN_UNFOCUSED_BMP,     NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_PLAY_NEXT_BTN_FOCUSED_BMP,       NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_NEXT_BTN_FOCUSED_BMP,       NULL,                NULL },    //pressed
        //play pause button
        { ID_MUSIC_PAUSE0_BMP,  NULL,                NULL },    //������ͣ, unfocused
        { ID_MUSIC_PAUSE0_BMP,      NULL,                NULL },                    //focused
        { ID_MUSIC_PAUSE1_BMP,      NULL,                NULL },                    //pressed

        //play backward button
        { ID_MUSIC_PLAY_BACKWARD_BTN_UNFOCUSED_BMP, NULL,                NULL },    //����, unfocused
        { ID_MUSIC_PLAY_BACKWARD_BTN_FOCUSED_BMP,   NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_BACKWARD_BTN_FOCUSED_BMP,   NULL,                NULL },    //pressed

        //play forward button
        { ID_MUSIC_PLAY_FORWARD_BTN_UNFOCUSED_BMP,  NULL,                NULL },    //���, unfocused
        { ID_MUSIC_PLAY_FORWARD_BTN_FOCUSED_BMP,    NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_FORWARD_BTN_FOCUSED_BMP,    NULL,                NULL },    //pressed

        //play status button,
        { ID_MUSIC_PAUSE0_BMP,              NULL,               NULL },                                 //���ڲ���״̬
        { ID_MUSIC_PLAY0_BMP,                   NULL,               NULL },                                 //��ͣ״̬
        { ID_MUSIC_PREVIOUS_STATUS_BMP,             NULL,               NULL },                                 //��һ��״̬
        { ID_MUSIC_NEXT_STATUS_BMP,                 NULL,               NULL },                                 //��һ��״̬

        //process bar
        { ID_MUSIC_PROCESS_BAR_HEAD_BMP,                NULL,               NULL },                                 //process bar ͷ��
        { ID_MUSIC_PROCESS_BAR_BODY_BMP,                NULL,               NULL },                                 //body
        { ID_MUSIC_PROCESS_BAR_TAIL_BMP,                NULL,               NULL },                                 //β��
        { ID_MUSIC_PROCESS_BAR_POINT_BMP,           NULL,               NULL },                                 //���ȵ�

        //������Ϣͼ��
        { ID_MUSIC_PERFORMER_NAME_BMP,              NULL,               NULL },                                 //����ͼ��
        { ID_MUSIC_FILE_SIZE_BMP,                   NULL,               NULL },                                 //
        { ID_MUSIC_ALBUM_NAME_BMP,                  NULL,               NULL },                                 //
        { ID_MUSIC_BIT_RATE_BMP,                        NULL,               NULL },                                 //

        //Ƶ��ͼͼ��
        { ID_MUSIC_PARTICLE_BMP,                        NULL,               NULL },
        { ID_MUSIC_BOUY_BMP,                            NULL,               NULL },

        //�������������
        { ID_MUSIC_LIST_SCROLL_BG_BMP,              NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_HEAD_BMP,                NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_BODY_BMP,                NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_TAIL_BMP,                NULL,               NULL },

        //{ID_MUSIC_MENU}
        //bg
        { ID_MUSIC_BG_BMP,                  NULL,               NULL },
        { ID_MUSIC_INFO_BAR_BMP,                NULL,               NULL },
        { ID_MUSIC_PLAYBAR_BMP,             NULL,               NULL },
        //flag
        {ID_MUSIC_PROCESS_BAR_FLAG_BMP,         NULL,               NULL},

        //bottom
        {ID_MUSIC_BACK0_BMP,            NULL,               NULL},
        {ID_MUSIC_EQ_CHANGE0_BMP,       NULL,               NULL},
        {ID_MUSIC_PLAY_MODE0_BMP,       NULL,               NULL},
        {ID_MUSIC_EQ_LYRICS0_BMP,       NULL,               NULL},
        {ID_MUSIC_AB0_BMP,              NULL,               NULL},//112350 ID_MUSIC_AB0_BMP
        {ID_MUSIC_BACK1_BMP,            NULL,               NULL},
        {ID_MUSIC_EQ_CHANGE1_BMP,       NULL,               NULL},
        {ID_MUSIC_PLAY_MODE1_BMP,       NULL,               NULL},
        {ID_MUSIC_EQ_LYRICS1_BMP,       NULL,               NULL},
        {ID_MUSIC_AB1_BMP,              NULL,               NULL},//112350 ID_MUSIC_AB1_BMP
        {ID_MUSIC_BOTTOM_BMP,           NULL,               NULL},
        //play pause
        {ID_MUSIC_PLAY0_BMP,            NULL,               NULL},
        {ID_MUSIC_PLAY1_BMP,            NULL,               NULL},

        {ID_MUSIC_MENU_UNFOCUSED_BMP,    NULL,               NULL},
        {ID_MUSIC_MENU_FOCUSED_BMP,      NULL,               NULL},

        {ID_MUSIC_MODE_UNFCS_BMP,       NULL,               NULL},
        {ID_MUSIC_MODE_FCS_BMP,          NULL,               NULL},
        {ID_MUSIC_ALBUM_BMP,             NULL,               NULL},
        {ID_MUSIC_HEADBAR_BG_BMP,             NULL,               NULL},
        { 0,                             NULL,                  NULL }          //browser_icon_max
    },
};


static music_ui_t  music_ui_800_480 =
{
    //layer window
    {
        PIXEL_COLOR_ARGB8888,       //fmt
        { 800, 480 },               //fb size
        { 0, 0, 800, 480 },         //src rect
        { 0, 0, 800, 480 },         //scn rect
        { 0 },                      //reserved
    },

    //layer window
    {
        PIXEL_COLOR_ARGB8888,       //fmt
        { 246, 456 },               //fb size
        { 0, 0, 246, 456 },         //src rect
        { 0, 0, 246, 456 },         //scn rect
        { 0 },                      //reserved
    },
    //list window, ����ļ�
    {
        //{0, 46, 300, 424},                        //listbar area
        //{0, 0, 300, 106},                     //item rectangle
        //{0, 0, 300, 106},                     //focus icon(select border)
        //{0, 46+10, 300, 424},                     //listbar area
        {0, 46, 300, 432},                      //listbar area
        //{0, 0, 300, 106},                     //item rectangle
        //{0, 0, 300, 106},                     //focus icon(select border)
        {0, 0, 300, 54},                        //item rectangle
        {0, 0, 300, 54},                        //focus icon(select border)
        //{8,(106-42)/2,42, 42 },                       //file icon
        {8, (54 - 42) / 2, 42, 42 },                        //file icon
        //{55, 0, 300-55, 106},     //text rectangle
        {55, 0, 300 - 55, 54},      //text rectangle
    },

    //scroll bar ui
    {
        {300, 46,   10, 434},                       //scroll bar background
        {300, 46,   10, 434},                       //scroll
        6,                                      //head
        6,                                      //body
        6,                                      //tail
    },

    //head bar ui
    {
        {800 - 494, 42, 494, 88},                   //headbar background
#if BEETLES_RTC_EXIST
        {450, 210, 24, 20},                             //����ģʽ״̬ͼ��
        {450, 189, 24, 20},                             //EQģʽ״̬ͼ��
        {226, 5, 26, 16},                               //����ر�ʱ��ͼ��
        {278, 5, 26, 16},                               //����ͼ��
        {304, 3, 18, 16},                               //����text
        {326, 6, 31, 12},                               //���ͼ��
        {359, 3, 40, 16},                               //ʱ����ʾ����
#else
        {450, 210, 24, 20},                             //����ģʽ״̬ͼ��
        {450, 189, 24, 20},                             //EQģʽ״̬ͼ��
        {284, 5, 26, 16},                               //����ر�ʱ��ͼ��
        {315, 5, 26, 16},                               //����ͼ��
        {341, 4, 18, 16},                               //����text
        {364, 6, 31, 12},                               //���ͼ��
        {0, 0, 0, 16},                                  //ʱ����ʾ����
#endif
    },
    //palyer ui
    {
        {270,   40, 160,    80},                    //���ư�ť��������()��Ƶ�׹���
        {230, 120, 150, 16},                            //������ʾ�ַ�����
        {450, 210, 24, 20},                             //����ģʽ��ť
        {450, 189, 24, 20},                             //EQģʽ��ť
        {324, 68, 26, 16},                              // ����ʱ����ư�ť

#ifndef MUSIC_VIEW_CHANGE_20160811
        {713, 362, 42, 22},                         //usb disk��ť
        {713, 329, 42, 22},                         //sd card��ť
#else
        {713, 362 - 80, 42, 22},                        //usb disk��ť
        {713, 329 - 80, 42, 22},                        //sd card��ť
#endif
        {377 + 38, 164, 22, 18},                            //��һ�װ�ť
        {410 + 38, 164, 22, 18},                            //��һ�װ�ť
        {271 + 38, 164, 22, 18},                            //������ͣ��ť
        {307 + 38, 164, 22, 18},                            //���˰�ť
        {335 + 38, 164, 22, 18},                            //�����ť


        {271, 164, 22, 18},                         //����״̬ͼ��(��ͣ�򲥷�)

        {327, 105, 54 + 40, 20},                        //��ǰ����ʱ��
        {800 - 100, 105, 54 + 40, 20},                      //��ʱ����ʾ����

        //������
        {327, 76, 24, 20},                              //������head
        {327, 88, 454, 6},                              //������body
        {327, 76, 2, 9},                                //������point,��head֮����˼�5
        {327 + 454 - 24, 76, 24, 20},                           //������tail
        {327, 88, 24, 20},                              //flag

        {216,       187, 26, 16},                       //�ݳ���ͼ��
        {213 + 26,  187, 70 + 20, 16},                      //�ݳ�������
        {310 + 20,  187, 26, 16},                       //�ļ���Сͼ��
        {310 + 26 + 20, 187, 70, 16},                       //�ļ���С��ʾ

        {216,       211, 26, 16},                       //ר������ͼ��
        {213 + 26,  211, 70 + 20, 16},                      //ר������
        {310 + 20,  211, 26, 16},                       //bitrateͼ��
        {310 + 26 + 20, 211, 70, 16},                       //bitrate��С��ʾ

        //menu
        {326, 150, 93, 58},                   //menu btn
        {326, 220, 100, 30},                   //menu text
        {686, 150, 93, 58},                   // show mode btn
        {686, 220, 105, 30},                      //show mode btn text
        {326, 260, 32, 28},                   // ab btn
        {326 + 32, 260, 50, 30},                      //ab btn text

        {686, 260, 105, 30},                    //show tack text
    },

    //volume bar ui
    {
        {0, 0, 0, 0},                                   //��������������ʾ����
        {0, 0, 0, 0},                                   //���speakerͼ����ʾ��������״̬
        {0, 0, 0, 0},                                   //�м�������ʾ������
        {0, 0, 0, 0},                                   //�ұ�speakerͼ����ʾ
    },

    //spectrum
    {
        //{408,284,360,96}
        //408,284-46+35,254,98
        {400, 284 - 46 + 25, 300, 98}

    },

    //bg
    {
        {0, 0, 800, 480},
        //      {215, 186, 260, 44},
        //      {258 + 38, 162, 189, 24}
    },

    {
        {10 + 20 * 0 + 300/*+MUSIC_BOTTOM_TEXT_LEN*0*/, 238 + 40, 32, 28}, //24
        {72 + 20 * 1 + 300/*+MUSIC_BOTTOM_TEXT_LEN*1*/, 238 + 40, 32, 28}, //93
        {139 + 20 * 2 + 300/*+MUSIC_BOTTOM_TEXT_LEN*2*/, 238 + 40, 32, 28}, //172
        {206 + 20 * 3 + 300/*+MUSIC_BOTTOM_TEXT_LEN*3*/, 238 + 40, 32, 28}, //245
        {308 + 20 * 4 + 300/*+MUSIC_BOTTOM_TEXT_LEN*4*/, 238 + 40, 32, 28}, //245
        {0, 235, 480, 38},//0
    },

    //album
    {
        { 420, 145, 254, 254},
        NULL,
    },

    //icon table
    {
        //ID                                                            res handle,     res buffer
        //���������
        { ID_MUSIC_PALETTE_BMP,                     NULL,                NULL },    //
        { NULL,    NULL,                 NULL },    //
        { ID_MUSIC_FILE_UNFOCUSED_BMP,              NULL,                NULL },    //
        { ID_MUSIC_FILE_UNFOCUSED_BMP,              NULL,                NULL },    //�ļ�ͼ�꣬δ��ѡ��ʱ
        { ID_MUSIC_FILE_FOCUSED_BMP,                NULL,                NULL },    //�ļ�ͼ�꣬��ѡ��ʱ
        { ID_MUSIC_LIST_ITEM_FOCUS_BMP,             NULL,                NULL },    //����ѡ���
        { ID_MUSIC_LIST_ITEM_UNFOCUS_BMP,           NULL,                NULL },    //�ǽ���ѡ���
        { ID_MUSIC_LONG_STR_SCROLL_BMP,             NULL,                NULL },     //���ļ�������������
        //header bar
        //play mode
        { ID_MUSIC_PLAY_ALL_BMP,                    NULL,                NULL },    //  ����ȫ��,���ظ�
        { ID_MUSIC_PLAY_ONE_BMP,                    NULL,                NULL },    //����һ��
        { ID_MUSIC_PLAY_RANDOM_BMP,                 NULL,                NULL },    //�漴����
        { ID_MUSIC_REPEAT_ALL_BMP,                  NULL,                NULL },    //�ظ�����
        { ID_MUSIC_REPEAT_ONE_BMP,                  NULL,                NULL },    //�ظ�һ��

        //EQ mode
        { ID_MUSIC_EQ_NORMAL_BMP,                   NULL,                NULL },    //EQ normal
        { ID_MUSIC_EQ_DBB_BMP,                      NULL,                NULL },    //EQ classic
        { ID_MUSIC_EQ_POP_BMP,                      NULL,                NULL },    //  EQ POP
        { ID_MUSIC_EQ_ROCK_BMP,                     NULL,                NULL },    //  EQ Rock
        { ID_MUSIC_EQ_CLASSIC_BMP,                  NULL,                NULL },    //EQ classic
        { ID_MUSIC_EQ_JAZZ_BMP,                     NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_VOCAL_BMP,                    NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_DANCE_BMP,                    NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_SOFT_BMP,                     NULL,                NULL },    //  EQ JAZZ

        //BL Time
        { ID_MUSIC_BL_ON_BMP,                       NULL,                NULL },    //  Backlight never off
        { ID_MUSIC_BL_10S_BMP,                      NULL,                NULL },    //last 10 second
        { ID_MUSIC_BL_30S_BMP,                      NULL,                NULL },    //
        { ID_MUSIC_BL_60S_BMP,                      NULL,                NULL },    //
        { ID_MUSIC_BL_90S_BMP,                      NULL,                NULL },    //

        //volume
        { ID_MUSIC_VOLUME_STATUS_BMP,               NULL,                NULL },    //

        //bettery
        { ID_MUSIC_BETTERY0_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY1_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY2_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY3_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY4_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY5_BMP,                    NULL,                NULL },    //

        //������
        //play mode button
        { ID_MUSIC_PLAY_ALL_BMP,                    NULL,                NULL },        //����ȫ��, unfocused
        { ID_MUSIC_PLAY_ALL_BTN_FOCUSED_BMP,        NULL,                NULL },        //����ȫ��, focused
        { ID_MUSIC_PLAY_ALL_BTN_PRESSED_BMP,        NULL,                NULL },        //����ȫ��, pressed

        { ID_MUSIC_PLAY_ONE_BMP,                    NULL,                NULL },        //����һ��, unfocused
        { ID_MUSIC_PLAY_ONE_BTN_FOCUSED_BMP,        NULL,                NULL },        //����һ��, focused
        { ID_MUSIC_PLAY_ONE_BTN_PRESSED_BMP,        NULL,                NULL },        //����һ��, pressed

        { ID_MUSIC_PLAY_RANDOM_BMP,                 NULL,                NULL },        //�������, unfocused
        { ID_MUSIC_PLAY_RANDOM_BTN_FOCUSED_BMP,     NULL,                NULL },        //�������, focused
        { ID_MUSIC_PLAY_RANDOM_BTN_PRESSED_BMP,     NULL,                NULL },        //�������, pressed

        { ID_MUSIC_REPEAT_ALL_BMP,                  NULL,                NULL },        //�ظ�����ȫ��, unfocused
        { ID_MUSIC_REPEAT_ALL_BTN_FOCUSED_BMP,      NULL,                NULL },        //�ظ�����ȫ��, focused
        { ID_MUSIC_REPEAT_ALL_BTN_PRESSED_BMP,      NULL,                NULL },        //�ظ�����ȫ��, pressed

        { ID_MUSIC_REPEAT_ONE_BMP,                  NULL,                NULL },        //�ظ�����һ��, unfocused
        { ID_MUSIC_REPEAT_ONE_BTN_FOCUSED_BMP,      NULL,                NULL },        //�ظ�����һ��, focused
        { ID_MUSIC_REPEAT_ONE_BTN_PRESSED_BMP,      NULL,                NULL },        //�ظ�����һ��, pressed

        //EQ mode button
        { ID_MUSIC_EQ_MODE_BTN_UNFOCUSED_BMP,       NULL,                NULL },        //EQ mode button, unfocused
        { ID_MUSIC_EQ_MODE_BTN_FOCUSED_BMP,         NULL,                NULL },        //focused
        { ID_MUSIC_EQ_MODE_BTN_PRESSED_BMP,         NULL,                NULL },        //pressed

        //BL time button
        { ID_MUSIC_BL_TIME_BTN_UNFOCUSED_BMP,       NULL,                NULL },        //EQ mode button, unfocused
        { ID_MUSIC_BL_TIME_BTN_FOCUSED_BMP,         NULL,                NULL },        //focused
        { ID_MUSIC_BL_TIME_BTN_PRESSED_BMP,         NULL,                NULL },        //pressed

        { ID_MUSIC_USB_UNFOCUSED_BMP,               NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_USB_UNFOCUSED_BMP,                   NULL,                NULL },    //focused
        { ID_MUSIC_USB_FOCUSED_BMP,                 NULL,                NULL },        //pressed
        //play next button
        { ID_MUSIC_SD_UNFOCUSED_BMP,                NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_SD_UNFOCUSED_BMP,                    NULL,                NULL },    //focused
        { ID_MUSIC_SD_FOCUSED_BMP,                  NULL,                NULL },    //pressed

        //play previous button
        { ID_MUSIC_PLAY_PRE_BTN_UNFOCUSED_BMP,      NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_PLAY_PRE_BTN_FOCUSED_BMP,        NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_PRE_BTN_FOCUSED_BMP,        NULL,                NULL },        //pressed
        //play next button
        { ID_MUSIC_PLAY_NEXT_BTN_UNFOCUSED_BMP,     NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_PLAY_NEXT_BTN_FOCUSED_BMP,       NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_NEXT_BTN_FOCUSED_BMP,       NULL,                NULL },    //pressed
        //play pause button
        { ID_MUSIC_PAUSE0_BMP,  NULL,                NULL },    //������ͣ, unfocused
        { ID_MUSIC_PAUSE0_BMP,      NULL,                NULL },                    //focused
        { ID_MUSIC_PAUSE1_BMP,      NULL,                NULL },                    //pressed

        //play backward button
        { ID_MUSIC_PLAY_BACKWARD_BTN_UNFOCUSED_BMP, NULL,                NULL },    //����, unfocused
        { ID_MUSIC_PLAY_BACKWARD_BTN_FOCUSED_BMP,   NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_BACKWARD_BTN_FOCUSED_BMP,   NULL,                NULL },    //pressed

        //play forward button
        { ID_MUSIC_PLAY_FORWARD_BTN_UNFOCUSED_BMP,  NULL,                NULL },    //���, unfocused
        { ID_MUSIC_PLAY_FORWARD_BTN_FOCUSED_BMP,    NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_FORWARD_BTN_FOCUSED_BMP,    NULL,                NULL },    //pressed

        //play status button,
        { ID_MUSIC_PAUSE0_BMP,              NULL,               NULL },                                 //���ڲ���״̬
        { ID_MUSIC_PLAY0_BMP,                   NULL,               NULL },                                 //��ͣ״̬
        { ID_MUSIC_PREVIOUS_STATUS_BMP,             NULL,               NULL },                                 //��һ��״̬
        { ID_MUSIC_NEXT_STATUS_BMP,                 NULL,               NULL },                                 //��һ��״̬

        //process bar
        { ID_MUSIC_PROCESS_BAR_HEAD_BMP,                NULL,               NULL },                                 //process bar ͷ��
        { ID_MUSIC_PROCESS_BAR_BODY_BMP,                NULL,               NULL },                                 //body
        { ID_MUSIC_PROCESS_BAR_TAIL_BMP,                NULL,               NULL },                                 //β��
        { ID_MUSIC_PROCESS_BAR_POINT_BMP,           NULL,               NULL },                                 //���ȵ�

        //������Ϣͼ��
        { ID_MUSIC_PERFORMER_NAME_BMP,              NULL,               NULL },                                 //����ͼ��
        { ID_MUSIC_FILE_SIZE_BMP,                   NULL,               NULL },                                 //
        { ID_MUSIC_ALBUM_NAME_BMP,                  NULL,               NULL },                                 //
        { ID_MUSIC_BIT_RATE_BMP,                        NULL,               NULL },                                 //

        //Ƶ��ͼͼ��
        { ID_MUSIC_PARTICLE_BMP,                        NULL,               NULL },
        { ID_MUSIC_BOUY_BMP,                            NULL,               NULL },

        //�������������
        { ID_MUSIC_LIST_SCROLL_BG_BMP,              NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_HEAD_BMP,                NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_BODY_BMP,                NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_TAIL_BMP,                NULL,               NULL },

        //{ID_MUSIC_MENU}
        //bg
        { ID_MUSIC_BG_BMP,                  NULL,               NULL },
        { ID_MUSIC_INFO_BAR_BMP,                NULL,               NULL },
        { ID_MUSIC_PLAYBAR_BMP,             NULL,               NULL },
        //flag
        {ID_MUSIC_PROCESS_BAR_FLAG_BMP,         NULL,               NULL},

        //bottom
        {ID_MUSIC_BACK0_BMP,            NULL,               NULL},
        {ID_MUSIC_EQ_CHANGE0_BMP,       NULL,               NULL},
        {ID_MUSIC_PLAY_MODE0_BMP,       NULL,               NULL},
        {ID_MUSIC_EQ_LYRICS0_BMP,       NULL,               NULL},
        {ID_MUSIC_AB0_BMP,              NULL,               NULL},//112350 ID_MUSIC_AB0_BMP
        {ID_MUSIC_BACK1_BMP,            NULL,               NULL},
        {ID_MUSIC_EQ_CHANGE1_BMP,       NULL,               NULL},
        {ID_MUSIC_PLAY_MODE1_BMP,       NULL,               NULL},
        {ID_MUSIC_EQ_LYRICS1_BMP,       NULL,               NULL},
        {ID_MUSIC_AB1_BMP,              NULL,               NULL},//112350 ID_MUSIC_AB1_BMP
        {ID_MUSIC_BOTTOM_BMP,           NULL,               NULL},
        //play pause
        {ID_MUSIC_PLAY0_BMP,            NULL,               NULL},
        {ID_MUSIC_PLAY1_BMP,            NULL,               NULL},

        {ID_MUSIC_MENU_UNFOCUSED_BMP,    NULL,               NULL},
        {ID_MUSIC_MENU_FOCUSED_BMP,      NULL,               NULL},

        {ID_MUSIC_MODE_UNFCS_BMP,       NULL,               NULL},
        {ID_MUSIC_MODE_FCS_BMP,          NULL,               NULL},
        {ID_MUSIC_ALBUM_BMP,             NULL,               NULL},
        {ID_MUSIC_HEADBAR_BG_BMP,             NULL,               NULL},
        { 0,                             NULL,                  NULL }          //browser_icon_max
    },
};


//720*480

static music_ui_t  music_ui_720_480 =
{
    //layer window
    {
        PIXEL_COLOR_ARGB8888,       //fmt
        { 720, 480 },               //fb size
        { 0, 0, 720, 480 },         //src rect
        { 0, 0, 720, 480 },         //scn rect
        { 0 },                      //reserved
    },

    //layer window
    {
        PIXEL_COLOR_ARGB8888,       //fmt
        { 220 - 54, 456 },             //fb size
        { 0, 0, 220 - 54, 456 },       //src rect
        { 0, 0, 220 - 54, 456 },       //scn rect
        { 0 },                      //reserved
    },
    //list window, ����ļ�
    {
        //{0, 46, 300, 424},                        //listbar area
        //{0, 0, 300, 106},                     //item rectangle
        //{0, 0, 300, 106},                     //focus icon(select border)
        {10, 46 + 10, 300 - 80 - 10, 424},                      //listbar area
        {10, 0, 300 - 80 - 10, 106},                        //item rectangle
        {10, 0, 300 - 80 - 10, 106},                        //focus icon(select border)
        {18, 12, 42, 42 },                      //file icon
        {55 + 10, 0 + 20, 200 - 80, 36},        //text rectangle
    },

    //scroll bar ui
    {
        {300 - 80, 46,  10, 434},                       //scroll bar background
        {300 - 80, 46,  10, 434},                       //scroll
        6,                                      //head
        6,                                      //body
        6,                                      //tail
    },

    //head bar ui
    {
        {720 - 494, 40 + 20, 494 - 10, 88},                 //headbar background
#if BEETLES_RTC_EXIST
        {450 - 80, 210, 24, 20},                                //����ģʽ״̬ͼ��
        {450 - 80, 189, 24, 20},                                //EQģʽ״̬ͼ��
        {226 - 80, 5, 26, 16},                              //����ر�ʱ��ͼ��
        {278 - 80, 5, 26, 16},                              //����ͼ��
        {304 - 80, 3, 18, 16},                              //����text
        {326 - 80, 6, 31, 12},                              //���ͼ��
        {359 - 80, 3, 40, 16},                              //ʱ����ʾ����
#else
        {450 - 80, 210, 24, 20},                                //����ģʽ״̬ͼ��
        {450 - 80, 189, 24, 20},                                //EQģʽ״̬ͼ��
        {284 - 80, 5, 26, 16},                              //����ر�ʱ��ͼ��
        {315 - 80, 5, 26, 16},                              //����ͼ��
        {341 - 80, 4, 18, 16},                              //����text
        {364 - 80, 6, 31, 12},                              //���ͼ��
        {0, 0, 0, 16},                                  //ʱ����ʾ����
#endif
    },
    //palyer ui
    {
        {270 - 80,  40, 160 - 10,   80},                    //���ư�ť��������()��Ƶ�׹���
        {230 - 80, 120, 150 - 10, 16},                          //������ʾ�ַ�����
        {450 - 80, 210, 24, 20},                                //����ģʽ��ť
        {450 - 80, 189, 24, 20},                                //EQģʽ��ť
        {324 - 80, 68, 26, 16},                             // ����ʱ����ư�ť


        {713 - 80, 362, 42, 22},                            //usb disk��ť
        {713 - 80, 329, 42, 22},                            //sd card��ť
        {377 + 38 - 80, 164, 22, 18},                           //��һ�װ�ť
        {410 + 38 - 80, 164, 22, 18},                           //��һ�װ�ť
        {271 + 38 - 80, 164, 22, 18},                           //������ͣ��ť
        {307 + 38 - 80, 164, 22, 18},                           //���˰�ť
        {335 + 38 - 80, 164, 22, 18},                           //�����ť


        {271 - 80, 164, 22, 18},                            //����״̬ͼ��(��ͣ�򲥷�)

        {327 - 80, 120, 54 + 40, 20},                       //��ǰ����ʱ��
        {720 - 100, 120, 54 + 40, 20},                      //��ʱ����ʾ����

        //������  //chengf  80
        {327 - 90/*80*/, 76, 24, 20},                       //������head
        {327 - 90/*80*/, 88, 454 - 10 - 10 - 10, 6},                //������body
        {327 - 90/*80*/, 76, 2, 9},                         //������point,��head֮����˼�5
        {327 + 454 - 24 - 120/*90*//*80*/ - 10 - 10 - 10, 76, 24, 20},      //������tail  //chengf
        {327 - 90/*80*/ - 10, 76, 24, 20},                  //flag

        {216 - 80,      187, 26, 16},                       //�ݳ���ͼ��
        {213 + 26 - 80, 187, 70 + 20, 16},                      //�ݳ�������
        {310 + 20 - 80, 187, 26, 16},                       //�ļ���Сͼ��
        {310 + 26 + 20 - 80,    187, 70, 16},                       //�ļ���С��ʾ

        {216 - 80,      211, 26, 16},                       //ר������ͼ��
        {213 + 26 - 80, 211, 70 + 20, 16},                      //ר������
        {310 + 20 - 80, 211, 26, 16},                       //bitrateͼ��
        {310 + 26 + 20 - 80,    211, 70, 16},                       //bitrate��С��ʾ

        //menu
        {326 - 80 - 18, 150, 93, 58},               //menu btn              //chengf
        {326 - 80 - 18, 220, 100, 30},              //menu text             //chengf
        {686 - 80 - 20, 150, 93, 58},                   // show mode btn        //chengf
        {686 - 80 - 25, 220, 100, 30},                  //show mode btn text    //chengf
        {686 + 26 - 80 - 25, 260, 105, 30},            //show tack text        //zyq
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },

    //volume bar ui
    {
        {0, 0, 0, 0},                                   //��������������ʾ����
        {0, 0, 0, 0},                                   //���speakerͼ����ʾ��������״̬
        {0, 0, 0, 0},                                   //�м�������ʾ������
        {0, 0, 0, 0},                                   //�ұ�speakerͼ����ʾ
    },

    //spectrum
    {
        //{408,284,360,96}
        //400-80-50,284-46+35,300-10,98
        {400 - 80, 284 - 46 + 25, 300, 98}

    },

    //bg
    {
        {0, 24, 720, 208},
        //  {215 - 80, 186, 260 - 10, 44},
        //   {258 + 38 - 80 - 10, 162, 189 - 10, 24}
    },

    {
        {10 + 20 * 0 + 300 - 80 - 10/*+MUSIC_BOTTOM_TEXT_LEN*0*/, 238 + 40, 32, 28}, //24
        {72 + 20 * 1 + 300 - 80 - 10/*+MUSIC_BOTTOM_TEXT_LEN*1*/, 238 + 40, 32, 28}, //93
        {139 + 20 * 2 + 300 - 80 - 10/*+MUSIC_BOTTOM_TEXT_LEN*2*/, 238 + 40, 32, 28}, //172
        {206 + 20 * 3 + 300 - 80 - 10/*+MUSIC_BOTTOM_TEXT_LEN*3*/, 238 + 40, 32, 28}, //245
        {308 + 20 * 4 + 300 - 80 - 10/*+MUSIC_BOTTOM_TEXT_LEN*4*/, 238 + 40, 32, 28}, //245
        {0, 235, 480, 38},//0
    },

    //album
    {
        { 420 - 80 - 20, 145, 254, 254},
        NULL,
    },

    //icon table
    {
        //ID                                                            res handle,     res buffer
        //���������
        { ID_MUSIC_PALETTE_BMP,                     NULL,                NULL },    //
        { NULL,             NULL,                NULL },    //
        { ID_MUSIC_FILE_UNFOCUSED_BMP,              NULL,                NULL },    //
        { ID_MUSIC_FILE_UNFOCUSED_BMP,              NULL,                NULL },    //�ļ�ͼ�꣬δ��ѡ��ʱ
        { ID_MUSIC_FILE_FOCUSED_BMP,                NULL,                NULL },    //�ļ�ͼ�꣬��ѡ��ʱ
        { ID_MUSIC_LIST_ITEM_FOCUS0_BMP,                NULL,                NULL },    //����ѡ���
        { ID_MUSIC_LIST_ITEM_UNFOCUS0_BMP,          NULL,                NULL }, //{ ID_MUSIC_LIST_ITEM_UNFOCUS_BMP,            NULL,                NULL },    //�ǽ���ѡ���
        { ID_MUSIC_LONG_STR_SCROLL_BMP,             NULL,                NULL },     //���ļ�������������
        //header bar
        //play mode
        { ID_MUSIC_PLAY_ALL_BMP,                    NULL,                NULL },    //  ����ȫ��,���ظ�
        { ID_MUSIC_PLAY_ONE_BMP,                    NULL,                NULL },    //����һ��
        { ID_MUSIC_PLAY_RANDOM_BMP,                 NULL,                NULL },    //�漴����
        { ID_MUSIC_REPEAT_ALL_BMP,                  NULL,                NULL },    //�ظ�����
        { ID_MUSIC_REPEAT_ONE_BMP,                  NULL,                NULL },    //�ظ�һ��

        //EQ mode
        { ID_MUSIC_EQ_NORMAL_BMP,                   NULL,                NULL },    //EQ normal
        { ID_MUSIC_EQ_DBB_BMP,                      NULL,                NULL },    //EQ classic
        { ID_MUSIC_EQ_POP_BMP,                      NULL,                NULL },    //  EQ POP
        { ID_MUSIC_EQ_ROCK_BMP,                     NULL,                NULL },    //  EQ Rock
        { ID_MUSIC_EQ_CLASSIC_BMP,                  NULL,                NULL },    //EQ classic
        { ID_MUSIC_EQ_JAZZ_BMP,                     NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_VOCAL_BMP,                    NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_DANCE_BMP,                    NULL,                NULL },    //  EQ JAZZ
        { ID_MUSIC_EQ_SOFT_BMP,                     NULL,                NULL },    //  EQ JAZZ



        //BL Time
        { ID_MUSIC_BL_ON_BMP,                       NULL,                NULL },    //  Backlight never off
        { ID_MUSIC_BL_10S_BMP,                      NULL,                NULL },    //last 10 second
        { ID_MUSIC_BL_30S_BMP,                      NULL,                NULL },    //
        { ID_MUSIC_BL_60S_BMP,                      NULL,                NULL },    //
        { ID_MUSIC_BL_90S_BMP,                      NULL,                NULL },    //

        //volume
        { ID_MUSIC_VOLUME_STATUS_BMP,               NULL,                NULL },    //

        //bettery
        { ID_MUSIC_BETTERY0_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY1_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY2_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY3_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY4_BMP,                    NULL,                NULL },    //
        { ID_MUSIC_BETTERY5_BMP,                    NULL,                NULL },    //

        //������
        //play mode button
        { ID_MUSIC_PLAY_ALL_BMP,                    NULL,                NULL },        //����ȫ��, unfocused
        { ID_MUSIC_PLAY_ALL_BTN_FOCUSED_BMP,        NULL,                NULL },        //����ȫ��, focused
        { ID_MUSIC_PLAY_ALL_BTN_PRESSED_BMP,        NULL,                NULL },        //����ȫ��, pressed

        { ID_MUSIC_PLAY_ONE_BMP,                    NULL,                NULL },        //����һ��, unfocused
        { ID_MUSIC_PLAY_ONE_BTN_FOCUSED_BMP,        NULL,                NULL },        //����һ��, focused
        { ID_MUSIC_PLAY_ONE_BTN_PRESSED_BMP,        NULL,                NULL },        //����һ��, pressed

        { ID_MUSIC_PLAY_RANDOM_BMP,                 NULL,                NULL },        //�������, unfocused
        { ID_MUSIC_PLAY_RANDOM_BTN_FOCUSED_BMP,     NULL,                NULL },        //�������, focused
        { ID_MUSIC_PLAY_RANDOM_BTN_PRESSED_BMP,     NULL,                NULL },        //�������, pressed

        { ID_MUSIC_REPEAT_ALL_BMP,                  NULL,                NULL },        //�ظ�����ȫ��, unfocused
        { ID_MUSIC_REPEAT_ALL_BTN_FOCUSED_BMP,      NULL,                NULL },        //�ظ�����ȫ��, focused
        { ID_MUSIC_REPEAT_ALL_BTN_PRESSED_BMP,      NULL,                NULL },        //�ظ�����ȫ��, pressed

        { ID_MUSIC_REPEAT_ONE_BMP,                  NULL,                NULL },        //�ظ�����һ��, unfocused
        { ID_MUSIC_REPEAT_ONE_BTN_FOCUSED_BMP,      NULL,                NULL },        //�ظ�����һ��, focused
        { ID_MUSIC_REPEAT_ONE_BTN_PRESSED_BMP,      NULL,                NULL },        //�ظ�����һ��, pressed

        //EQ mode button
        { ID_MUSIC_EQ_MODE_BTN_UNFOCUSED_BMP,       NULL,                NULL },        //EQ mode button, unfocused
        { ID_MUSIC_EQ_MODE_BTN_FOCUSED_BMP,         NULL,                NULL },        //focused
        { ID_MUSIC_EQ_MODE_BTN_PRESSED_BMP,         NULL,                NULL },        //pressed

        //BL time button
        { ID_MUSIC_BL_TIME_BTN_UNFOCUSED_BMP,       NULL,                NULL },        //EQ mode button, unfocused
        { ID_MUSIC_BL_TIME_BTN_FOCUSED_BMP,         NULL,                NULL },        //focused
        { ID_MUSIC_BL_TIME_BTN_PRESSED_BMP,         NULL,                NULL },        //pressed


        { ID_MUSIC_USB_UNFOCUSED_BMP,               NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_USB_UNFOCUSED_BMP,                   NULL,                NULL },    //focused
        { ID_MUSIC_USB_FOCUSED_BMP,                 NULL,                NULL },        //pressed
        //play next button
        { ID_MUSIC_SD_UNFOCUSED_BMP,                NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_SD_UNFOCUSED_BMP,                    NULL,                NULL },    //focused
        { ID_MUSIC_SD_FOCUSED_BMP,                  NULL,                NULL },    //pressed

        //play previous button
        { ID_MUSIC_PLAY_PRE_BTN_UNFOCUSED_BMP,      NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_PLAY_PRE_BTN_FOCUSED_BMP,        NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_PRE_BTN_FOCUSED_BMP,        NULL,                NULL },        //pressed
        //play next button
        { ID_MUSIC_PLAY_NEXT_BTN_UNFOCUSED_BMP,     NULL,                NULL },    //������һ��, unfocused
        { ID_MUSIC_PLAY_NEXT_BTN_FOCUSED_BMP,       NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_NEXT_BTN_FOCUSED_BMP,       NULL,                NULL },    //pressed
        //play pause button
        { ID_MUSIC_PAUSE0_BMP,  NULL,                NULL },    //������ͣ, unfocused
        { ID_MUSIC_PAUSE0_BMP,      NULL,                NULL },                    //focused
        { ID_MUSIC_PAUSE1_BMP,      NULL,                NULL },                    //pressed

        //play backward button
        { ID_MUSIC_PLAY_BACKWARD_BTN_UNFOCUSED_BMP, NULL,                NULL },    //����, unfocused
        { ID_MUSIC_PLAY_BACKWARD_BTN_FOCUSED_BMP,   NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_BACKWARD_BTN_FOCUSED_BMP,   NULL,                NULL },    //pressed

        //play forward button
        { ID_MUSIC_PLAY_FORWARD_BTN_UNFOCUSED_BMP,  NULL,                NULL },    //���, unfocused
        { ID_MUSIC_PLAY_FORWARD_BTN_FOCUSED_BMP,    NULL,                NULL },    //focused
        { ID_MUSIC_PLAY_FORWARD_BTN_FOCUSED_BMP,    NULL,                NULL },    //pressed

        //play status button,
        { ID_MUSIC_PAUSE0_BMP,              NULL,               NULL },                                 //���ڲ���״̬
        { ID_MUSIC_PLAY0_BMP,                   NULL,               NULL },                                 //��ͣ״̬
        { ID_MUSIC_PREVIOUS_STATUS_BMP,             NULL,               NULL },                                 //��һ��״̬
        { ID_MUSIC_NEXT_STATUS_BMP,                 NULL,               NULL },                                 //��һ��״̬

        //process bar
        { ID_MUSIC_PROCESS_BAR_HEAD_BMP,                NULL,               NULL },                                 //process bar ͷ��
        { ID_MUSIC_PROCESS_BAR_BODY0_BMP,           NULL,               NULL },                                 //body
        { ID_MUSIC_PROCESS_BAR_TAIL_BMP,                NULL,               NULL },                                 //β��
        { ID_MUSIC_PROCESS_BAR_POINT_BMP,           NULL,               NULL },                                 //���ȵ�

        //������Ϣͼ��
        { ID_MUSIC_PERFORMER_NAME_BMP,              NULL,               NULL },                                 //����ͼ��
        { ID_MUSIC_FILE_SIZE_BMP,                   NULL,               NULL },                                 //
        { ID_MUSIC_ALBUM_NAME_BMP,                  NULL,               NULL },                                 //
        { ID_MUSIC_BIT_RATE_BMP,                        NULL,               NULL },                                 //

        //Ƶ��ͼͼ��
        { ID_MUSIC_PARTICLE_BMP,                        NULL,               NULL },
        { ID_MUSIC_BOUY_BMP,                            NULL,               NULL },

        //�������������
        { ID_MUSIC_LIST_SCROLL_BG_BMP,              NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_HEAD_BMP,                NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_BODY_BMP,                NULL,               NULL },
        { ID_MUSIC_LIST_SCROLL_TAIL_BMP,                NULL,               NULL },

        //{ID_MUSIC_MENU}
        //bg
        { ID_MUSIC_BG_BMP,                  NULL,               NULL },
        { ID_MUSIC_INFO_BAR_BMP,                NULL,               NULL },
        { ID_MUSIC_PLAYBAR_BMP,             NULL,               NULL },
        //flag
        {ID_MUSIC_PROCESS_BAR_FLAG_BMP,         NULL,               NULL},

        //bottom
        {ID_MUSIC_BACK0_BMP,            NULL,               NULL},
        {ID_MUSIC_EQ_CHANGE0_BMP,       NULL,               NULL},
        {ID_MUSIC_PLAY_MODE0_BMP,       NULL,               NULL},
        {ID_MUSIC_EQ_LYRICS0_BMP,       NULL,               NULL},
        {ID_MUSIC_AB0_BMP,              NULL,               NULL},//112350 ID_MUSIC_AB0_BMP
        {ID_MUSIC_BACK1_BMP,            NULL,               NULL},
        {ID_MUSIC_EQ_CHANGE1_BMP,       NULL,               NULL},
        {ID_MUSIC_PLAY_MODE1_BMP,       NULL,               NULL},
        {ID_MUSIC_EQ_LYRICS1_BMP,       NULL,               NULL},
        {ID_MUSIC_AB1_BMP,              NULL,               NULL},//112350 ID_MUSIC_AB1_BMP
        {ID_MUSIC_BOTTOM_BMP,           NULL,               NULL},
        //play pause
        {ID_MUSIC_PLAY0_BMP,            NULL,               NULL},
        {ID_MUSIC_PLAY1_BMP,            NULL,               NULL},

        {ID_MUSIC_MENU_UNFOCUSED_BMP,    NULL,               NULL},
        {ID_MUSIC_MENU_FOCUSED_BMP,      NULL,               NULL},

        {ID_MUSIC_MODE_UNFCS_BMP,       NULL,               NULL},
        {ID_MUSIC_MODE_FCS_BMP,          NULL,               NULL},
        {ID_MUSIC_ALBUM_BMP,             NULL,               NULL},
        {ID_MUSIC_HEADBAR_BG_BMP,             NULL,               NULL},
        { 0,                             NULL,                  NULL }          //browser_icon_max
    },
};




GUI_RECT *music_get_touch_rect(__u32 iconRectID)
{
    if (iconRectID >= MUSIC_TOUCH_ICON_MAX)
    {
        return NULL;
    }

    return &music_touch_rect[iconRectID];
}

__bool music_is_touch_icon(__u32 iconRectID, __u16 x, __u16 y)
{
    if (iconRectID >= MUSIC_TOUCH_ICON_MAX)
    {
        return EPDK_FALSE;
    }

    if (x >= music_touch_rect[iconRectID].x0 && x <= music_touch_rect[iconRectID].x1 &&
        y >= music_touch_rect[iconRectID].y0 && y <= music_touch_rect[iconRectID].y1)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}

//�������explorer AP��Ƶ�UI����
music_ui_t *music_get_ui_param(void)
{
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        return &(music_ui_800_480);
    }
    else if((screen_size.width == 480) && (screen_size.height == 640))
    {
        //return &(music_ui_480_640);
        return &(music_ui_800_480);
    }
    else //if((screen_size.width == 720 )&&( screen_size.height == 480))
    {
        return &(music_ui_720_480);
    }
}

//������������UI����
music_list_ui_t *music_get_list_ui_param(void)
{
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        return &(music_ui_800_480.list_ctrl);
    }
    else //if((screen_size.width == 720 )&&( screen_size.height == 480))
    {
        return &(music_ui_720_480.list_ctrl);
    }
}

//��ö���HeadbarUI����
music_headbar_ui_t *music_get_headbar_ui_param(void)
{
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        return &(music_ui_800_480.headbar);
    }
    else //if((screen_size.width == 720 )&&( screen_size.height == 480))
    {
        return &(music_ui_720_480.headbar);
    }
}

//��ö���playerUI����
music_palyer_ui_t *music_get_player_ui_param(void)
{
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        return &(music_ui_800_480.player);
    }
    else //if((screen_size.width == 720 )&&( screen_size.height == 480))
    {
        return &(music_ui_720_480.player);
    }
}

//���volume barUI����
music_volume_bar_ui_t *music_get_volume_bar_ui_param(void)
{
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        //__here__;
        return &(music_ui_800_480.volume_bar);
    }
    else //if((screen_size.width == 720 )&&( screen_size.height == 480))
    {
        //__here__;
        return &(music_ui_720_480.volume_bar);
    }
}
/*
************************************************************************************************************************
*Function   :         void* explorer_get_listview_icon_res(__u32 index)
*
*Description    :
*
*Arguments      :
*
*Return         :
*
************************************************************************************************************************
*/
void *music_get_icon_res(__u32 index)
{
    music_icon_data_t *icon_table = NULL;
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        icon_table = music_ui_800_480.icon_table;
    }
    else //if((screen_size.width == 720 )&&( screen_size.height == 480))
    {
        icon_table = music_ui_720_480.icon_table;
    }

    if (index > MUSIC_ICON_MAX)
    {
        return NULL;
    }

    //__msg("----icon_id: %d, res_id: %d\n", index, icon_table[index].res_id);
    if (icon_table == NULL)
    {
        return NULL;
    }
    else if (icon_table[index].res_id == 0)
    {
        return NULL;
    }
    else if (icon_table[index].res_handle == NULL)
    {
#if 1
        icon_table[index].res_handle = theme_open(icon_table[index].res_id);
        icon_table[index].res_buffer = theme_hdl2buf(icon_table[index].res_handle);
        return icon_table[index].res_buffer;
#else
        {
            int i = 0;
            __msg("------------open all of the picture at first\n");
            app_print_memory_usage("new memory for picture resource", NULL);
            app_print_memory_usage("new memory for picture resource", NULL);

            for (i = 0; i < MUSIC_ICON_MAX; i++)
            {
                if (icon_table[i].res_handle == NULL)
                {
                    icon_table[i].res_handle = theme_open(icon_table[i].res_id);
                    icon_table[i].res_buffer = theme_hdl2buf(icon_table[i].res_handle);
                }
            }

            app_print_memory_usage("new memory for picture resource finished", NULL);
            return icon_table[index].res_buffer;
        }
#endif
    }
    else
    {
        return icon_table[index].res_buffer;
    }
}

void *music_get_album_buf(void)
{
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        if (music_ui_800_480.album.albumBuf == NULL)
        {
            music_ui_800_480.album.albumBuf = esMEMS_Balloc(music_ui_800_480.album.albumRect.width * music_ui_800_480.album.albumRect.height * 4 + 0x100);
        }

        return music_ui_800_480.album.albumBuf;
    }
    else //if((screen_size.width == 720 )&&( screen_size.height == 480))
    {
        if (music_ui_720_480.album.albumBuf == NULL)
        {
            music_ui_720_480.album.albumBuf = esMEMS_Balloc(music_ui_720_480.album.albumRect.width * music_ui_720_480.album.albumRect.height * 4 + 0x100);
        }

        return music_ui_720_480.album.albumBuf;
    }
}

/*
************************************************************************************************************************
*Function   :         __s32 explorer_free_listview_icon_res(void)
*
*Description    :
*
*Arguments      :
*
*Return         :
*
************************************************************************************************************************
*/
__s32 music_free_icon_res(void)
{
    __s32 i = 0;
    music_icon_data_t *icon_table = NULL;
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        icon_table = music_ui_800_480.icon_table;

        if (music_ui_800_480.album.albumBuf)
        {
            esMEMS_Bfree(music_ui_800_480.album.albumBuf, music_ui_800_480.album.albumRect.width * music_ui_800_480.album.albumRect.height * 4 + 0x100);
            music_ui_800_480.album.albumBuf = NULL;
        }
    }
    else
    {
        icon_table = music_ui_720_480.icon_table;

        if (music_ui_720_480.album.albumBuf)
        {
            esMEMS_Bfree(music_ui_720_480.album.albumBuf, music_ui_720_480.album.albumRect.width * music_ui_720_480.album.albumRect.height * 4 + 0x100);
            music_ui_720_480.album.albumBuf = NULL;
        }
    }

    if (icon_table == NULL)
    {
        return EPDK_FAIL;
    }
    else
    {
        //app_print_memory_usage("release memory for picture resource", NULL);
        //app_print_memory_usage("release memory for picture resource", NULL);
        for (i = 0; i < MUSIC_ICON_MAX; i++)
        {
            if (icon_table[i].res_handle != NULL)
            {
                theme_close(icon_table[i].res_handle);
                icon_table[i].res_handle = NULL;
                icon_table[i].res_buffer = NULL;
            }
        }

        //app_print_memory_usage("release memory for picture resource finished", NULL);
        return EPDK_OK;
    }
}

//���Explorer ����layer size
__s32 music_get_screen_rect(RECT *screen_rect)
{
    music_ui_t *ui_param;
    ui_param = music_get_ui_param();
    screen_rect->x = ui_param->layer.scn_rect.x;
    screen_rect->y = ui_param->layer.scn_rect.y;
    screen_rect->width = ui_param->layer.scn_rect.width;
    screen_rect->height = ui_param->layer.scn_rect.height;
    return EPDK_OK;
}

//����listbarÿҳ������ʾ����Ŀ����
__u32 music_count_listbar_pagenum(void)
{
    __u32 pagenum = 0;
    music_ui_t *ui_param;
    ui_param = music_get_ui_param();
    pagenum = ui_param->list_ctrl.listbar_rect.height / ui_param->list_ctrl.item_rect.height;
    return pagenum;
}

void music_modify_icon_res(__s32 rat_root_type)
{
    music_icon_data_t *icon_table = NULL;
    SIZE screen_size;
    __s32 screen_type;
    dsk_display_get_size(&screen_size.width, &screen_size.height);

    if ((screen_size.width == 800) && (screen_size.height == 480))
    {
        icon_table = music_ui_800_480.icon_table;
    }
    else
    {
        icon_table = music_ui_720_480.icon_table;
    }

    if (RAT_TF == rat_root_type)
    {
        icon_table[ID_MUSIC_SD_CARD_BTN_UNFOCUSED_ICON].res_id =
            icon_table[ID_MUSIC_SD_CARD_BTN_PRESSED_ICON].res_id;
        icon_table[ID_MUSIC_USB_DISK_BTN_UNFOCUSED_ICON].res_id =
            icon_table[ID_MUSIC_USB_DISK_BTN_FOCUSED_ICON].res_id;
    }
    else if (RAT_USB == rat_root_type)
    {
        icon_table[ID_MUSIC_SD_CARD_BTN_UNFOCUSED_ICON].res_id =
            icon_table[ID_MUSIC_SD_CARD_BTN_FOCUSED_ICON].res_id;
        icon_table[ID_MUSIC_USB_DISK_BTN_UNFOCUSED_ICON].res_id =
            icon_table[ID_MUSIC_USB_DISK_BTN_PRESSED_ICON].res_id;
    }
    else
    {
        __wrn("reg_root_type error...\n");
    }
}



