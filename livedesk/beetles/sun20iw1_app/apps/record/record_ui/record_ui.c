/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*
* File      : record_ui.c
* By        : Kingvan
* Func      : record view function
* Version   : v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  Kingvan  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#include "record_ui.h"
#include <log.h>

static GUI_RECT record_touch_rect[RECORD_VIEW_TOUCH_ICON_MAX] =
{
    {54, 362, 158, 420},        //¼������
    {178, 362, 282, 420},   //¼������
    {302, 362, 502, 420},   //¼����ʼ
    {522, 362, 626, 420},   //¼������
    {646, 362, 750, 420},   //¼���б�
    //..{388,235,475,271},  //¼����ʽ

    {662, 290, 772, 321}, //������
    {662, 245, 772, 280}, //һ������
    {662, 202, 772, 235}, //������


    {410, 142, 490, 165},   //WMA
    {410, 168, 490, 200},   //WAV
    {410, 200, 490, 218},   //MP3

    {360, 250, 427, 300},   //¼�����ŷ���
    {447, 250, 514, 300},   //¼�����ſ���
    {534, 250, 601, 300},   //¼��������ͣ
    {621, 250, 688, 300},   //¼�����ſ��
    {708, 250, 775, 300},   //¼��ɾ��

    {367, 150, 767, 170},   //¼�����Ž�����


    {182, 109, 299, 136},   //USBѡ��
    {182, 138, 299, 163},   //SDѡ��

};

//ӳ��ͼƬ
static const res_mapping_info_t record_res_mapping[MENU_MAX] =
{
#if !RECORD_CHANGE_0815
    {STRING_BACK_RC, ID_RECORD_MENU_BACK_0_BMP, ID_RECORD_MENU_BACK_1_BMP},
#endif
    {STRING_MUSCI_LIST_RC, ID_RECORD_MENU_LIST_0_BMP, ID_RECORD_MENU_LIST_1_BMP},
    {STRING_RECORD_RC, ID_RECORD_MENU_RECORD_0_BMP, ID_RECORD_MENU_RECORD_1_BMP},
    {STRING_SAVE_RC, ID_RECORD_MENU_SAVE_0_BMP, ID_RECORD_MENU_SAVE_1_BMP},
#if !RECORD_CHANGE_0815
    {STRING_QUALITY_RC, ID_RECORD_MENU_QUALITY_0_BMP, ID_RECORD_MENU_QUALITY_1_BMP},
#endif
    //..{STRING_TYPE_RC, ID_RECORD_MENU_QUALITY_0_BMP,ID_RECORD_MENU_QUALITY_1_BMP}
};
static const res_mapping_info_t record_res_music_mapping[MUSIC_MENU_MAX] =
{
    {STRING_BACK_RC, ID_RECORD_BACK_0_BMP, ID_RECORD_BACK_1_BMP},
    {STRING_BACKWARD_RC, ID_RECORD_PRE_0_BMP, ID_RECORD_PRE_1_BMP},
    {STRING_PLAY_RC, ID_RECORD_PLAY_PAUSE_BMP, ID_RECORD_PLAY_PAUSE1_BMP},
    {STRING_PAUSE_RC, ID_RECORD_PLAY_START_BMP, ID_RECORD_PLAY_START1_BMP},
    {STRING_FORWARD_RC, ID_RECORD_NEXT_0_BMP, ID_RECORD_NEXT_1_BMP},
    {STRING_DELETE_RC, ID_RECORD_DEL_0_BMP, ID_RECORD_DEL_1_BMP}
};
static const GUI_POINT          record_res_mapping_bmp_coor[MENU_MAX] =
{
#if !RECORD_CHANGE_0815
    {54, 362},
#endif
    {178, 362}, {302, 362}, {522, 362}
#if !RECORD_CHANGE_0815
    ,
    {646, 362} //..,{402,235}
#endif

};
static const GUI_POINT          record_res_music_mapping_bmp_coor[MUSIC_MENU_MAX] =
{
    {360, 250}, {447, 250}, {534, 250}, {534, 250}, {621, 250}, {708, 250}
};
static GUI_RECT                 record_res_mapping_lang_rect[MENU_MAX] =
{
#if !RECORD_CHANGE_0815
    {84, 442, 128, 471},
#endif
    {208, 442, 252, 471}, {380, 442, 424, 471}, {552, 442, 596, 471}
#if !RECORD_CHANGE_0815
    , {676, 442, 720, 471} //..,{437,235,479,264},
#endif


};
static GUI_RECT                 record_res_music_mapping_lang_rect[MUSIC_MENU_MAX] =
{
    {269, 102, 308, 120}, {269, 102, 308, 120}, {269, 102, 308, 120}, {269, 102, 308, 120}, {269, 102, 308, 120}, {269, 102, 308, 120}
};

