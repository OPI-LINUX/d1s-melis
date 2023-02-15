#include <log.h>
#include "multi_screen_ui.h"


static const __s32  multi_screen_uf_icon[HOME_UNFOCUS_ICON_NUM] =
{
    //�̶�Ӧ��
    ID_HOME_NEW_EXPLORER_UF_BMP,
    ID_HOME_NEW_MOVIE_UF_BMP,
    ID_HOME_NEW_MUSIC_UF_BMP,
    ID_HOME_NEW_PICTURE_UF_BMP,
    ID_HOME_NEW_EBOOK_UF_BMP,
    ID_HOME_NEW_FM_UF_BMP,
    ID_HOME_NEW_RECORD_UF_BMP,
    ID_HOME_NEW_SETTING_UF_BMP,
};
static const __s32  multi_screen_f_icon[HOME_FOCUS_ICON_NUM] =
{
    //�̶�Ӧ��ѡ��
    ID_HOME_NEW_EXPLORER_FC_BMP,
    ID_HOME_NEW_MOVIE_FC_BMP,
    ID_HOME_NEW_MUSIC_FC_BMP,
    ID_HOME_NEW_PICTURE_FC_BMP,
    ID_HOME_NEW_EBOOK_FC_BMP,
    ID_HOME_NEW_FM_FC_BMP,
    ID_HOME_NEW_RECORD_FC_BMP,
    ID_HOME_NEW_SETTING_FC_BMP
};

static const int32_t  multi_screen_string[MULTI_SCREEN_STRING_MAX] =
{
    STRING_HOME_EXPLORER,
    STRING_HOME_MOVIE,
    STRING_HOME_MUSIC,
    STRING_HOME_PHOTO,
    STRING_HOME_EBOOK,
    STRING_HOME_FM,
    STRING_HOME_RECORD,
    STRING_HOME_SETTING
};

int32_t init_multi_screen_res(pmulti_screen_ui_t pui, __s16 current_focus)
{
    uint32_t   i;

    __inf("enter %s\r\n", __FUNCTION__);

    //��ʼ��unfocus ICON ��Դ
    for (i = HOME_UNFOCUS_ICON_START; i < HOME_UNFOCUS_ICON_NUM; i++)
    {
        if (multi_screen_uf_icon[i])
        {
            pui->bmp_uf[i] = dsk_theme_open(multi_screen_uf_icon[i]);

            if (pui->bmp_uf[i] == NULL)
            {
                __wrn("init_multi_screen_res() index:%d open fail\n", i);
            }
        }
        else
        {
            pui->bmp_uf[i] = NULL;
        }
    }

    /*focus icon*/
    pui->bmp_f[current_focus] = dsk_theme_open(multi_screen_f_icon[current_focus]);
/*
    if (pui->bmp_f[i] == NULL)
    {
        __wrn("init_multi_screen_res() index:%d open fail\n", i);
    }
*/
    //��ʼ��String ��Դ
    for (i = 0; i < MULTI_SCREEN_STRING_MAX; i++)
    {
        dsk_langres_get_menu_text(multi_screen_string[i], pui->lang[i], GUI_NAME_MAX);
    }

    return EPDK_OK;
}
__s32 uninit_multi_screen_res(pmulti_screen_ui_t pui)
{
    __u32   i;

    //�ͷ�ICON ��Դ
    for (i = HOME_UNFOCUS_ICON_START; i < HOME_UNFOCUS_ICON_NUM; i++)
    {
        if (pui->bmp_uf[i])
        {
            __inf("i = %d\n", i);
            dsk_theme_close(pui->bmp_uf[i]);
            pui->bmp_uf[i] = NULL;
        }
    }

    for (i = HOME_FOCUS_ICON_START; i < HOME_FOCUS_ICON_NUM; i++)
    {
        if (pui->bmp_f[i])
        {
            __log("i = %d\n", i);
            dsk_theme_close(pui->bmp_f[i]);
            pui->bmp_f[i] = NULL;
        }
    }

    //�ͷ�string ��Դ
    for (i = 0; i < MULTI_SCREEN_STRING_MAX; i++)
    {
        pui->lang[i][0] = '\0';
    }

    return EPDK_OK;
}



void draw_multi_screen_icon_coor(pmulti_screen_ui_t pui, __s32 index, __s32 X, __s32 Y)
{
    void *pbmp;
#if 1

    if (pui->bmp_uf[index] != NULL)
    {
        pbmp = dsk_theme_hdl2buf(pui->bmp_uf[index]);
        GUI_BMP_Draw(pbmp, X, Y);
    }

#else
    __u8 *pack = pui->pack;
    pbmp = pack + (*(__u32 *)(pack + 16 + multi_screen_icon[index] * 4));
    GUI_BMP_Draw(pbmp, X, Y);
#endif
}

void draw_multi_screen_icon_coor_rect(pmulti_screen_ui_t pui, __s32 index, __s32 bIs_focus, GUI_RECT *rect)
{
    if (-1 == rect->x0)
    {
        __wrn("-1 == rect->x0\n");
        return;
    }

    if (bIs_focus == EPDK_TRUE)
    {
        if (pui->bmp_f[index] == NULL)
        {
            pui->bmp_f[index] = dsk_theme_open(multi_screen_f_icon[index]);
        }

        GUI_ClearRectEx(rect);
        GUI_BMP_Draw(dsk_theme_hdl2buf(pui->bmp_f[index]), rect->x0, rect->y0);
    }
    else
    {
        if (pui->bmp_uf[index] == NULL)
        {
            pui->bmp_uf[index] = dsk_theme_open(multi_screen_uf_icon[index]);
        }

        GUI_ClearRectEx(rect);
        GUI_BMP_Draw(dsk_theme_hdl2buf(pui->bmp_uf[index]), rect->x0, rect->y0);
    }
}


void draw_multi_screen_string_center(pmulti_screen_ui_t pui, __s32 index, __s32 fnt_color, GUI_RECT *rect)
{
    if (pui->lang[index][0] != '\0')
    {
        GUI_SetColor(fnt_color);
        GUI_DispStringInRect(pui->lang[index], rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
    }
}


