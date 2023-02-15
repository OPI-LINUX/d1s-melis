#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <rtthread.h>
#include <hal_mem.h>
#include"mod_defs.h"
//#include"mod_willow.h"

#include"log.h"
#if 1

/*willow support command*/
typedef enum
{
    WILLOW_CMD_QUERY_STATUS          = 0x0,  /* ��ѯ״̬��
                                              * ����ֵΪ__willow_img_status_t
                                              */

    WILLOW_CMD_GET_THUMBS            = 0x1,  /* ���û�ȡ����ͼ����Ҫ����Ϣ
                                              * pbuffer = (__willow_get_thumbs_param_t *)
                                              */

    WILLOW_CMD_SHOW_IMG_FROM_FILE    = 0x2,  /* ������ʾͼƬ���ļ���Ϣ
                                              * pbuffer = (__willow_show_file_param_t *)
                                              */

    WILLOW_CMD_SHOW_IMG_FROM_BUFFER  = 0x3,  /* ������ʾ��argb����
                                              * pbuffer = (__willow_show_buffer_param_t *)
                                              * ע�⣬������Ҫ��buffer�е�����Ϊargb8888,
                                              * �����ǽ���buffer�е�ͼƬ��
                                              * ����buffer�е�ͼƬʹ������WILLOW_CMD_CFG_INPUT
                                              */

    WILLOW_CMD_SET_SWITCH_MODE       = 0x4,  /* ����ͼƬ���ʱ����л�ģʽ
                                              * aux = (__willow_switch_mode_t)
                                              */


    WILLOW_CMD_GET_SHOW_PARAM        = 0x5,  /* ��ȡ��ǰ��ʾ����
                                              * pbuffer = (__willow_showing_img_info_t *)
                                              */

    WILLOW_CMD_GET_IMG_INFO          = 0x6,  /* ��ȡͼƬ��Ϣ
                                              * pbuffer = (__willow_img_info_t *)
                                              */
    /*change current img show*/
    WILLOW_CMD_SCALE                 = 0x7,  /* ��С/�Ŵ�ͼƬ
                                              * aux = -3��-2��-1��0��1��2��3
                                              * �ֱ������С8/4/2����ԭʼ��С���Ŵ�2/4/8��
                                              */

    WILLOW_CMD_ROTATE                = 0x8,  /* ��תͼƬ
                                              * aux = -270, -180, -90, 0, 90, 180, 270
                                              * ������ʾ��ʱ����ת,������ʾ˳ʱ����ת
                                              */

    WILLOW_CMD_MOVE                  = 0x9,  /* �ƶ�ͼƬ����ͼƬ�Ŵ����Ч
                                              * pbuffer = (__pos_t *) Ϊ�ƶ���offset
                                              */

    WILLOW_CMD_COME_BACK             = 0xA,  /* ������С/�Ŵ�ͼƬ�󷵻�ԭʼ��С*/

    WILLOW_CMD_OPEN_SHOW             = 0xB,  /* ��ͼƬ���*/
    WILLOW_CMD_SHUT_SHOW             = 0xC,  /* �ر�ͼƬ���*/

    WILLOW_CMD_CHECK_IMG             = 0xD,  /* ���ͼƬ�Ƿ�֧��*/
    WILLOW_CMD_START_DEC             = 0xE,  /* ��ʼ����*/
    WILLOW_CMD_START_SHOW            = 0xF,  /* ��ʾͼƬ����ȡ����ͼ*/
    WILLOW_CMD_STOP                  = 0x10, //

    WILLOW_CMD_SET_ALBUM_ART         = 0x11, /* ����ר��ͼƬ��Ϣ,
                                              * pbuffer = (__willow_get_albumart_param_t *)
                                              */
    WILLOW_CMD_SET_SCN               = 0x12, /* ����slider show����ʾ����
                                              * pbuffer = __rect_t
                                              */
    WILLOW_CMD_GET_VER               = 0x13, /* ��ȡwillow�汾��
                                              * (__u32 *)pbuffer = WILLOW_VERSION
                                              */
    WILLOW_CMD_START_SHOW_EXT        = 0x14, /* ��ȡ����ͼ,Ԥ���û�ָ����䷽ʽ,
                                              * pbuffer = (__u32 *)(fill_mode | clear_flag)
                                              * fill_mode����Ϊ: WILLOW_SCALE_RATIO,
                                              *                  WILLOW_SCALE_STRETCH
                                              *                  WILLOW_SCALE_RATIO_SCALE
                                              *                  WILLOW_SCALE_RATIO_CUTOFF
                                              * clear_flag����Ϊ WILLOW_SCALE_CLEAR_BK
                                              *                  WILLOW_SCALE_CLEAR_NONE
                                              * ���庬����ο��궨���е�ע�͡�
                                              */

    WILLOW_CMD_CATCH_SCREEN          = 0x15, /* ��ȡ��ǰ��ʾ��fb
                                              * pbuffer = (FB *)
                                              */

    WILLOW_CMD_AUTO_ROTATE           = 0x16, /* ���߶� > ���ʱ���Ƿ���Ҫ�Զ���ת��
                                              * aux = __willow_rotate_mode_t
                                              */

    WILLOW_CMD_GET_ERRNO             = 0x17, /* ��ȡ����ţ�Ŀǰδ�ṩ*/

    WILLOW_CMD_SET_SHOW_MODE         = 0x18, /* ������ʾģʽ
                                              * aux = __willow_image_show_mode_t��
                                              * ��aux = WILLOW_IMAGE_SHOW_USRDEF�ǣ�pbuffer = __pCBK_t��
                                              * ����Ϊ __willow_showmod_usr_para_t
                                              */

    WILLOW_CMD_CFG_LYR               = 0x19, /* ����willowͼ������,
                                              * aux = __willow_lyr_cfg_t
                                              */

    WILLOW_CMD_ROT_LYR               = 0x20, /* ��ͼƬ��תһ���ǶȺ���ʾ, ע��ͼƬ������ʾ����ת
                                              * ����ΪWILLOW_CMD_ROT_LYR�� WILLOW_CMD_AUTO_ROTATE��
                                              * �߹�ͬ���õĽ����
                                              * aux = __willow_rotate_mode_t
                                              */

    WILLOW_CMD_CFG_OUTPUT            = 0x21, /* ���������ָ����fb�У�
                                              * pbuffer = (__willow_output_t *)
                                              */

    WILLOW_CMD_CFG_INPUT             = 0x22, /* ����ָ�������뷽ʽ��
                                              * aux = __willow_input_src_e
                                              * ��aux = WILLOW_INPUT_BUFFERʱ,pbuffer = (__willow_buf_info_t *)
                                              */

    WILLOW_CMD_SET_PLAYLIST          = 0x23, /* ����ͼƬԤ������Ϣ
                                              * pbuffer = (__willow_img_plst_info_t *)
                                              */

    WILLOW_CMD_MOV_TRACK             = 0x24, /* ������ק�˶��켣
                                              * pbuffer = (slider_step_para_t *)
                                              */

    WILLOW_CMD_GET_CUR_INDEX         = 0x25, /* ��ȡ��ק������ͼƬ�ڲ����б��е�index
                                              * ����index
                                              */

    WILLOW_CMD_SCALE_EXT             = 0x26, /* �޼�����ͼƬ,��������ʵ��С��WILLOW_SCALER_MAX_RATIO��������ͼƬ
                                              * aux = �Ŵ��� * WILLOW_SCALER_BASE_RATIO
                                              * aux = 2300��ʾ�Ŵ� 2.3��
                                              */
    WILLOW_CMD_CHECK_CUR             = 0x27,     /* ��ȡ��ק�������ȡ��ǰͼƬ�Ƿ�֧�� */

    WILLOW_CMD_SET_3D_MOD            = 0x100, /* ����3D ��ʾģʽ
                                               * aux = __willow_show_3d_mode_e
                                               */
    WILLOW_CMD_SET_COLOR_MOD         = 0x101, //����ɫ��ģʽ,�ڰ�or��ɫ
    WILLOW_CMD_SET_SW_SCALE          = 0x201, //�ͻ�ѡ����Ӳ��scale�������scale.ͬ�Ե�ʱ��deһֱ��ռ���ˣ�����willow����ʹ��Ӳ��scale.�����������ֻ��ʹ�����scale
    WILLOW_CMD_SET_USR_LAYER_CNT,    /*Ӧ�����öര����ʾͼƬ��1,2,3,4Ĭ����1��*/
} __willow_main_cmd_t;
/*willow status*/
typedef enum
{
    WILLOW_STATUS_INIT,
    WILLOW_STATUS_BUSY,
    WILLOW_STATUS_READY,
    WILLOW_STATUS_FINISH
} __willow_img_status_t;