//��ͨͼƬ
static const __s32              record_res_bmp[MAX_RECORD_BMP_ITEM] =
{
    //����ͼƬ
    ID_RECORD_0_BMP, ID_RECORD_1_BMP, ID_RECORD_2_BMP, ID_RECORD_3_BMP, ID_RECORD_4_BMP,
    ID_RECORD_5_BMP, ID_RECORD_6_BMP, ID_RECORD_7_BMP, ID_RECORD_8_BMP, ID_RECORD_9_BMP, ID_RECORD_10_BMP,
    //���ֲ��Ž���
    ID_RECORD_66_BMP, ID_RECORD_66_LEFT_BMP, ID_RECORD_66_RIGHT_BMP, ID_RECORD_66_MID_BMP, ID_RECORD_66_FLAG_BMP,
    //¼������
    ID_RECORD_ANI_BG_BMP, ID_RECORD_ANI_0_BMP, ID_RECORD_ANI_1_BMP, ID_RECORD_ANI_2_BMP, ID_RECORD_ANI_3_BMP, ID_RECORD_ANI_4_BMP,
    ID_RECORD_ANI_5_BMP, ID_RECORD_ANI_6_BMP, ID_RECORD_ANI_7_BMP,
    //�ײ��Ͷ���bar
    ID_RECORD_BAR_BOTTOM_BMP, ID_RECORD_BAR_TOP_BMP,
    //���״̬
    ID_RECORD_BATTERY_0_BMP, ID_RECORD_BATTERY_1_BMP, ID_RECORD_BATTERY_2_BMP, ID_RECORD_BATTERY_3_BMP, ID_RECORD_BATTERY_4_BMP, ID_RECORD_BATTERY_5_BMP,
    ID_RECORD_BATTERY_ADAPTER_BMP, ID_RECORD_BATTERY_ADD_BMP, ID_RECORD_BATTERY_NO_BMP,
    //�������
    ID_RECORD_VOL_OFF_BMP, ID_RECORD_VOL_ON_BMP,
    //ɾ���Ի���
    ID_RECORD_DEL_BOX_BMP, ID_RECORD_DEL_SEL_0_BMP, ID_RECORD_DEL_SEL_1_BMP,
    //�����Ի���
    ID_RECORD_QUALITY_BK_BMP, ID_RECORD_QUALITY_FLAG_BMP, ID_RECORD_QUALITY_BK1_BMP,
    //¼����ʽ
    ID_RECORD_FORMAT_BK_BMP,
    //�����б����
    ID_RECORD_MUSIC_PROGRESS_BG_BMP, ID_RECORD_MUSIC_PROGRESS_DOWN_BMP,
    ID_RECORD_MUSIC_PROGRESS_FLAG_BMP, ID_RECORD_MUSIC_PROGRESS_UP_BMP, ID_RECORD_MUSIC_PROGRESS_FOCUS_BMP,
    //����
    ID_RECORD_MUSIC_PARTICLE_BMP, ID_RECORD_MUSIC_BOUY_BMP,
    //¼��ֹͣ, ¼����ʼ, ¼����ͣ
    ID_RECORD_MENU_RECORD_2_BMP, ID_RECORD_MENU_RECORD_0_BMP, ID_RECORD_MENU_RECORD_1_BMP,
    //��������
    ID_RECORD_VOL_BAR_BG_BMP, ID_RECORD_VOL_BAR_BODY_BMP, ID_RECORD_VOL_LEFT_BMP,
    ID_RECORD_VOL_MID_BMP, ID_RECORD_VOL_RIGHT_BMP, ID_RECORD_VOL_SPEAKER_RIGHT_BMP,
    //¼�����ű���
    ID_RECORD_MUSICLIST_BG_BMP, ID_RECORD_MUSICPLAY_BG_BMP,
    //¼���б���
    ID_RECORD_LIST_WIN_BG_BMP, ID_RECORD_LIST_BG_BMP,
    //¼������
    ID_RECORD_BG_BMP, ID_RECORD_BG0_BMP


};
static const GUI_POINT          record_res_bmp_coor[MAX_RECORD_BMP_ITEM] =
{
    //����ͼƬ
    {250, 168}, {250, 168}, {250, 168}, {250, 168}, {250, 168},
    {250, 168}, {250, 168}, {250, 168}, {250, 168}, {250, 168}, {250, 168},
    //���ֲ��Ž���
    {367, 160}, {367, 160}, {367, 160}, {367, 160}, {367, 160},
    //¼������
    {231, 287}, {231, 287}, {231, 287}, {231, 287}, {231, 287}, {231, 287}, {231, 287}, {231, 287}, {231, 287},
    //�ײ��Ͷ���bar
    {0, 362}, {0, 0},
    //���״̬
    {348, 4}, {348, 4}, {348, 4}, {348, 4}, {348, 4}, {348, 4},
    {348, 4}, {348, 4}, {348, 4},
    //�������
    {280, 4}, {280, 4},
    //ɾ���Ի���
    {101, 54}, {158, 124}, {155, 124},
    //�����Ի���
    {637, 222}, {653, 317}, {637, 222},
    //¼����ʽ
    {361, 168},
    //�����б����
    {158, 33}, {156, 230},
    {158, 33}, {156, 22}, {0, 33},
    //����
    {0, 0}, {0, 0},
    //¼��ֹͣ, ¼����ʼ, ¼����ͣ
    {174, 235}, {302, 362}, {302, 362}, //..
    //��������
    {0, 219}, {76, 228}, {76, 228},
    {76, 228}, {76, 228}, {335, 222},
    //¼�����ű���
    {0, 66}, {367, 150},
    //¼���б���
    {0, 0}, {0, 29},
    //¼������
    {0, 0}, {248, 170}
};
static const __s32              record_res_lang[MAX_RECORD_LANG_ITEM] =
{
    //����
    STRING_POOR_QUALITY_RC,
    STRING_NORMAL_QUALITY_RC,
    STRING_GOOD_QUALITY_RC,
    //¼����,���Ż�
    STRING_RECORDER_RC,
    STRING_RECORD_PLAYER_RC,
    //��¼��ʱ��
    STRING_RECORD_TIME_RC,
    //��ͣ,������..
    STRING_PAUSE_RC,
    STRING_PLAYING_RC,
    //ֹͣ
    STRING_STOP_RC,
    //ɾ���Ի���
    STRING_DELETE_RC,
    STRING_MSG4_RC,
    STRING_YES_RC,
    STRING_NO_RC,
    //¼��ֹͣ, ¼����ʼ, ¼����ͣ
    STRING_STOP_RC, STRING_RECORD_RC, STRING_PAUSE_RC,
    //��¼���ļ�
    STRING_MSG3_RC

};
static GUI_RECT                 record_res_lang_rect[MAX_RECORD_LANG_ITEM] =
{
    //����
    {662, 313, 772, 341},
    {662, 263, 772, 302},
    {662, 221, 772, 261},
    //¼����,���Ż�
    {0, 0, 70, 18},
    {0, 0, 70, 18},
    //��¼��ʱ��
    {10, 27, 106, 48},
    //��ͣ,������..
    {255, 189, 325, 205},
    {255, 189, 325, 205},
    //ֹͣ
    {255, 189, 325, 205},
    //ɾ���Ի���
    {170, 58, 205, 76},
    {145, 83, 272, 110},
    {159, 126, 196, 143},
    {230, 125, 266, 144},
    //¼��ֹͣ, ¼����ʼ, ¼����ͣ
    {145, 215, 198, 240}, {145, 215, 198, 240}, {145, 215, 198, 240},
    //��¼���ļ�
    {360, 80, 460, 121}
};

