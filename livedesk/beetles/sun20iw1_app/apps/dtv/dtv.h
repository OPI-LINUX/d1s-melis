/*
*********************************************************************************************************
*                                                   ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              calendar app sample
*
*                                   (c) Copyright 2006-2009, ANDY, China
*                                            All Rights Reserved
*
* File    : movie.h
* By      :kaiyuanyang@sina.com
* Version : V1.00
*********************************************************************************************************
*/
#ifndef __DVBPLAY_H__
#define __DVBPLAY_H__

#include "beetles_app.h"
#include "dtv_mid.h"
//#define  APP_DTV  "APP_DTV"


#define DELETE_SCENE_EXP_NO_CHANNEL   (1<<0)

#define  MAX_CHARGE_LEVEL  6

typedef struct dtv_s
{
    __u8        thread_id;

    __hdle      vedio_handl;


    __krnl_event_t *sem;


    H_LYR       h_lyr;              //ȫ��ͼ�㣬���ڽ�Ŀ�б�Ƶ���б����õĿ���

    H_WIN       h_loading;
    H_WIN       h_scan;             //����
    H_WIN       h_serverlist;      //Ƶ���б�
    H_WIN       h_epg;             //��ǰ��Ŀ�б���Ϣ
    H_WIN       h_setting;         //���ý���

#if((MAPLE_TV_STANDAR == MAPLE_TV_ISDB_T)||(MAPLE_TV_STANDAR == MAPLE_TV_DTMB)\
        || (MAPLE_TV_STANDAR == MAPLE_TV_DVB_T)|| (MAPLE_TV_STANDAR == MAPLE_TV_ATSC))
    //#error "test"
    H_WIN       h_subtitle;
    __bool      b_has_subtitle; //�л�����������ʱ���ڼ�¼��ǰ��Ļ�Ƿ���� ���˳�ʱ�ָ�
#endif

    __s32       ch_timer;//����ѡ̨��ʱ��
    __s32       usr_ch; //�û������̨

    __s32       cur_user_index; //���û��Ƕȿ�����̨��0 , 1 2 3 4 5 6 7 ^^^^^^


    __u8        old_decode_val;
    __u8        old_decode_cnt;

    HTHEME      h_battery_bmp[MAX_CHARGE_LEVEL];

    __s32       low_battery_cnt;
    power_level_e battery_level;
    __bool      battery_warning;
    __bool      battery_wink;
    __bool      battery_icon_on;

    H_LYR       h_battery_lyr;

    H_LYR       lyr_sel_num;
#if 0
    HTHEME      h_radio_program_bmp;
    H_LYR       lyr_radio_program;
    HTHEME      h_switch_program_bmp;
    H_LYR       lyr_switch_program;
#endif


    __u32 sel_num;
    __u32 sel_sta;//0:nothing  1:����ѡ̨2:������ʾ 3:�ֶ���̨
    __u32 sel_cnt;
    __u8  play_cnt; //����ͬ��play  ��stop
    __u8  hide_vol_cnt;

    __hdle      h_no_channel;
    __hdle      h_no_signal;//�ź���
    __hdle      h_no_auth;//δ��Ȩ
    __hdle      h_radio_tip;
#ifdef APP_ADD_RECORD_VEDIO
    __u8        record_ts_remark_num;
#endif
} dtv_ctr_t;


/***********************************************************************************/


__s32  dtv_get_cur_user_index(void);
__s32 dtv_set_cur_user_index(__s32 index);
__bool dtv_has_preveiw_scene(void);
__bool dtv_show_vol_status(void);
__bool dtv_show_singal_status(void);
signed long  app_dtv_create(root_para_t  *para);


#endif  /* __DVBPLAY_H__ */


