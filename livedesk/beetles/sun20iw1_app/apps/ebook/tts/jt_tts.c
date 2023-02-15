/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2010, ANDY, China
*                                             All Rights Reserved
*
* File      : npl.c
* By        : Andy.zhang
* Func      : now play list
* Version   : v1.0
* ============================================================================================================
* 2009-11-3 9:39:42  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#define TTS_DATA_COMPRESS_FLAG 1

#include "app_ebook_i.h"
#include "jt_tts.h"
#include "jt_tts_char.h"
#include "jt_tts_reg.h"
#include "mbook/mbook_decode.h"
#include <log.h>
#include "dfs_posix.h"

#define strlen(s)                       eLIBs_strlen(s)

#if 1 == TTS_DATA_COMPRESS_FLAG
#include "sft_decode.h"
#endif

#undef BEETLES_APP_ROOT
#define BEETLES_APP_ROOT "d:\\"

int f_audiodev;
/*��Դ���ط�ʽ������ѡ����һ */
#define DATA_MODE_SMALL_FILE        1    // �ļ���ʽ������Դ
//#define DATA_MODE_FIX_CONST       1 // �ڴ淽ʽ������Դ

#ifdef DATA_MODE_FIX_CONST
ES_FILE             *fpVoiceDat;        // ��Դ���ļ�
ES_FILE                        *fpENVoiceDta;     //Ӣ�Ŀ��ļ�
__u32                iDicLen;           // ��Դ�ⳤ��
__u32                           iDicLen_En;
void                    *pDicData = NULL;   // Const�ڴ������Դ�����׵�ַ
void                              *pDicData_En = NULL; //
#endif