GUI_RECT *record_get_touch_rect(__u32 iconRectID)
{
    if (iconRectID >= RECORD_VIEW_TOUCH_ICON_MAX)
    {
        return NULL;
    }

    return &record_touch_rect[iconRectID];
}

__bool record_is_touch_icon(__u32 iconRectID, __u16 x, __u16 y)
{
    if (iconRectID >= RECORD_VIEW_TOUCH_ICON_MAX)
    {
        return EPDK_FALSE;
    }

    if (x >= record_touch_rect[iconRectID].x0 && x <= record_touch_rect[iconRectID].x1 &&
        y >= record_touch_rect[iconRectID].y0 && y <= record_touch_rect[iconRectID].y1)
    {
        return EPDK_TRUE;
    }
    else
    {
        return EPDK_FALSE;
    }
}

/*
    ������Դ
*/
__s32 init_record_res(precord_ui_t pui)
{
    __u32   i;

    //com_set_palette_by_id(ID_RECORD_WKM_BMP);
    //Ԥ�ȼ���ӳ��ͼƬ
    for (i = 0; i < MENU_MAX; i++)
    {
        pui->mapping_bmp[i].focus = NULL;
        pui->mapping_bmp[i].unfocus = NULL;
        pui->mapping_lang[i][0] = '\0';
    }

    for (i = 0; i < MUSIC_MENU_MAX; i++)
    {
        pui->music_mapping_bmp[i].focus = NULL;
        pui->music_mapping_bmp[i].unfocus = NULL;
        pui->music_mapping_lang[i][0] = '\0';
    }

    //��ʼ��һ��ͼƬ
    for (i = 0; i < MAX_RECORD_BMP_ITEM; i++)
    {
        pui->bmp[i] = NULL;
    }

    //��ʼ��һ������
    for (i = 0; i < MAX_RECORD_LANG_ITEM; i++)
    {
        pui->lang[i][0] = '\0';
    }

    return EPDK_OK;
}

