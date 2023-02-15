/*
*********************************************************************************************************
*                                                   ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              record app sample
*
*                                   (c) Copyright 2006-2009, China
*                                            All Rights Reserved
*
* File    : mbook_decode.c
* By      : lyn
* Version : V1.00
*********************************************************************************************************
*/
#include <log.h>
#include <kapi.h>
#include "mbook_private.h"

extern void *file_core;
typedef     void   *H_BMK;
#define     MAX_MARK_ITEM   10//..
#define     MAX_CONTEXT     100
#define     BOOK_MARK_PATH      "e:\\book_mark\\"//..
typedef     void   *H_BMK;//..

void *My_Malloc(void *heap, __u32 size);
void  My_Mfree(void *heap, void *f);
void *My_Palloc(__u32 npage, __u32 mode);
void  My_Pfree(void *mblk, __u32 npage);

typedef struct tag_ITEM_CONTEXT
{
    __s32           number;
    __s32           start;
    char            context[MAX_CONTEXT];
    void           *data;
} __item_context_t;

typedef struct tag_MARK_ITEM
{
    char                    file_path[1024];
    __epdk_charset_enm_e    file_charset;

    __item_context_t        book_mark[MAX_MARK_ITEM];
} __mark_item_t;

typedef struct tag_BOOK_MARK
{
    ES_FILE        *file_hdle;              // �ļ����

    __u32           mark_size;              // ��ǩ�Ĵ�С
    __mark_item_t   mark_item;              // ��ǩ�����ݽṹ
    __u32           mark_state;             // ��ǩ�Ĵ�״̬
    __u32           mark_offset;            // ��ǩ���ļ�ƫ��
} __book_mark_t; //..


