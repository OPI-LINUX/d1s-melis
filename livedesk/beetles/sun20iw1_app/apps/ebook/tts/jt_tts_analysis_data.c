#include "jt_tts.h"
#include <log.h>
#include "dfs_posix.h"
#define   MAX_CHARS_NO   (50)   /*Ŀǰaudio dev �����Ļ������������50�������ҵ���Ƶ��ֵ��С*/

static __u8 g_new_buf[MAX_CHARS_NO * 3 + 3]; //��ӽ�β�ո��TTS

/****************************************************************************
���ļ����ݽ��з��������������ĵ�һ�仰������Ƶ������
ÿ��������?!��ʱ����ߴ���50 ������,���ַ������(Ӣ�������
����50���ַ��������һ���ո���Ϊһ�仰�Ľ���)
��ʱ����Ϊ��һ�仰�Ľ���
*****************************************************************************/

/*
**********************************************************************************************************************
*                                               __tts_analysis_utf8_oneline
*
* Description: �����������ݵ����ݣ����ص�һ�������ݵ���ʼ��ַ�ͳ��ȣ�UTF8�����ʽ��
*            ��Ϊһ�仰�Ĺ���Ϊ:
                            1��˫�ֽ�������!   ? ʱ�򣬷���= 50 ���ַ���Ϊ��һ��
                            2������50���ֽ������һ���ո����Ϊһ�䣬��������.!?ҲΪһ�䣬
                                50��Ҳ��Ϊһ��
                            3���������з�Ҳ��Ϊ��һ��
* Arguments  :
*           @data: ��������ݵ�ַ
*           @size:��������ݴ�С
*           @outputdata: ���������ĵ�һ�仰����ʼ��ַ
*           @output_size:���������ĵ�һ�仰�ĳ���
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/
static void __tts_analysis_utf8_oneline(char *data, __u32  size, char **outputdata, __u32 *output_size)
{
    __u16                   tmp;
    char                   *p_data;
    __u32 index_size = 0;       /*��¼�Ƿ��Ǵﵽ���ݽ�β*/
    __s32 one_byt_no = 0;       /*�������ֽ��ۼ���*/
    __s32 char_no = 0;          /*�������ַ���Ŀ*/
    char                   *p_first_one_byt;        /*ָ���һ���������ֽ�������λ��*/
    *outputdata =  data;
    *output_size = 0;
    p_data = data;

    while (index_size < size)
    {
        if ((*p_data == 0x0a) || (*p_data == 0x0d))
        {
            //���з����
            if (*(p_data + 1) == 0x0a)
            {
                p_data = p_data + 2;
                *output_size += 2;
                index_size += 2;
                char_no++;
            }
            else
            {
                p_data = p_data + 1;
                *output_size += 1;
                index_size += 1;
                char_no++;
            }

            __msg("--------------------------�������!\n");
            break;
        }
        else if ((((*p_data) & 0xff) == 0xff) && ((*(p_data + 1) & 0xff) == 0xfe))
        {
            /*�Ѿ������ݵĽ�β��*/
            __msg("--------------------------�������!\n");
            break;
        }
        else if ((*p_data & 0xf0) == 0xe0)
        {
            // ���ֽ�������
            tmp = ((*p_data & 0x0f) << 12) | ((*(p_data + 1) & 0x3f) << 6) | (*(p_data + 2) & 0x3f);
            char_no++;
            *output_size += 3;
            index_size += 3;
            p_data = p_data + 3;

            if (0/*tmp == (((0xef & 0x0f) << 12) | ((0xbc & 0x3f) << 6) | (0x81 & 0x3f))*/)
            {
                /*����!��*/
                __msg("--------------------------�������!\n");
                break;
            }

            if (0/*tmp == (((0xe3 & 0x0f) << 12) | ((0x80 & 0x3f) << 6) | (0x82 & 0x3f))*/)
            {
                /*���*/
                __msg("--------------------------�������!\n");
                break;
            }

            if (0/*tmp == (((0xef & 0x0f) << 12) | ((0xbc & 0x3f) << 6) | (0x8c & 0x3f))*/)
            {
                /*����*/
                __msg("--------------------------�������!\n");
                break;
            }

            if (0/*tmp == (((0xef & 0x0f) << 12) | ((0xbc & 0x3f) << 6) | (0x9f & 0x3f))*/)
            {
                /*�ʺ�*/
                __msg("--------------------------�������!\n");
                break;
            }

            if (char_no >= MAX_CHARS_NO)
            {
                /*����50 ������*/
                __msg("--------------------------�������!\n");
                break;
            }

            one_byt_no = 0;
        }
        else if ((*p_data & 0xe0) == 0xc0)
        {
            // ˫�ֽ�������
            tmp = ((*p_data & 0x3f) << 6) | (*(p_data + 1) & 0x3f);
            *output_size += 2;
            char_no++;
            one_byt_no = 0;
            p_data = p_data + 2;
            index_size += 2;

            if (char_no >= MAX_CHARS_NO)
            {
                /*����50 ������*/
                __msg("--------------------------�������!\n");
                break;
            }
        }
        else
        {
            // ���ֽ�������
            tmp = *p_data & 0xff;

            if (one_byt_no == 0)        //��ʼ��¼�����ַ���λ��
            {
                p_first_one_byt = p_data;
            }

            p_data = p_data + 1;
            *output_size += 1 ;

            if (tmp == 9)
            {
                tmp = 0x20;
            }

            if (tmp == 0x21)
            {
                /*̾��*/
                __msg("--------------------------�������!\n");
                break;
            }

            if (tmp == 0x3f)
            {
                /*�ʺ�*/
                __msg("--------------------------�������!\n");
                break;
            }

            if (tmp == 0x2c)
            {
                /*����*/
                __msg("--------------------------�������!\n");
                break;
            }

            one_byt_no++;
            char_no++;
            index_size += 1;

            if (char_no >= MAX_CHARS_NO) /*����50 ������*/
            {
                if (one_byt_no >= MAX_CHARS_NO)
                {
                    /*�Ѿ�������50 ���ǵ��ֽ��������ˣ�������Ӣ���ļ��������һ���ո�ĵط�*/
                    int i;
                    __u32  old_size = *output_size ;

                    for (i = MAX_CHARS_NO; i > 0; i--)
                    {
                        if ((*p_data & 0xff) == 0x20 || (*p_data & 0xff) == 9)
                        {
                            __msg("--------------------------�������!\n");
                            break;
                        }

                        p_data -- ;
                        *output_size -= 1 ;
                    }

                    if (i == 0)         /*��ǰ��ѯ��MAX_CHARS_NO ���ַ�����û�пո��*/
                    {
                        *output_size = old_size;
                        __msg("--------------------------�������!\n");
                        break;
                    }
                }
                else
                {
                    /*�˻ص����һ�����ǵ��ֽ����������Ǹ��ط�*/
                    *output_size = *output_size - (p_data - p_first_one_byt);
                }

                __msg("--------------------------�������!\n");
                break;
            }
        }
    }
}



