/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2010, ANDY, China
*                                             All Rights Reserved
*
* File      : mnpl.h
* By        : Andy.zhang
* Func      : now play list
* Version   : v1.0
* ============================================================================================================
* 2009-11-3 9:39:42  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#ifndef __JT_TTS_H__
#define __JT_TTS_H__

#include <emodules/mod_defs.h>
#include "eJTTS.h"
#include "app_ebook_i.h"


#define MAX_PAGE_CHAR                   4096
#define MAX_WORD_COUNT      1500    //һҳ�����а����Ͼ������(��",",".","?","!"Ϊ�ֽ��)


extern  int f_audiodev;


/* �����ʽ */
typedef enum
{
    MK_CHAR_ENCODE_UNKNOWN = 0,
    MK_CHAR_ENCODE_UTF8,
    MK_CHAR_ENCODE_GBK,
    MK_CHAR_ENCODE_UNICODE,
    MK_CHAR_ENCODE_UNICODE_BIG
} MkEncode_jt_tts;

typedef struct TTS_THREAD_ARGMEN            /*�����̵߳Ĳ����Ľṹ��*/
{
    unsigned long   woner_win;
    char *data_add;
    __s32 char_num;                     /*���ݵ��ֽ���*/
} __tts_thread_argmen_t;





typedef struct   tag_TTS_PLAY_ST
{
    ES_FILE  *fd;

    __u32    tts_thread;        // TTS  �����ʶ�������
    __krnl_event_t     *tts_sem;  //TTS�����ʶ������ź���
    unsigned long  jt_hTTS;    // �ϳ�������


    /****and by gan****/
    char   *tts_text_curt_statr;   /*��ǰҳ���ݵ���ʼ��ַ*/
    char   *tts_text_line_add; /*��ǰҳ��������һ�仰�ĵ�ַ*/
    __s32  tts_page_leave_char;    /*��ǰҳʣ����ֽ���*/
    __bool  tts_from_page_stat;    /*�Ƿ����ص�ǰҳ����ͷ����0 ��ʾ�ǣ�����ӷ������ľ��*/
    /****************/


    __u32    encode_type;


} tts_char_parser_play;

typedef struct tagUserData                // �ص��û����ݶ���

{
    ES_FILE    *pInputFile;     // �ı������ļ�
    ES_FILE    *pOutputFile;    // �ϳ�������������ļ�
    unsigned long   hTTS;   // �ϳ�������
} jtUserData;

//define wave header, for decode pcm data
typedef struct __WAVE_HEADER1
{
    __u32       uRiffFcc;       // four character code, "RIFF"
    __u32       uFileLen;       // file total length, don't care it

    __u32       uWaveFcc;       // four character code, "WAVE"

    __u32       uFmtFcc;        // four character code, "fmt "
    __u32       uFmtDataLen;    // Length of the fmt data (=16)
    __u16       uWavEncodeTag;  // WAVE File Encoding Tag
    __u16       uChannels;      // Channels: 1 = mono, 2 = stereo
    __u32       uSampleRate;    // Samples per second: e.g., 44100
    __u32       uBytesPerSec;   // sample rate * block align
    __u16       uBlockAlign;    // channels * bits/sample / 8
    __u16       uBitsPerSample; // 8 or 16

    __u32       uDataFcc;       // four character code "data"
    __u32       uSampDataSize;  // Sample data size(n)

} __wave_header_t1;//extern const unsigned char CNPackage[];

extern tts_char_parser_play  *_tts_play_struct;

extern void   TTS_init_all_res(void);
extern   __s32  TTS_res_release(void);
extern jtErrCode TTS_play_server_init(void *arg, void *arg1);

extern void TTS_stop_server(void);
extern __s32  get_cur_audio_buffer(void);

extern void TTS_play_server_start(void *arg, void *arg1, __s32 char_num, H_WIN  woner_win);

//extern void  delete_cuckoo( void );
__bool  cur_audio_speark_over(void);

jtErrCode tts_speaker_deint(void);



#endif /* __JT_TTS_H__ */


