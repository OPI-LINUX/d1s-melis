/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
*                                    (c) Copyright 2007-2011, CHIPHD, China
*                                            All Rights Reserved
*
* File      : app_anole.c
* By        : CQQ
* Func      :
* Version   : v1.0
* ============================================================================================================
* 2011/07/04 16:57  create this file, implements the fundamental interface;
**************************************************************************************************************
*/
#include <log.h>
#include "app_anole.h"
#include "photo_adapter.h"
#include <emodules/mod_touchpanel.h>
//////////////////////////////////////////////////////////////////////////
#define GetDiagonal()                  552 // = sqrt(480*480 + 272*272)

__s32 g_ANOLE_EnableTouch = EPDK_FALSE;

//��Ч
static __s32 switchmode[] =
{
    DEFAULT_SWITCH,             //Ĭ�����л�Ч��
    FADE_IN_OUT,                //���뵭��
    PERSIAN_BLIND_H,            //ˮƽ��Ҷ��
    PERSIAN_BLIND_V,            //��ֱ��Ҷ��
    SLID_UP,                    //���ϻ���
    SLID_DOWN,                  //���»���
    SLID_LEFT,                  //���󻬶�
    SLID_RIGHT,                 //���һ���
    STRETCH_UP,                 //����չ��
    STRETCH_DOWN,               //����չ��
    STRETCH_LEFT,               //����չ��
    STRETCH_RIGHT,              //����չ��
    MOSAIC,                     //������
    ROOM_IN,                    //��С
    ROOM_OUT,                   //�Ŵ�
};

#define ANOLE_RandomMode_ID  BEETLES_TBL_SIZE(switchmode)
#define IsRandomMode(_id)   ((_id) >= ANOLE_RandomMode_ID)

//
#define GetDataFromBmpFile(_filedata)   ((__u8*)_filedata + 54)

//ȡͼƬ��Դ���
#define GET_BMP_W_H_(_hbmp, _resData, _w, _h) do \
    {\
        _hbmp = bmp_open(_resData); \
        if (_hbmp != NULL) \
        { \
            _w = bmp_get_width(_hbmp); \
            _h = bmp_get_height(_hbmp); \
            bmp_close(_hbmp); \
        } \
        else \
        { \
            _w = 0; \
            _h = 0; \
            __msg("open bmp fail\n"); \
        } \
    }while(0)

//ȡͼƬ��Դ���
#define GET_BMP_W_H_Ex(_resData, _w, _h) do \
    {\
        HBMP _hbmp = NULL; \
        _hbmp = bmp_open(_resData); \
        if (_hbmp != NULL) \
        { \
            _w = bmp_get_width(_hbmp); \
            _h = bmp_get_height(_hbmp); \
            bmp_close(_hbmp); \
        } \
        else \
        { \
            _w = 0; \
            _h = 0; \
            __msg("open bmp fail\n"); \
        } \
    }while(0)

//////////////////////////////////////////////////////////////////////////
//���س�ʼ������ֵ
static __s32 PHOTO_LoadData(__anole_para_t *anole_para)
{
    __s32  val;
    anole_para->PlayOrder  = ANOLE_PLAY_NEXT;
    get_photo_value(val, photo_get_speedflag);

    if (val == PHOTO_SPEEDFLAG_FAST)
    {
        anole_para->interval = ANOLE_FAST;
    }
    else if (val == PHOTO_SPEEDFLAG_MIDDLE)
    {
        anole_para->interval = ANOLE_MIDDLE;
    }
    else if (val == PHOTO_SPEEDFLAG_SLOW)
    {
        anole_para->interval = ANOLE_SLOW;
    }
    else
    {
        anole_para->interval = ANOLE_CUSTOM;
    }

    anole_para->switchmodearray = switchmode;
    anole_para->switchnum = BEETLES_TBL_SIZE(switchmode);
    get_photo_value(val, photo_get_effect);

    if (IsRandomMode(val))
    {
        anole_para->SwitchMode = ANOLE_FLAG_RANDOM_MODE;
    }
    else
    {
        anole_para->SwitchMode = val;
    }

    DEBUG_photo("anole_para: PlayOrder = %d, interval = %d, SwitchMode = %d\n",
                anole_para->PlayOrder, anole_para->interval, anole_para->SwitchMode);
    return val;
}