static void __tts_analysis_utf8(char *data, __u32  size, __u32 *output_size)
{
    char *outputdata;
    __msg("һҳ��Сsize = %d\n", size);
    __tts_analysis_utf8_oneline(_tts_play_struct->tts_text_line_add, _tts_play_struct->tts_page_leave_char, &outputdata, output_size);
    __msg("-----data = %x   -outputdata = %x, output_size = %d,  tota_ananlysis = %d\n", data, outputdata, *output_size);

    if (0)
    {
        __s32 i;
        __inf("utf8\n");

        for (i = 0 ; i < *output_size ; i++)
        {
            __inf("0x%.2x ", outputdata[i]);
        }

        __inf("\n");
    }

    jtTTS_SynthesizeText(_tts_play_struct->jt_hTTS, (const void *)outputdata, (*output_size) * sizeof(char));

    while (1)
    {
        if (cur_audio_speark_over() == EPDK_TRUE)
        {
            ioctl(f_audiodev, AUDIO_DEV_CMD_FLUSH_BUF, 0);
            break;
        }

        if (*output_size <= 5)          /*��ֹһҳ���������ѭ��*/
        {
            esKRNL_TimeDly(20);     /*�ӳ�20 ����audiodev ����5��С����Ƶ����*/
            break;
        }

        esKRNL_TimeDly(5);
    }
}