typedef struct
{
    char        *filename;                                    //file name
    __u32        img_no;
} __willow_show_file_param_t;

#define ANOLE_SYN_DELAY_TIME 50//

//#include "../hal/source/g2d_rcq/g2d_driver.h"
extern unsigned long dma_coherent_alloc(unsigned long *virt_addr, unsigned long size);
extern void dma_coherent_free(unsigned long virt_addr);
extern void esKRNL_TimeDly(__u16 ticks);

__mp *mod_willow;
__u8 mid_willow;

static int init_flag;

static void __wait_willow_ready(void)
{
    __u8 status;
    __s32 count;
    count = ANOLE_SYN_DELAY_TIME;
    /*check willow status*/
#ifndef SIM_PC_WIN

    while (1)
    {
        //__here__;
        status = esMODS_MIoctrl(mod_willow, WILLOW_CMD_QUERY_STATUS, 0, 0);

        //__here__;
        if (status != WILLOW_STATUS_BUSY)
        {
            break;
        }

        //esKRNL_TimeDly(10);
        esKRNL_TimeDly(1);
        count--;
        __msg("count=%d", count);
    }

#endif
}
/*
**********************************************************************************************************************
*                                               __wait_willow_finish
*
* Description:
*
* Arguments  :
*
* Returns    :
*
* Notes      :
*
**********************************************************************************************************************
*/