/*
    ж����Դ
*/
__s32 uninit_record_res(precord_ui_t pui)
{
    __u32   i;

    //�ͷ�ӳ��ͼƬ
    for (i = 0; i < MENU_MAX; i++)
    {
        if (pui->mapping_bmp[i].focus)
        {
            dsk_theme_close(pui->mapping_bmp[i].focus);
            pui->mapping_bmp[i].focus = NULL;
        }

        if (pui->mapping_bmp[i].unfocus)
        {
            dsk_theme_close(pui->mapping_bmp[i].unfocus);
            pui->mapping_bmp[i].unfocus = NULL;
        }

        pui->mapping_lang[i][0] = '\0';
    }

    for (i = 0; i < MUSIC_MENU_MAX; i++)
    {
        if (pui->music_mapping_bmp[i].focus)
        {
            dsk_theme_close(pui->music_mapping_bmp[i].focus);
            pui->music_mapping_bmp[i].focus = NULL;
        }

        if (pui->music_mapping_bmp[i].unfocus)
        {
            dsk_theme_close(pui->music_mapping_bmp[i].unfocus);
            pui->music_mapping_bmp[i].unfocus = NULL;
        }

        pui->music_mapping_lang[i][0] = '\0';
    }

    //�ͷ�һ��ͼƬ
    for (i = 0; i < MAX_RECORD_BMP_ITEM; i++)
    {
        if (pui->bmp[i])
        {
            dsk_theme_close(pui->bmp[i]);
            pui->bmp[i] = NULL;
        }
    }

    //�ͷ�һ������
    for (i = 0; i < MAX_RECORD_LANG_ITEM; i++)
    {
        pui->lang[i][0] = '\0';
    }

    return EPDK_OK;
}

/*
 *  �滭ӳ��ͼƬ
 */