/*
**********************************************************************************************************************
*                                               __tts_analysis_utf8_oneline
*
* Description: �����������ݵ����ݣ����ص�һ�������ݵ���ʼ��ַ�ͳ��ȣ�utf16_be�����ʽ��
*            ��Ϊһ�仰�Ĺ���Ϊ:
                            1��˫�ֽ�������!   ? ʱ�򣬷���= 50 ���ַ���Ϊ��һ��
                            2������50���ֽ������һ���ո����Ϊһ�䣬��������.!?ҲΪһ�䣬
                                50��Ҳ��Ϊһ��
                            3���������з�Ҳ��Ϊ��һ��
* Arguments  :
*           @data: ��������ݵ�ַ
*           @size:��������ݴ�С
*           @outputdata: ���������ĵ�һ�仰����ʼ��ַ
*           @output_size:���������ĵ�һ�仰�ĳ���
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/

static void __tts_analysis_utf16_be_oneline(char *data, __u32  size, char **outputdata, __u32 *output_size)
{
    __u16                   tmp;
    __u8                   *p_data;
    __u32 index_size = 0;       /*��¼�Ƿ��Ǵﵽ���ݽ�β*/
    __s32 char_no = 0;          /*�������ַ���Ŀ*/
    (*outputdata) = data;
    p_data = data;
    *output_size = 0;

    while (index_size < size)
    {
        tmp = ((*p_data & 0xff) << 8) + (*(p_data + 1) & 0xff);     /*����Ҫת�ַ�*/
        char_no ++;

        if ((tmp == 0x0d) || (tmp == 0x0a))
        {
            if ((*(p_data + 3) & 0xff) == 0x0a)
            {
                p_data = p_data + 4;
                *output_size += 4;
                index_size += 4;
            }
            else
            {
                p_data = p_data + 2;
                *output_size += 2;
                index_size += 2;
            }

            break;
        }
        else if (tmp == 0xfffe)
        {
            break;
        }
        else
        {
            p_data += 2;
            *output_size += 2;

            if (tmp == ((0x00 & 0xff) << 8) + (0x3f & 0xff)/* || tmp == ((0xff & 0xff)<< 8)+ (0x1f & 0xff)*/)
            {
                /*�ʺ�*/
                break;
            }

            if (tmp == ((0x00 & 0xff) << 8) + (0x21 & 0xff) /*|| tmp == ((0xff & 0xff)<< 8)+ (0x01 & 0xff)*/)
            {
                /*̾��*/
                break;
            }

            if (tmp == ((0x00 & 0xff) << 8) + (0x2c & 0xff) /*|| tmp == ((0xff & 0xff)<< 8)+ (0x0c & 0xff)*/)
            {
                /*����*/
                break;
            }

            if (0/* tmp == ((0x30 & 0xff)<< 8)+ (0x02 & 0xff)*/)
            {
                /*���*/
                break;
            }

            if (char_no >= MAX_CHARS_NO)    /*�ַ�����MAX_CHARS_NO,�п�����Ӣ�ģ���ǰ�ҵ�����һ�����ո��λ��*/
            {
                __s32 i ;
                __u32 old_size = *output_size;

                for (i = MAX_CHARS_NO; i > 0; i --)
                {
                    tmp = ((*p_data & 0xff) << 8) + (*(p_data + 1) & 0xff);

                    if ((tmp == ((0x00 & 0xff) << 8) + 0x20) & 0xff)
                    {
                        break;
                    }

                    *output_size -= 2;
                }

                if (i == 0)
                {
                    *output_size = old_size;
                }

                break;
            }
        }
    }
}