static void __wait_willow_finish(void)
{
    __u8 status;
    __s32 count;
    count = ANOLE_SYN_DELAY_TIME;

    /*check willow status*/
    while (1)
    {
        //__here__;
        status = esMODS_MIoctrl(mod_willow, WILLOW_CMD_QUERY_STATUS, 0, 0);

        //__here__;
        if (status == WILLOW_STATUS_FINISH)
        {
            break;
        }

        //esKRNL_TimeDly(10);
        esKRNL_TimeDly(1);      ///by linyaoshu
        count--;
    }
}

static __s32 __do_play_file(__willow_show_file_param_t *param)
{
    __msg("index = %d, file name = %s", param->img_no, param->filename);

    //set show img node
    __wait_willow_ready();
    //__here__;
    esMODS_MIoctrl(mod_willow, WILLOW_CMD_SHOW_IMG_FROM_FILE, 0, param);
    //__here__;
    __wait_willow_finish();
    //__here__;

    //check if the format is supported
    if (esMODS_MIoctrl(mod_willow, WILLOW_CMD_CHECK_IMG, 0, param) == EPDK_FAIL)
    {
        __msg("the file format is not supported!");
        __wait_willow_finish();
        //__here__;
        return EPDK_FAIL;
    }

    //__here__;
    //start decoding
    esMODS_MIoctrl(mod_willow, WILLOW_CMD_START_DEC, 0, 0);
    //__here__;
    __wait_willow_finish();
    //__here__;
    //TestTimerReport(__FILE__, __LINE__);
    //start show
    __wait_willow_ready();
    //__here__;
    //TestTimerReport(__FILE__, __LINE__);

    esMODS_MIoctrl(mod_willow, WILLOW_CMD_START_SHOW, 0, 0);
    //__here__;
    //TestTimerReport(__FILE__, __LINE__);
    __wait_willow_finish();
    //__here__;
    //TestTimerReport(__FILE__, __LINE__);
    return EPDK_OK;
}
#endif

char *g_file[6] =
{
    "e:\\1.jpg",
    "e:\\2.jpg",
    "e:\\3.jpg",
    "e:\\4.jpg",
    "e:\\5.jpg",
    "e:\\6.jpg"
};

void cmd_willow_close(void)
{
    //__hdle   hnode = NULL;
    //char     key_value[ELIBS_KEY_VALUE_LEN + 1];

    __log("********before __wait_willow_ready********");
    __wait_willow_ready();
    __msg("********after __wait_willow_ready********");
    esMODS_MIoctrl(mod_willow, WILLOW_CMD_SHUT_SHOW, 0, 0);
    __msg("********before __wait_willow_finish********");
    __wait_willow_finish();
    __log("********after __wait_willow_finish********");
    /* �رպ�ж���м��willow   */
    esMODS_MClose(mod_willow);
    //__here__
    esMODS_MUninstall(mid_willow);
    //__here__;
	mod_willow = NULL;
	mid_willow = 0;
    return;
}

