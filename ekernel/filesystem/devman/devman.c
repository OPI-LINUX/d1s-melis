/*
**********************************************************************************************************************
*                                                   ePOS
*                                  the Easy Portable/Player Operation System
*
*                                (c) Copyright 2007-2008, Steven.ZGJ.China
*                                           All Rights Reserved
*
* File    : devman.c
* By      : steven.ZGJ
* Version : V1.00
* update  : kevin.z.m, 2010-9-7 17:08, clear code.
**********************************************************************************************************************
*/
#include "devman.h"
#include <kapi.h>
#include <string.h>
#include <port.h>
#include <log.h>

__hdle pDevSem      = NULL;           /* for lock parts table */

__dev_classnode_t   devclass_nop = {.name = DEV_CLASS_NOP, .nodetype = NODETYPE_CLASS,};

/*
**********************************************************************************************************************
*                                     SetDevAttr
*
* Description: set dev node attribute.
*
* Arguments  : class name
*              dev name
*              devnode
*
* Returns    : always EPDK_OK
**********************************************************************************************************************
*/
int32_t SetDevAttr(char *classname, char *devname, __dev_node_t *devnode)
{
    devnode->attrib     = 0;

    if (!strcmp(classname, DEV_CLASS_DMS))
    {
        devnode->attrib         |= DEV_NODE_ATTR_CTL | DEV_NODE_ATTR_PART | DEV_NODE_ATTR_FS | DEV_NODE_ATTR_SYNMNT;
        devnode->pletter        = PART_LETTER_DMS;
    }
    else if (!strcmp(classname, DEV_CLASS_DISK))
    {
        devnode->attrib         |= DEV_NODE_ATTR_RD | DEV_NODE_ATTR_WR | DEV_NODE_ATTR_BLK | DEV_NODE_ATTR_PART | DEV_NODE_ATTR_FS;
        devnode->pletter        = PART_LETTER_FREE;

        if (!strcmp(devname, DEV_NAME_RAMDISK))
        {
            devnode->pletter    = PART_LETTER_RAMDISK;
            devnode->attrib     |= DEV_NODE_ATTR_SYNMNT;
        }
        else if (!strcmp(devname, DEV_NAME_ROOTFS))
        {
            devnode->pletter    = PART_LETTER_ROOTFS;
            devnode->attrib     |= DEV_NODE_ATTR_SYNMNT;
        }
        else if (!strcmp(devname, DEV_NAME_SYSDATAFS))
        {
            devnode->pletter    = PART_LETTER_SYSDATA;
            devnode->attrib     |= DEV_NODE_ATTR_SYNMNT;
        }
        else if (!strcmp(devname, DEV_NAME_SYSBOOTFS))
        {
            devnode->pletter    = PART_LETTER_SYSBOOT;
            devnode->attrib     |= DEV_NODE_ATTR_SYNMNT;
        }
        else if (!strcmp(devname, DEV_NAME_BOOTFS))
        {
            devnode->pletter    = PART_LETTER_BOOTFS;
            devnode->attrib     |= DEV_NODE_ATTR_SYNMNT;
        }
        else if (!strncmp(devname, DEV_NAME_USERDISK, strlen(DEV_NAME_USERDISK)))
        {
            /* �����û��Զ����������������ʽΪ"USERDISKxx",
             * ����xx��ʾ00~99���������ָ������û��ķ�����.
             * �����û��Զ��������W~U��Χ�ڸ���ע��˳����(W~U)����.
             */
            devnode->pletter    = PART_LETTER_USER;
            devnode->attrib     |= DEV_NODE_ATTR_SYNMNT;
        }
        else if (!strcmp(devname, DEV_NAME_SDCARD0) || !strcmp(devname, DEV_NAME_SDCARD1))
        {
            devnode->attrib     |= DEV_NODE_ATTR_MOVABLE | DEV_NODE_ATTR_USBSLOT | DEV_NODE_ATTR_SYNMNT;
        }
        else if (!strcmp(devname, DEV_NAME_SCSI_DISK_00))
        {
            devnode->attrib     |= DEV_NODE_ATTR_MOVABLE | DEV_NODE_ATTR_USBSLOT;
        }
    }
    else
    {
        devnode->attrib         |= DEV_NODE_ATTR_RD | DEV_NODE_ATTR_WR | DEV_NODE_ATTR_CTL | DEV_NODE_ATTR_SYNMNT;
    }

    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                     esDEV_DevReg
*
* Description: module init function, this function will be called by system module management: MInstall,
*               user will never call it.
* Arguments  : void
*
* Returns    : if success return EPDK_OK
*              esle return EPDK_FAIL
**********************************************************************************************************************
*/
void* esDEV_DevReg(const char *classname, const char *name, const __dev_devop_t *pDevOp, void *pOpenArg)
{
    uint8_t             newclassflag = 0;
    __hdle              res;
    __dev_node_t        *devnode;
    __dev_node_t        *predevnode;
    uint8_t             err;

    __dev_classnode_t   *classnode      = &devclass_nop;
    __dev_classnode_t   *preclassnode   = &devclass_nop;

    esKRNL_SemPend(pDevSem, 0, &err);
    if (err)
    {
        __err("pend semphore faiure.");
        return NULL;
    }

    /* step 1: Ѱ��class�����������û���ҵ������½�һ�������� */
    while (classnode)
    {
        if (strcmp(classname, classnode->name) == 0)
        {
            break;
        }
        preclassnode    = classnode;
        classnode       = classnode->next;
    }
    if (classnode == 0)
    {
        /*����ռ�                              */
        classnode       = k_malloc(sizeof(__dev_classnode_t) + strlen(classname) + 1/*'/0'*/);
        if (classnode == 0)
        {
            __wrn("k_malloc fail!");
            res     = NULL;
            goto out;
        }
        /* ��classnode�ҽӵ�list��              */
        preclassnode->next      = classnode;
        classnode->next         = 0;

        /* copy name into classnode             */
        classnode->nodetype     = NODETYPE_CLASS;
        classnode->name         = (char *)((unsigned long)classnode + sizeof(__dev_classnode_t));
        strcpy(classnode->name, classname);

        /* nodelist is empty                    */
        classnode->nodelist     = 0;

        /* new class is created                 */
        newclassflag            = 1;
    }

    /* step 2: �豸����list���Ƿ�����ͬ���豸��������У��򷵻ش���     */
    devnode         = classnode->nodelist;
    predevnode      = 0;

    while (devnode)
    {
        if (strcmp(devnode->name, name) == 0)
        {
            break;
        }
        predevnode  = devnode;
        devnode     = devnode->next;
    }

    if (devnode != 0)
    {
        __wrn("dev is already registered, or name conflict!");
        res = NULL;
        goto out;
    }

    /* step 3: ����sizeofdrv�����豸�ڵ�    */
    devnode     = k_malloc(sizeof(__dev_node_t) + strlen(name) + 1/*'/0'*/);
    if (devnode == 0)
    {
        __wrn("k_malloc fail!");
        if (newclassflag)       /* ���classΪ�½����ͷţ�������class list */
        {
            k_free(classnode);
            preclassnode->next = 0;
        }
        res = NULL;
        goto out;
    }

    //��¼�豸��
    devnode->name       = (char *)((unsigned long)devnode + sizeof(__dev_node_t));
    strcpy(devnode->name, name);
    //��һ��Ϊ��
    devnode->next       = NULL;
    //��¼class
    devnode->classnode  = classnode;
    //�򿪴���Ϊ0
    devnode->opentimes  = 0;
    //��ڵ�
    devnode->status     = DEV_STAT_ACTIVE;
    //����device����
    devnode->DevOp      = *pDevOp;
    /* save open arg    */
    devnode->pOpenArg   = pOpenArg;

    devnode->hDev       = NULL;

    devnode->sem        = esKRNL_SemCreate(1);
    if (devnode->sem == NULL)
    {
        __err("create sem. failure");
        res = NULL;
        goto out;
    }

    /* �ҽӵ�list��(link to rear of the list    */
    if (predevnode == 0)
    {
        classnode->nodelist = devnode;
    }
    else
    {
        predevnode->next    = devnode;
    }

    __inf("device \"%s\\%s\" is setup.", classname, name);

    /* step 4: �����豸�ڵ������ */
    SetDevAttr((char *)classname, (char *)name, devnode);

    /* step 5: ����ע��������ļ�ϵͳ   */
    if (devnode->attrib & DEV_NODE_ATTR_PART)
    {
        esFSYS_mntparts((__hdle)devnode);
    }

    res = (__hdle)devnode;

out:
    esKRNL_SemPost(pDevSem);
    return res;
}

/*
**********************************************************************************************************************
*                                     esDEV_DevUnreg
*
* Description: module init function, this function will be called by system module management: MInstall,
*               user will never call it.
* Arguments  : void
*
* Returns    : if success return EPDK_OK
*              esle return EPDK_FAIL
**********************************************************************************************************************
*/

int32_t  esDEV_DevUnreg(__hdle hNode)
{
    int32_t         res;
    __dev_node_t    *pNode = (__dev_node_t *)hNode;
    uint8_t         err;

    esKRNL_SemPend(pDevSem, 0, &err);
    if (err)
    {
        __err("create semphore faile");
        return EPDK_FAIL;
    }

    /* ֪ͨ�ļ�ϵͳ */
    esFSYS_umntparts(hNode, 1);

    if (pNode->opentimes == 0)                      /* ���nodeû�б��κ��û�ʹ�ã�ɾ��node���ͷſռ� */
    {
        __dev_node_t *p, **pp = &(pNode->classnode->nodelist);

        for (p = *pp; p && (pNode != p); pp = &(p->next), p = p->next);

        if (!p)
        {
            __wrn("BUG when unres dev: try to destroy a devnode not exsit in node list!");
            res = EPDK_FAIL;
            goto out;
        }
        *pp = pNode->next;

        esKRNL_SemDel(pNode->sem, 0, &err);
        k_free((void *)pNode); /* �ͷ�node�ռ�  */
        res = EPDK_OK;
        goto out;
    }
    else                                            /* ������Ҫ��node��״̬����Ϊ��node   */
    {
        pNode->DevOp.Close(pNode->hDev);            /* �ر��豸���                         */
        pNode->status = DEV_STAT_INACTIVE;          /* ���豸�ڵ�Ϊ���ڵ�                   */
    }

    res = EPDK_OK;

out:
    esKRNL_SemPost(pDevSem);
    return res;
}

/*
**********************************************************************************************************************
*                                     esDEV_Open
*
* Description: module init function, this function will be called by system module management: MInstall,
*               user will never call it.
* Arguments  : void
*
* Returns    : if success return EPDK_OK
*              esle return EPDK_FAIL
**********************************************************************************************************************
*/
int32_t esDEV_Lock(__hdle hNode)
{
    esKRNL_SemPend(((__dev_node_t *)hNode)->sem, 0, NULL);
    return EPDK_OK;
}
/*
**********************************************************************************************************************
*                                     esDEV_Close
*
* Description: �ر��豸
*
* Arguments  : void
*
* Returns    : if success return EPDK_OK
*              esle return EPDK_FAIL
**********************************************************************************************************************
*/
int32_t  esDEV_Unlock(__hdle hNode)
{
    esKRNL_SemPost(((__dev_node_t *)hNode)->sem);
    return EPDK_OK;
}

/*
**********************************************************************************************************************
*                                     esDEV_Open
*
* Description: module init function, this function will be called by system module management: MInstall,
*               user will never call it.
* Arguments  : void
*
* Returns    : if success return EPDK_OK
*              esle return EPDK_FAIL
**********************************************************************************************************************
*/
__hdle esDEV_Open(__hdle hNode, uint32_t Mode)
{
    __dev_dev_t     *pDev;
    __hdle          hDev;
    __dev_node_t    *pNode = (__dev_node_t *)hNode;

    if (pNode->status == DEV_STAT_INACTIVE)
    {
        __wrn("Device has been killed when unreg!");
        return NULL;
    }

    if (pNode->opentimes == 0)
    {
        hDev    = pNode->DevOp.Open(pNode->pOpenArg, Mode);/* ���豸 */
        if (hDev == NULL)
        {
            __wrn("dev cannot be open!");
            return NULL;
        }
        pNode->hDev = hDev;                     /* �����豸��� */
    }                                           /* �����豸�������ռ� */

    pDev = (__dev_dev_t *)k_malloc(sizeof(__dev_dev_t));
    if (pDev == NULL)
    {
        __err("alloc structure failure");
        return NULL;
    }

    pDev->devnode   = pNode;                    /* ��¼nodeָ�� */
    pDev->DevOp     = pNode->DevOp;             /* �豸������ڣ�ֱ�Ӵ�__dev_node_t��copy���Է���ʹ�� */
    pDev->hDev      = pNode->hDev;              /* �豸�����ֱ�Ӵ�__dev_node_t��copy���Է���ʹ�� */

    pNode->opentimes ++;                        /* �豸�ڵ�򿪴�����1 */

    return pDev;                                /* �����豸������ */
}
/*
**********************************************************************************************************************
*                                     esDEV_Close
*
* Description: �ر��豸
*
* Arguments  : void
*
* Returns    : if success return EPDK_OK
*              esle return EPDK_FAIL
**********************************************************************************************************************
*/
int32_t  esDEV_Close(__hdle hDev)
{
    __dev_dev_t     *pDev  = (__dev_dev_t *)hDev;
    __dev_node_t    *pNode = pDev->devnode;
    uint8_t         err;

    if (pNode->opentimes)
    {
        pNode->opentimes--;    /* �豸�ڵ�򿪴�����1 */
    }

    if (pNode->opentimes == 0)  /* ����豸�ڵ��Ѿ�û������ʹ�� */
    {
        /* ����豸�ڵ�û���û�ʹ�ã�ͬʱ���������ڵ㣬����ע��Ĳ��� */
        if (pNode->status == DEV_STAT_INACTIVE)
        {
            __dev_node_t *p, **pp   = &(pNode->classnode->nodelist);

            for (p = *pp; p && (pNode != p); pp = &(p->next), p = p->next);

            if (!p)
            {
                __wrn("BUG when close dev: try to destroy a devnode not exsit in node list!");
                return EPDK_FAIL;
            }

            *pp     = pNode->next;

            /* �ͷ��豸�ڵ�ռ�õĿռ� */
            esKRNL_SemDel(pNode->sem, 0, &err);
            k_free((void *)pNode);
        }
        else /* �����Ѿ�û���û��򿪴��豸�ڵ㣬ͬʱ���豸�ڵ㲻�����ڵ� */
        {
            if (pDev->DevOp.Close(pNode->hDev) == EPDK_FAIL) /* �ر��豸��� */
            {
                __err("close device[%s] fail!", pNode->name);
            }
        }
    }

    k_free((void *)pDev);              /* �ͷ��豸���ռ�õĿռ� */

    return EPDK_OK;
}
/*
**********************************************************************************************************************
*                                     esDEV_Read
*
* Description: ��
*
* Arguments  :  pdata       ��Ҫ����������ָ��
*               size        ��Ĵ�С
*               n           ����
*               hDev        �豸���
*
* Returns    : ʵ�ʶ����Ŀ���
*
**********************************************************************************************************************
*/
uint32_t esDEV_Read(void *pdata, uint32_t size, uint32_t n, __hdle hDev)
{
    __dev_node_t *pNode = ((__dev_dev_t *)hDev)->devnode;

    if (pNode->status == DEV_STAT_INACTIVE)
    {
        __wrn("Device has been killed when unreg!");
        return 0;
    }

    return (((__dev_dev_t *)hDev)->DevOp.Read)(pdata, size, n, ((__dev_dev_t *)hDev)->hDev);
}


/*
**********************************************************************************************************************
*                                     esDEV_Write
*
* Description: д
*
* Arguments  :  pdata       ��Ҫд�������ָ��
*               size        ��Ĵ�С
*               n           ����
*               hDev        �豸���
*
* Returns    : ʵ��д��Ŀ���
*
**********************************************************************************************************************
*/
uint32_t  esDEV_Write(const void *pdata, uint32_t size, uint32_t n, __hdle hDev)
{
    __dev_node_t *pNode = ((__dev_dev_t *)hDev)->devnode;


    if (pNode->status == DEV_STAT_INACTIVE)
    {
        __wrn("Device has been killed when unreg!");
        return 0;
    }

    return (((__dev_dev_t *)hDev)->DevOp.Write)(pdata, size, n, ((__dev_dev_t *)hDev)->hDev);
}


/*
**********************************************************************************************************************
*                                     esDEV_Ioctrl
*
* Description: �豸����
*
* Arguments  : hDev         �豸���
*              cmd          ����
*              aux          ����
*              pbuffer      ����buffer
*
* Returns    : device defined
*
**********************************************************************************************************************
*/
int32_t  esDEV_Ioctl(__hdle hDev, uint32_t cmd, long aux, void *pbuffer)
{
    __dev_node_t *pNode = ((__dev_dev_t *)hDev)->devnode;

    if (pNode->status == DEV_STAT_INACTIVE)
    {
        __wrn("Device has been killed when unreg!");
        return EPDK_FAIL;
    }

    if (IS_DEVIOCSYS(cmd))
    {
        switch (cmd)
        {
            case DEV_IOC_SYS_GET_CLSNAME:
                *((char **)pbuffer)         = pNode->classnode->name;
                return EPDK_OK;

            case DEV_IOC_SYS_GET_DEVNAME:
                *((char **)pbuffer)         = pNode->name;
                return EPDK_OK;

            case DEV_IOC_SYS_GET_ATTRIB:
                *((int32_t *)pbuffer)       = pNode->attrib;
                return EPDK_OK;

            case DEV_IOC_SYS_GET_LETTER:
                *((char *)pbuffer)          = pNode->pletter;
                return EPDK_OK;

            case DEV_IOC_SYS_GET_OPENARGS:
                *((void **)pbuffer)         = pNode->pOpenArg;
                return EPDK_OK;

            default:
                return EPDK_FAIL;
        }
    }
    else
        return (((__dev_dev_t *)hDev)->DevOp.Ioctl)(((__dev_dev_t *)hDev)->hDev, cmd, aux, pbuffer);
}

/*
**********************************************************************************************************************
*
*             FS_Init
*
*  Description:
*  API function. Start the file system.
*
*  Parameters:
*  None.
*
*  Return value:
*  ==0         - File system has been started.
*  !=0         - An error has occured.
**********************************************************************************************************************
*/
int32_t dev_init(void)
{
    pDevSem = esKRNL_SemCreate(1);
    if (pDevSem == NULL)
    {
        __err("create sem failure.");
        return EPDK_FAIL;
    }

    return EPDK_OK;
}

/*
**********************************************************************************************************************
*
*             FS_Exit
*
*  Description:
*  API function. Stop the file system.
*
*  Parameters:
*  None.
*
*  Return value:
*  ==0         - File system has been stopped.
*  !=0         - An error has occured.
**********************************************************************************************************************
*/
int32_t dev_exit(void)
{
    uint32_t    x = EPDK_OK;
   uint8_t     err = 0;

    if (pDevSem)
    {
        esKRNL_SemDel(pDevSem, 0, &err);
        pDevSem = NULL;
    }

    return  x;
}