static void __tts_analysis_utf16_be(char *data, __u32  size, __u32 *output_size)
{
    char *outputdata;
    __u32 tota_ananlysis = 0;
    __msg("һҳ��Сsize = %d\n", size);
    __tts_analysis_utf16_be_oneline(_tts_play_struct->tts_text_line_add, _tts_play_struct->tts_page_leave_char, &outputdata, output_size);

    if (0)
    {
        __s32 i;
        __inf("utf16_be\n");

        for (i = 0 ; i < *output_size ; i++)
        {
            __inf("0x%.2x ", outputdata[i]);
        }

        __inf("\n");
    }

    jtTTS_SynthesizeText(_tts_play_struct->jt_hTTS, (const void *)outputdata, (*output_size) * sizeof(char));
    __msg("tota_ananlysis  = %d\n", tota_ananlysis);

    while (1)
    {
        if (cur_audio_speark_over() == EPDK_TRUE)
        {
            ioctl(f_audiodev, AUDIO_DEV_CMD_FLUSH_BUF, 0);
            break;
        }

        esKRNL_TimeDly(5);
    }
}




/*
**********************************************************************************************************************
*                                               __tts_analysis_utf16_le_oneline
*
* Description: �����������ݵ����ݣ����ص�һ�������ݵ���ʼ��ַ�ͳ��ȣ�utf16_be�����ʽ��
*            ��Ϊһ�仰�Ĺ���Ϊ:
                            1��˫�ֽ�������!   ? ʱ�򣬷���= 50 ���ַ���Ϊ��һ��
                            2������50���ֽ������һ���ո����Ϊһ�䣬��������.!?ҲΪһ�䣬
                                50��Ҳ��Ϊһ��
                            3���������з�Ҳ��Ϊ��һ��
* Arguments  :
*           @data: ��������ݵ�ַ
*           @size:��������ݴ�С
*           @outputdata: ���������ĵ�һ�仰����ʼ��ַ
*           @output_size:���������ĵ�һ�仰�ĳ���
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/

static void __tts_analysis_utf16_le_oneline(char *data, __u32  size, char **outputdata, __u32 *output_size)
{
    __u16                   tmp;
    __u8                   *p_data;
    __u32 index_size = 0;       /*��¼�Ƿ��Ǵﵽ���ݽ�β*/
    __s32 char_no = 0;          /*�������ַ���Ŀ*/
    *outputdata = data;
    *output_size = 0;
    p_data = data;

    while (index_size < size)
    {
        tmp = (*p_data & 0xff) + ((*(p_data + 1) & 0xff) << 8);     /*����Ҫת�ַ�*/
        char_no ++;

        if ((tmp == 0x0d) || (tmp == 0x0a))
        {
            if ((*(p_data + 2) & 0xff) == 0x0a)
            {
                p_data = p_data + 4;
                *output_size += 4;
                index_size += 4;
            }
            else
            {
                p_data = p_data + 2;
                *output_size += 2;
                index_size += 2;
            }

            break;
        }
        else if (tmp == 0xfeff)
        {
            break;
        }
        else
        {
            p_data += 2;
            *output_size += 2;

            if (tmp == ((0x3f & 0xff) << 8) + (0x00 & 0xff)/* || tmp == ((0x1f & 0xff)<< 8)+ (0xff & 0xff)*/)
            {
                /*�ʺ�*/
                break;
            }

            if (tmp == ((0x21 & 0xff) << 8) + (0x00 & 0xff) /*|| tmp == ((0x01 & 0xff)<< 8)+ (0xff & 0xff)*/)
            {
                /*̾��*/
                break;
            }

            if (tmp == ((0x2c & 0xff) << 8) + (0x00 & 0xff)/* || tmp == ((0x0c & 0xff)<< 8)+ (0xff & 0xff)*/)
            {
                /*����*/
                break;
            }

            if (0/* tmp == ((0x02 & 0xff)<< 8)+ (0x30 & 0xff)*/)
            {
                /*���*/
                break;
            }

            if (char_no >= MAX_CHARS_NO)    /*�ַ�����MAX_CHARS_NO,�п�����Ӣ�ģ���ǰ�ҵ�����һ�����ո��λ��*/
            {
                __s32 i ;
                __u32 old_size = *output_size;

                for (i = MAX_CHARS_NO; i > 0; i --)
                {
                    tmp = ((*p_data & 0xff) << 8) + (*(p_data + 1) & 0xff);

                    if ((tmp == ((0x20 & 0xff) << 8) + 0x00) & 0xff)
                    {
                        break;
                    }

                    *output_size -= 2;
                }

                if (i == 0)
                {
                    *output_size = old_size;
                }

                break;
            }
        }
    }
}