static int cmd_willow_test(int argc, const char **argv)
{
    //__mp *mod_willow;
  //  __u8 mid_willow;
   __willow_show_file_param_t param;
   char filename[]="e:\\test.png";
   int ret =-1;
   FILE *fp = NULL;
   int ii;


    printf("----------------cmd_willow_test--------!\n");
    printf("hello cmd_willow_test\n");
    printf("argv:");
    for(ii=0;ii<argc;ii++)
    {
        printf("%s,\n",argv[ii]);
    }
    printf("\n");

     fp = fopen(argv[1],"r+");
     if(fp==NULL)
     {
         printf("----------fp==NULL--------!\n");

     }
     else
     {
        fclose(fp);

        printf("---------fopen ok-------!\n");
     }

        /* install willow */
   // mid_willow = esMODS_MInstall("/data/mod/willow/willow.mod", 0);
	mid_willow = esMODS_MInstall("/mnt/E/mod/willow/willow.mod", 0);

    if (!mid_willow)
    {
        printf("install willow fail!");
        return -1;
    }
    printf("-------cmd_willow_test mid_willow:0x%x--------!\n",mid_willow);

    /*open willow*/
    mod_willow = esMODS_MOpen(mid_willow, 0);
    printf("-------cmd_willow_test mod_willow:0x%x--------!\n",mod_willow);

    if (mod_willow == NULL)
    {
        printf("open willow fail!");
        return -1;
    }
#if 0
    if (mod_willow)
    {
        esMODS_MClose(mod_willow);
        mod_willow = 0;
    }

    if (mid_willow)
    {
        esMODS_MUninstall(mid_willow);
        mid_willow = 0;
    }

    printf("----------------cmd_willow_test22222222222222--------!\n");
        /* install willow */
	mid_willow = esMODS_MInstall("/mnt/E/mod/willow/willow.mod", 0);

    if (!mid_willow)
    {
        printf("install willow fail!");
        return -1;
    }
    printf("-------cmd_willow_test mid_willow:0x%x--------!\n",mid_willow);

    /*open willow*/
    mod_willow = esMODS_MOpen(mid_willow, 0);

    if (mod_willow == 0)
    {
        printf("open willow fail!");
        return -1;
    }
 #endif
    printf("----------------cmd_willow_test 33333333--------!\n");
    esMODS_MIoctrl(mod_willow, WILLOW_CMD_OPEN_SHOW, 0, 0);

if(argc ==1)
{

    while(1)
    {
        ii = rand()%6;
        param.img_no    = ii;
        param.filename  = g_file[ii];//argv[1];

        printf("[%d]-param.filename:%s\n",ii,param.filename);
        ret = __do_play_file(&param);

        printf("-----ret:%d-------!\n",ret);
        esKRNL_TimeDly(500);
    }
}
else
{
         param.img_no    = 0;
        param.filename  = argv[1];

        printf("[]-param.filename:%s\n",param.filename);
        ret = __do_play_file(&param);

        printf("-----ret:%d-------!\n",ret);
}
   // __log("**********open cmd_willow_test 111********** \n");
    printf("----------------cmd_willow_test 22--------!\n");
    return 0;
}

int willow_test(void)
{
    //__mp *mod_willow;
  //  __u8 mid_willow;
   __willow_show_file_param_t param;
   //char filename[]="e:\\test.png";
   char filename[]="e:\\jianjin.jpg";
   int ret =-1;
   FILE *fp = NULL;
   int ii;


    printf("----------------cmd_willow_test--------!\n");
    printf("hello cmd_willow_test\n");

        /* install willow */
   // mid_willow = esMODS_MInstall("/data/mod/willow/willow.mod", 0);
	mid_willow = esMODS_MInstall("/mnt/E/mod/willow/willow.mod", 0);

    if (!mid_willow)
    {
        printf("install willow fail!");
        return -1;
    }
    printf("-------cmd_willow_test mid_willow:0x%x--------!\n",mid_willow);

    /*open willow*/
    mod_willow = esMODS_MOpen(mid_willow, 0);
    printf("-------cmd_willow_test mod_willow:0x%x--------!\n",mod_willow);

    if (mod_willow == NULL)
    {
        printf("open willow fail!");
        return -1;
    }
#if 0
    if (mod_willow)
    {
        esMODS_MClose(mod_willow);
        mod_willow = 0;
    }

    if (mid_willow)
    {
        esMODS_MUninstall(mid_willow);
        mid_willow = 0;
    }

    printf("----------------cmd_willow_test22222222222222--------!\n");
        /* install willow */
	mid_willow = esMODS_MInstall("/mnt/E/mod/willow/willow.mod", 0);

    if (!mid_willow)
    {
        printf("install willow fail!");
        return -1;
    }
    printf("-------cmd_willow_test mid_willow:0x%x--------!\n",mid_willow);

    /*open willow*/
    mod_willow = esMODS_MOpen(mid_willow, 0);

    if (mod_willow == 0)
    {
        printf("open willow fail!");
        return -1;
    }
 #endif
    printf("----------------cmd_willow_test 33333333--------!\n");
    esMODS_MIoctrl(mod_willow, WILLOW_CMD_OPEN_SHOW, 0, 0);


{
         param.img_no    = 0;
        param.filename  = filename;

        printf("[]-param.filename:%s\n",param.filename);
        ret = __do_play_file(&param);

        printf("-----ret:%d-------!\n",ret);
}
   // __log("**********open cmd_willow_test 111********** \n");
    printf("----------------cmd_willow_test 22--------!\n");
    return 0;
}


FINSH_FUNCTION_EXPORT_ALIAS(cmd_willow_test, __cmd_willow_test, willow_test);
FINSH_FUNCTION_EXPORT_ALIAS(cmd_willow_close, __cmd_willow_close, willow_close
);

