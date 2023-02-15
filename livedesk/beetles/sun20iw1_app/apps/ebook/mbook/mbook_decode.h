/*
**************************************************************************************************************
*                                                   ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              FM app sample
*
*                                   (c) Copyright 2006-2007, ANDY, China
*                                            All Rights Reserved
*
* File    : mbook_decode.h
* By      : lyn
* Version : V1.00
* time    : 2009-8-14
**************************************************************************************************************
*/
#ifndef __MBOOK_DECODE_H__
#define __MBOOK_DECODE_H__

#define     DECODE_MBOOK_NUM     3

typedef     void   *H_BMK;
typedef     void   *H_DECODE;

typedef struct tag_DECODE_CONFIG
{
    GUI_FONT  *char_font;
    __u32       font_size;              // �����С
    __u32       border_width;           // �������׿��
    __u32       bottom_width;           // �ױ����׿��
    __u32       scroll_width;           //�������Ŀ��
    __u32       show_width;             // ��ʾ�Ŀ��
    __u32       show_height;            // ��ʾ�ĸ߶�
    __u32       row_space;              // �м��
    __u32       col_space;              // �м��
    __u32       font_color;             // �������ɫ
    __u32       back_color;             // ��������ɫ
    __u32       show_rotate;            // ��������ʾ
} __decode_config_t;


#define     MAX_FILE_NAME       1024


typedef enum tag__DECODE_MODE
{
    DECODE_NULL = 0,                                /* �������κ�ҳ         */
    DECODE_CURR,                                    /* ���뵱ǰҳ           */
    DECODE_PREV,                                    /* ���뵱ǰҳ��ǰ��ҳ   */
    DECODE_NEXT,                                    /* ��������ҳ�ĺ���ҳ   */
} __decode_mode_e;

typedef struct tag_MBOOK_CACHE               // ҳ����
{
    __s32       page_sta;                   // ҳ�����ɹ���ʾ��1 �ɹ��� 0 ʧ��
    __s32       page_no;                    // �������ҳ���
    H_WIN       page_lyr;                   // ����ҳ��Ӧ����ͼ��
} __page_cache_t;


typedef struct tag_MBOOK_DECODE
{
    char                    file_path[MAX_FILE_NAME];       // �ļ�·��
    ES_FILE                *file_hdle;                      // �ļ����
    void              *analysis_hdle;                  // �ļ��������
    void                   *show_hdle;                      // ҳ����ʾ���

    void                   *font_hdle;                      // ����������

    __s64                   file_size;                      // �ļ���С
    __epdk_charset_enm_e    default_charset;                // Ĭ�ϱ����ʽ
    GUI_FONT               *char_font;                      // �ֿ���
    __s32                  *char_table;                     // �ַ��ֿ��

    H_WIN                   show_lyr;                       // ��ʾͼ��
    __page_cache_t          page[DECODE_MBOOK_NUM];          // ҳ�滺����
    __u32                   data_len;                       // һҳ���ݵĴ�С
    __u8                   *page_data;                      // һҳ���ݴ�ŵ�ַ

    __u8                   *curt_page_data;              //��ǰҳ���ݴ�ŵ�ַ��Ϊtts����

    __s32                   total_page;                     // �ļ���ҳ��
    __s32                   current_page;                   // ��ǰ��ʾҳ��

    __s32                   move_state;                     // ҳ���ƶ����Ʊ�ʾ
    __s32                   move_height;                    // ҳ���ƶ��߶�

    __u32                   decode_tsk;                     // Ԥ���߳�
    __s32                   decode_cnt;                     // Ԥ��ҳѭ������
    __krnl_event_t         *decode_sem;                     // Ԥ������ź���
    __decode_mode_e         decode_sta;                     // Ԥ��ģʽ

    __decode_config_t       config;                         // ������Ϣ
    __epdk_charset_enm_e    charset;                        // �ļ�����
} __mbook_decode_t;






H_DECODE   MBOOK_Decode_Init(char *filename, __u8 *err);

__s32   MBOOK_Decode_Config(H_DECODE hdle, __decode_config_t *config);

__s32   MBOOK_Decode_ShowPage(H_DECODE hdle, __u32 offset);

__s32   MBOOK_Decode_ShowNext(H_DECODE hdle);

__s32   MBOOK_Decode_ShowPrev(H_DECODE hdle);

__s32   MBOOK_Decode_SeekPage(H_DECODE hdle, __u32 page_no);

__s32   MBOOK_Decode_SeekOffset(H_DECODE hdle, __u32 offset);

__s32   MBOOK_Decode_MovePage(H_DECODE hdle, __s32 x, __s32 y);

__s32   MBOOK_Decode_RotatePage(H_DECODE hdle);

__s32   MBOOK_Decode_GetRotate(H_DECODE hdle);

__s32   MBOOK_Decode_GetTotalPage(H_DECODE hdle);

__s32   MBOOK_Decode_GetCurrentPage(H_DECODE hdle);

__s32   MBOOK_Decode_GetPageOffset(H_DECODE hdle, __u32 page_no);

__s32   MBOOK_Decode_GetFileName(H_DECODE hdle, char *name);

__s64   MBOOK_Decode_GetFileSize(H_DECODE hdle);

__s32   MBOOK_Decode_GetFilePath(H_DECODE hdle, char *path);

__s32   MBOOK_Decode_GetFileContext(H_DECODE hdle, char *context);

__s32   MBOOK_Decode_SetRowSpace(H_DECODE hdle, __u32 space);

__s32   MBOOK_Decode_SetColSpace(H_DECODE hdle, __u32 space);

__s32   MBOOK_Decode_SetBorderWidth(H_DECODE hdle, __u32 width);

__s32   MBOOK_Decode_SetBackColor(H_DECODE hdle, __u32 color);

__s32   MBOOK_Decode_SetFontColor(H_DECODE hdle, __u32 color);

__s32   MBOOK_Decode_SetFontSize(H_DECODE hdle, __u32 size);

__s32   MBOOK_Decode_SetDefaultCharset(H_DECODE hdle, __epdk_charset_enm_e charset);

__s32   MBOOK_Decode_Uninit(H_DECODE hdle);

void MBOOK_TTS_court_page(H_DECODE hdle, char  **tts_curt_page_data, __s32 *char_num);

__epdk_charset_enm_e EBookGetSysDefaultCharset(void);

__s32   BOOK_Mark_SetPage(H_BMK hdle, __u32 num, __u32 offset, char *context);
__s32   MBOOK_Decode_GetPage(H_DECODE hdle, __u32 page_offset);

__s32   BOOK_Mark_GetPageOffset(H_BMK hdle, __u32 num);
__s32   MBOOK_Decode_GetPage(H_DECODE hdle, __u32 page_offset);
__s32   BOOK_Mark_Close(H_BMK hdle);
__s32   BOOK_Mark_Uninit(H_BMK hdle);
__s32   BOOK_Mark_Open(H_BMK hdle, char *file);
H_BMK   BOOK_Mark_Init(char *mark_file);
__s32  MBOOK_Decode_get_filename(char *filename);
__s32   MBOOK_Show_LastPage(H_DECODE hdle, __u32 page_no);








#endif /* __MBOOK_DECODE_H__ */