//////////////////////////////////////////////////////////////////////////
//           �����л���ʼ��������ʼ��
//////////////////////////////////////////////////////////////////////////
#define ANOLE_SLIDER_FB_W                     68
#define ANOLE_SLIDER_FB_H                     68
#define ANOLE_SLIDER_FB_DECODE_BUF_SIZE       (ANOLE_SLIDER_FB_W * ANOLE_SLIDER_FB_H * 4) //byte
#define ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE  ANOLE_SLIDER_FB_DECODE_BUF_SIZE
#define B2K_(_b)                              ( (((_b) >> 10) > 1) ? (((_b) >> 10) + 1) : 2 )

static __u8 *decoding_buf_for_slid = NULL;
static __u8 *unsupported_buf_for_slid = NULL;
static __u8 *show_error_buf_for_slid = NULL;

static void __convert_2_argb(__u32 *data, __u32 len)
{
    __u32 tmp;
    __u32 cnt;
    __u8  R, G, B, A;

    for (cnt = 0; cnt < len; cnt++)
    {
        tmp = *(data + cnt);
        B = ((tmp >> 24) & 0xff);
        G = ((tmp >> 16) & 0xff);
        R = ((tmp >> 8)  & 0xff);
        A = ((tmp >> 0)  & 0xff);
        *(data + cnt) = ((A << 24) | (R << 16) | (G << 8) | B);
    }
}

static __s32 APP_ANOLE_SliderInit(void *unsupported_buf, void *decoding_buf)
{
    FB decoding_fb = {0};
    FB unsupported_fb = {0};
    APP_NewMem_P(decoding_buf_for_slid, B2K_(ANOLE_SLIDER_FB_DECODE_BUF_SIZE), 0);
    APP_NewMem_P(unsupported_buf_for_slid, B2K_(ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE), 0);
    APP_NewMem_P(show_error_buf_for_slid, B2K_(ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE), 0);
    ZeroMemory(decoding_buf_for_slid, B2K_(ANOLE_SLIDER_FB_DECODE_BUF_SIZE) << 10);
    ZeroMemory(unsupported_buf_for_slid, B2K_(ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE) << 10);
    ZeroMemory(show_error_buf_for_slid, B2K_(ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE) << 10);
    eLIBs_memcpy(decoding_buf_for_slid, GetDataFromBmpFile(decoding_buf), ANOLE_SLIDER_FB_DECODE_BUF_SIZE);
    decoding_fb.fmt.type = FB_TYPE_RGB;
    decoding_fb.fmt.cs_mode = YCC;
    decoding_fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;
    decoding_fb.fmt.fmt.rgb.br_swap = 0;
    decoding_fb.fmt.fmt.rgb.pixseq = RGB_SEQ_ARGB;
    decoding_fb.size.height = ANOLE_SLIDER_FB_H;
    decoding_fb.size.width = ANOLE_SLIDER_FB_W;
    decoding_fb.addr[0] = (void *)decoding_buf_for_slid;
    decoding_fb.addr[1] = decoding_fb.addr[2];
    eLIBs_memcpy(unsupported_buf_for_slid, GetDataFromBmpFile(unsupported_buf), ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE);
    eLIBs_memcpy(show_error_buf_for_slid, GetDataFromBmpFile(unsupported_buf), ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE);
    __convert_2_argb((__u32 *)show_error_buf_for_slid, ANOLE_SLIDER_FB_W * ANOLE_SLIDER_FB_H);
    unsupported_fb.fmt.type = FB_TYPE_RGB;
    unsupported_fb.fmt.cs_mode = YCC;
    unsupported_fb.fmt.fmt.rgb.pixelfmt = PIXEL_COLOR_ARGB8888;
    unsupported_fb.fmt.fmt.rgb.br_swap = 0;
    unsupported_fb.fmt.fmt.rgb.pixseq = RGB_SEQ_ARGB;
    unsupported_fb.size.height = ANOLE_SLIDER_FB_H;
    unsupported_fb.size.width = ANOLE_SLIDER_FB_W;
    unsupported_fb.addr[0] = (void *)unsupported_buf_for_slid;
    unsupported_fb.addr[1] = unsupported_fb.addr[2];
    return anole_slider_init(&decoding_fb, &unsupported_fb);
}