/*
**********************************************************************************************************************
*                                               __decode_get_charset
*
* Description: ��ȡ��ǰ�ļ������ʽ
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_get_charset(__mbook_decode_t *hdle)
{
    char                buff[3];
    __mbook_decode_t    *p_decode;
    p_decode = hdle;
    eLIBs_fseek(p_decode->file_hdle, 0, SEEK_SET);
    eLIBs_fread(buff, 3, sizeof(char), p_decode->file_hdle);

    if ((buff[0] == 0xff) && (buff[1] == 0xfe))
    {
        p_decode->charset = EPDK_CHARSET_ENM_UTF16LE;
    }
    else if ((buff[0] == 0xfe) && (buff[1] == 0xff))
    {
        p_decode->charset = EPDK_CHARSET_ENM_UTF16BE;
    }
    else if ((buff[0] == 0xef) && (buff[1] == 0xbb) && (buff[2] == 0xbf))
    {
        p_decode->charset = EPDK_CHARSET_ENM_UTF8;
    }
    else
    {
        p_decode->charset = p_decode->default_charset;
    }

    __wrn("p_decode->charset=%d\n", p_decode->charset);
}

/*
**********************************************************************************************************************
*                                               MBOOK_TTS_court_page
*
* Description: ��ȡ��ǰҳ�ļ�Ϊ������������,Ϊ��
*
* Arguments  :
*           @   hdle    :������
*           @   tts_curt_page_data: �����ָ��ǰҳ�ļ�����
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
void MBOOK_TTS_court_page(H_DECODE hdle, char  **tts_curt_page_data, __s32 *char_num)
{
    __u32               size;
    __one_page_t        tmp_page;
    __mbook_decode_t    *p_decode;
    p_decode = hdle;
    __msg(" --------p_decode->file_hdle=%d------p_decode->current_page  = %d\n", p_decode->file_hdle, p_decode->current_page);
    // ���ļ��ж�ȡ��ǰҳ����������
    MBOOK_Analysis_GetInfo(p_decode->analysis_hdle, p_decode->current_page, &tmp_page);
    size = tmp_page.page_end - tmp_page.page_start;
    __msg("--------ҳbuff�Ĵ�С��%d, page_start = %d,page_end = %d \n", size, tmp_page.page_start, tmp_page.page_end);
    eLIBs_fseek(p_decode->file_hdle, tmp_page.page_start, SEEK_SET);
    eLIBs_fread(p_decode->curt_page_data, 1, size, p_decode->file_hdle);
    *(p_decode->curt_page_data + size) = 0xff;
    *(p_decode->curt_page_data + size + 1) = 0xfe;
    *tts_curt_page_data = p_decode->curt_page_data;
    *char_num = size;
    //  tts_page_data=p_decode->curt_page_data;
}




/*
**********************************************************************************************************************
*                                               __decode_read_page
*
* Description: ��ȡһҳ���ݵ�����BUF�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_read_page(__mbook_decode_t *hdle)
{
    __u32               size;
    __u32               read_len;
    __one_page_t        tmp_page;
    __mbook_decode_t    *p_decode;
    p_decode = hdle;
    // ���ļ��ж�ȡ����
    MBOOK_Analysis_GetInfo(p_decode->analysis_hdle, p_decode->current_page + p_decode->decode_cnt - DECODE_MBOOK_NUM / 2, &tmp_page);
    __msg("start = %d, end = %d, page = %d \n", tmp_page.page_start, tmp_page.page_end, tmp_page.page_no);
    size = tmp_page.page_end - tmp_page.page_start;
    __msg("size = %d\r\n", size);
    eLIBs_fseek(p_decode->file_hdle, tmp_page.page_start, SEEK_SET);
    read_len = eLIBs_fread(p_decode->page_data, 1, size, p_decode->file_hdle);
    __msg("read_len:%d p_decode->page_data = %s\r\n", read_len, p_decode->page_data);
    *(p_decode->page_data + size) = 0xff;
    *(p_decode->page_data + size + 1) = 0xfe;

    // ����ȡ������д����ͼ����
    if (p_decode->config.show_rotate == 0)
    {
        MBOOK_Layer_Rotate(p_decode->page[p_decode->decode_cnt].page_lyr, p_decode->config.show_width,
                           p_decode->config.show_height - p_decode->config.bottom_width, 0);
    }
    else
    {
        MBOOK_Layer_Rotate(p_decode->page[p_decode->decode_cnt].page_lyr,
                           p_decode->config.show_width - p_decode->config.bottom_width, p_decode->config.show_height, 1);
    }

    p_decode->page[p_decode->decode_cnt].page_sta = 1;
    p_decode->page[p_decode->decode_cnt].page_no = p_decode->current_page + p_decode->decode_cnt - DECODE_MBOOK_NUM / 2;
    MBOOK_Show_Page(p_decode->show_hdle, p_decode->page[p_decode->decode_cnt].page_lyr, p_decode->page_data);
}

/*
**********************************************************************************************************************
*                                               __decode_show_page
*
* Description: ��ȡһҳ���ݵ�����BUF�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_show_page(__mbook_decode_t *hdle)
{
    __s32               i, tmp;
    __u8               *pdes = NULL;
    __u8               *psrc1 = NULL;
    __u8               *psrc2 = NULL;
    __u8                bpp = 0;
    FB                  fb_des;
    FB                  fb_src1;
    FB                  fb_src2;
    __mbook_decode_t    *p_decode = NULL;
    __wrn("Enter %s\r\n", __FUNCTION__);
    p_decode = hdle;
    eLIBs_memset(&fb_des, 0x00, sizeof(FB));
    eLIBs_memset(&fb_src1, 0x00, sizeof(FB));
    eLIBs_memset(&fb_src2, 0x00, sizeof(FB));
    GUI_LyrWinGetFB(p_decode->page[DECODE_MBOOK_NUM / 2].page_lyr, &fb_src1);
    GUI_LyrWinGetFB(p_decode->page[DECODE_MBOOK_NUM / 2 + 1].page_lyr, &fb_src2);
    GUI_LyrWinGetFB(p_decode->show_lyr, &fb_des);

    switch (fb_des.fmt.fmt.rgb.pixelfmt)
    {
        case PIXEL_COLOR_ARGB8888:
        {
            bpp = 32 >> 3;
        }
        break;

        default:
        {
            bpp = 8 >> 3;
        }
        break;
    }

    if (p_decode->config.show_rotate == 0)
    {
        eLIBs_memcpy(fb_des.addr[0], (void *)((unsigned long)fb_src1.addr[0] + p_decode->move_height * fb_des.size.width * bpp),
                     fb_des.size.width * (fb_des.size.height - p_decode->move_height) * bpp);
        eLIBs_memcpy((void *)((unsigned long)fb_des.addr[0] + (fb_des.size.height - p_decode->move_height) * fb_des.size.width * bpp), fb_src2.addr[0],
                     fb_des.size.width * p_decode->move_height * bpp);
    }
    else
    {
        tmp     = (fb_des.size.width - p_decode->move_height) * bpp;
        pdes    = fb_des.addr[0];
        psrc1   = fb_src1.addr[0];
        psrc2   = fb_src2.addr[0];
        psrc1   = psrc1 + p_decode->move_height * bpp;

        for (i = 0; i < fb_des.size.height; i++)
        {
            eLIBs_memcpy(pdes, psrc1, tmp);
            pdes    = pdes + tmp;
            psrc1   = psrc1 + fb_des.size.width * bpp;
            eLIBs_memcpy(pdes, psrc2, p_decode->move_height * bpp);
            pdes    = pdes + p_decode->move_height * bpp;
            psrc2   = psrc2 + fb_des.size.width * bpp;
        }
    }
}

/*
**********************************************************************************************************************
*                                               __decode_brush_page
*
* Description: ˢ��ҳ�滺�������Ϣ
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_brush_page(__mbook_decode_t *hdle, __decode_mode_e mode)
{
    __u32               i;
    __page_cache_t      tmp_page;
    __mbook_decode_t    *p_decode;
    p_decode = hdle;

    if (mode == DECODE_PREV)
    {
        tmp_page.page_no    = p_decode->page[DECODE_MBOOK_NUM - 1].page_no;
        tmp_page.page_lyr   = p_decode->page[DECODE_MBOOK_NUM - 1].page_lyr;

        for (i = DECODE_MBOOK_NUM - 1; i > 0; i--)
        {
            p_decode->page[i].page_sta  = p_decode->page[i - 1].page_sta;
            p_decode->page[i].page_no   = p_decode->page[i - 1].page_no;
            p_decode->page[i].page_lyr  = p_decode->page[i - 1].page_lyr;
        }

        p_decode->page[0].page_sta  = 0;
        p_decode->page[0].page_no   = tmp_page.page_no;
        p_decode->page[0].page_lyr  = tmp_page.page_lyr;
    }
    else if (mode == DECODE_NEXT)
    {
        tmp_page.page_no    = p_decode->page[0].page_no;
        tmp_page.page_lyr   = p_decode->page[0].page_lyr;

        for (i = 0; i < DECODE_MBOOK_NUM - 1; i++)
        {
            p_decode->page[i].page_sta  = p_decode->page[i + 1].page_sta;
            p_decode->page[i].page_no   = p_decode->page[i + 1].page_no;
            p_decode->page[i].page_lyr  = p_decode->page[i + 1].page_lyr;
        }

        p_decode->page[DECODE_MBOOK_NUM - 1].page_sta   = 0;
        p_decode->page[DECODE_MBOOK_NUM - 1].page_no    = tmp_page.page_no;
        p_decode->page[DECODE_MBOOK_NUM - 1].page_lyr   = tmp_page.page_lyr;
    }
    else if (mode == DECODE_CURR)
    {
        for (i = 0; i < DECODE_MBOOK_NUM; i++)
        {
            p_decode->page[i].page_sta = 0;
        }
    }
}

/*
**********************************************************************************************************************
*                                               __decode_get_page
*
* Description: ���һ���ѽ�õ�ҳ��
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_get_page(__mbook_decode_t *hdle, __decode_mode_e mode)
{
    __u8                err;
    __mbook_decode_t    *p_decode;
    p_decode = hdle;
    // ��ס�����ź���decode_sem��
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);

    // ˢ��ҳ�滺���
    if (mode == DECODE_CURR)
    {
        __decode_brush_page(p_decode, DECODE_CURR);
    }
    else if (mode == DECODE_PREV)
    {
        p_decode->current_page--;
        __decode_brush_page(p_decode, DECODE_PREV);
    }
    else if (mode == DECODE_NEXT)
    {
        p_decode->current_page++;
        __decode_brush_page(p_decode, DECODE_NEXT);
    }

    // �жϵ�ǰҳ�����Ƿ���ɣ�δ��������̽⵱ǰҳ��
    p_decode->decode_cnt = DECODE_MBOOK_NUM / 2;

    if ((p_decode->page[p_decode->decode_cnt].page_sta == 0)
        && ((p_decode->current_page + p_decode->decode_cnt - DECODE_MBOOK_NUM / 2) >= 1)
        && ((p_decode->current_page + p_decode->decode_cnt - DECODE_MBOOK_NUM / 2) <= p_decode->total_page))
    {
        //__here__;
        __decode_read_page(p_decode);
    }

    p_decode->decode_sta = mode;
    // �ͷŽ����ź���decode_sem��
    esKRNL_SemPost(p_decode->decode_sem);
}

/*
**********************************************************************************************************************
*                                               __decode_get_next_page
*
* Description: ��õ�ǰҳ����һҳ��
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_get_next_page(__mbook_decode_t *hdle)
{
    __u8                err;
    __mbook_decode_t    *p_decode;
    p_decode = hdle;
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    p_decode->decode_cnt = DECODE_MBOOK_NUM / 2 + 1;

    if ((p_decode->page[p_decode->decode_cnt].page_sta == 0)
        && ((p_decode->current_page + p_decode->decode_cnt - DECODE_MBOOK_NUM / 2) > 0))
    {
        __decode_read_page(p_decode);
    }

    esKRNL_SemPost(p_decode->decode_sem);
}

/*
**********************************************************************************************************************
*                                               __decode_get_prev_page
*
* Description: ��õ�ǰҳ����һҳ��
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_get_prev_page(__mbook_decode_t *hdle)
{
    __u8                err;
    __mbook_decode_t    *p_decode;
    p_decode = hdle;
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    p_decode->decode_cnt = DECODE_MBOOK_NUM / 2 - 1;

    if ((p_decode->page[p_decode->decode_cnt].page_sta == 0)
        && ((p_decode->current_page + p_decode->decode_cnt - DECODE_MBOOK_NUM / 2) > 0))
    {
        __decode_read_page(p_decode);
    }

    esKRNL_SemPost(p_decode->decode_sem);
}

/*
**********************************************************************************************************************
*                                               __decode_move_down
*
* Description: ҳ�����д��ͼ���߳�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_move_down(__mbook_decode_t *hdle, __s32 step)
{
    __s32                   tmp_h;
    __mbook_decode_t       *p_decode;
    p_decode = hdle;
    // ��ȡ��ǰ��ʾҳ����Ļ������ʼY��
    tmp_h   = MBOOK_Layer_GetSrcH(p_decode->show_lyr);

    if ((p_decode->current_page >= p_decode->total_page) && (p_decode->move_state == 0))
    {
        return;
    }

    p_decode->move_state = 1;

    if ((p_decode->move_height + step) >= tmp_h)
    {
        // ��ҳ
        __decode_get_page(p_decode, DECODE_NEXT);

        if (p_decode->current_page < p_decode->total_page)
        {
            __decode_get_next_page(p_decode);
            p_decode->move_height = p_decode->move_height + step - tmp_h;
            __decode_show_page(p_decode);
        }
        else
        {
            p_decode->move_height = 0;
            p_decode->move_state = 0;
            __decode_show_page(p_decode);
        }
    }
    else
    {
        p_decode->move_height = p_decode->move_height + step;
        __decode_show_page(p_decode);
    }
}

/*
**********************************************************************************************************************
*                                               __decode_move_up
*
* Description: ҳ�����д��ͼ���߳�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_move_up(__mbook_decode_t *hdle, __s32 step)
{
    __s32                   tmp_h;
    __mbook_decode_t       *p_decode;
    p_decode = hdle;
    // ��ȡ��ǰ��ʾҳ����Ļ������ʼY��
    tmp_h   = MBOOK_Layer_GetSrcH(p_decode->show_lyr);

    if ((p_decode->current_page <= 1) && (p_decode->move_state == 0))
    {
        return;
    }

    p_decode->move_state = -1;

    if ((p_decode->move_height - step) <= 0)
    {
        // ��ҳ
        if (p_decode->current_page > 1)
        {
            __decode_get_page(p_decode, DECODE_PREV);
            p_decode->move_height = p_decode->move_height - step + tmp_h;
            __decode_show_page(p_decode);
        }
        else
        {
            p_decode->move_height = 0;
            p_decode->move_state = 0;
            __decode_show_page(p_decode);
        }
    }
    else
    {
        p_decode->move_height = p_decode->move_height - step;
        __decode_show_page(p_decode);
    }
}

/*
**********************************************************************************************************************
*                                               __decode_task
*
* Description: ҳ�����д��ͼ���߳�
*
* Arguments  :
*
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __decode_task(void *p_arg)
{
    __u8                err;
    __mbook_decode_t    *p_decode = NULL;
    p_decode = (__mbook_decode_t *)p_arg;

    while (1)
    {
        if (esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {
            goto EXIT_DEC_TASK;
        }

        esKRNL_TimeDly(5);
        esKRNL_SemPend(p_decode->decode_sem, 0, &err);

        if (p_decode->decode_sta != DECODE_NULL)
        {
            if ((p_decode->page[p_decode->decode_cnt].page_sta == 0)
                && ((__s32)(p_decode->current_page + p_decode->decode_cnt - DECODE_MBOOK_NUM / 2) >= 1)
                && ((__s32)(p_decode->current_page + p_decode->decode_cnt - DECODE_MBOOK_NUM / 2) <= p_decode->total_page))
            {
                __decode_read_page(p_decode);
            }

            p_decode->decode_cnt++;

            if (p_decode->decode_cnt == DECODE_MBOOK_NUM)
            {
                p_decode->decode_cnt = 0;
            }
        }

        esKRNL_SemPost(p_decode->decode_sem);
    }

EXIT_DEC_TASK:
    esKRNL_TDel(EXEC_prioself);
    return;
}

/*
************************************************************************************************************************
*                                       __decode_show_config
*
*Description: ���û���ģ����ز���
*
*Arguments  : hdle�� �������
*
*
*Return     : NULL
*
************************************************************************************************************************
*/
static void __decode_show_config(__mbook_decode_t *hdle)
{
    __show_config_t         show_cfg;
    __mbook_decode_t       *p_decode;
    __decode_config_t      *p_config;
    p_decode = hdle;
    p_config = &p_decode->config;

    // �������ݶ�дģ��
    if (p_config->show_rotate == 0)
    {
        show_cfg.show_start = p_config->border_width;
        show_cfg.show_width = p_config->show_width - 2 * p_config->border_width + p_config->row_space - p_config->scroll_width;
        show_cfg.page_row   = (p_config->show_height - p_config->bottom_width) / (p_config->font_size + p_config->row_space);
        show_cfg.row_height = p_config->font_size + p_config->row_space;
        show_cfg.col_width  = p_config->font_size + p_config->col_space;
        show_cfg.font_color = p_config->font_color;
        show_cfg.back_color = p_config->back_color;
        show_cfg.char_font  = p_decode->char_font;
        show_cfg.char_table = p_decode->char_table;
        show_cfg.charset    = p_decode->charset;
    }
    else
    {
        show_cfg.show_start = p_config->border_width;
        show_cfg.show_width = p_config->show_height - 2 * p_config->border_width + p_config->row_space - p_config->scroll_width;
        show_cfg.page_row   = (p_config->show_width - p_config->bottom_width) / (p_config->font_size + p_config->row_space);
        show_cfg.row_height = p_config->font_size + p_config->row_space;
        show_cfg.col_width  = p_config->font_size + p_config->col_space;
        show_cfg.font_color = p_config->font_color;
        show_cfg.back_color = p_config->back_color;
        show_cfg.char_font  = p_decode->char_font;
        show_cfg.char_table = p_decode->char_table;
        show_cfg.charset    = p_decode->charset;
    }

    MBOOK_Show_Config(p_decode->show_hdle, &show_cfg);
}

