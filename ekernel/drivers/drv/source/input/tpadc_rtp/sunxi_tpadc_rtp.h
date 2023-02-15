/*
 * ===========================================================================================
 *
 *       Filename:  sunxi_tpadc_rtp.h
 *
 *    Description:  implemtaton of tpadc driver core based on hal.
 *
 *        Version:  Melis3.0
 *         Create:  2020-7-14
 *       Revision:  none
 *
 *         Author:  liujiaming@allwinnertech.com
 *   Organization:  PDC-PD1
 *  Last Modified:  2021-7-14
 *
 * ===========================================================================================
 */
#include <kapi.h>
#define CHANNEL_X 0
#define CHANNEL_Y 1
#define DATA_UP   2


/* interrupt control */
#define  TP_DATA_IRQ           (0x01 << 16)// (0x01 << 5)
#define  TP_LTOUCH_IRQ         (0x01 << 1)
#define  TP_FTOUCH_IRQ         (0x01 << 0)
#define  TP_FIFO_OV_IRQ        (0x01 <<17)//(0x01 << 6)
#define  TP_FIFO_DATA_DRQ      (0x01 <<7)

/////////触摸屏动作////////
#define TCHSCR_ACTION_NULL				0	    //空白动作或无法识别的动作
#define TCHSCR_ACTION_CLICK				1	    //触摸屏单击
#define TCHSCR_ACTION_DOWN				(0x02)	//触摸屏下滑
#define TCHSCR_ACTION_UP				(0x12)	//触摸屏上滑
#define TCHSCR_ACTION_LEFT				(0x03)	//触摸屏左滑
#define TCHSCR_ACTION_RIGHT				(0x13)	//触摸屏右滑
#define TCHSCR_ACTION_ANTICLOCKWISE		(0x04)	//触摸屏逆时针滑动
#define TCHSCR_ACTION_CLOCKWISE			(0x14)	//触摸屏顺时针滑动
#define TCHSCR_ACTION_LD2RU				(0x05)	//触摸屏斜线滑动,左下到右上
#define TCHSCR_ACTION_RU2LD      		(0x15)	//触摸屏斜线滑动,右上到左下
#define TCHSCR_ACTION_LU2RD	    		(0x06)	//触摸屏反斜线滑动,左上到右下
#define TCHSCR_ACTION_RD2LU 			(0x16)	//触摸屏反斜线滑动,右下到左上

#define RIGHTWARD						(TCHSCR_ACTION_RIGHT)
#define LEFTWARD						(TCHSCR_ACTION_LEFT)
#define UPWARD							(TCHSCR_ACTION_UP)
#define DOWNWARD						(TCHSCR_ACTION_DOWN)

int rtp_manager_get_data(__u32 channel, __u32 data);
__u32 sent_data_to_upper(__u32 channel);