static void __tts_analysis_utf16_le(char *data, __u32  size, __u32 *output_size)
{
    char *outputdata;
    __u32 tota_ananlysis = 0;
    __msg("һҳ��Сsize = %d\n", size);
    __tts_analysis_utf16_le_oneline(_tts_play_struct->tts_text_line_add, _tts_play_struct->tts_page_leave_char, &outputdata, output_size);

    if (0)
    {
        __s32 i;
        __inf("utf16_le\n");

        for (i = 0 ; i < *output_size ; i++)
        {
            __inf("0x%.2x ", outputdata[i]);
        }

        __inf("\n");
    }

    jtTTS_SynthesizeText(_tts_play_struct->jt_hTTS, (const void *)outputdata, (*output_size) * sizeof(char));
    __msg("tota_ananlysis  = %d\n", tota_ananlysis);

    while (1)
    {
        if (cur_audio_speark_over() == EPDK_TRUE)
        {
            ioctl(f_audiodev, AUDIO_DEV_CMD_FLUSH_BUF, 0);
            break;
        }

        esKRNL_TimeDly(5);
    }
}





static __u32 __local_language_count(__epdk_charset_enm_e enm)
{
    switch (enm)
    {
        case EPDK_CHARSET_ENM_GB2312:
        case EPDK_CHARSET_ENM_BIG5:
        case EPDK_CHARSET_ENM_GBK:
        case EPDK_CHARSET_ENM_SJIS:
        case EPDK_CHARSET_ENM_EUC_JP:
        case EPDK_CHARSET_ENM_EUC_KR:
        case EPDK_CHARSET_ENM_CP874:
            return 2;

        default:
            return 1;
    }
}


/*
**********************************************************************************************************************
*                                               __tts_analysis_utf16_le_oneline
*
* Description: �����������ݵ����ݣ����ص�һ�������ݵ���ʼ��ַ�ͳ��ȣ�utf16_be�����ʽ��
*            ��Ϊһ�仰�Ĺ���Ϊ:
                            1��˫�ֽ�������!   ? ʱ�򣬷���= 50 ���ַ���Ϊ��һ��
                            2������50���ֽ������һ���ո����Ϊһ�䣬��������.!?ҲΪһ�䣬
                                50��Ҳ��Ϊһ��
                            3���������з�Ҳ��Ϊ��һ��
* Arguments  :
*           @data: ��������ݵ�ַ
*           @size:��������ݴ�С
*           @outputdata: ���������ĵ�һ�仰����ʼ��ַ
*           @output_size:���������ĵ�һ�仰�ĳ���
* Returns    :

* Notes      :
*
**********************************************************************************************************************
*/