static void APP_ANOLE_SliderUninit(void)
{
    APP_DelMem_P(decoding_buf_for_slid, B2K_(ANOLE_SLIDER_FB_DECODE_BUF_SIZE));
    APP_DelMem_P(unsupported_buf_for_slid, B2K_(ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE));
    APP_DelMem_P(show_error_buf_for_slid, B2K_(ANOLE_SLIDER_FB_UNSUPPORTED_BUF_SIZE));
}

/************************************************************************/
//��ʼ��anole, para --- anole��س�ʼ������ָ��
// pInitData --- ap��س�ʼ������ָ��
/************************************************************************/
static int32_t ANOLE_Init(__anole_para_t *para, APP_ANOLE_INIT_T *pInitData)
{
    int32_t     ret;

    ASSERT(para);
    ASSERT(pInitData);

    //CheckAPPMEM();
    anole_set_cb(ANOLE_EVENT_SHOW_PIC_FAIL, esKRNL_GetCallBack(pInitData->cb_err), pInitData->pUserPara);

    ret = anole_init();//ʹ����0x160C Pages;(2011.08.18)
    __log("");
    //CheckAPPMEM();
    APP_ANOLE_SetShowMode(photo_get_ratio());
    __log("");

    if (ret == 0)
    {
        //init ok
        anole_set_play_order(para->PlayOrder);

        if (para->interval > ANOLE_MAX_INTERVAL)//for debug
        {
            para->interval = ANOLE_MAX_INTERVAL;
        }
        else if (para->interval < ANOLE_MIN_INTERVAL)
        {
            para->interval = ANOLE_MIN_INTERVAL;
        }
        __log("");

        anole_set_play_interval(para->interval);
        anole_set_switch_mode_array_for_play(para->switchmodearray, para->switchnum);
        anole_set_switch_mode(ANOLE_FLAG_RANDOM_MODE);
        //anole_set_show_mode(0);//temp
        //APP_ANOLE_SliderInit();
        __log("");

        APP_ANOLE_SliderInit(pInitData->pErrFileData, pInitData->pDecodingData);
        __log("");

        APP_ANOLE_PlayCurFile();
        __log("");

        anole_set_cb(ANOLE_EVENT_SHOW_PIC_NEXT, esKRNL_GetCallBack(pInitData->cb_pic_change), pInitData->pUserPara);
        __log("");

        APP_ANOLE_EnableTouch(EPDK_TRUE);
    }
    else if (ret == 1)
    {
        __log("");

        __inf("anole_init failed! ret = 1\n");
        APP_ANOLE_ShowBufPic(pInitData->pErrFileData);
    }
    else
    {
        __log("");

        __inf("anole_init failed!\n");
    }
    __log("");

    return ret;
}

#define DEVICE_TP_NAME        "b:\\INPUT\\TP"
#define DEVICE_TP_OPEN_MODE   "r+"
/************************************************************************/
// �����㴥��ģʽ
/************************************************************************/
static int32_t OpenTpDualMode(void)
{
    int32_t     ret = EPDK_FAIL;
#if 0/*langaojie temp test*/
    ES_FILE     *tp_hdl = NULL;

    tp_hdl = eLIBs_fopen(DEVICE_TP_NAME, DEVICE_TP_OPEN_MODE);

    if (tp_hdl == NULL)
    {
        __msg("open tp handle fail...\n");
    }
    else
    {
        ret =  esMODS_MIoctrl(tp_hdl, DRV_TP_CMD_SET_WORKMODE, DRV_TP_MODE_DUAL, NULL);

        if (ret == EPDK_FAIL)
        {
            __msg("set tp dual mode fail...\n");
        }

        __msg("Open Tp Dual Mode...\n");
        eLIBs_fclose(tp_hdl);
    }
#endif
    return ret;
}

/************************************************************************/
// �ر����㴥��ģʽ(��Ϊ��ͨģʽ)
/************************************************************************/
static int32_t CloseTpDualMode(void)
{
    int32_t     ret = EPDK_FAIL;
#if 0/*langaojie temp test*/
    ES_FILE     *tp_hdl = NULL;

    tp_hdl  = eLIBs_fopen(DEVICE_TP_NAME, DEVICE_TP_OPEN_MODE);

    if (tp_hdl == NULL)
    {
        __msg("open tp handle fail...\n");
    }
    else
    {
        ret =  esMODS_MIoctrl(tp_hdl, DRV_TP_CMD_SET_WORKMODE, DRV_TP_MODE_NORMAL, NULL);

        if (ret == EPDK_FAIL)
        {
            __msg("set tp normal mode fail...\n");
        }

        __msg("Close Tp Dual Mode(MODE_NORMAL, now)...\n");
        eLIBs_fclose(tp_hdl);
    }
#endif
    return ret;
}