/*
************************************************************************************************************************
*                                       __decode_analysis_config
*
*Description: ���ý���ģ����ز���
*
*Arguments  : hdle�� �������
*
*
*Return     : NULL
*
************************************************************************************************************************
*/
static void __decode_analysis_config(__mbook_decode_t *hdle)
{
    __one_page_t            tmp_page;
    __analysis_config_t     analysis_cfg;
    __mbook_decode_t       *p_decode;
    __decode_config_t      *p_config;
    p_decode = hdle;
    p_config = &p_decode->config;
    // ��ȡ��ǰҳ����Ϣ
    MBOOK_Analysis_GetInfo(p_decode->analysis_hdle, p_decode->current_page, &tmp_page);

    // �����ļ�����ģ��
    if (p_config->show_rotate == 0)
    {
        analysis_cfg.show_width = p_config->show_width - 2 * p_config->border_width + p_config->row_space;
        analysis_cfg.page_row   = (p_config->show_height - p_config->bottom_width) / (p_config->font_size + p_config->row_space);
        analysis_cfg.col_width  = p_config->font_size + p_config->col_space;
        analysis_cfg.char_table = p_decode->char_table;
        analysis_cfg.charset    = p_decode->charset;
    }
    else
    {
        analysis_cfg.show_width = p_config->show_height - 2 * p_config->border_width + p_config->row_space;
        analysis_cfg.page_row   = (p_config->show_width - p_config->bottom_width) / (p_config->font_size + p_config->row_space);
        analysis_cfg.col_width  = p_config->font_size + p_config->col_space;
        analysis_cfg.char_table = p_decode->char_table;
        analysis_cfg.charset    = p_decode->charset;
    }

    MBOOK_Analysis_Config(p_decode->analysis_hdle, &analysis_cfg);
    // �����ļ�����
    MBOOK_Analysis_Work(p_decode->analysis_hdle);
    // ��ȡ��ҳ��
    p_decode->total_page = MBOOK_Analysis_GetTotalPage(p_decode->analysis_hdle);
    // ��λ��ǰҳ
    p_decode->current_page = MBOOK_Analysis_GetPage(p_decode->analysis_hdle, tmp_page.page_start);
}


__s32   MBOOK_Show_LastPage(H_DECODE hdle, __u32 page_no)
{
    __u8                    err;
    __mbook_decode_t        *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if ((page_no > p_decode->total_page) || (page_no < 1))
    {
        return p_decode->current_page;
    }

    //  if(p_decode->current_page == page_no)
    //  {
    //      return p_decode->current_page;
    //  }
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    p_decode->current_page = page_no;
    esKRNL_SemPost(p_decode->decode_sem);
    p_decode->move_height = 0;
    p_decode->move_state = 0;
    __decode_get_page(p_decode, DECODE_CURR);
    __decode_show_page(p_decode);
    __wrn("p_decode->show_lyr=%d\n", p_decode->show_lyr);
    MBOOK_Layer_OpenShow(p_decode->show_lyr);
    GUI_LyrWinSetTop(p_decode->show_lyr);
    return p_decode->current_page;
}

