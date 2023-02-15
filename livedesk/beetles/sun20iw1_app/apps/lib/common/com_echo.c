#include <log.h>
#include "apps.h"
#include "com_echo.h"

ES_FILE *g_fp_rec = NULL;

//type:AUDIO_REC_USR_MIC/AUDIO_REC_USR_FM....
//auto_clean_buf:0�������Զ����buffer(¼��Ӧ��)��Ϊ1�����Զ����buffer(��¼����Ӧ��)
//��ϸ���÷����μ�SDK��app_root_scene.c��record_view_doc.c
__s32 com_audio_echo_open(__audio_rec_user_t type, __s32 auto_clean_buf)
{
#if 0/*langaojie temp test*/
    __s32 aux;
    __audio_dev_para_t adev_para;

    if (g_fp_rec)
    {
        __wrn("rec hdl alreadly open...\n");
        return EPDK_FAIL;
    }

    g_fp_rec = eLIBs_fopen("b:\\AUDIO\\REC", "r+");

    if (NULL == g_fp_rec)
    {
        __wrn("open rec hdl fail...\n");
        return EPDK_FAIL;
    }

    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_REG_USERMODE, type, 0);
    adev_para.chn = 2;
    adev_para.bps = 16;
    adev_para.fs = 44100;
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_SET_PARA, 0, (void *)&adev_para);
    //esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_REG_USERMODE, type, 0);
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_SET_SAMPCNT, 0, 0);//��adc buffer������;
    aux = 1;
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO, aux, 0);
    aux = auto_clean_buf;
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_BUF_MODE, aux, 0);
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_START, 0, 0);
    com_audio_echo_set_mic_pre_gain(1);
    com_audio_echo_set_mic_gain(0);
#endif
    return EPDK_OK;
}

//��ϸ���÷����μ�SDK��app_root_scene.c��record_view_doc.c
__s32 com_audio_echo_close(void)
{
    __s32 aux;
#if 0/*langaojie temp test*/

    if (NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }

    aux = 0;
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_ENABLE_ECHO, aux, 0);
    aux = 0;
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_BUF_MODE, aux, 0);
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_STOP, 0, 0);

    if (g_fp_rec)
    {
        eLIBs_fclose(g_fp_rec);
        g_fp_rec = NULL;
    }
#endif
    return EPDK_OK;
}

//�ú���һ�㲻Ҫ���ã�����Ĭ�ϵľ�����
//0<= pre_gain <= 3
__s32 com_audio_echo_set_mic_pre_gain(__s32 pre_gain)
{
#if 0/*langaojie temp test*/

    pre_gain &= 0x03;
    *(volatile unsigned long *)(0xf1c23c28) &= ~(0x03 << 25);
    *(volatile unsigned long *)(0xf1c23c28) |= (pre_gain << 25);
#endif
    return EPDK_OK;
}

//�ú���һ�㲻Ҫ���ã�����Ĭ�ϵľ�����
//0<= gain <= 7
__s32 com_audio_echo_set_mic_gain(__s32 gain)
{
#if 0/*langaojie temp test*/

    gain &= 0x07;
    *(volatile unsigned long *)(0xf1c23c28) &= ~(0x07 << 20);
    *(volatile unsigned long *)(0xf1c23c28) |= (gain << 20);
#endif
    return EPDK_OK;
}

//����mic������-32��32�����Ե���mic����
__s32 com_audio_echo_gain_incr(void)
{
    __s32 aux;
    
    __wrn("com_audio_echo_gain_incr...\n");
#if 0/*langaojie temp test*/

    if (NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }

    aux = esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_GET_ECHO_REC_GAIN, 0, 0);

    if (aux < 32)
    {
        aux++;
    }

    __wrn("com_audio_echo_gain_incr, aux=%d\n", aux);
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_REC_GAIN, aux, 0);
#endif
    return EPDK_OK;
}

//��Сmic���� -32��32�����Ե���mic����
__s32 com_audio_echo_gain_decr(void)
{
    __s32 aux;
#if 0/*langaojie temp test*/

    if (NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }

    aux = esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_GET_ECHO_REC_GAIN, 0, 0);

    if (aux > -32)
    {
        aux--;
    }

    __wrn("com_audio_echo_gain_decr, aux=%d\n", aux);
    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_REC_GAIN, aux, 0);
#endif
    return EPDK_OK;
}

//�ú��������ã�������
__s32 com_audio_echo_pt_regs(__u32 start, __u32 end)
{
    __u32 i;
    eLIBs_printf("*****com_audio_echo_pt_regs begin******\n");
#if 0/*langaojie temp test*/

    for (i = start ; i <= end ; i += 4)
    {
        eLIBs_printf("0x%8x:0x%8x\n", i, *(volatile unsigned long *)i);
    }

    eLIBs_printf("*****com_audio_echo_pt_regs end******\n");
#endif
    return EPDK_OK;
}

/*aux: 0<= aux <= 10 ���û�����ʱ������Ĭ��Ϊ3*/
__s32 com_audio_echo_set_delay_num(__s32 aux)
{
    __wrn("com_audio_echo_set_delay_num...\n");
#if 0/*langaojie temp test*/

    if (NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }

    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_DELAY_NUM, aux, 0);
#endif
    return EPDK_OK;
}

/*aux:10<= aux <= 3000,Ĭ��Ϊ100���Ժ���Ϊ��λ*/
__s32 com_audio_echo_set_delay_time(__s32 aux)
{
    __wrn("com_audio_echo_set_delay_num...\n");
#if 0/*langaojie temp test*/

    if (NULL == g_fp_rec)
    {
        __wrn("rec hdl not open...\n");
        return EPDK_FAIL;
    }

    esMODS_MIoctrl(g_fp_rec, AUDIO_DEV_CMD_SET_ECHO_DELAY_TIME, aux, 0);
#endif
    return EPDK_OK;
}