static void __tts_analysis_gbk_oneline(char   *data, __u32 size, char **outputdata, __u32 *output_size)
{
    __u16                   tmp;
    __u8                   *p_data;
    __u32 index_size = 0;       /*��¼�Ƿ��Ǵﵽ���ݽ�β*/
    __s32 char_no = 0;          /*�������ַ���Ŀ*/
    __s32 one_byt_no = 0;       /*�������ֽ��ۼ���*/
    char                   *p_first_one_byt;        /*ָ���һ���������ֽ�������λ��*/
    p_data = data;
    *outputdata = data;
    *output_size = 0;

    while (index_size < size)
    {
        tmp = *p_data & 0xff;

        if ((tmp == 0x0d) || (tmp == 0x0a))
        {
            if ((*(p_data + 1) & 0xff) == 0x0a)
            {
                p_data = p_data + 2;
                *output_size += 2;
            }
            else
            {
                p_data = p_data + 1;
                *output_size += 1;
                char_no++;
                continue;
            }

            char_no ++;
            __msg("----��������\n") ;
            break;
        }
        else if (tmp == 0xff)
        {
            if ((*(p_data + 1) & 0xff) == 0xfe)
            {
                __msg("----��������\n") ;
                break;
            }
            else
            {
                *output_size += 1;
                p_data += 1;
                char_no += 1;
            }
        }
        else if (tmp > 0x7f) //�������ַ���ʱ��
        {
#if 0

            if (local_language_count(get_isn_charset()) == 2)
            {
                __u8 tmp_tem[8];
                tmp_tem[1] = *(p_data + 1) & 0xff;
                //tmp = (tmp << 8) + *(p_data + 1);
                tmp_tem[0] = *p_data & 0xff;//tmp;
                esCHS_Char2Uni(get_isn_charset(), tmp_tem, 8, &unicode);
                tmp  = unicode;
                tmp_w = p_config->col_width;//GUI_GetCharDistX(unicode);//p_config->col_width;
                p_data = p_data + 2;
            }
            else if (local_language_count(get_isn_charset()) == 1)
            {
                __u8 tmp_tem[8];
                tmp_tem[0] = tmp;
                esCHS_Char2Uni(get_isn_charset(), tmp_tem, 4, &unicode);
                tmp  = unicode;
                tmp_w = GUI_GetCharDistX(unicode);// p_config->col_width;
                p_data = p_data + 1;
            }

#else
            __u16  unicode;
            one_byt_no = 0;

            if (__local_language_count(dsk_get_isn_charset()) == 2)
            {
                __u8 tmp_tem[8];
                tmp_tem[1] = *(p_data + 1) & 0xff;
                //tmp = (tmp << 8) + *(p_data + 1);
                tmp_tem[0] = *p_data & 0xff;//tmp;
                esCHS_Char2Uni(dsk_get_isn_charset(), tmp_tem, 8, &unicode);
                tmp  = unicode;
                *output_size += 2;
                p_data += 2;
#if 0

                if (*p_data_temp == 0xa3 && *p_data_temp == 0xa1)
                {
                    /*̾��*/
                    break;
                }

                if (*p_data_temp == 0xa3 && *p_data_temp == 0xb1)
                {
                    /*�ʺ�*/
                    break;
                }

                if (*p_data_temp == 0xa3 && *p_data_temp == 0xac)
                {
                    /*����*/
                    break;
                }

                if (*p_data_temp == 0xa1 && *p_data_temp == 0xb3)
                {
                    /*����*/
                    break;
                }

#endif
            }
            else if (__local_language_count(dsk_get_isn_charset()) == 1)
            {
                __u8 tmp_tem[8];
                tmp_tem[0] = tmp;
                esCHS_Char2Uni(dsk_get_isn_charset(), tmp_tem, 4, &unicode);
                tmp  = unicode;
                *output_size += 1;
                p_data += 1;
#if 0

                if (*p_data_temp == 0xa3 && *p_data_temp == 0xa1)
                {
                    /*̾��*/
                    break;
                }

                if (*p_data_temp == 0xa3 && *p_data_temp == 0xb1)
                {
                    /*�ʺ�*/
                    break;
                }

                if (*p_data_temp == 0xa3 && *p_data_temp == 0xac)
                {
                    /*����*/
                    break;
                }

                if (*p_data_temp == 0xa1 && *p_data_temp == 0xb3)
                {
                    /*����*/
                    break;
                }

#endif
            }

            char_no++;

            /***********�Դ�˻��������бȽ�****************/
            if (unicode == ((0x00 & 0xff) << 8) + (0x3f & 0xff)/* || unicode == ((0xff & 0xff)<< 8)+ (0x1f & 0xff)*/)
            {
                /*�ʺ�*/
                break;
            }

            if (unicode == ((0x00 & 0xff) << 8) + (0x21 & 0xff)/* || unicode == ((0xff & 0xff)<< 8)+ (0x01 & 0xff)*/)
            {
                /*̾��*/
                break;
            }

            if (unicode == ((0x00 & 0xff) << 8) + (0x2c & 0xff) /*|| unicode == ((0xff & 0xff)<< 8)+ (0x0c & 0xff)*/)
            {
                /*����*/
                break;
            }

            if (0 /*unicode == ((0x30 & 0xff)<< 8)+ (0x02 & 0xff)*/)
            {
                /*���*/
                break;
            }

            if (char_no >= MAX_CHARS_NO)
            {
                break;
            }

#endif
        }
        else
        {
            *output_size += 1;
            p_data += 1;
            char_no += 1;

            if (one_byt_no == 0)        //��ʼ��¼�����ַ���λ��
            {
                p_first_one_byt = p_data;
            }

            one_byt_no += 1;

            if (tmp == 0x21 || tmp == 0x3f || tmp == 0x2c)             // ! ? , �־����
            {
                __msg("----��������\n") ;
                break;
            }

            if (char_no >= MAX_CHARS_NO)    /*�п�����Ӣ�ĵ������*/
            {
                if (one_byt_no >= MAX_CHARS_NO)
                {
                    int i;
                    __u32  old_size = *output_size ;

                    for (i = MAX_CHARS_NO; i > 0; i--)
                    {
                        if (*p_data == 0x20)
                        {
                            __msg("----��������\n") ;
                            break;
                        }

                        p_data -- ;
                        *output_size -= 1 ;
                    }

                    if (i == 0)
                    {
                        *output_size = old_size ;
                        __msg("----��������\n") ;
                    }

                    break;
                }
                else
                {
                    /*�˻ص����һ�����ǵ��ֽ����������Ǹ��ط�*/
                    int i ;

                    for (i = MAX_CHARS_NO; i > 0; i--)
                    {
                        if (*p_data == 0x20         //�ո�
                            || *p_data == 0x2E    //.
                            || *p_data == 0x2c)    //,
                        {
                            p_first_one_byt = p_data;
                            break;
                        }

                        p_data--;
                    }

                    __msg("----------p_data = %x, p_first_one_byt = %x\n", p_data, p_first_one_byt);
                    *output_size = *output_size - ((unsigned long)p_data - (unsigned long)p_first_one_byt);
                    __msg("----��������\n") ;
                    break;
                }
            }
        }
    }
}

