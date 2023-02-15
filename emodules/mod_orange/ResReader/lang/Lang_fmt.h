/*
**********************************************************************************************************************
*                                                    ePDK
*                                    the Easy Portable/Player Develop Kits
*                                              eMOD Sub-System
*
*                                   (c) Copyright 2007-2009, SW.China
*                                             All Rights Reserved
*
* Moudle  : Lemon
* File    : Lang_fmt.h
*
* By      : Andy
* Version : v1.0
* Date    : 2008-11-9 9:13:15
**********************************************************************************************************************
*/

#ifndef __LANG_FMT_H__
#define __LANG_FMT_H__

#define SEC_NAME        ".langdat"  //8�ֽڳ��ȣ�����

//������Դ�ļ����ݸ�ʽ����
typedef struct tag_LANG_HEAD
{
    __s8        SecName[8];     //����
    __u16       version;        //�汾��0x0100
    __u16       size;           //LANG_HEAD���ݽṹ��С
    __u32       LangTabOff;     //LANG����ʼ��ַƫ����----------------->
    __u16       LangSize;       //LANG����ÿ����Ŀ��С
    __u32       LangNum;        //LANG����Ŀ����,�������������
    __u16       StringSize;     //�ַ���size
    __u32       StringNum;      //�ַ���������
    __u32       align;          //���ݱ߽����ģʽ��
    __u32       flags;          //��־:SYSRES,LIBRES,APPRES
} __attribute__((__packed__)) LANG_HEAD; //34 byte
typedef struct tag_LANG
{
    __u16       LangID;             //0X400 0X420
    __u32       LangOffSet;         //LANG ��Ŀ��ʼ��ַƫ����
    __u32       LangSize;           //LANG ��Ŀ��С
    __u32       StringTabOff;       //�ַ�����ʼ��ַ
} __attribute__((__packed__)) LANG; //

typedef struct tag_STRING
{
    __u16       LangID;         //LangID
    __u16       StringID;       //such as:0x0001
    __u32       offset;         //string dataλ��
    __u32       size;           //data���ݳ���
} __attribute__((__packed__)) LANG_STRING;

#endif /* __LANG_FMT_H__  */