void draw_record_mapping(precord_ui_t pui, __s32 mappingIndex, FOCUSE_STATE focus, __s16 mappOffset)
{
    void *pbmp;
    GUI_COLOR mapping_txt_color;

    if (focus >= FOCUSE_STATE_MAX || mappingIndex >= MENU_MAX)
    {
        __wrn("focus >= FOCUSE_STATE_MAX || mappingIndex >= MENU_MAX\r\n");
        return;
    }

    if (focus == FOCUSE_NORMAL || focus == FOCUSE_OVER)
    {
        if (pui->mapping_bmp[mappingIndex].unfocus == NULL)
        {
            pui->mapping_bmp[mappingIndex].unfocus = dsk_theme_open(record_res_mapping[mappingIndex].unfocus);
        }

        if (focus == FOCUSE_NORMAL)
        {
            mapping_txt_color = GUI_GRAY;
        }
        else
        {
            mapping_txt_color = GUI_RED;
        }

        pbmp = dsk_theme_hdl2buf(pui->mapping_bmp[mappingIndex].unfocus);
    }
    else
    {
        if (pui->mapping_bmp[mappingIndex].focus == NULL)
        {
            pui->mapping_bmp[mappingIndex].focus = dsk_theme_open(record_res_mapping[mappingIndex].focus);
        }

        mapping_txt_color = GUI_RED;
        pbmp = dsk_theme_hdl2buf(pui->mapping_bmp[mappingIndex].focus);
    }

    GUI_BMP_Draw(pbmp, record_res_mapping_bmp_coor[mappingIndex + mappOffset].x, record_res_mapping_bmp_coor[mappingIndex + mappOffset].y);

    if (pui->mapping_lang[mappingIndex][0] == '\0')
    {
        dsk_langres_get_menu_text(record_res_mapping[mappingIndex].lang, pui->mapping_lang[mappingIndex], GUI_NAME_MAX);
    }

    GUI_SetColor(mapping_txt_color);
    GUI_DispStringInRect(pui->mapping_lang[mappingIndex], &record_res_mapping_lang_rect[mappingIndex + mappOffset], GUI_TA_HCENTER | GUI_TA_VCENTER);
}
void draw_record_music_mapping(precord_ui_t pui, __s32 mappingIndex, FOCUSE_STATE focus)
{
    void *pbmp;

    if (focus >= FOCUSE_STATE_MAX || mappingIndex >= MUSIC_MENU_MAX)
    {
        return;
    }

    if (focus == FOCUSE_NORMAL)
    {
        if (pui->music_mapping_bmp[mappingIndex].unfocus == NULL)
        {
            pui->music_mapping_bmp[mappingIndex].unfocus = dsk_theme_open(record_res_music_mapping[mappingIndex].unfocus);
        }

        pbmp = dsk_theme_hdl2buf(pui->music_mapping_bmp[mappingIndex].unfocus);
    }
    else if (focus == FOCUSE_OVER || focus == FOCUSE_UP)
    {
        if (pui->music_mapping_bmp[mappingIndex].focus == NULL)
        {
            pui->music_mapping_bmp[mappingIndex].focus = dsk_theme_open(record_res_music_mapping[mappingIndex].focus);
        }

        pbmp = dsk_theme_hdl2buf(pui->music_mapping_bmp[mappingIndex].focus);
        //..if( pui->music_mapping_lang[mappingIndex][0] == '\0' )
        //..dsk_langres_get_menu_text(record_res_music_mapping[mappingIndex].lang, pui->music_mapping_lang[mappingIndex], GUI_NAME_MAX);
        //..GUI_SetColor(GUI_WHITE);
        //..GUI_DispStringInRect(pui->music_mapping_lang[mappingIndex], &record_res_music_mapping_lang_rect[mappingIndex], GUI_TA_HCENTER|GUI_TA_VCENTER);
    }
    else
    {
        if (pui->music_mapping_bmp[mappingIndex].unfocus == NULL)
        {
            pui->music_mapping_bmp[mappingIndex].unfocus = dsk_theme_open(record_res_music_mapping[mappingIndex].unfocus);
        }

        pbmp = dsk_theme_hdl2buf(pui->music_mapping_bmp[mappingIndex].unfocus);
        //..if( pui->music_mapping_lang[mappingIndex][0] == '\0' )
        //..dsk_langres_get_menu_text(record_res_music_mapping[mappingIndex].lang, pui->music_mapping_lang[mappingIndex], GUI_NAME_MAX);
        //..GUI_SetColor(GUI_WHITE);
        //GUI_DispStringInRect(pui->music_mapping_lang[mappingIndex], &record_res_music_mapping_lang_rect[mappingIndex], GUI_TA_HCENTER|GUI_TA_VCENTER);
    }

    if (pbmp == NULL)
    {
        __wrn("sorry\n");
    }

    GUI_BMP_Draw(pbmp, record_res_music_mapping_bmp_coor[mappingIndex].x, record_res_music_mapping_bmp_coor[mappingIndex].y);
}
/*
 * ���ӳ��ͼƬ
 */