/************************************************************************/
//��ʼ��anole
/************************************************************************/
int32_t APP_ANOLE_Init(APP_ANOLE_INIT_T *pInitData)
{
    __anole_para_t  anole_para;

    OpenTpDualMode();

    PHOTO_LoadData(&anole_para);

    return ANOLE_Init(&anole_para, pInitData);
}

/************************************************************************/
//�˳�anole
/************************************************************************/
void APP_ANOLE_Exit(void)
{
    CloseTpDualMode();

    anole_set_cb(ANOLE_EVENT_SHOW_PIC_FAIL, NULL, 0);

    anole_set_cb(ANOLE_EVENT_SHOW_PIC_NEXT, NULL, 0);

    anole_exit();

    APP_ANOLE_SliderUninit();
}

/************************************************************************/
//ȡ�ļ���Ϣ
/************************************************************************/
int32_t APP_ANOLE_GetCurPicInfo(APP_ANOLE_FILE_INFO_T *file_info)
{
    int32_t     ret;
    char        *p;
    uint32_t    index = anole_npl_get_cur();

    file_info->pic.name = file_info->path_name;
    //��ȡ�ļ���
    anole_npl_index2file(index, file_info->path_name);

    eLIBs_GetFileCTime(file_info->path_name, &file_info->time);

    p   = eLIBs_strchrlast(file_info->path_name, '\\');

    if (p)
    {
        p[0] = '\0';
        file_info->pic_name = p + 1;
    }
    else
    {
        file_info->pic_name = file_info->path_name;
    }

    ret = anole_get_pic_info(index, &file_info->pic);
    __msg("file info(%s, %s, %d x %d, %d)\n",
          file_info->path_name, file_info->pic_name,
          file_info->pic.size.width, file_info->pic.size.height,
          file_info->pic.file_len);
    return ret;
}

/************************************************************************/
//�����б��е�ǰ���ļ�
/************************************************************************/
int32_t APP_ANOLE_PlayCurFile(void)
{
    int32_t     ret;
    char        Buf[FSYS_DIRNAME_MAX];

    ZeroMemory(Buf, sizeof(Buf));

    ret     = anole_npl_get_cur();

    if (-1 == ret)
    {
        __msg("anole_npl_get_cur fail...\n");
        return EPDK_FAIL;
    }

    ret     = anole_npl_index2file(ret, Buf);

    if (/*Buf[0] != 0 && */0 == ret)
    {
        anole_play_file(Buf);
        __msg("anole_play_file file=%s\n", Buf);
    }
    else
    {
        __msg("APP_ANOLE_PlayCurFile fail\n");
    }

    return ret;
}

/************************************************************************/
//������Ч
/************************************************************************/
int32_t APP_ANOLE_SetSwitchMode(uint32_t Id)
{
    set_photo_value(Id, photo_set_effect);

    if (IsRandomMode(Id))
    {
        Id = ANOLE_FLAG_RANDOM_MODE;
    }

    DEBUG_photo("anole_set_switch_mode (%d)\n", Id);
    return anole_set_switch_mode(Id);
}

/************************************************************************/
//ȡ��ЧID
/************************************************************************/
int32_t APP_ANOLE_GetSwitchMode(void)
{
    int32_t     ret;

    ret = anole_get_switch_mode();

    if (ret == ANOLE_FLAG_RANDOM_MODE)
    {
        ret = ANOLE_RandomMode_ID;
    }

    DEBUG_photo("APP_ANOLE_GetSwitchMode = %d\n", ret);
    return ret;
}

/************************************************************************/
//���ò����ٶ�
/************************************************************************/
int32_t APP_ANOLE_SetPlayInterval(int32_t id_flag, int32_t para)
{
    ASSERT(0 <= id_flag);

    switch (id_flag)
    {
        case 0:
            para = ANOLE_FAST;
            break;

        case 1:
            para = ANOLE_MIDDLE;
            break;

        case 2:
            para = ANOLE_SLOW;
            break;

        default:
            break;
    }

    return anole_set_play_interval(para);
}