__s32  MBOOK_Decode_get_filename(char *filename)
{
    HRATNPL             h_rat_npl;
    __s32               npl_index;
    h_rat_npl = rat_npl_open(RAT_MEDIA_TYPE_EBOOK);
    npl_index = rat_npl_get_cur(h_rat_npl);
    rat_npl_index2file(h_rat_npl, npl_index, filename);
    __msg("------------filename=%s\n----------", filename);
    rat_npl_close(h_rat_npl);
    filename = eLIBs_strchrlast(filename, '\\') + 1;
    __msg("------------filename=%s\n----------", filename);
    return EPDK_OK;
}

__epdk_charset_enm_e EBookGetSysDefaultCharset(void)
{
    __epdk_language_enm_e e_lang;
    __epdk_charset_enm_e e_charset;
    reg_system_para_t *para;
    para = (reg_system_para_t *)dsk_reg_get_para_by_app(REG_APP_SYSTEM);

    if (!para)
    {
        return EPDK_CHARSET_ENM_GBK;
    }

    e_lang = para->language;

    if (EPDK_LANGUAGE_ENM_CHINESES == e_lang)
    {
        e_charset = EPDK_CHARSET_ENM_GBK;
    }
    else if (EPDK_LANGUAGE_ENM_CHINESET == e_lang)
    {
        e_charset = EPDK_CHARSET_ENM_BIG5;
    }
    else
    {
        e_charset = EPDK_CHARSET_ENM_GBK;
    }

    return e_charset;
}