void clean_record_mapping(precord_ui_t pui, __s32 mappingIndex)
{
    void *pbmp;
    __s16 x0, y0, x1, y1;

    if (pui->mapping_bmp[mappingIndex].focus)
    {
        pbmp = dsk_theme_hdl2buf(pui->mapping_bmp[mappingIndex].focus);
        x0 = record_res_mapping_bmp_coor[mappingIndex].x;
        y0 = record_res_mapping_bmp_coor[mappingIndex].y;
        x1 = x0 + GUI_BMP_GetXSize(pbmp);
        y1 = y0 + GUI_BMP_GetYSize(pbmp);
        GUI_ClearRect(x0, y0, x1, y1);
        dsk_theme_close(pui->mapping_bmp[mappingIndex].focus);
        pui->mapping_bmp[mappingIndex].focus = NULL;
    }

    if (pui->mapping_bmp[mappingIndex].unfocus)
    {
        pbmp = dsk_theme_hdl2buf(pui->mapping_bmp[mappingIndex].unfocus);
        x0 = record_res_mapping_bmp_coor[mappingIndex].x;
        y0 = record_res_mapping_bmp_coor[mappingIndex].y;
        x1 = x0 + GUI_BMP_GetXSize(pbmp);
        y1 = y0 + GUI_BMP_GetYSize(pbmp);
        GUI_ClearRect(x0, y0, x1, y1);
        dsk_theme_close(pui->mapping_bmp[mappingIndex].unfocus);
        pui->mapping_bmp[mappingIndex].unfocus = NULL;
    }
}
void clean_record_music_mapping(precord_ui_t pui, __s32 mappingIndex)
{
    void *pbmp;
    __s16 x0, y0, x1, y1;

    if (pui->music_mapping_bmp[mappingIndex].focus)
    {
        pbmp = dsk_theme_hdl2buf(pui->music_mapping_bmp[mappingIndex].focus);
        x0 = record_res_music_mapping_bmp_coor[mappingIndex].x;
        y0 = record_res_music_mapping_bmp_coor[mappingIndex].y;
        x1 = x0 + GUI_BMP_GetXSize(pbmp);
        y1 = y0 + GUI_BMP_GetYSize(pbmp);
        GUI_ClearRect(x0, y0, x1, y1);
        dsk_theme_close(pui->music_mapping_bmp[mappingIndex].focus);
        pui->music_mapping_bmp[mappingIndex].focus = NULL;
    }

    if (pui->music_mapping_bmp[mappingIndex].unfocus)
    {
        pbmp = dsk_theme_hdl2buf(pui->music_mapping_bmp[mappingIndex].unfocus);
        x0 = record_res_music_mapping_bmp_coor[mappingIndex].x;
        y0 = record_res_music_mapping_bmp_coor[mappingIndex].y;
        x1 = x0 + GUI_BMP_GetXSize(pbmp);
        y1 = y0 + GUI_BMP_GetYSize(pbmp);
        GUI_ClearRect(x0, y0, x1, y1);
        dsk_theme_close(pui->music_mapping_bmp[mappingIndex].unfocus);
        pui->music_mapping_bmp[mappingIndex].unfocus = NULL;
    }
}

void *get_record_bmp_data(precord_ui_t pui, __s32 index)
{
    void *pbmp;

    if (pui->bmp[index] == NULL)
    {
        pui->bmp[index] = dsk_theme_open(record_res_bmp[index]);
    }

    pbmp = dsk_theme_hdl2buf(pui->bmp[index]);
    return pbmp;
}
/*
 *  �滭��ͨͼƬ���ִ�
 */
