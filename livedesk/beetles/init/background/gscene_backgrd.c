/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2010, ANDY, China
*                                            All Rights Reserved
*
* File      : gscene_backgrd.c
* By        : Andy.zhang
* Func      : desktop toolbar control interface
* Version   : v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "mod_init_i.h"
#include "./fb_lib/backlayer_lib.h"
#include "./fb_lib/fb_file.h"
#include <kconfig.h>
#include <log.h>

#define BG_MAX_CHAR_LEN     768

#define BG_DEFAULT_PATH     BEETLES_APP_ROOT"res\\"


typedef struct tag_bgd_com
{
    __mp     *de_hdle;               // display driver
    FB          fb;                     // FB
    __u8        *buffer;                // FB ��Ӧ�� buffer
    __s32       len;                    // buffer length;
    unsigned long       layer_hdle;             // ͼ����
    bgd_status_t status;                // ͼ��״̬
    SIZE        screen_size;            // ͼ��ߴ�
    char        filename[BG_MAX_CHAR_LEN];  // ����ͼƬ�ļ�����
} bgd_com_t;

static bgd_com_t *bgd_com = NULL;

/**********************************************************************************************************************/
static __s32   gscene_bgd_get_default_file(char *filename);
static __s32   gscene_bgd_get_setting_file(char *filename);
static void   gscene_bgd_set_status_show(void);
static void   gscene_bgd_set_status_hide(void);
static __s32  gscene_bgd_is_file_exist(char *file);
static __s32 gscene_bgd_set_setting_file(char *filename);
static __s32 gscene_bgd_update_filename(void);


/*�����ʾĬ�ϱ��������û����õı����ı��
 flag : EPDK_TRUE : Ĭ�ϱ���ͼƬ
      : EPDK_FALSE: ���ñ���ͼƬ
 */
__bool gscene_bgd_get_flag(void)
{
    __bool          ret;
    reg_init_para_t *para;

    para = (reg_init_para_t *)dsk_reg_get_para_by_app(REG_APP_INIT);

    if (para)
    {
        ret = para->bgd_flag;
    }
    else
    {
        ret = EPDK_TRUE;
    }

    return ret;
}

/*������ʾĬ�ϱ��������û����õı����ı��
 flag : EPDK_TRUE : Ĭ�ϱ���ͼƬ
      : EPDK_FALSE: ���ñ���ͼƬ
 */
void gscene_bgd_set_flag(__bool flag)
{
    reg_init_para_t *para;
    para = (reg_init_para_t *)dsk_reg_get_para_by_app(REG_APP_INIT);

    if (para)
    {
        para->bgd_flag = flag;
        //dsk_reg_flush();
    }
    else
    {
        __msg("dsk_reg_get_default_para_by_app fail...");
    }
}


/**********************************************************************************************************************/
int32_t gscene_bgd_init(SIZE *p_size, bgd_status_t status, __fb_type_t ftype)
{
    if (NULL != bgd_com)
    {
        return EPDK_OK;
    }

    bgd_com     = (bgd_com_t *)esMEMS_Malloc(0, sizeof(bgd_com_t));

    if (!bgd_com)
    {
        __err(" background com handle malloc error ");
        return -1;
    }

    eLIBs_memset(bgd_com, 0, sizeof(bgd_com_t));

    gscene_bgd_update_filename();

    bgd_com->de_hdle    = esKSRV_Get_Display_Hld();

    if (bgd_com->de_hdle == ((__mp *)0))
    {
        __err("can not open display driver");
        return -1;
    }

    bgd_com->screen_size.width  = p_size->width;
    bgd_com->screen_size.height = p_size->height;
    bgd_com->fb.fmt.type        = ftype;

	__log("bgd_com->screen_size.width = %d  bgd_com->screen_size.height = %d  ftype = %d",bgd_com->screen_size.width,bgd_com->screen_size.height,ftype);

    if (bgd_com->fb.fmt.type == FB_TYPE_YUV)
    {
        bgd_com->len = 2 * (p_size->width) * (p_size->height) / 1024;
    }
    else
    {
        bgd_com->len = 4 * (p_size->width) * (p_size->height) / 1024;
    }

    if (status == BGD_STATUS_SHOW)
    {
        gscene_bgd_set_status_show();
    }
    else
    {
    
        gscene_bgd_set_status_hide();
    }

    return 0;
}