static void __tts_analysis_gbk(char *data, __u32  size, __u32 *output_size)
{
    char *outputdata;
    __msg("һҳ��Сsize = %d, _tts_play_struct->tts_text_line_add= %s\n", size, _tts_play_struct->tts_text_line_add);
    __tts_analysis_gbk_oneline(_tts_play_struct->tts_text_line_add, _tts_play_struct->tts_page_leave_char, &outputdata, output_size);

    if (0)
    {
        __s32 i;
        __inf("_gbk\n");

        for (i = 0 ; i < *output_size ; i++)
        {
            __inf("0x%.2x ", outputdata[i]);
        }

        __inf("\n");
    }

    jtTTS_SynthesizeText(_tts_play_struct->jt_hTTS, (const void *)outputdata, (*output_size) * sizeof(char));
    __msg("------һ�仰�Ĵ�Сoutput_size  = %d\n", *output_size);

    while (1)
    {
        if (cur_audio_speark_over() == EPDK_TRUE)
        {
            ioctl(f_audiodev, AUDIO_DEV_CMD_FLUSH_BUF, 0);
            break;
        }

        esKRNL_TimeDly(5);
    }
}


/*
************************************************************************************************************************
*                                       TTS_ANALYSYS_Page
*
*Description: дһҳ���ݵ�ָ������ͼ����
*
*Arguments  : hdle: �������
*             lyr: ͼ����
*
*Return     : EPDK_OK: �ɹ�
*             EPDK_FAIL: ʧ��
*
************************************************************************************************************************
*/
__s32   TTS_ANALYSYS_Page(__epdk_charset_enm_e  charset,  void *data, __u32  size, __u32 *output_size)
{
#if defined Have_eJTTS_lib
    switch (charset)
    {
        case MK_CHAR_ENCODE_UTF8 :
        {
            __tts_analysis_utf8(data, size, output_size);
        }
        break;

        case MK_CHAR_ENCODE_UNICODE_BIG:
        {
            __tts_analysis_utf16_be(data, size, output_size);
        }
        break;

        case MK_CHAR_ENCODE_UNICODE:
        {
            __tts_analysis_utf16_le(data, size, output_size);
        }
        break;

        case MK_CHAR_ENCODE_GBK:
        {
            __tts_analysis_gbk(data, size, output_size);
        }
        break;
#if 0

        case MK_CHAR_ENCODE_GBK:
        {
            __show_page_big5(data, size);
        }
        break;
#endif

        default:
            __tts_analysis_gbk(data, size, output_size);
            break;
    }
#endif
    return EPDK_OK;
}




