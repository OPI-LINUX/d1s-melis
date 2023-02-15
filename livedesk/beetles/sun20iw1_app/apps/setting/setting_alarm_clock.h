
#ifndef __SETTING_ALARM_CLOCK_H__
#define __SETTING_ALARM_CLOCK_H__




#include "beetles_app.h"

#define ALARM_CLOCK_MSG_CLOSE           GUI_MSG_USER_DEF+800    //���͹ر���Ϣ��������

typedef enum setting_alarm_clock_bmp_e
{
    //����
    ALARM_CLOCK_BG_BMP,
    ALARM_CLOCK_CLOCK_BG_BMP,
    //ҳ
    ALARM_CLOCK_PAGE_BMP,
    //����
    ALARM_CLOCK_0_BMP,
    ALARM_CLOCK_1_BMP,
    ALARM_CLOCK_2_BMP,
    ALARM_CLOCK_3_BMP,
    ALARM_CLOCK_4_BMP,
    ALARM_CLOCK_5_BMP,
    ALARM_CLOCK_6_BMP,
    ALARM_CLOCK_7_BMP,
    ALARM_CLOCK_8_BMP,
    ALARM_CLOCK_9_BMP,
    //��ť
    ALARM_CLOCK_DOWN_UNF_BMP,
    ALARM_CLOCK_DOWN_F_BMP,
    ALARM_CLOCK_UP_UNF_BMP,
    ALARM_CLOCK_UP_F_BMP,
    //���
    ALARM_CLOCK_BMP_MAX
} setting_alarm_clock_bmp;

typedef enum setting_alarm_clock_button_e
{
    //��ť
    ALARM_CLOCK_HOUR_UP,        //Сʱ+
    ALARM_CLOCK_HOUR_DOWN,      //Сʱ-
    ALARM_CLOCK_MINUTE_UP,      //����+
    ALARM_CLOCK_MINUTE_DOWN,    //����-
    ALARM_CLOCK_OK,             //����
    ALARM_CLOCK_CANCEL,         //ȡ��
    //���
    ALARM_CLOCK_BUTTON_MAX
} setting_alarm_clock_button;

typedef struct tag_setting_alarm_clock_attr
{
    H_LYR                       layer;
    reg_alarm_para_t           *reg_alarm;

    __awos_time_t                   clock;
    setting_alarm_clock_button  buttonF;

    HTHEME                      h_bmp_alarm_clock[ALARM_CLOCK_BMP_MAX];

} setting_alarm_clock_attr, *psetting_alarm_clock_attr;






extern H_WIN setting_alarm_clock_frm_create(H_WIN parentWin, __u16 id, reg_alarm_para_t *reg_alarm);


extern __s32 setting_alarm_clock_frm_destory(H_WIN alarmWin);






























#endif//__SETTING_ALARM_CLOCK_H__