static void gscene_bgd_set_status_show(void)
{
    char    sel;

    gscene_bgd_update_filename();

    if ((!bgd_com->buffer) && (get_logo_mode() == BMP_MODE))
    {
        bgd_com->buffer     = esMEMS_Palloc(bgd_com->len + 1, 0);
        get_fb_from_file(&(bgd_com->fb), bgd_com->buffer, &(bgd_com->screen_size), BACKLAYER_MOD_RATIO, (__u8 *)bgd_com->filename);
    }

	__msg("bgd_com->fb = %d  %d",bgd_com->fb.size.width,bgd_com->fb.size.height);

    if (bgd_com->filename)
    {
        __log("bkpic: %s.", bgd_com->filename);
    }

    if (!bgd_com->layer_hdle)
    {
        bgd_com->layer_hdle = backlayer_create_layer(bgd_com->de_hdle);
        backlayer_set_bottom(bgd_com->de_hdle, bgd_com->layer_hdle);
    }

    backlayer_set_fb(bgd_com->de_hdle, bgd_com->layer_hdle, bgd_com->filename, &(bgd_com->fb), BACKLAYER_MOD_STRETCH);
    bgd_com->status     = BGD_STATUS_SHOW;
}

static void gscene_bgd_set_status_hide(void)
{
    if (bgd_com->layer_hdle)
    {
        backlayer_delete_layer(bgd_com->de_hdle, bgd_com->layer_hdle);
        bgd_com->layer_hdle = 0;
    }

    if (bgd_com->buffer)
    {
        esMEMS_Pfree(bgd_com->buffer, bgd_com->len + 1);
        bgd_com->buffer     = 0;
    }

    bgd_com->status         = BGD_STATUS_HIDE;
}

void gscene_bgd_set_state(bgd_status_t status)
{

    if (NULL == bgd_com)
    {
        __wrn("NULL == bgd_com...");
        return;
    }

    __inf("bgd_com->status=%d status=%d", bgd_com->status, status);

    if (bgd_com->status == status)
    {
        return;
    }
    else
    {
        if (status == BGD_STATUS_SHOW)
        {
            gscene_bgd_set_status_show();
        }
        else
        {
            gscene_bgd_set_status_hide();
        }
    }

    gscene_bgd_set_bottom();
}

bgd_status_t gscene_bgd_get_state(void)
{
    return bgd_com->status;
}

void gscene_bgd_set_top(void)
{
    if (bgd_com->status == BGD_STATUS_SHOW)
    {
        backlayer_set_top(bgd_com->de_hdle, bgd_com->layer_hdle);
    }
}

void gscene_bgd_set_bottom(void)
{
    if (bgd_com->status == BGD_STATUS_SHOW)
    {

        backlayer_set_bottom(bgd_com->de_hdle, bgd_com->layer_hdle);
    }
}

__s32 gscene_bgd_save_fb(FB *fb, char *path)
{
    __s32 ret;
    ret = create_fb_file(fb, (__u8 *)path);

    if (EPDK_FAIL == ret)
    {
        __msg("create_fb_file fail...");
        return EPDK_FAIL;
    }

    ret = gscene_bgd_set_setting_file(path);

    if (EPDK_FAIL == ret)
    {
        __msg("gscene_bgd_set_setting_file fail...");
        return EPDK_FAIL;
    }

    eLIBs_strcpy(bgd_com->filename, path);
    gscene_bgd_set_flag(EPDK_FALSE);
    {
        reg_init_para_t *para;
        para = (reg_init_para_t *)dsk_reg_get_para_by_app(REG_APP_INIT);

        if (para)
        {
            para->bgd_flag_in_main = 0;
            //dsk_reg_flush();
        }
        else
        {
            __msg("dsk_reg_get_default_para_by_app fail...");
            return EPDK_FAIL;
        }
    }
    return EPDK_OK;
}

void gscene_bgd_set_fb_type(__fb_type_t ftype)
{
__err(".........gscene_bgd_set_fb_type");

    if (bgd_com->fb.fmt.type == ftype)
    {
        return;
    }

    bgd_com->fb.fmt.type = ftype;

    if (bgd_com->status == BGD_STATUS_HIDE)
    {
        if (bgd_com->fb.fmt.type == FB_TYPE_YUV)
        {
            bgd_com->len = 2 * (bgd_com->screen_size.width) * (bgd_com->screen_size.height) / 1024;
        }
        else
        {
            bgd_com->len = 4 * (bgd_com->screen_size.width) * (bgd_com->screen_size.height) / 1024;
        }

        return;
    }
    else
    {
        gscene_bgd_set_status_hide();

        if (bgd_com->fb.fmt.type == FB_TYPE_YUV)
        {
            bgd_com->len = 2 * (bgd_com->screen_size.width) * (bgd_com->screen_size.height) / 1024;
        }
        else
        {
            bgd_com->len = 4 * (bgd_com->screen_size.width) * (bgd_com->screen_size.height) / 1024;
        }

        gscene_bgd_set_status_show();
        return;
    }
}