void draw_record_bmp(precord_ui_t pui, __s32 index)
{
    void *pbmp;

    if (pui->bmp[index] == NULL)
    {
        pui->bmp[index] = dsk_theme_open(record_res_bmp[index]);
    }

    pbmp = dsk_theme_hdl2buf(pui->bmp[index]);
    GUI_BMP_Draw(pbmp, record_res_bmp_coor[index].x, record_res_bmp_coor[index].y);
}
void draw_record_bmp_ext(precord_ui_t pui, __s32 index, __s16 offsetX, __s16 offsetY)
{
    void *pbmp;

    if (pui->bmp[index] == NULL)
    {
        pui->bmp[index] = dsk_theme_open(record_res_bmp[index]);
    }

    pbmp = dsk_theme_hdl2buf(pui->bmp[index]);
    GUI_BMP_Draw(pbmp, record_res_bmp_coor[index].x + offsetX, record_res_bmp_coor[index].y + offsetY);
}
void draw_record_bmp_coor(precord_ui_t pui, __s32 index, __s16 X, __s16 Y)
{
    void *pbmp;

    if (pui->bmp[index] == NULL)
    {
        pui->bmp[index] = dsk_theme_open(record_res_bmp[index]);
    }

    pbmp = dsk_theme_hdl2buf(pui->bmp[index]);
    GUI_BMP_Draw(pbmp, X, Y);
}
void draw_record_bmp_cut(precord_ui_t pui, __s32 index, __s16 xCoor, __s16 yCoor, __s16 height)
{
    void *pbmp;
    __s16 x0, y0, x1, y1;

    if (pui->bmp[index] == NULL)
    {
        pui->bmp[index] = dsk_theme_open(record_res_bmp[index]);
    }

    pbmp = dsk_theme_hdl2buf(pui->bmp[index]);
    x0 = xCoor;
    y0 = yCoor;
    x1 = x0 + GUI_BMP_GetXSize(pbmp) - 1;
    y1 = y0 + (GUI_BMP_GetYSize(pbmp) - height) - 1;
    GUI_BMP_Draw(pbmp, xCoor, yCoor);
    GUI_ClearRect(x0, y0, x1, y1);
}
void draw_record_lang(precord_ui_t pui, __s32 index, __s32 fnt_color)
{
    GUI_SetColor(fnt_color);

    if (pui->lang[index][0] == '\0')
    {
        dsk_langres_get_menu_text(record_res_lang[index], pui->lang[index], RECORD_NAME_MAX);
    }

    GUI_DispStringInRect(pui->lang[index], &record_res_lang_rect[index], GUI_TA_HCENTER | GUI_TA_VCENTER);
}
void draw_record_lang_ext(precord_ui_t pui, __s32 index, __s32 fnt_color, GUI_RECT *rect)
{
    GUI_SetColor(fnt_color);

    if (pui->lang[index][0] == '\0')
    {
        dsk_langres_get_menu_text(record_res_lang[index], pui->lang[index], RECORD_NAME_MAX);
    }

    GUI_DispStringInRect(pui->lang[index], rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
}
void draw_record_str(precord_ui_t pui, char *str, __s32 fnt_color, GUI_RECT *rect)
{
    GUI_SetColor(fnt_color);
    GUI_DispStringInRect(str, rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
}
/*
 *  �����ͨͼƬ���ִ�
 */
void clean_record_bmp(precord_ui_t pui, __s32 index)
{
    void *pbmp;
    __s16 x0, y0, x1, y1;

    if (pui->bmp[index] == NULL)
    {
        return;
    }

    pbmp = dsk_theme_hdl2buf(pui->bmp[index]);
    x0 = record_res_bmp_coor[index].x;
    y0 = record_res_bmp_coor[index].y;
    x1 = x0 + GUI_BMP_GetXSize(pbmp);
    y1 = y0 + GUI_BMP_GetYSize(pbmp);
    GUI_ClearRect(x0, y0, x1, y1);
    dsk_theme_close(pui->bmp[index]);
    pui->bmp[index] = NULL;
}
void clean_record_lang(precord_ui_t pui, __s32 index)
{
    GUI_ClearRectEx(&record_res_lang_rect[index]);
}
void clean_record_str(precord_ui_t pui, const GUI_RECT *rect)
{
    GUI_ClearRectEx(rect);
}

__s32 flush_record_res(precord_ui_t pui)
{
    __u32   i;

    //flushӳ��ͼƬ
    for (i = 0; i < MENU_MAX; i++)
    {
        clean_record_mapping(pui, i);
    }

    for (i = 0; i < MUSIC_MENU_MAX; i++)
    {
        clean_record_music_mapping(pui, i);
    }

    //flushһ��ͼƬ
    for (i = 0; i < MAX_RECORD_BMP_ITEM; i++)
    {
        clean_record_bmp(pui, i);
    }

    //flushһ���ַ�
    for (i = 0; i < MAX_RECORD_LANG_ITEM; i++)
    {
        clean_record_lang(pui, i);
    }

    return EPDK_OK;
}



