/*
************************************************************************************************************************
*                                       MBOOK_Decode_Init
*
*Description: ��ʼ��ҳ�����ģ��
*
*Arguments  : filename���ļ�ȫ·����
*
*
*Return     : H_DECODE: ���ؽ��������� err :0 �����޴���1�����ļ������� 2������������
*
************************************************************************************************************************
*/
H_DECODE   MBOOK_Decode_Init(char *filename, __u8 *err)
{
    __mbook_decode_t    *p_decode = NULL;
    *err = 0;
    p_decode = (__mbook_decode_t *)My_Malloc(0, sizeof(__mbook_decode_t));  // �����ڴ�

    if (p_decode == 0)
    {
        *err = 2;
        __err(" anole core malloc error \n");
        return NULL;
    }

    eLIBs_memset(p_decode, 0, sizeof(__mbook_decode_t));

    if (filename == NULL)
    {
        __s32       npl_index;
        HRATNPL     h_rat_npl;
        h_rat_npl = rat_npl_open(RAT_MEDIA_TYPE_EBOOK);
        npl_index = rat_npl_get_cur(h_rat_npl);
        rat_npl_index2file(h_rat_npl, npl_index, p_decode->file_path);
        rat_npl_close(h_rat_npl);
    }
    else
    {
        eLIBs_memcpy(p_decode->file_path, filename, MAX_FILE_NAME);
    }

    __wrn("file_path:%s\r\n", p_decode->file_path);
    p_decode->file_hdle = eLIBs_fopen(p_decode->file_path, "rb");

    if (p_decode->file_hdle == NULL)
    {
        *err = 1;
        __wrn("eLIBs_fopen fail p_decode->file_hdle == NULL\r\n");
        goto INIT_ERROR_1;
    }

    p_decode->file_size = eLIBs_GetFileSize(p_decode->file_path);
    __wrn("file_size:%d\r\n", p_decode->file_size);
    p_decode->analysis_hdle = MBOOK_Analysis_Init(p_decode->file_hdle);

    if (p_decode->analysis_hdle == NULL)
    {
        *err = 2;
        __wrn("MBOOK_Analysis_Init fail p_decode->analysis_hdle == NULL\r\n");
        goto INIT_ERROR_2;
    }

    p_decode->show_hdle = MBOOK_Show_Init();

    if (p_decode->show_hdle == NULL)
    {
        *err = 2;
        __wrn("MBOOK_Show_Init fail p_decode->show_hdle == NULL\r\n");
        goto INIT_ERROR_3;
    }

    p_decode->data_len = 15;
    p_decode->page_data = (void *)My_Palloc(p_decode->data_len, 0);     // �����ڴ�
    p_decode->curt_page_data = (void *)esMEMS_Palloc(p_decode->data_len, 0);  //���뵱ǰҳ�����ڴ棬Ϊtts����

    if (p_decode->page_data == 0 || p_decode->curt_page_data == NULL)
    {
        *err = 2;
        __wrn("My_Palloc Create fail\r\n");
        goto INIT_ERROR_4;
    }

    p_decode->total_page        = 0;
    p_decode->current_page      = 0;
    p_decode->move_height       = 0;
    p_decode->move_state        = 0;
    p_decode->charset           = EBookGetSysDefaultCharset();
    p_decode->default_charset   = EBookGetSysDefaultCharset();
    p_decode->decode_cnt    = DECODE_MBOOK_NUM / 2;
    p_decode->decode_sta    = DECODE_NULL;  // ����״̬λ
    p_decode->decode_sem    = esKRNL_SemCreate(1);  // ��������ź���
    p_decode->decode_tsk    = esKRNL_TCreate(__decode_task, (void *)p_decode, 0x2000, KRNL_priolevel5);     // ����Ԥ���߳�
    //__here__;
    return p_decode;
INIT_ERROR_4:

    if (p_decode->show_hdle)
    {
        MBOOK_Show_Uninit(p_decode->show_hdle);
        p_decode->show_hdle = NULL;
    }

INIT_ERROR_3:

    if (p_decode->analysis_hdle)
    {
        MBOOK_Analysis_Uninit(p_decode->analysis_hdle);
        p_decode->analysis_hdle = NULL;
    }

INIT_ERROR_2:

    if (p_decode->file_hdle)
    {
        eLIBs_fclose(p_decode->file_hdle);
        p_decode->file_hdle = NULL;
    }

INIT_ERROR_1:

    if (p_decode)
    {
        My_Mfree(0, p_decode);
        p_decode = NULL;
    }

    return NULL;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_Config
*
*Description: ����ҳ�����ģ��
*
*Arguments  : hdle: �������
*             config: ����������Ϣָ��
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_Config(H_DECODE hdle, __decode_config_t *config)
{
    __u32                   i;
    __show_config_t         show_cfg;
    __analysis_config_t     analysis_cfg;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        __wrn("%s hdle == NULL\r\n", __FUNCTION__);
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;
    // ��ȡ�ļ������ʽ
    __decode_get_charset(p_decode);
    // ��ȡ�ַ��ֿ���Ϣ
    eLIBs_memcpy(&(p_decode->config), config, sizeof(__decode_config_t));
    p_decode->font_hdle     = MBOOK_Font_Init(config->char_font, config->font_size);
    p_decode->char_font     = MBOOK_Font_GetFont(p_decode->font_hdle);
    p_decode->char_table    = MBOOK_Font_GetTable(p_decode->font_hdle);
    __wrn("config->show_rotate=%d\n", config->show_rotate);

    if (config->show_rotate == 0)
    {
        for (i = 0; i < DECODE_MBOOK_NUM; i++)
        {
            p_decode->page[i].page_sta  = 0;
            p_decode->page[i].page_no   = 0;
            p_decode->page[i].page_lyr  = MBOOK_Layer_CreatLayer(config->show_width, config->show_height - config->bottom_width, 1);    // ������ͼ��

            if (p_decode->page[i].page_lyr == NULL)
            {
                __wrn("MBOOK_Layer_CreatLayer fail...\n");
                goto INIT_ERROR_1;
            }
        }

        // ������ʾͼ��
        p_decode->show_lyr  = MBOOK_Layer_CreatLayer(config->show_width, config->show_height - config->bottom_width, 1);    // ������ʾͼ��
        __wrn("p_decode->show_lyr=%d\n", p_decode->show_lyr);

        if (p_decode->show_lyr == NULL)
        {
            __wrn("MBOOK_Layer_CreatLayer fail...\n");
            goto INIT_ERROR_2;
        }

        // �����ļ�����ģ��
        analysis_cfg.show_width = config->show_width - 2 * config->border_width + config->row_space;
        analysis_cfg.page_row   = (config->show_height - config->bottom_width) / (config->font_size + config->row_space);
        analysis_cfg.col_width  = config->font_size + config->col_space;
        analysis_cfg.char_table = p_decode->char_table;
        analysis_cfg.charset    = p_decode->charset;
        MBOOK_Analysis_Config(p_decode->analysis_hdle, &analysis_cfg);
        // �������ݶ�дģ��
        show_cfg.show_start = config->border_width;
        show_cfg.show_width = config->show_width - 2 * config->border_width + config->row_space - config->scroll_width;
        show_cfg.page_row   = (config->show_height - config->bottom_width) / (config->font_size + config->row_space);
        show_cfg.row_height = config->font_size + config->row_space;
        show_cfg.col_width  = config->font_size + config->col_space;
        show_cfg.font_color = config->font_color;
        show_cfg.back_color = config->back_color;
        show_cfg.char_font  = p_decode->char_font;
        show_cfg.char_table = p_decode->char_table;
        show_cfg.charset    = p_decode->charset;
        MBOOK_Show_Config(p_decode->show_hdle, &show_cfg);
        // �����ļ�����
        MBOOK_Analysis_Work(p_decode->analysis_hdle);
    }
    else
    {
        for (i = 0; i < DECODE_MBOOK_NUM; i++)
        {
            p_decode->page[i].page_sta  = 0;
            p_decode->page[i].page_no   = 0;
            p_decode->page[i].page_lyr  = MBOOK_Layer_CreatLayer(config->show_width - config->bottom_width, config->show_height, 1);    // ������ͼ��

            if (p_decode->page[i].page_lyr == NULL)
            {
                __wrn("MBOOK_Layer_CreatLayer fail...\n");
                goto INIT_ERROR_1;
            }
        }

        p_decode->show_lyr  = MBOOK_Layer_CreatLayer(config->show_width - config->bottom_width, config->show_height, 1);    // ������ʾͼ��

        if (p_decode->show_lyr == NULL)
        {
            __wrn("MBOOK_Layer_CreatLayer fail...\n");
            goto INIT_ERROR_2;
        }

        // �����ļ�����ģ��
        analysis_cfg.show_width = config->show_height - 2 * config->border_width + config->row_space - config->scroll_width;
        analysis_cfg.page_row   = (config->show_width - config->bottom_width) / (config->font_size + config->row_space);
        analysis_cfg.col_width  = config->font_size + config->col_space;
        analysis_cfg.char_table = p_decode->char_table;
        analysis_cfg.charset    = p_decode->charset;
        MBOOK_Analysis_Config(p_decode->analysis_hdle, &analysis_cfg);
        // �������ݶ�дģ��
        show_cfg.show_start = config->border_width;
        show_cfg.show_width = config->show_height - 2 * config->border_width + config->row_space;
        show_cfg.page_row   = (config->show_width - config->bottom_width) / (config->font_size + config->row_space);
        show_cfg.row_height = config->font_size + config->row_space;
        show_cfg.col_width  = config->font_size + config->col_space;
        show_cfg.font_color = config->font_color;
        show_cfg.back_color = config->back_color;
        show_cfg.char_font  = p_decode->char_font;
        show_cfg.char_table = p_decode->char_table;
        show_cfg.charset    = p_decode->charset;
        MBOOK_Show_Config(p_decode->show_hdle, &show_cfg);
        // �����ļ�����
        MBOOK_Analysis_Work(p_decode->analysis_hdle);
    }

    //__here__
    // ��ȡ��ҳ��
    p_decode->current_page  = 1;
    p_decode->total_page  = MBOOK_Analysis_GetTotalPage(p_decode->analysis_hdle);
    //__here__
    return EPDK_OK;
INIT_ERROR_2:
INIT_ERROR_1:

    for (i = 0; i < DECODE_MBOOK_NUM; i++)
    {
        if (p_decode->page[i].page_lyr)
        {
            MBOOK_Layer_DeleteLayer(p_decode->page[i].page_lyr);
            p_decode->page[i].page_lyr = NULL;
        }
    }

    return EPDK_FAIL;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_ShowPage
*
*Description: ��ʾ��ǰҳ��
*
*Arguments  : hdle: �������
*
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_ShowPage(H_DECODE hdle, __u32 offset)
{
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;
    p_decode->current_page = MBOOK_Analysis_GetPage(p_decode->analysis_hdle, offset);
    __decode_get_page(p_decode, DECODE_CURR);
    __decode_show_page(p_decode);
    MBOOK_Layer_OpenShow(p_decode->show_lyr);
    GUI_LyrWinSetTop(p_decode->show_lyr);
    return p_decode->current_page;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_ShowNext
*
*Description: ��ʾ��һҳ��
*
*Arguments  : hdle: �������
*
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_ShowNext(H_DECODE hdle)
{
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        __wrn("%s hdle == NULL\r\n", __FUNCTION__);
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->current_page < p_decode->total_page)
    {
        p_decode->move_height = 0;
        p_decode->move_state = 0;
        __decode_get_page(p_decode, DECODE_NEXT);
        __decode_show_page(p_decode);
    }

    return p_decode->current_page;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_ShowPrev
*
*Description: ��ʾ��һҳ��
*
*Arguments  : hdle: �������
*
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_ShowPrev(H_DECODE hdle)
{
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->current_page > 1)
    {
        p_decode->move_height = 0;
        p_decode->move_state = 0;
        __decode_get_page(p_decode, DECODE_PREV);
        __decode_show_page(p_decode);
    }

    return p_decode->current_page;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_SeekPage
*
*Description: ��ת��ָ��ҳ��
*
*Arguments  : hdle: �������
*             page_no: Ҫ��ʾ��ҳ��
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_SeekPage(H_DECODE hdle, __u32 page_no)
{
    __u8                    err;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if ((page_no > p_decode->total_page) || (page_no < 1))
    {
        return p_decode->current_page;
    }

    if (p_decode->current_page == page_no)
    {
        return p_decode->current_page;
    }

    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    p_decode->current_page = page_no;
    esKRNL_SemPost(p_decode->decode_sem);
    p_decode->move_height = 0;
    p_decode->move_state = 0;
    __decode_get_page(p_decode, DECODE_CURR);
    __decode_show_page(p_decode);
    return p_decode->current_page;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_SeekOffset
*
*Description: ��ת��ָ��ƫ����
*
*Arguments  : hdle: �������
*             offset: �ļ�ƫ����
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_SeekOffset(H_DECODE hdle, __u32 offset)
{
    __u8                    err;
    __u32                   tmp_page;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;
    tmp_page = MBOOK_Analysis_GetPage(p_decode->analysis_hdle, offset);
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    p_decode->current_page = tmp_page;
    esKRNL_SemPost(p_decode->decode_sem);
    p_decode->move_height = 0;
    p_decode->move_state = 0;
    __decode_get_page(p_decode, DECODE_CURR);
    __decode_show_page(p_decode);
    return p_decode->current_page;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_MovePage
*
*Description: ��ת��ָ��ҳ��
*
*Arguments  : hdle: �������
*             x, y: �ƶ���ƫ����
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_MovePage(H_DECODE hdle, __s32 x, __s32 y)
{
    __s32                   tmp_step;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->config.show_rotate == 0)
    {
        tmp_step = y;
    }
    else
    {
        tmp_step = x;
    }

    if ((tmp_step > 200) || ((tmp_step > 0) && (tmp_step < 20)))
    {
        tmp_step = 25;
    }
    else if (((tmp_step > -20) && (tmp_step < 0)) || (tmp_step < -200))
    {
        tmp_step = -25;
    }

    if (tmp_step < 0)
    {
        __decode_move_down(p_decode, 0 - tmp_step);
    }
    else
    {
        __decode_move_up(p_decode, tmp_step);
    }

    return p_decode->current_page;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_RotatePage
*
*Description: ��ת��ǰҳ��
*
*Arguments  : hdle: �������
*
*
*Return     : �ɹ���0 - 0�ȣ�90 - 90��
*             ʧ�ܣ�-1
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_RotatePage(H_DECODE hdle)
{
    __u32                   i;
    __u8                    err;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return -1;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->config.show_rotate == 0)
    {
        p_decode->config.show_rotate = 90;
    }
    else
    {
        p_decode->config.show_rotate = 0;
    }

    // ����Ԥ���߳�
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);

    // ɾ��ͼ��
    for (i = 0; i < DECODE_MBOOK_NUM; i++)
    {
        if (p_decode->page[i].page_lyr)
        {
            MBOOK_Layer_DeleteLayer(p_decode->page[i].page_lyr);
            p_decode->page[i].page_lyr = NULL;
        }
    }

    if (p_decode->show_lyr)
    {
        MBOOK_Layer_DeleteLayer(p_decode->show_lyr);
        p_decode->show_lyr = NULL;
    }

    //����ͼ��
    if (p_decode->config.show_rotate == 0)
    {
        for (i = 0; i < DECODE_MBOOK_NUM; i++)
        {
            p_decode->page[i].page_sta  = 0;
            p_decode->page[i].page_no   = 0;
            p_decode->page[i].page_lyr  = MBOOK_Layer_CreatLayer(p_decode->config.show_width, p_decode->config.show_height - p_decode->config.bottom_width, 1); // ������ͼ��

            if (p_decode->page[i].page_lyr == NULL)
            {
                goto INIT_ERROR_1;
            }
        }

        p_decode->show_lyr  = MBOOK_Layer_CreatLayer(p_decode->config.show_width, p_decode->config.show_height - p_decode->config.bottom_width, 1); // ������ʾͼ��

        if (p_decode->show_lyr == NULL)
        {
            goto INIT_ERROR_2;
        }
    }
    else
    {
        for (i = 0; i < DECODE_MBOOK_NUM; i++)
        {
            p_decode->page[i].page_sta  = 0;
            p_decode->page[i].page_no   = 0;
            p_decode->page[i].page_lyr  = MBOOK_Layer_CreatLayer(p_decode->config.show_width - p_decode->config.bottom_width, p_decode->config.show_height, 1); // ������ͼ��

            if (p_decode->page[i].page_lyr == NULL)
            {
                goto INIT_ERROR_1;
            }
        }

        p_decode->show_lyr  = MBOOK_Layer_CreatLayer(p_decode->config.show_width - p_decode->config.bottom_width, p_decode->config.show_height, 1); // ������ʾͼ��

        if (p_decode->show_lyr == NULL)
        {
            goto INIT_ERROR_2;
        }
    }

    // ���ý���
    __decode_analysis_config(p_decode);
    // ������ʾ
    __decode_brush_page(p_decode, DECODE_CURR);
    __decode_show_config(p_decode);
    // �ָ�Ԥ���߳�
    esKRNL_SemPost(p_decode->decode_sem);
    // ��ȡ��ǰҳ
    p_decode->move_height   = 0;
    p_decode->move_state    = 0;
    __decode_get_page(p_decode, DECODE_CURR);
    __decode_show_page(p_decode);
    MBOOK_Layer_OpenShow(p_decode->show_lyr);
    return p_decode->config.show_rotate;
INIT_ERROR_2:
INIT_ERROR_1:

    for (i = 0; i < DECODE_MBOOK_NUM; i++)
    {
        if (p_decode->page[i].page_lyr)
        {
            MBOOK_Layer_DeleteLayer(p_decode->page[i].page_lyr);
            p_decode->page[i].page_lyr = NULL;
        }
    }

    return -1;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_RotatePage
*
*Description: ��ת��ǰҳ��
*
*Arguments  : hdle: �������
*
*
*Return     : �ɹ���0�ȣ�90��
*             ʧ�ܣ����� EPDK_FAIL
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_GetRotate(H_DECODE hdle)
{
    __mbook_decode_t        *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;
    return p_decode->config.show_rotate;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_GetTotalPage
*
*Description: ��ת��ǰҳ��
*
*Arguments  : hdle: �������
*
*
*Return     : �ɹ����ط���ҳ��
*             ʧ�ܣ����� 0
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_GetTotalPage(H_DECODE hdle)
{
    __mbook_decode_t        *p_decode;

    if (hdle == NULL)
    {
        return 0;
    }

    p_decode = (__mbook_decode_t *)hdle;
    return p_decode->total_page;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_GetCurrentPage
*
*Description: ��ת��ǰҳ��
*
*Arguments  : hdle: �������
*
*
*Return     : �ɹ����طŵ�ǰҳ��
*             ʧ�ܣ����� 0
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_GetCurrentPage(H_DECODE hdle)
{
    __mbook_decode_t        *p_decode;

    if (hdle == NULL)
    {
        return 0;
    }

    p_decode = (__mbook_decode_t *)hdle;
    return p_decode->current_page;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_GetCurrentPage
*
*Description: ��ȡָ��ҳ��ҳ��ƫ��ֵ
*
*Arguments  : hdle: �������
*             page: ָ��ҳ��
*
*Return     : �ɹ���ָ��ҳ��ƫ����
*             ʧ�ܣ�����-1
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_GetPageOffset(H_DECODE hdle, __u32 page_no)
{
    __one_page_t        tmp_page;
    __mbook_decode_t   *p_decode;

    if (hdle == NULL)
    {
        return -1;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if ((page_no > p_decode->total_page) || (page_no < 1))
    {
        return -1;
    }

    MBOOK_Analysis_GetInfo(p_decode->analysis_hdle, page_no, &tmp_page);
    return tmp_page.page_start;
}
/*
************************************************************************************************************************
*                                       MBOOK_Decode_GetPage
*
*Description: ��ȡָ����ƫ��ֵ����ȡ��Ӧ��ҳ��
*
*Arguments  : hdle: �������
*             page_offset: ָ��ҳ��ƫ����
*
*Return     : �ɹ���ָ��ҳ��ƫ��������ҳ��ҳ��
*             ʧ�ܣ�����-1
*
************************************************************************************************************************
*/

//..
__s32   MBOOK_Decode_GetPage(H_DECODE hdle, __u32 page_offset)
{
    __mbook_decode_t   *p_decode;
    __s32   page_no ;

    if (hdle == NULL)
    {
        return -1;
    }

    p_decode = (__mbook_decode_t *)hdle;
    page_no = MBOOK_Analysis_GetPage(p_decode->analysis_hdle, page_offset);
    return page_no ;
}
/*
************************************************************************************************************************
*                                       BOOK_Mark_SetPage
*
*Description: ����һ����ǩ
*
*Arguments  : hdle: �������
*             num ��Ҫ�������ǩ���
*             offset������ļ���ƫ����
*             context: ��ǩ��������Ϣ
*
*Return     : �ɹ���EPDK_OK
*             ʧ�ܣ�EPDK_FAIL
*
************************************************************************************************************************
*/

//..
__s32   BOOK_Mark_SetPage(H_BMK hdle, __u32 num, __u32 offset, char *context)
{
    __book_mark_t      *p_mark;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_mark = (__book_mark_t *)hdle;

    if ((num < 1) || (num >= MAX_MARK_ITEM))
    {
        return EPDK_FAIL;
    }

    p_mark->mark_item.book_mark[num].number    = num;
    p_mark->mark_item.book_mark[num].start     = offset;
    eLIBs_strncpy(p_mark->mark_item.book_mark[num].context, context, MAX_CONTEXT);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       BOOK_Mark_Init
*
*Description: ������ǩģ��
*
*Arguments  : mark_file: Ҫ�򿪵���ǩ�ļ�
*
*
*Return     : �ɹ������ز������
*             ʧ�ܣ�����NULL
*
************************************************************************************************************************
*/

//..
H_BMK   BOOK_Mark_Init(char *mark_file)
{
    char                path[1024];
    ES_DIR             *dir;
    __book_mark_t      *p_mark;
    p_mark = (__book_mark_t *)esMEMS_Malloc(0, sizeof(__book_mark_t));  // �����ڴ�

    if (p_mark == 0)
    {
        __err(" book mark malloc error \n");
        return NULL;
    }

    eLIBs_memset(p_mark, 0, sizeof(__book_mark_t));
    dir = eLIBs_opendir(BOOK_MARK_PATH);

    if (dir == NULL)
    {
        __wrn("%s not exist, Create %s\r\n", BOOK_MARK_PATH, BOOK_MARK_PATH);
        eLIBs_mkdir(BOOK_MARK_PATH);
    }
    else
    {
        __wrn("%s is exist\r\n", BOOK_MARK_PATH);
        eLIBs_closedir(dir);
    }

    eLIBs_strcpy(path, BOOK_MARK_PATH);
    eLIBs_strcat(path, mark_file);
    p_mark->file_hdle = eLIBs_fopen(path, "r");

    if (p_mark->file_hdle == NULL)
    {
        __wrn("%s not exist, Create %s\r\n", path, path);
        p_mark->file_hdle = eLIBs_fopen(path, "wb");
        eLIBs_fclose(p_mark->file_hdle);
        p_mark->file_hdle = NULL;
    }
    else
    {
        __wrn("%s is exist\r\n", path);
        eLIBs_fclose(p_mark->file_hdle);
        p_mark->file_hdle = NULL;
    }

    __wrn("open file %s\r\n", path);
    p_mark->file_hdle = eLIBs_fopen(path, "rb+");
    p_mark->mark_size = sizeof(__mark_item_t);
    return p_mark;
}
/*
************************************************************************************************************************
*                                       BOOK_Mark_Open
*
*Description: ��һ���ļ���ǩ
*
*Arguments  : hdle: �������
*             file��Ҫ�򿪵��ļ�
*
*Return     : �ɹ���EPDK_OK
*             ʧ�ܣ�EPDK_FAIL
*
************************************************************************************************************************
*/

//..
__s32   BOOK_Mark_Open(H_BMK hdle, char *file)
{
    __u32               len;
    __s32               code;
    __book_mark_t      *p_mark;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_mark = (__book_mark_t *)hdle;
    p_mark->mark_state = 0;
    p_mark->mark_offset = 0;

    while (1)
    {
        eLIBs_fseek(p_mark->file_hdle, p_mark->mark_offset, SEEK_SET);
        len = eLIBs_fread(&p_mark->mark_item, 1, p_mark->mark_size, p_mark->file_hdle);

        if (len < p_mark->mark_size)
        {
            break;
        }

        if (0 == eLIBs_strcmp(p_mark->mark_item.file_path, file))
        {
            p_mark->mark_state = 1;
            break;
        }

        p_mark->mark_offset = p_mark->mark_offset + p_mark->mark_size;  //ָ����һ����ǩ
    }

    if (p_mark->mark_state == 1)
    {
        return EPDK_OK;
    }
    else
    {
        eLIBs_strcpy(p_mark->mark_item.file_path, file);
        eLIBs_GetFSCharset(file, (__s32 *)&code);
        p_mark->mark_item.file_charset = (__epdk_charset_enm_e)code;
        eLIBs_memset(p_mark->mark_item.book_mark, 0xff, sizeof(__item_context_t) * MAX_MARK_ITEM);
        return EPDK_FAIL;
    }
}
/*
************************************************************************************************************************
*                                       BOOK_Mark_Close
*
*Description: ��ȡһ��ָ����ǩ��������Ϣ
*
*Arguments  : hdle: �������
*
*
*Return     : �ɹ���EPDK_OK
*             ʧ�ܣ�EPDK_FAIL
*
************************************************************************************************************************
*/
//..

__s32   BOOK_Mark_Close(H_BMK hdle)
{
    __book_mark_t      *p_mark;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_mark = (__book_mark_t *)hdle;
    __msg("p_mark->mark_offset=%d\n", p_mark->mark_offset);
    eLIBs_fseek(p_mark->file_hdle, p_mark->mark_offset, SEEK_SET);
    //__here__;
    eLIBs_fwrite(&p_mark->mark_item, 1, p_mark->mark_size, p_mark->file_hdle);
    //__here__;
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       BOOK_Mark_Uninit
*
*Description: ж����ǩģ��
*
*Arguments  : hdle: �������
*
*
*Return     : �ɹ���EPDK_OK
*             ʧ�ܣ�EPDK_FAIL
*
************************************************************************************************************************
*/
//..
__s32   BOOK_Mark_Uninit(H_BMK hdle)
{
    __book_mark_t      *p_mark;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_mark = (__book_mark_t *)hdle;

    if (p_mark->file_hdle)
    {
        eLIBs_fclose(p_mark->file_hdle);
        p_mark->file_hdle = NULL;
    }

    esMEMS_Mfree(0, p_mark);
    p_mark = NULL;
    return EPDK_OK;
}


/*
************************************************************************************************************************
*                                       BOOK_Mark_GetPageOffset
*
*Description: ��ȡһ��ָ����ǩ���ļ�ƫ����
*
*Arguments  : hdle: �������
*             num ����ǩ���
*
*Return     : �ɹ��������ļ���ƫ����
*             ʧ�ܣ�-1
*
************************************************************************************************************************
*/
//..
__s32   BOOK_Mark_GetPageOffset(H_BMK hdle, __u32 num)
{
    __book_mark_t      *p_mark;

    if (hdle == NULL)
    {
        return -1;
    }

    p_mark = (__book_mark_t *)hdle;

    if ((num < 1) || (num >= MAX_MARK_ITEM))
    {
        return -1;
    }

    return p_mark->mark_item.book_mark[num].start;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_GetFileName
*
*Description: ��ȡ�ļ����ļ���
*
*Arguments  : hdle: �������
*             name: �ļ���
*
*Return     : �ɹ���PDK_OKK
*             ʧ�ܣ�EPDK_FAIL
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_GetFileName(H_DECODE hdle, char *name)
{
    char                   *p;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;
    p = eLIBs_strchrlast(p_decode->file_path, '\\');
    eLIBs_strcpy(name, p + 1);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_GetFileSize
*
*Description: ��ȡ�ļ����ļ���
*
*Arguments  : hdle: �������
*
*
*Return     : �ɹ����ܴ�С
*             ʧ�ܣ�-1
*
************************************************************************************************************************
*/
__s64   MBOOK_Decode_GetFileSize(H_DECODE hdle)
{
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return -1;
    }

    p_decode = (__mbook_decode_t *)hdle;
    return p_decode->file_size;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_GetFilePath
*
*Description: ��ȡ�ļ���ȫ·����
*
*Arguments  : hdle: �������
*             path: ·����
*
*Return     : �ɹ���PDK_OKK
*             ʧ�ܣ�EPDK_FAIL
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_GetFilePath(H_DECODE hdle, char *path)
{
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;
    eLIBs_strcpy(path, p_decode->file_path);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_GetFileContext
*
*Description: ��ȡ��ǰҳ�����Ϣ
*
*Arguments  : hdle: �������
*             context: �ļ�����
*
*Return     : �ɹ���PDK_OKK
*             ʧ�ܣ�EPDK_FAIL
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_GetFileContext(H_DECODE hdle, char *context)
{
    __one_page_t        tmp_page;
    __mbook_decode_t    *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;
    MBOOK_Analysis_GetInfo(p_decode->analysis_hdle, p_decode->current_page, &tmp_page);
    eLIBs_fseek(p_decode->file_hdle, tmp_page.page_start, SEEK_SET);
    eLIBs_fread(context, 1, sizeof(context), p_decode->file_hdle);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_SetRowSpace
*
*Description: �����м��
*
*Arguments  : hdle: �������
*             space: �м��
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_SetRowSpace(H_DECODE hdle, __u32 space)
{
    __u8                    err;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->config.row_space == space)
    {
        return EPDK_OK;
    }

    p_decode->config.row_space = space;
    // ����Ԥ���߳�
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    // ���ý���
    __decode_analysis_config(p_decode);
    // ������ʾ
    __decode_brush_page(p_decode, DECODE_CURR);
    __decode_show_config(p_decode);
    // �ָ�Ԥ���߳�
    esKRNL_SemPost(p_decode->decode_sem);
    // ��ȡ��ǰҳ
    __decode_get_page(p_decode, DECODE_CURR);

    if (p_decode->move_height)
    {
        __decode_get_next_page(p_decode);
    }

    __decode_show_page(p_decode);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_SetColSpace
*
*Description: �����м��
*
*Arguments  : hdle: �������
*             space: �м��
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_SetColSpace(H_DECODE hdle, __u32 space)
{
    __u8                    err;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->config.col_space == space)
    {
        return EPDK_OK;
    }

    p_decode->config.col_space = space;
    // ����Ԥ���߳�
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    // ���ý���
    __decode_analysis_config(p_decode);
    // ������ʾ
    __decode_brush_page(p_decode, DECODE_CURR);
    __decode_show_config(p_decode);
    // �ָ�Ԥ���߳�
    esKRNL_SemPost(p_decode->decode_sem);
    // ��ȡ��ǰҳ
    __decode_get_page(p_decode, DECODE_CURR);

    if (p_decode->move_height)
    {
        __decode_get_next_page(p_decode);
    }

    __decode_show_page(p_decode);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_SetBorderWidth
*
*Description: ���ñ߾�
*
*Arguments  : hdle: �������
*             space: �߾�
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_SetBorderWidth(H_DECODE hdle, __u32 width)
{
    __u8                    err;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->config.border_width == width)
    {
        return EPDK_OK;
    }

    p_decode->config.border_width = width;
    __msg("p_decode->config.border_width = %d \n", p_decode->config.border_width);
    // ����Ԥ���߳�
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    // ���ý���
    __decode_analysis_config(p_decode);
    // ������ʾ
    __decode_show_config(p_decode);
    __decode_brush_page(p_decode, DECODE_CURR);
    // �ָ�Ԥ���߳�
    esKRNL_SemPost(p_decode->decode_sem);
    // ��ȡ��ǰҳ
    __decode_get_page(p_decode, DECODE_CURR);

    if (p_decode->move_height)
    {
        __decode_get_next_page(p_decode);
    }

    __decode_show_page(p_decode);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_SetBackColor
*
*Description: ������ʾ�ı���ɫ
*
*Arguments  : hdle: �������
*             color: ��ɫ
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_SetBackColor(H_DECODE hdle, __u32 color)
{
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->config.back_color == color)
    {
        return EPDK_OK;
    }

    p_decode->config.back_color = color;
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_SetFontColor
*
*Description: ������ʾ���ֵ���ɫ
*
*Arguments  : hdle: �������
*             color: ��ɫ
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_SetFontColor(H_DECODE hdle, __u32 color)
{
    __mbook_decode_t       *p_decode;
    __u8 err;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->config.font_color == color)
    {
        return EPDK_OK;
    }

    p_decode->config.font_color = color;
    // ����Ԥ���߳�
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    // ���ý���
    __decode_analysis_config(p_decode);
    // ������ʾ
    __decode_brush_page(p_decode, DECODE_CURR);
    __decode_show_config(p_decode);
    // �ָ�Ԥ���߳�
    esKRNL_SemPost(p_decode->decode_sem);
    // ��ȡ��ǰҳ
    __decode_get_page(p_decode, DECODE_CURR);
    __decode_show_page(p_decode);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_SetDefaultCharset
*
*Description: ����Ĭ�ϱ����ʽ
*
*Arguments  : hdle: �������
*             charset: Ĭ�ϱ���
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_SetDefaultCharset(H_DECODE hdle, __epdk_charset_enm_e charset)
{
    __u8                    err;
    __mbook_decode_t       *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;
    __wrn("p_decode->default_charset=%d\n", p_decode->default_charset);
    __wrn("charset=%d\n", charset);

    if (p_decode->default_charset == charset)
    {
        return EPDK_OK;
    }

    p_decode->default_charset = charset;

    if ((p_decode->charset == EPDK_CHARSET_ENM_UTF16LE)
        || (p_decode->charset == EPDK_CHARSET_ENM_UTF16BE)
        || (p_decode->charset == EPDK_CHARSET_ENM_UTF8))
    {
        // ��δ֪����
        return EPDK_OK;
    }

    if (p_decode->decode_sta == DECODE_NULL)
    {
        // δ��ʼ����
        return EPDK_OK;
    }

    // ����Ԥ���߳�
    esKRNL_SemPend(p_decode->decode_sem, 0, &err);
    // ��ȡ�ļ������ʽ
    __decode_get_charset(p_decode);
    // ���ý���
    __decode_analysis_config(p_decode);
    // ������ʾ
    __decode_brush_page(p_decode, DECODE_CURR);
    __decode_show_config(p_decode);
    // �ָ�Ԥ���߳�
    esKRNL_SemPost(p_decode->decode_sem);
    // ��ȡ��ǰҳ
    __decode_get_page(p_decode, DECODE_CURR);

    if (p_decode->move_height)
    {
        __decode_get_next_page(p_decode);
    }

    __decode_show_page(p_decode);
    return EPDK_OK;
}

/*
************************************************************************************************************************
*                                       MBOOK_Decode_Uninit
*
*Description: ɾ��ҳ�����ģ��
*
*Arguments  : hdle: �������
*
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   MBOOK_Decode_Uninit(H_DECODE hdle)
{
    __s32                   i;
    __u8                    err;
    __mbook_decode_t        *p_decode;

    if (hdle == NULL)
    {
        return EPDK_FAIL;
    }

    p_decode = (__mbook_decode_t *)hdle;

    if (p_decode->decode_tsk)
    {
        while (1)
        {
            if (esKRNL_TDelReq(p_decode->decode_tsk) == OS_TASK_NOT_EXIST)
            {
                break;
            }

            esKRNL_TimeDly(1);
        }

        p_decode->decode_tsk = 0;
    }

    if (p_decode->decode_sem)
    {
        esKRNL_SemDel(p_decode->decode_sem, OS_DEL_ALWAYS, &err);
        p_decode->decode_sem = NULL;
    }

    if (p_decode->curt_page_data)
    {
        esMEMS_Pfree(p_decode->curt_page_data, p_decode->data_len);
        p_decode->curt_page_data = NULL;
    }

    if (p_decode->page_data)
    {
        My_Pfree(p_decode->page_data, p_decode->data_len);
        p_decode->page_data = NULL;
    }

    for (i = 0; i < DECODE_MBOOK_NUM; i++)
    {
        if (p_decode->page[i].page_lyr)
        {
            MBOOK_Layer_DeleteLayer(p_decode->page[i].page_lyr);
            p_decode->page[i].page_lyr = NULL;
        }
    }

    if (p_decode->show_lyr)
    {
        MBOOK_Layer_DeleteLayer(p_decode->show_lyr);
        p_decode->show_lyr = NULL;
    }

    if (p_decode->show_hdle)
    {
        MBOOK_Show_Uninit(p_decode->show_hdle);
        p_decode->show_hdle = NULL;
    }

    if (p_decode->analysis_hdle)
    {
        MBOOK_Analysis_Uninit(p_decode->analysis_hdle);
        p_decode->analysis_hdle = NULL;
    }

    if (p_decode->font_hdle)
    {
        MBOOK_Font_Uninit(p_decode->font_hdle);
        p_decode->font_hdle = NULL;
    }

    if (p_decode->file_hdle)
    {
        eLIBs_fclose(p_decode->file_hdle);
        p_decode->file_hdle = NULL;
    }

    if (p_decode)
    {
        My_Mfree(0, p_decode);
        p_decode = NULL;
    }

    return EPDK_OK;
}