ES_FILE         *fpOutPCM;                  //��Ƶ����������
//ES_FILE       *fpinPCM;                   //��Ƶ�����ļ����
unsigned char   *pHeap;             // �ѿռ��ַ
long                nSize;              // �ѿռ��С
/*TTS ��Դ���ļ�·������*/
#if 1 == TTS_DATA_COMPRESS_FLAG
const signed char                   strCNFileName6[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage_e.dat";
const signed char                          strENFileName6[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage_e.dat";

//const signed char             strCNFileName6[] = BEETLES_APP_ROOT"res\\tts\\CNPackage_Girl4_zhaqian.dat";
//const signed char                      strENFileName6[] = BEETLES_APP_ROOT"res\\tts\\ENPackage_zhaqian.dat";

const signed char                   strCNFileName5[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage_e.dat";
const signed char                          strENFileName5[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage_e.dat";
const signed char                   strCNFileName4[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage_e.dat";
const signed char                          strENFileName4[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage_e.dat";
const signed char                   strCNFileName3[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage_e.dat";
const signed char                          strENFileName3[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage_e.dat";
const signed char                   strCNFileName2[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage_e.dat";
const signed char                          strENFileName2[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage_e.dat";
const signed char                   strCNFileName1[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage_e.dat";
const signed char                          strENFileName1[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage_e.dat";

#else
const signed char                   strCNFileName6[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage.dat";
const signed char                          strENFileName6[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage.dat";

//const signed char             strCNFileName6[] = BEETLES_APP_ROOT"res\\tts\\CNPackage_Girl4_zhaqian.dat";
//const signed char                      strENFileName6[] = BEETLES_APP_ROOT"res\\tts\\ENPackage_zhaqian.dat";

const signed char                   strCNFileName5[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage.dat";
const signed char                          strENFileName5[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage.dat";
const signed char                   strCNFileName4[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage.dat";
const signed char                          strENFileName4[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage.dat";
const signed char                   strCNFileName3[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage.dat";
const signed char                          strENFileName3[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage.dat";
const signed char                   strCNFileName2[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage.dat";
const signed char                          strENFileName2[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage.dat";
const signed char                   strCNFileName1[] = BEETLES_APP_ROOT"res\\tts\\data\\CNPackage.dat";
const signed char                          strENFileName1[] = BEETLES_APP_ROOT"res\\tts\\data\\ENPackage.dat";

#endif

const signed char                     *strTTSCNFileName[] = {strCNFileName6, strCNFileName1, strCNFileName2,
                                                             strCNFileName3, strCNFileName4, strCNFileName5
                                                            };

const signed char                     *strTTSENFileName[] = {strENFileName6, strENFileName1, strENFileName2,
                                                             strENFileName3, strENFileName4, strENFileName5
                                                            };
__s32  get_cur_audio_totalbuffer(void);

__s32   TTS_ANALYSYS_Page(__epdk_charset_enm_e  charset,  void *data, __u32  size, __u32 *output_size);

/*һЩ����Ƶ���йغ�������ת��*/
ES_FILE *jt_Fopen(const char *ES_FILEname, const char *mode)
{
#if 1 == TTS_DATA_COMPRESS_FLAG
    return   sft_fopen(ES_FILEname, mode);
#else
    return   eLIBs_fopen(ES_FILEname, mode);
#endif
}

int      jt_Fclose(ES_FILE *stream)
{
#if 1 == TTS_DATA_COMPRESS_FLAG
    return  sft_fclose(stream);
#else
    return  eLIBs_fclose(stream);
#endif
}
__size jt_Fread(void *ptr, __size size, __size nmemb, ES_FILE *stream)
{
#if 1 == TTS_DATA_COMPRESS_FLAG
    return   sft_fread(ptr, size, nmemb, stream);
#else
    return   eLIBs_fread(ptr, size, nmemb, stream);
#endif
}
__s32 jt_Fseek(ES_FILE *stream, __s32 offset, int whence)
{
#if 1 == TTS_DATA_COMPRESS_FLAG
    return  sft_fseek(stream, offset, whence);
#else
    return  eLIBs_fseek(stream, offset, whence);
#endif
}
#if 0/*langaojie temp test*/
void *malloc(__u32 num_bytes)
{
    return esMEMS_Malloc(0, num_bytes);
}
void free(void *FirstByte)
{
    esMEMS_Mfree(0, FirstByte);
}
#endif
void jt_Printf(const char *format, ...)
{
    //  eLIBs_printf( format,NULL);
}
/*---------------------TTS����ӿڲ���--------------------------------------*/

//long size_sum=0;
// �ϳ�������������ص�����
// ���ϳ�һ������ʱ,�˺����ᱻ��ε���,ÿ�����һ��������������
// �û����ڴ˺����ڽ���������������Ƶ���Žӿ�
jtErrCode jtExam_OutputVoiceProc(void *pParameter,
                                 long iOutputFormat, void *pData, long iSize)
{
    static long size_sum = 0;
    jtUserData  *pUserData;

    // ע�����δ�����û����ݣ���Ҫʹ�ô��ж�
    if (pParameter == NULL)
    {
        //__here__
        return jtTTS_ERR_NONE;
    }

    pUserData = (jtUserData *)pParameter;

    // ���iSizeΪ-1�����ǵ�ǰ�ı���
    // ���кϳ����ݾ�������������Ҫֹͣ���棬
    // ��������������
    if (iSize == -1)
    {
        //__here__
        // jtTTS_SynthesizeText��jtTTS_Synthesize
        // ��������ı��ϳ���Ϻ󣬻��Զ��˳���
        // ���Բ�����jtTTS_SynthStop,��
        // jtTTS_SynthStart�ϳ���Ϻ󣬲��������˳���
        // �����jtTTS_SynthStopʹ֮�˳�
#if defined Have_eJTTS_lib
        jtTTS_SynthStop(pUserData->hTTS);
#endif
        return jtTTS_ERR_NONE;
    }

#if 1

    if (f_audiodev)
    {
        if (0)
        {
            __s32 i;
            char *p = pData;
            __inf("pcm buf:\n");

            for (i = 0 ; i < iSize ; i++)
            {
                __inf("0x%.2x ", p[i]);
            }

            __inf("\n");
        }

        //char sz_empty[4];
        //eLIBs_fwrite(pData, 1, iSize, f_audiodev);//�����ɵ���Ƶ���ݶ�����Ƶ�ӿڽ���
        write(f_audiodev,pData,iSize);
        //eLIBs_fwrite(sz_empty, 1, sizeof(sz_empty), f_audiodev);//�����ɵ���Ƶ���ݶ�����Ƶ�ӿڽ���
    }

#else
    eLIBs_fwrite(pData, 1, iSize, pUserData->pOutputFile);
#endif
    //  __msg("   size is %d total_buffsize is %d data_buffsize is %d, freed_buffsize =is %d\n",iSize,get_cur_audio_totalbuffer(),get_cur_audio_buffer(),get_cur_audio_freedbuffer());
    size_sum = size_sum + iSize;
    //  __msg("------------------size_sum is %d\n",size_sum);
    return jtTTS_ERR_NONE;
}

// �����ı��ص�
jtErrCode jtExam_InputTextProc(void *pParameter,
                               void *pText, long *piSize)
{
    jtUserData *pUserData = (jtUserData *)pParameter;
    // �����ȡ�����ݳ���Ϊ0������0���ظ����棬
    // ���潫��Ϊû���ı���Ҫ�ϳ��ˣ����ڶ�ʽ�ص�
    // �ϳɷ�ʽ���ϳɹ��̽�������������ƪ��ģʽ��
    // ���潫�᲻�ϵ���ѯ�ûص��������Ի���ı�
    __msg("-----������piSize = %d\n", *piSize);
    *piSize = eLIBs_fread(pText, 1, *piSize, pUserData->pInputFile);
    return jtTTS_ERR_NONE;
}



/*---------------------TTS����ӿڲ���--------------------------------------*/


/*��Ƶ�ӿ�������ʼ��*/
void tts_tone_init(void)
{
    /*��ʼ���ײ������Ϣ��׼��Ӳ��     */
    __audio_dev_para_t      pbuf2;
    unsigned long arg[4] = {0};
    //set audio sample parameter
    pbuf2.bps  = 16;
    pbuf2.chn = 1;
    pbuf2.fs = 16000;
    //config audio device
    f_audiodev = open("/dev/audio_play0", O_RDWR);
    ioctl(f_audiodev, AUDIO_DEV_CMD_STOP, 0);
    arg[0] = 384 * 1024;
    arg[1] = 0;
    ioctl(f_audiodev, AUDIO_DEV_CMD_RESIZE_BUF,(void*)arg);
    ioctl(f_audiodev, AUDIO_DEV_CMD_START, 0);
    __msg("----------------------AUDIO SIZE = %d\n", get_cur_audio_totalbuffer());
    arg[0] = 0;
    arg[1] = (unsigned long)&pbuf2;
    ioctl(f_audiodev, AUDIO_DEV_CMD_SET_PARA,(void*)arg);
    //ioctl(f_audiodev, AUDIO_DEV_CMD_REG_USERMODE, AUDIO_PLAY_USR_MASTER, 0); // AUDIO_PLAY_USR_MASTER, 0);
    ioctl(f_audiodev, AUDIO_DEV_CMD_START, 0);
    ioctl(f_audiodev, AUDIO_DEV_CMD_FLUSH_BUF, 0);
}
/*���˳�TTS��ʱ����Ҫ����ǰ����Ƶ�ӿڷ���ʼ��Ϊ������ģʽ*/
__s32  tts_tone_deinit(void)
{
    __audio_dev_para_t    pbuf2;
    unsigned long arg[4] = {0};
    __wave_header_t1      wav;
    {
        ES_FILE             *pfile2;
        pfile2 = eLIBs_fopen(BEETLES_APP_ROOT"res\\sounds\\chord.wav", "rb");

        if (pfile2 == 0)
        {
            return EPDK_FAIL;
        }

        eLIBs_fread(&wav, 1, sizeof(__wave_header_t1), pfile2);
        eLIBs_fclose(pfile2);
    }
    pbuf2.bps  = wav.uBitsPerSample;
    pbuf2.chn = wav.uChannels;
    pbuf2.fs = wav.uSampleRate;

    arg[0] = 0;
    arg[1] = (unsigned long)&pbuf2;
    ioctl(f_audiodev, AUDIO_DEV_CMD_SET_PARA,(void*)arg);
    //ioctl(f_audiodev, AUDIO_DEV_CMD_REG_USERMODE, AUDIO_PLAY_USR_KEY, 0);
    ioctl(f_audiodev, AUDIO_DEV_CMD_START, 0);
    return EPDK_OK;
}
/*�˳�ʱ��Ҫ����ǰ����Ƶ�ӿ��ͷ�*/
static  void  tts_tone_exit(void)
{
    unsigned long arg[4] = {0};
    tts_tone_deinit();
    ioctl(f_audiodev, AUDIO_DEV_CMD_STOP, 0);
    arg[0] = 32*1024;
    arg[1] = 0;
    ioctl(f_audiodev, AUDIO_DEV_CMD_RESIZE_BUF,(void*)arg);
    ioctl(f_audiodev, AUDIO_DEV_CMD_START, 0);
    __msg("----------------------AUDIO SIZE = %d\n", get_cur_audio_totalbuffer());
    close(f_audiodev);
    f_audiodev  = -1;
}


/*TTS  �����ʼ��*/
static jtErrCode tts_res_init(void)
{
    jtErrCode           dwError = jtTTS_ERR_NONE;            // ������
    unsigned char       byMajor;            // ���汾��
    unsigned char       byMinor;            // �ΰ汾��
    unsigned long     iRevision;            // �޶��汾��
    unsigned int    tts_speaker_index;   // ������������
    tts_tone_init();
    _tts_play_struct->tts_sem  = esKRNL_SemCreate(1);
    // ��ȡ��ǰ����Ĳ���������
    tts_speaker_index = tts_get_voice_speaker();//0
#if defined Have_eJTTS_lib
    dwError = jtTTS_GetVersion(&byMajor, &byMinor, &iRevision);     /* ��ȡTTS�汾��Ϣ */
#endif
    if (dwError != jtTTS_ERR_NONE)
    {
        return jtTTS_ERR_LICENCE;
    }

    //  fpinPCM = eLIBs_fopen(inputfilename, "rb");
    fpOutPCM = NULL;//eLIBs_fopen(BEETLES_APP_ROOT"res\\tts\\Output.pcm", "wb");
    // ׼����Դ����
#ifdef DATA_MODE_SMALL_FILE
    // �ļ���ʽ����Ҫ����
#elif DATA_MODE_FIX_CONST
    // ֱ�Ӷ�ȡ���ڴ棩��ʽ�轫��Դ���ݷ����ڴ�
    fpVoiceDat = eLIBs_fopen(strTTSCNFileName[tts_speaker_index], "rb");
    eLIBs_fseek(fpVoiceDat, 0, SEEK_END);
    iDicLen = eLIBs_ftell(fpVoiceDat);
    fpENVoiceDta = eLIBs_fopen(strTTSENFileName[tts_speaker_index], "rb");
    eLIBs_fseek(fpENVoiceDta, 0, SEEK_END);
    iDicLen_En = eLIBs_ftell(fpENVoiceDta);
    pDicData = (const signed char *)esMEMS_Malloc(0, iDicLen);
    eLIBs_fseek(fpVoiceDat, 0, SEEK_SET);
    eLIBs_fread(pDicData, 1, iDicLen, fpVoiceDat);
    eLIBs_fclose(fpVoiceDat);
    pDicData_En = (const signed char *)esMEMS_Malloc(0, iDicLen_En);
    eLIBs_fseek(fpENVoiceDta, 0, SEEK_SET);
    eLIBs_fread(pDicData_En, 1, iDicLen_En, fpENVoiceDta);
    eLIBs_fclose(fpENVoiceDta);
#endif
    /* ���HEAP��С */
#ifdef DATA_MODE_SMALL_FILE
    __msg("-----------tts_speaker_index = %d\n", tts_speaker_index);
    dwError = jtTTS_GetExtBufSize(strTTSCNFileName[tts_speaker_index], strTTSENFileName[tts_speaker_index], NULL, &nSize);
#elif DATA_MODE_FIX_CONST
    dwError = jtTTS_GetExtBufSize(pDicData, pDicData_En, NULL, &nSize);
#endif

    //__here__;
    if (dwError != jtTTS_ERR_NONE)
    {
        return jtTTS_ERR_INPUT_PARAM;
    }

    /* ����� */
    pHeap = (unsigned char *)esMEMS_Balloc(nSize);
    //pHeap = (unsigned char *)esMEMS_Malloc(0,nSize);
    eLIBs_memset(pHeap, 0, nSize);
    /* ��ʼ������ */
#ifdef DATA_MODE_SMALL_FILE
    // �ļ���ȡ��ʽ
    dwError = jtTTS_Init(strTTSCNFileName[tts_speaker_index], strTTSENFileName[tts_speaker_index], NULL, &_tts_play_struct->jt_hTTS, pHeap);
#elif DATA_MODE_FIX_CONST
    // ֱ�Ӷ�ȡ��ʽ
    dwError = jtTTS_Init(pDicData, pDicData_En, NULL, &_tts_play_struct->jt_hTTS, pHeap);
#endif

    //__here__;
    if (dwError != jtTTS_ERR_NONE)
    {
        return jtTTS_ERR_NOT_INIT;
    }

    /*��TTS��������������*/
    //  ����ֱ���ı�����
    //   dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_DIRECT);
    //  dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_CALLBACK);
    /* ���ûص��ı����� */
    //  dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_CALLBACK);
    return jtTTS_ERR_NONE;
}

/*TTS�������л���������Ա��ʱ����Ҫ��TTS������з���ʼ��*/
jtErrCode tts_speaker_deint(void)
{
#if defined Have_eJTTS_lib
    jtErrCode           dwError;            // ������
    __s32                     tts_speaker_index;
    jtUserData             myUserData;
    /* ���ʼ�� */
    jtTTS_End(_tts_play_struct->jt_hTTS);
    /* �ͷ��û��ռ���Դ */
#ifdef DATA_MODE_FIX_CONST

    if (pDicData != NULL)
    {
        esMEMS_Mfree(0, pDicData);
    }

    if (pDicData_En != NULL)
    {
        esMEMS_Mfree(0, pDicData_En);
    }

#endif

    if (pHeap != NULL)
    {
        esMEMS_Bfree(pHeap, nSize);
    }

    tts_speaker_index = tts_get_voice_speaker();
    __msg("tts_speaker_index=%d\n", tts_speaker_index);
#ifdef DATA_MODE_SMALL_FILE
    // �ļ���ʽ����Ҫ����
#elif DATA_MODE_FIX_CONST
    // ֱ�Ӷ�ȡ���ڴ棩��ʽ�轫��Դ���ݷ����ڴ�
    fpVoiceDat = eLIBs_fopen(strTTSCNFileName[tts_speaker_index], "rb");
    eLIBs_fseek(fpVoiceDat, 0, SEEK_END);
    iDicLen = eLIBs_ftell(fpVoiceDat);
    fpENVoiceDta = eLIBs_fopen(strTTSENFileName[tts_speaker_index], "rb");
    eLIBs_fseek(fpENVoiceDta, 0, SEEK_END);
    iDicLen_En = eLIBs_ftell(fpENVoiceDta);
    pDicData = (const signed char *)esMEMS_Malloc(0, iDicLen);
    eLIBs_fseek(fpVoiceDat, 0, SEEK_SET);
    eLIBs_fread(pDicData, 1, iDicLen, fpVoiceDat);
    eLIBs_fclose(fpVoiceDat);
    pDicData_En = (const signed char *)esMEMS_Malloc(0, iDicLen_En);
    eLIBs_fseek(fpENVoiceDta, 0, SEEK_SET);
    eLIBs_fread(pDicData_En, 1, iDicLen_En, fpENVoiceDta);
    eLIBs_fclose(fpENVoiceDta);
#endif
    /* ���HEAP��С */
#ifdef DATA_MODE_SMALL_FILE
    dwError = jtTTS_GetExtBufSize(strTTSCNFileName[tts_speaker_index], strTTSENFileName[tts_speaker_index], NULL, &nSize);
#elif DATA_MODE_FIX_CONST
    dwError = jtTTS_GetExtBufSize(pDicData, pDicData_En, NULL, &nSize);
#endif

    if (dwError != jtTTS_ERR_NONE)
    {
        return jtTTS_ERR_INPUT_PARAM;
    }

    /* ����� */
    pHeap = (unsigned char *)esMEMS_Balloc(nSize);
    eLIBs_memset(pHeap, 0, nSize);
    /* ��ʼ������ */
#ifdef DATA_MODE_SMALL_FILE
    // �ļ���ȡ��ʽ
    dwError = jtTTS_Init(strTTSCNFileName[tts_speaker_index], strTTSENFileName[tts_speaker_index], NULL, &_tts_play_struct->jt_hTTS, pHeap);
#elif DATA_MODE_FIX_CONST
    // ֱ�Ӷ�ȡ��ʽ
    dwError = jtTTS_Init(pDicData, pDicData_En, NULL, &_tts_play_struct->jt_hTTS, pHeap);
#endif

    if (dwError != jtTTS_ERR_NONE)
    {
        return jtTTS_ERR_NOT_INIT;
    }

    //  ����ֱ���ı�����
    dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_DIRECT);
    //  ���úϳ��ı�����ص�
    dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_INPUT_CALLBACK, (long)jtExam_InputTextProc);
    //  ������Ƶ����ص�
    dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_OUTPUT_CALLBACK, (long)jtExam_OutputVoiceProc);
    //  ���ûص��û�����
    myUserData.pInputFile = NULL;
    myUserData.pOutputFile = fpOutPCM;
    myUserData.hTTS = _tts_play_struct->jt_hTTS;
    dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_CALLBACK_USERDATA, (long)&myUserData);
#endif //#if defined Have_eJTTS_lib
    return jtTTS_ERR_NONE;
}
void   TTS_init_all_res(void)
{
#if defined Have_eJTTS_lib

    jtUserData             myUserData;
    jtErrCode           dwError;            // ������
    tts_char_parser_init();//  ��ʼ��TXT������Դ
    //  ��ʼ��TTS ϵͳ��Դ
    tts_res_init();
    //__here__;
    //  ����ֱ���ı�����
    dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_DIRECT);
    //__here__;
    //   dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_CALLBACK);
    /* ������Ƶ����ص� */
    //  dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_INPUT_CALLBACK, (long)jtExam_InputTextProc);
    //  ������Ƶ����ص�
    dwError =  jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_OUTPUT_CALLBACK, (long)jtExam_OutputVoiceProc);
    //  ���ûص��û�����
    //   myUserData.pInputFile = fpinPCM;
    myUserData.pInputFile = NULL;
    myUserData.pOutputFile = fpOutPCM;
    myUserData.hTTS = _tts_play_struct->jt_hTTS;
    dwError =  jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_CALLBACK_USERDATA, (long)&myUserData);
#endif //#if defined Have_eJTTS_lib
}

__s32 TTS_res_release(void)
{
#if defined Have_eJTTS_lib
    __u8 err;
    tts_tone_exit();
    /* ���ʼ�� */
    jtTTS_End(_tts_play_struct->jt_hTTS);
    /* �ͷ��û��ռ���Դ */

    if (fpOutPCM)
    {
        eLIBs_fclose(fpOutPCM);
        fpOutPCM = NULL;
    }

    //      eLIBs_fclose(fpinPCM);
#ifdef DATA_MODE_FIX_CONST

    if (pDicData != NULL)
    {
        esMEMS_Mfree(0, pDicData);
    }

    if (pDicData_En != NULL)
    {
        esMEMS_Mfree(0, pDicData_En);
    }

#endif

    if (pHeap != NULL)
    {
        esMEMS_Bfree(pHeap, nSize);
    }

    esKRNL_SemDel(_tts_play_struct->tts_sem, OS_DEL_ALWAYS, &err);
    tts_char_parser_exit();
    return EPDK_OK;
#endif  //#if defined Have_eJTTS_lib
return EPDK_FAIL;
}

/*��ȡ��ǰ��Ƶ����������ݿռ�buffer�ж���*/
__s32  get_cur_audio_buffer(void)
{
    __s32    buffer_size = 0;
    unsigned long arg[4] = {0};
    if (f_audiodev)
    {
        arg[0] = AUDIO_DEV_QUERY_BUF_SIZE_DATA,
        arg[1] = 0,    
        buffer_size = ioctl(f_audiodev, AUDIO_DEV_CMD_QUERY_BUFSIZE,(void*)arg);
    }

    return buffer_size;
}

__s32  get_cur_audio_totalbuffer(void)
{
    __s32    buffer_size = 0;
    unsigned long arg[4] = {0};
    if (f_audiodev)
    {
        arg[0] = AUDIO_DEV_QUERY_BUF_SIZE_TOTAL,
        arg[1] = 0,    
        buffer_size = ioctl(f_audiodev, AUDIO_DEV_CMD_QUERY_BUFSIZE, (void*)arg);
    }

    return buffer_size;
}

__s32 set_cur_audio_totalbuffer(__u32 buf_size)
{
    __s32 ret = 0;
    unsigned long arg[4] = {0};
    if (f_audiodev)
    {
        arg[0] = buf_size,
        arg[1] = 0,
        ret =  ioctl(f_audiodev, AUDIO_DEV_CMD_RESIZE_BUF, (void*)arg);
    }

    return ret;
}




__s32  get_cur_audio_freedbuffer(void)
{
    __s32    buffer_size = 0;
    unsigned long arg[4] = {0};
    if (f_audiodev)
    {
        arg[0] = AUDIO_DEV_QUERY_BUF_SIZE_FREE,
        arg[1] = 0,
        buffer_size = ioctl(f_audiodev, AUDIO_DEV_CMD_QUERY_BUFSIZE,(void*)arg);
    }

    return buffer_size;
}


/*�ж��Ƿ�Ի��������������*/
__bool  cur_audio_speark_over(void)
{
    __s32    buffer_size = 0;
    __bool   ret_boot;
    unsigned long arg[4];
    if (f_audiodev)
    {
        arg[0] = AUDIO_DEV_QUERY_BUF_SIZE_DATA;
        arg[1] = 0;
        buffer_size = ioctl(f_audiodev, AUDIO_DEV_CMD_QUERY_BUFSIZE,(void*)arg);
        //__msg("----f_audiodev = %x    ,buffer_size = %d\n ",f_audiodev, buffer_size)  ;

        if (buffer_size <= 0)
        {
            ret_boot = EPDK_TRUE;
        }
        else
        {
            ret_boot = EPDK_FALSE;
        }
    }
    else
    {
        ret_boot = EPDK_FALSE;
    }

    return ret_boot;
}



#if 0
/**/
void  delete_cuckoo(void)
{
    __s32  ret;
    /*if music is play, kill it*/
    ret = dsk_kill_app("music.app");

    if (ret != -1)
    {
        while (is_app_exist(APP_MUSIC))
        {
            esKRNL_TimeDly(1);
        }
    }
}
#endif

static void tts_play_thread_start(void *arg)
{
    __u8         err;
    __u32  output_size = 0; /*���˸ոտ�ʼ����֮�⣬������ֵ���ú������ص�*/
    __bool  b_analysys_first_line = EPDK_TRUE;      /*�Ƿ��Ƿ�����һ�����*/
    jtErrCode           dwError;            // ������
    __tts_thread_argmen_t *pthread_argment = (__tts_thread_argmen_t *)arg;

    while (1)
    {
        if (esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
        {
            esKRNL_TDel(OS_PRIO_SELF);
            esMEMS_Mfree(0, pthread_argment);
            return;
        }

        if (b_analysys_first_line == EPDK_TRUE)
        {
            __msg("---���ݳ���=%d      = %s \n", pthread_argment->char_num, pthread_argment->data_add);
            _tts_play_struct->tts_text_line_add = (char *)pthread_argment->data_add;
            _tts_play_struct->tts_page_leave_char = pthread_argment->char_num;
            _tts_play_struct->tts_text_curt_statr = (char *)pthread_argment->data_add;  /*��¼��ǰҳ��ʼλ��*/
            TTS_ANALYSYS_Page(_tts_play_struct->encode_type, pthread_argment->data_add, _tts_play_struct->tts_page_leave_char, &output_size);
            b_analysys_first_line = EPDK_FALSE;
            continue;
        }

        __msg("--------------output_size = %d\n", output_size);

        if (output_size != 0)
        {
            esKRNL_TimeDly(5);
            _tts_play_struct->tts_text_line_add = _tts_play_struct->tts_text_line_add + output_size;
            _tts_play_struct->tts_page_leave_char -= output_size;
            TTS_ANALYSYS_Page(_tts_play_struct->encode_type, _tts_play_struct->tts_text_line_add, _tts_play_struct->tts_page_leave_char, &output_size);
        }
        else
        {
            /*һҳ�������Ѿ��������*/
            __gui_msg_t my_msg;
            my_msg.h_deswin = (H_WIN)pthread_argment->woner_win;
            my_msg.id = GUI_MSG_COMMAND;
            my_msg.dwAddData1 = MAKELONG(EBOOK_TTS_ID, CMD_TTS_SET_NEXT);

            if (b_analysys_first_line == EPDK_FALSE)
            {
                GUI_SendMessage(&my_msg);
                __msg("------------------����Ϣ������һҳ���Ķ�\n");
                b_analysys_first_line = EPDK_TRUE;
            }
        }
    }
}


/*
************************************************************************************************************************
*                                       tts_play_from_page_start
*
*Description: ��ȡ�Ƿ���Ҳ��ͷ����
*
*Arguments  :
*Return     : true �Ļ��������´�ͷ��ʼ��
*           flase �����ص�ǰ�俪ʼ��
*
************************************************************************************************************************
*/

__bool  tts_play_from_page_start(void)
{
    if (_tts_play_struct->tts_from_page_stat == EPDK_TRUE)
    {
        return EPDK_FALSE;
    }
    else
    {
        return EPDK_TRUE;
    }
}

__s32  tts_play_thread_end(void)
{
    jtErrCode jt_errcode;
#if defined Have_eJTTS_lib
    jt_errcode =  jtTTS_SynthStop(_tts_play_struct->jt_hTTS);       /*�����ϳ�*/
    __msg("%s-------------jt_errcode = %d\n", __FUNCTION__, jt_errcode);
    esMODS_MIoctrl(f_audiodev, AUDIO_DEV_CMD_FLUSH_BUF, 0, 0);   /*�����������*/

    /* delete autoplay thread */
    if (_tts_play_struct->tts_thread != 0)
    {
        while (esKRNL_TDelReq(_tts_play_struct->tts_thread) != OS_TASK_NOT_EXIST)
        {
            esKRNL_TimeDly(10);
        }

        _tts_play_struct->tts_thread = 0;
        __msg("thread has been deleted.\n");
    }
#endif
    return 0;
}


/*
************************************************************************************************************************
*                                       TTS_play_server_init
*
*Description: �ڴ�tts ʱ���ʼ��TTS ������ڲ�����
*
*Arguments  : arg: �������Ӧ���ļ��������
*             arg1: �����鴫������BUF
*
*Return     : ���ش�����
*
************************************************************************************************************************
*/

jtErrCode   TTS_play_server_init(void *arg, void *arg1)
{
    __u8         err;
    jtErrCode           dwError = 0;            // ������
    int  encode;
    int tts_encode_type;
    __s32 buffsize;

#if defined Have_eJTTS_lib

    //  delete_cuckoo();
    __wrn("Enter %s\r\n", __FUNCTION__);
    TTS_init_all_res();
    encode = tts_get_txtCharEncodeType(arg);
    __msg("encode is %d\n", encode);
    tts_encode_type = tts_get_char_encode_type(encode);
    __msg("tts_encode_type is %d", tts_encode_type);
    //__here__;
    jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_VOLUME, jtTTS_VOLUME_NORMAL);
    jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_SPEED, jtTTS_SPEED_NORMAL);
    jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_PITCH, jtTTS_PITCH_NORMAL);

    if (arg1 != NULL)
    {
        __epdk_charset_enm_e charset;
        charset = EBookGetSysDefaultCharset();

        if (EPDK_CHARSET_ENM_BIG5 == charset)
        {
            /*��ʲôTTS*/
            dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_CODEPAGE, jtTTS_CODEPAGE_BIG5);
        }
        else if (EPDK_CHARSET_ENM_GBK == charset)
        {
            /*��ʲôTTS*/
            dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_CODEPAGE, jtTTS_CODEPAGE_GBK);
        }
        else
        {
            /*��ʲôTTS*/
            dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_CODEPAGE, jtTTS_CODEPAGE_GBK);
        }

        esKRNL_SemPend(_tts_play_struct->tts_sem, 0, &err);
        dwError = jtTTS_SynthesizeText(_tts_play_struct->jt_hTTS, arg1, strlen(arg1) * sizeof(char));
        esKRNL_SemPost(_tts_play_struct->tts_sem);
    }

    dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_CODEPAGE, tts_encode_type);
    _tts_play_struct->encode_type = encode;
#endif //#if defined Have_eJTTS_lib
    return dwError ;
}

void TTS_play_server_start(void *arg, void *arg1, __s32 char_num, H_WIN  woner_win)
{
#if defined Have_eJTTS_lib
    __tts_thread_argmen_t *thread_argment;
    thread_argment = (__tts_thread_argmen_t *)esMEMS_Malloc(0, sizeof(__tts_thread_argmen_t));
    thread_argment->data_add = (char *)arg1;
    __msg("------char_num = %d\n", char_num);
    thread_argment->char_num = char_num;
    thread_argment->woner_win = (unsigned int)woner_win;
    dsk_set_auto_off_time(0);

    if (_tts_play_struct == NULL)
    {
        __u8         err;
        jtErrCode           dwError;            // ������
        int  encode;
        int tts_encode_type;
        __s32 buffsize;
        //      delete_cuckoo();
        TTS_init_all_res();
        encode = tts_get_txtCharEncodeType(arg);
        __msg("encode is %d\n", encode);
        tts_encode_type = tts_get_char_encode_type(encode);
        __msg("tts_encode_type is %d", tts_encode_type);
        //__here__;
        dwError = jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_CODEPAGE, tts_encode_type);
        jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_VOLUME, jtTTS_VOLUME_NORMAL);
        jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_SPEED, jtTTS_SPEED_NORMAL);
        jtTTS_SetParam(_tts_play_struct->jt_hTTS, jtTTS_PARAM_PITCH, jtTTS_PITCH_NORMAL);
        //__here__;
        __msg("arg1 is %s\n", arg1);
        buffsize = get_cur_audio_totalbuffer();
        _tts_play_struct->encode_type = encode;
        __msg("totalbuffsize is %d\n", buffsize);
        //  __msg("arg1[0] is %d,arg1[1] is %d",arg1[0],arg1[1]);
    }

    _tts_play_struct->tts_from_page_stat = EPDK_TRUE;       //��־�´ζ��ӷ���������俪ʼ��
    _tts_play_struct->tts_thread = esKRNL_TCreate(tts_play_thread_start, thread_argment, 0x1000, KRNL_priolevel5);
    //  esKRNL_SemPend(_tts_play_struct->tts_sem, 0, &err);
    //  jtTTS_Synthesize(_tts_play_struct->jt_hTTS);
    //   dwError = jtTTS_SynthesizeText(_tts_play_struct->jt_hTTS, arg1, strlen(arg1) * sizeof(char));
    //__msg("arg1----= %d\n", strlen(arg1));
    //  TTS_ANALYSYS_Page(encode, arg1, strlen(arg1) * sizeof(char));
    //  esKRNL_SemPost(_tts_play_struct->tts_sem);
#endif //#if defined Have_eJTTS_lib
}

void TTS_stop_server(void)
{
    //���û�к�̨��Ƶ���������Զ��ػ�
    if (EPDK_FALSE == is_app_exist(APP_MUSIC)
#if 0// shiql set 0 for D100
        && EPDK_FALSE == is_app_exist(APP_LINEIN)
#endif
       )
    {
        reg_system_para_t *para;
        para = (reg_system_para_t *)dsk_reg_get_para_by_app(REG_APP_SYSTEM);

        if (para)
        {
            dsk_set_auto_off_time(para->poweroff);
            __wrn("para->poweroff=%d\n", para->poweroff);
        }
    }

    if (_tts_play_struct->tts_thread != 0)
    {
        tts_play_thread_end();
    }

    _tts_play_struct->tts_from_page_stat = EPDK_FALSE;      /*��־���´�ҳͷ���ݿ�ʼ��*/
    TTS_res_release();
}
