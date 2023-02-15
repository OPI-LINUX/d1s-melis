/*
*********************************************************************************************************
*                                                    eMOD
*                                         the melis Operation System
*                                               input sub-system
*                                          input system core module
*
*                                    (c) Copyright 2010-2012, sunny China
*                                              All Rights Reserved
*
* File   : input.h
* Version: V1.0
* By     : Sunny
* Date   : 2010-7-11
* Note   : input system core module.
*********************************************************************************************************
*/

#ifndef __INPUT_H__
#define __INPUT_H__
#include <typedef.h>
#include <sys_device.h>
#include <sys_input.h>
#include <ktype.h>
#include <port.h>

#define INPUT_DEVICE_ID_EV_MAX              0x1f
#define INPUT_DEVICE_ID_KEY_MIN_INTERESTING 0x71
#define INPUT_DEVICE_ID_KEY_MAX             0x2ff
#define INPUT_DEVICE_ID_REL_MAX             0x0f
#define INPUT_DEVICE_ID_ABS_MAX             0x3f
#define INPUT_DEVICE_ID_LED_MAX             0x0f
#define INPUT_DEVICE_ID_SND_MAX             0x07
#define INPUT_DEVICE_ID_FF_MAX              0x7f

#define INPUT_DEVICE_ID_MATCH_PRODUCT       4
#define INPUT_DEVICE_ID_MATCH_VERSION       8

#define INPUT_DEVICE_ID_MATCH_EVBIT         0x0010
#define INPUT_DEVICE_ID_MATCH_KEYBIT        0x0020
#define INPUT_DEVICE_ID_MATCH_RELBIT        0x0040
#define INPUT_DEVICE_ID_MATCH_ABSBIT        0x0080
#define INPUT_DEVICE_ID_MATCH_LEDBIT        0x0200
#define INPUT_DEVICE_ID_MATCH_SNDBIT        0x0400

typedef struct __INPUT_LID
{
    uint16_t    flags;
    uint16_t    product;
    uint16_t    version;

    uint32_t    evbit[INPUT_DEVICE_ID_EV_MAX / BITS_PER_LONG + 1];
    uint32_t    keybit[INPUT_DEVICE_ID_KEY_MAX / BITS_PER_LONG + 1];
    uint32_t    relbit[INPUT_DEVICE_ID_REL_MAX / BITS_PER_LONG + 1];
    uint32_t    absbit[INPUT_DEVICE_ID_ABS_MAX / BITS_PER_LONG + 1];
    uint32_t    ledbit[INPUT_DEVICE_ID_LED_MAX / BITS_PER_LONG + 1];
    uint32_t    sndbit[INPUT_DEVICE_ID_SND_MAX / BITS_PER_LONG + 1];
    uint32_t    ffbit[INPUT_DEVICE_ID_FF_MAX / BITS_PER_LONG + 1];
} __input_lid_t;

typedef struct __INPUT_GRABER __input_graber_t;

typedef struct __INPUT_LDEV
{
    /* ����4����ԱΪע��ͨ���豸���� */
    const char      *classname;
    const char      *name;
    const __dev_devop_t   *ops;
    void            *parg;

    /* ָ��ͨ���豸 */
    __hdle          devnode;

    /* ��ȫ���߼��豸�������е�index��� */
    unsigned char   seq;

    /* ����������Ա���ڽ����߼������豸�����������豸�����˹�ϵ */
    __input_dev_t   *children[INPUT_CHILDREN_MAX];  /* �������������豸������� */
    uint32_t        children_cnt;                /* ���������豸����         */

    /*
     * �߼��豸���¼���������Ա��input_event()����������event���Ͱ�event�ַ���
     * �߼������豸�����������豸���ó�Ա�����������͸��߼������豸��event
     */
    int32_t         (*event)(__input_dev_t *dev, __u32 type, __u32 code, __s32 value);

    /*
     * �߼��¼������������յ�input_sync()ʱ�����Ѹû��������¼����������ݸ�ÿ����
     * �߼��豸��ÿ��graber�Ļص�������
     */
    __hdle          ev_sem;
    uint32_t        evbuf[INPUT_EV_BUFFER_MAXLEN];
    int32_t         ev_head;            /* �¼�����������ָ��   */
    int32_t         ev_tail;            /* �¼���������βָ��   */
    int32_t         ev_len;             /* �¼��������ĳ���     */
    int32_t         ev_maxlen;

    /*
      * ��ָ��ָ��һ���߼������豸�����ʶ����顣���һ��һ����NULL��β��
      * �߼������豸����֧�ֶ������͵����������豸��ע�����������豸ʱ������ʹ�ø�
      * ���ʶ�������ʶ���Ƿ����������豸���߼������豸֧�֡�ֻҪ���������豸����
      * ���е�һ�飬������Ϊ���������豸�����߼������豸֧�֡�
      */
    const __input_lid_t     *id_table;

    /*
      * �߼������豸���ϲ��û�(apps/desktop_managers etc.)ÿ����һ��grabϵͳ���ã�
      * �����߼������豸��ע��һ��graber����grabersָ����������Щgraber��������ָ
      * �롣
      */
    __input_graber_t        *grabers;

    /* �߼������豸��graberɨ�����߳�id */
    __hdle          tid;
} __input_ldev_t;

typedef struct __INPUT_GRABER
{
    __pCBK_t                callback;  /* �ϲ�Ӧ�ó��򴴽�graberʱ�����ݵĻص����� */
    void                    *pGrabArg;  /*  ע��ص�����ʱ�����˽�о��            */
    __input_ldev_t          *ldev;      /* input�߼��豸���                        */
    struct __INPUT_GRABER   *next;
} __input_graber_t;


extern int32_t INPUT_CoreInit(void);
extern int32_t INPUT_CoreExit(void);
extern int32_t IsEventSupported(uint32_t code, unsigned long *bm, uint32_t max);
extern void  INPUT_RepeatKey(void *parg);
extern int32_t INPUT_SetRepPeriod(int32_t LDevID, uint32_t period);
extern int32_t esINPUT_RegLdev(__input_ldev_t *ldev);
extern int32_t esINPUT_UnregLdev(__input_ldev_t *ldev);

#endif  /* __INPUT_H__ */