/************************************************************************/
//���㴥���Ŵ�
/************************************************************************/
int32_t APP_ANOLE_TouchZoomIn(uint32_t pixel_change)
{
    __fp32      monoscale, scale;
    uint32_t    diagonal = GetDiagonal();

    if (pixel_change > TouchMinDist())
    {
        monoscale   = (__fp32)((__fp32)diagonal / (__fp32)(diagonal - pixel_change));
        scale       = monoscale * monoscale;

        anole_scale_inc(2 * scale);
    }

    return EPDK_OK;
}

/************************************************************************/
//���㴥����С
/************************************************************************/
int32_t APP_ANOLE_TouchZoomOut(uint32_t pixel_change)
{
    __fp32      monoscale, scale;
    uint32_t    diagonal = GetDiagonal();

    if (pixel_change > TouchMinDist())
    {
        monoscale   = (__fp32)((__fp32)diagonal / (__fp32)(diagonal - pixel_change));
        scale       = monoscale * monoscale;

        anole_scale_dec(2 * scale);
    }

    return EPDK_OK;
}

/************************************************************************/
//�����ƶ�
/************************************************************************/
int32_t APP_ANOLE_TouchMove(int32_t delta_x, int32_t delta_y)
{
    //��ǰ����scale  ״̬����ִ���ƶ�
    if (APP_ANOLE_GetSta() == ANOLE_MODE_ZOOM)
    {
        __pos_t move;

        move.x  = delta_x;
        move.y  = delta_y;
        return anole_scale_touch_move(move);
    }

    return EPDK_OK;
}

/************************************************************************/
//����(�ֶ�)�л�ͼƬ
// delta_x = x_up - x_down;
// delta_y = y_up - y_down;
/************************************************************************/
#if 0
int32_t APP_ANOLE_TouchSwitchPic(int32_t delta_x, int32_t delta_y)
{
    int32_t ret = EPDK_OK;

    if (APP_ANOLE_GetSta() == ANOLE_MODE_NORMAL)
    {
        int32_t mode;
        mode = APP_ANOLE_GetSwitchMode();

        if (ABS(delta_y) < ABS(delta_x))
        {
            //x����
            if (delta_x <= - TouchMinDist())
            {
                //              if (mode != SLID_LEFT && mode != STRETCH_LEFT)
                {
                    APP_ANOLE_SetSwitchMode(SLID_LEFT);
                }
                anole_play_next();
            }
            else if (TouchMinDist() <= delta_x)
            {
                //              if (mode != SLID_RIGHT && mode != STRETCH_RIGHT)
                {
                    APP_ANOLE_SetSwitchMode(SLID_RIGHT);
                }
                anole_play_prev();
            }
            else
            {
                __msg("delta x = %d\n", delta_x);
                ret = EPDK_FAIL;
            }
        }
        else
        {
            //y����
            if (delta_y <= -TouchMinDist())
            {
                //              if (mode != SLID_UP && mode != STRETCH_UP)
                {
                    APP_ANOLE_SetSwitchMode(SLID_UP);
                }
                anole_play_next();
            }
            else if (TouchMinDist() <= delta_y)
            {
                //              if (mode != SLID_DOWN && mode != STRETCH_DOWN)
                {
                    APP_ANOLE_SetSwitchMode(SLID_DOWN);
                }
                anole_play_prev();
            }
            else
            {
                __msg("delta y = %d\n", delta_y);
                ret = EPDK_FAIL;
            }
        }

        if (ret == EPDK_OK)
        {
            //��ԭ�Զ�������Ч
            APP_ANOLE_SetSwitchMode(mode);
        }
    }
    else
    {
        ret = EPDK_FAIL;
    }

    return ret;
}
#endif