void gscene_bgd_refresh(void)
{
__err(".........gscene_bgd_refresh");

    gscene_bgd_set_status_hide();
    gscene_bgd_set_status_show();
    /*
    void* ptmp_buf;

    if(BGD_STATUS_SHOW != bgd_com->status)
    {
        __msg("background not in show status...");
        return ;
    }

    //���û�����ڴ棬�������ڴ�
    if( !bgd_com->buffer )
    {
        bgd_com->buffer = esMEMS_Palloc( bgd_com->len + 1, 0 );
        if(!bgd_com->buffer)
        {
            __msg("mem not enough...");
            return;
        }
    }

    //���û������ʾͼ�㣬��������ʾͼ��
    if( !bgd_com->layer_hdle )
    {
        bgd_com->layer_hdle = backlayer_create_layer(bgd_com->de_hdle);
        if(!bgd_com->layer_hdle)
        {
            __msg("backlayer_create_layer fail...");
            return ;
        }
    }
    backlayer_set_bottom(bgd_com->de_hdle, bgd_com->layer_hdle);

    //����buffer��˫buffer���������ͺ���
    {
        ptmp_buf = esMEMS_Palloc( bgd_com->len + 1, 0 );
        if(!ptmp_buf)
        {
            __msg("mem not enough...");
            return;
        }
    }

    {
        __s32 ret;
        FB fb;

        eLIBs_memcpy(&fb, &(bgd_com->fb), sizeof(FB));
        ret = get_fb_from_file(&fb, ptmp_buf, &(bgd_com->screen_size), BACKLAYER_MOD_RATIO, (__u8 *)bgd_com->filename);
        if(EPDK_FAIL == ret)
        {
            __msg("get_fb_from_file...");
            if(ptmp_buf)
            {
                esMEMS_Pfree(ptmp_buf, bgd_com->len + 1);
                ptmp_buf = NULL;
            }
            return ;
        }
        backlayer_set_fb(bgd_com->de_hdle, bgd_com->layer_hdle, &fb, BACKLAYER_MOD_RATIO);
        eLIBs_memcpy(&(bgd_com->fb), &fb, sizeof(FB));
        if(bgd_com->buffer)
        {
            esMEMS_Pfree(bgd_com->buffer, bgd_com->len + 1);
        }
        bgd_com->buffer = ptmp_buf;
    }
    */
}

void gscene_bgd_restore(void)
{
    __s32 ret;
    char file[BG_MAX_CHAR_LEN] = {0};
    ret = gscene_bgd_get_default_file(file);

    if (EPDK_FAIL == ret)
    {
        __msg("gscene_bgd_get_default_file fail...");
        return ;
    }

    eLIBs_strcpy(bgd_com->filename, file);
    gscene_bgd_set_flag(EPDK_TRUE);
}

__s32 gscene_bgd_deinit(void)
{
    if (bgd_com->layer_hdle)
    {
        backlayer_delete_layer(bgd_com->de_hdle, bgd_com->layer_hdle);
        bgd_com->layer_hdle = 0;
    }

    if (bgd_com->buffer)
    {
        esMEMS_Pfree(bgd_com->buffer, bgd_com->len + 1);
        bgd_com->buffer = 0;
    }

    //eLIBs_fclose(bgd_com->de_hdle);
    bgd_com->de_hdle = NULL;
    esMEMS_Mfree(0, bgd_com);
    bgd_com = NULL;
    return 0;
}

int32_t gscene_bgd_get_default_bg_index(void)
{
    reg_init_para_t *para;

    para    = (reg_init_para_t *)dsk_reg_get_para_by_app(REG_APP_INIT);

    if (para)
    {
        return para->default_bgd_index;
    }
    else
    {
        __msg("dsk_reg_get_default_para_by_app fail...");
        return 0;
    }
}

int32_t gscene_bgd_set_default_bg_index(__u32 index)
{
    //__here__;
    if (EPDK_TRUE != gscene_bgd_get_flag()) //��ʹ��Ĭ�ϱ���
    {
        __msg("not using default background...");
        return EPDK_FAIL;
    }

    //__here__;
    if (index >= BG_MAX_NUM)
    {
        __msg("index too large, index=%d", index);
        return EPDK_FAIL;
    }

    //__here__;
    if (gscene_bgd_get_default_bg_index() == index)
    {
        __msg("default_bg_index == index, not need change bg");
        return EPDK_OK;
    }

    //__here__;
    {
        reg_init_para_t *para = NULL;
        para = (reg_init_para_t *)dsk_reg_get_para_by_app(REG_APP_INIT);

        if (para)
        {
            para->default_bgd_index = index;
            para->bgd_flag_in_main = 1;
            para->bgd_flag = 1;
            //dsk_reg_flush();
        }
        else
        {
            __msg("dsk_reg_get_default_para_by_app fail...");
            return EPDK_FAIL;
        }
    }
    {
        __s32 ret;
        ret = gscene_bgd_get_default_file(bgd_com->filename);

        if (EPDK_FAIL == ret)
        {
            __msg("gscene_bgd_get_default_file fail...");
            return EPDK_FAIL;
        }
    }
    __msg("change bg index=%d", index);
    gscene_bgd_refresh();
    return EPDK_OK;
}