/************************************************************************/
// anole touch ����
// touch_action --- __gui_msg_touchid_t
// return value --- 0, û����; ��0���Ѷ���Ϣ�����˴���
/************************************************************************/
int32_t APP_ANOLE_Touch(int16_t x, int16_t y, int32_t touch_action, int32_t pixel_change)
{
    static int32_t  last_touch_action = GUI_MSG_TOUCH_DOWN;
    static __pos_t  last_pos = {0, 0};
    static int32_t  is_slider = EPDK_FALSE;

#define EmptyTouchAction()      {last_touch_action = GUI_MSG_TOUCH_DOWN; last_pos.x = 0; last_pos.y = 0;}
#define RecordTouchPos(_x, _y)  {last_pos.x = _x; last_pos.y = _y;}
#define RecordTouch(_a, _x, _y) {last_touch_action = _a; RecordTouchPos(_x, _y);}

    int32_t         ret = 0;
    __anole_sta_t   sta;

    sta     = anole_get_sta();
    __msg("anole state = %d, action %d, pos(%d, %d) [%d]\n", sta, touch_action, x, y, pixel_change);

    if (! is_slider)
    {
        switch (touch_action)
        {
            case GUI_MSG_TOUCH_ZOOMIN:
            case GUI_MSG_TOUCH_OVERZOOMIN:
                APP_ANOLE_TouchZoomOut(pixel_change); //�ֲ���С(gui���͵���Ϣ���������෴��)
                return 2;

            case GUI_MSG_TOUCH_ZOOMOUT:
            case GUI_MSG_TOUCH_OVERZOOMOUT:
                APP_ANOLE_TouchZoomIn(pixel_change); //�ֲ��Ŵ�(gui���͵���Ϣ���������෴��)
                return 2;
        }
    }

    if (sta == ANOLE_MODE_NORMAL)
    {
        switch (touch_action)
        {
            case GUI_MSG_TOUCH_DOWN:
            case GUI_MSG_TOUCH_LONGDOWN:
            {
                RecordTouch(GUI_MSG_TOUCH_DOWN, x, y);//RecordTouch(touch_action, x, y);
            }
            break;

            case GUI_MSG_TOUCH_MOVE:
            {
                if (last_touch_action == GUI_MSG_TOUCH_DOWN)
                {
                    anole_set_switch_mode(TOUCH_SLID_H);
                    anole_set_layer_sta(ANOLE_LAYER_BOTTOM);
                    APP_ANOLE_SliderStart(last_pos.x, last_pos.y);
                    is_slider = EPDK_TRUE;
                }

                APP_ANOLE_SliderMove(x, y);
                esKRNL_TimeDly(20);  // TODO:check
                RecordTouch(GUI_MSG_TOUCH_MOVE, x, y);
                ret = 1;
            }
            break;

            case GUI_MSG_TOUCH_UP:
            {
                //              if (last_touch_action == GUI_MSG_TOUCH_MOVE)
                if (is_slider)
                {
                    APP_ANOLE_SliderFinish(x, y);
                    is_slider = EPDK_FALSE;
                    ret = 1;
                }

                EmptyTouchAction();
            }
            break;
        }
    }
    else if (sta == ANOLE_MODE_ZOOM)
    {
        switch (touch_action)
        {
            case GUI_MSG_TOUCH_DOWN:
            case GUI_MSG_TOUCH_LONGDOWN:
            {
                RecordTouch(GUI_MSG_TOUCH_DOWN, x, y);
            }
            break;

            case GUI_MSG_TOUCH_MOVE:
            {
                __pos_t move;

                move.x  = x - last_pos.x;
                move.y  = y - last_pos.y;
                anole_scale_touch_move(move);
                RecordTouch(GUI_MSG_TOUCH_MOVE, x, y);
                ret = 1;
            }
            break;

            case GUI_MSG_TOUCH_UP:
            {
                if (last_touch_action == GUI_MSG_TOUCH_MOVE)
                {
                    ret = 1;
                }

                EmptyTouchAction();
            }
            break;
        }
    }

    return ret;
}

//file_date --- bmp res file data
int32_t APP_ANOLE_ShowBufPic(void *file_data)
{
    int32_t     w, h;

    GET_BMP_W_H_Ex(file_data, w, h);

    if (w != 0 && h != 0)
    {
        return anole_show_pic_from_buffer((__u8 *)file_data + 54, w, h);
    }

    return 0;
}

int32_t APP_ANOLE_ShowErrorPic(void)
{
    if (unsupported_buf_for_slid)
    {
        //anole_show_pic_from_buffer(unsupported_buf_for_slid, ANOLE_SLIDER_FB_W, ANOLE_SLIDER_FB_H);
        anole_show_pic_from_buffer(show_error_buf_for_slid, ANOLE_SLIDER_FB_W, ANOLE_SLIDER_FB_H);
    }

    return EPDK_OK;
}

void APP_ANOLE_AutoPlay(void)
{
    anole_auto_play();
    dsk_set_auto_off_time(0);
}

void APP_ANOLE_Pause(void)
{
    anole_backnormal();

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
}