static int32_t gscene_bgd_get_default_file(char *filename)
{
    int32_t     index;
    char        path[BG_MAX_CHAR_LEN] = {0};

    index   = gscene_bgd_get_default_bg_index();
    if (get_logo_mode() == JPG_MODE)
    {
        eLIBs_sprintf(path, "%sbg_default%d.jpg", BG_DEFAULT_PATH, index);
    }
    else
    {
        eLIBs_sprintf(path, "%sbg_default%d.bgd", BG_DEFAULT_PATH, index);
    }

    if (EPDK_TRUE == gscene_bgd_is_file_exist(path))
    {
        eLIBs_strcpy(filename, path);
        return EPDK_OK;
    }
    else
    {
        __msg("warning, bg file does not exist, index=%d, load index 0", index);

        if (get_logo_mode() == JPG_MODE)
        {
            eLIBs_sprintf(path, "%sbg_default%d.jpg", BG_DEFAULT_PATH, 0);
        }
        else
        {
            eLIBs_sprintf(path, "%sbg_default%d.bgd", BG_DEFAULT_PATH, 0);
        }

        if (EPDK_TRUE == gscene_bgd_is_file_exist(path))
        {
            eLIBs_strcpy(filename, path);
            return EPDK_OK;
        }
        else
        {
            __err("file not exist:%s", path);
            return EPDK_FAIL;
        }
    }
}

static __s32 gscene_bgd_set_setting_file(char *filename)
{
    reg_init_para_t *para;
    para = (reg_init_para_t *)dsk_reg_get_para_by_app(REG_APP_INIT);

    if (para)
    {
        eLIBs_strcpy(para->setting_bgd_path, filename);
        //dsk_reg_flush();
    }
    else
    {
        __msg("dsk_reg_get_default_para_by_app fail...");
        return EPDK_FAIL;
    }

    return EPDK_OK;
}


static int32_t gscene_bgd_get_setting_file(char *filename)
{
    int32_t     index;
    char        path[BG_MAX_CHAR_LEN] = {0};
    reg_init_para_t *para;

    para    = (reg_init_para_t *)dsk_reg_get_para_by_app(REG_APP_INIT);

    if (para)
    {
        eLIBs_sprintf(path, "%s", para->setting_bgd_path);
    }
    else
    {
        __msg("dsk_reg_get_default_para_by_app fail...");
        return EPDK_FAIL;
    }

    if (EPDK_TRUE == gscene_bgd_is_file_exist(path))
    {
        eLIBs_strcpy(filename, path);
        return EPDK_OK;
    }
    else
    {
        return EPDK_FAIL;
    }
}

static int32_t  gscene_bgd_is_file_exist(char *file)
{
    ES_FILE     *fp;

    fp  = eLIBs_fopen(file, "rb");
    if (fp)
    {
        __log("file is exist:%s", file);
        eLIBs_fclose(fp);
        return EPDK_TRUE;
    }
    else
    {
        __err("file not exist:%s", file);
        return EPDK_FALSE;
    }
}

static int32_t gscene_bgd_update_filename(void)
{
    __bool  flag;

    flag    = gscene_bgd_get_flag();

    if (flag == EPDK_TRUE)
    {
        int32_t ret;

        ret     = gscene_bgd_get_default_file(bgd_com->filename);
        if (EPDK_OK != ret)
        {
            __err("gscene_bgd_get_default_file fail...");
            return EPDK_FAIL;
        }
    }
    else
    {
        int32_t ret;

        ret     = gscene_bgd_get_setting_file(bgd_com->filename);

        if (EPDK_OK != ret)
        {
            __msg("get setting file fail,  load default file...");
            ret = gscene_bgd_get_default_file(bgd_com->filename);

            if (EPDK_OK != ret)
            {
                __err("gscene_bgd_get_default_file fail...");
                return EPDK_FAIL;
            }

            gscene_bgd_set_flag(EPDK_TRUE);
        }
    }

    return EPDK_OK;
}
