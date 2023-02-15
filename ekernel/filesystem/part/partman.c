/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                  File System
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : partman.c
* By      : Sunny
* Version : v1.0
* Date    : 2011-3-15
* Descript: partition management of file system.
* Update  : date                auther      ver     notes
*           2011-3-15 14:58:06  Sunny       1.0     Create this file.
*********************************************************************************************************
*/
#include "errno.h"
#include "part.h"
#include "fsys_debug.h"
#include "fsys_libs.h"
#include <log.h>

/* ���ɷ���ķ����б� */
__fsys_part_t   *pPartFTbl[FSYS_MAX_FPARTS] = {0};

/* ϵͳ�ڲ��̶�����ķ����б� */
__fix_part_t    pPartXTbl[FSYS_MAX_XPARTS]  = { {'A', 0}, {'B', 0}, {'C', 0}, {'D', 0}, {'W', 0}, {'X', 0}, {'Y', 0}, {'Z', 0} };

/* �����û��Զ�������ķ����б� */
__fix_part_t    pPartUTbl[FSYS_MAX_UPARTS]  = { {'U', 0}, {'V', 0} };

static __hdle   mnt_parts_tid   = NULL;
static __hdle   pContolSem      = NULL;         /* ���������̻߳��ѡ�˯�߿����� */
__hdle          CurhNode        = NULL;
__hdle          pPartSem        = NULL;         /* for lock parts table */
__fsys_pd_t     *pPDRoot        = NULL;

int32_t         test_and_freeze_partfs(__hdle h_sb);
void            unfreezepart(__hdle h_sb);
static int32_t  __mount_parts(__hdle hNode);


static void wakeup_mnt_thread(void)
{
    esKRNL_SemPost(pContolSem);
}

static void mnt_parts_task(void *p_arg)
{
    while (1)
    {
        /* ���ط����߳�����˯�� */
        esKRNL_SemPend(pContolSem, 0, NULL);

        /* �յ���������֪ͨ���̱߳����ѣ���ʼ���ص�ǰ�豸�ڵ� */
        __mount_parts(CurhNode);
    }
}

int32_t fsys_vpart_init(void)
{
    uint32_t    err = 0;

    /************************************************************/
    /*  �����̻߳��ѡ�˯�߿����ź�����                          */
    /*  ���������̴߳�������˯��״̬��                        */
    /*  ֻ�й��ط���ʱ����������                                */
    /************************************************************/
    pContolSem  = esKRNL_SemCreate(0);
    err         = !pContolSem;

    /* �������������� */
    pPartSem    = esKRNL_SemCreate(1);
    err         |= !pPartSem;

    if (err)
    {
        __err("fail to create sem.");
        return EPDK_FAIL;
    }

    /* ��ʼ��ʱû�й����豸�ڵ� */
    CurhNode = NULL;

    /************************************************************/
    /*  �������������߳�                                        */
    /************************************************************/

	/*notes:  stack size too low, ntfs disk check log file will  fail, do not reduce this stach size*/
    mnt_parts_tid = awos_task_create("fs-pmnt", mnt_parts_task, NULL, 0x4000, RT_TIMER_THREAD_PRIO - 1, 10);

    return mnt_parts_tid ? EPDK_OK : EPDK_FAIL;
}

/*
******************************************************************
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
******************************************************************
*/

int32_t fsys_vpart_exit(void)
{
    awos_task_delete(mnt_parts_tid);
    return EPDK_OK;
}

/*
******************************************************************
*
*             fsys_regist_part
*
*  Description:
*   ע����豸���ļ�ϵͳ��
*
*  Parameters:
*   pFullName   - Fully qualified name.
*   pFilename   - Address of a pointer, which is modified to point
*                 to the file name part of pFullName.
*
*  Return value:
*   <0          - Unable to find the device.
*   >=0         - Index of the device in the device information
*                 table.
******************************************************************
*/
int32_t esFSYS_pdreg(__hdle hPD)
{
    int32_t         res;
    __fsys_pd_t     *p, *pd = (__fsys_pd_t *)hPD;

    esKRNL_SemPend(pPartSem, 0, NULL);

    /* �ж��ļ�ϵͳ�Ƿ��Ѿ���ע�� */
    for (p = pPDRoot; p; p = p->next)
    {
        if (strcmp(p->name, pd->name) == 0)
        {
            fs_log_info("pd already registered!");
            res = EPDK_FAIL;
            goto out;
        }
    }

    /* ��������Ϊ0��user                                        */
    pd->nUsr    = 0;
    /* ��������Ϊ��Ч����                                       */
    pd->status  = PD_STAT_ACTIVE;
    /* �������ҽӵ�pPDRoot��                                    */
    pd->next    = pPDRoot;
    pPDRoot     = pd;
    res         = EPDK_OK;

out:
    esKRNL_SemPost(pPartSem);

    return res;
}
/*
******************************************************************
*
*             fsys_regist_part
*
*  Description:
*   �γ����豸
*
*  Parameters:
*   pFullName   - Fully qualified name.
*   pFilename   - Address of a pointer, which is modified to point
*                 to the file name part of pFullName.
*
*  Return value:
*   <0          - Unable to find the device.
*   >=0         - Index of the device in the device information
*                 table.
******************************************************************
*/
int32_t esFSYS_pdunreg(__hdle hPD)
{
    uint32_t        res;
    __fsys_pd_t     *pd = (__fsys_pd_t *)hPD;
    __fsys_pd_t     *p, **pp = &pPDRoot;

    esKRNL_SemPend(pPartSem, 0, NULL);

    if (pd->nUsr)
    {
        fs_log_info("part driver is used by someone!");
        res = EPDK_FAIL;
        goto out;
    }

    for (p = *pp; p && (pd != p); pp = &(p->next), p = p->next);

    if (!p)
    {
        fs_log_info("BUG when unres pd: try to destroy a fs not exsit in pd list!");
        res = EPDK_FAIL;
        goto out;
    }
    pd->status = PD_STAT_INACTIVE;
    *pp = pd->next;
    res = EPDK_OK;

out:
    esKRNL_SemPost(pPartSem);

    return res;
}

/*
******************************************************************
*
*             esFSYS_mntparts
*
*  Description:
*   ע����豸���ļ�ϵͳ��
*
*  Parameters:
*   hNode       - �豸�ڵ���.
*
*  Return value:
*   EPDK_OK     - registered ok
*   EPDK_FAIL   - fail.
******************************************************************
*/
static int32_t __mount_parts(__hdle hNode)
{
    int32_t         i, j, res;
    int32_t         nPart;
    __hdle          hDev = NULL;
    __fsys_pd_t     *pPD = NULL;
    uint8_t         *classname  = NULL, *devname = NULL, dletter = 0;
    void            *openargs   = NULL;
    int32_t         devattrib   = 0;
    uint32_t        last_lun    = 0;

    last_lun    = 0;
    esKRNL_SemPend(pPartSem, 0, NULL);
    if (NULL == hNode)
    {
        res = EPDK_FAIL;
        goto out;
    }

    /************************************************************/
    /* ʶ���豸�ϵķ�������                                     */
    /************************************************************/
    hDev    = esDEV_Open(hNode, 0);
    if (!hDev)
    {
        res = EPDK_FAIL;
        goto out;
    }

    esDEV_Ioctl(hDev, DEV_IOC_SYS_GET_CLSNAME,  0,  &classname) ;
    esDEV_Ioctl(hDev, DEV_IOC_SYS_GET_DEVNAME,  0,  &devname)   ;
    esDEV_Ioctl(hDev, DEV_IOC_SYS_GET_ATTRIB,   0,  &devattrib) ;
    esDEV_Ioctl(hDev, DEV_IOC_SYS_GET_LETTER,   0,  &dletter)   ;

    __log("classname=%s",   classname);
    __log("devname=%s",     devname);

    if (0 == strncmp(DEV_CLASS_DISK,    classname, strlen(DEV_CLASS_DISK)) &&
        ( 0 == strncmp(DEV_NAME_SCSI_DISK_00,   devname, strlen(DEV_NAME_SCSI_DISK_00)) ||
          0 == strncmp(DEV_NAME_USB_CDROM_00,   devname, strlen(DEV_NAME_USB_CDROM_00)) ||
          0 == strncmp(DEV_NAME_SDCARD,         devname, strlen(DEV_NAME_SDCARD))
        ))
    {
        esDEV_Ioctl(hDev, DEV_IOC_SYS_GET_OPENARGS, 0, &openargs);
        if (openargs)
        {
            last_lun = (uint32_t)(openargs);
        }
        else
        {
            last_lun = 1;
        }
    }
    else//��������������豸����Ĭ�������һ����
    {
        last_lun = 1;
    }

    __inf("last_lun=%d",    last_lun);

    if (1 == last_lun)
    {
        __log("find last lun.");
    }

    for (pPD = pPDRoot, nPart = 0; pPD && !nPart; pPD = pPD->next)
    {
        if (pPD->Ops.identify && pPD->status == PD_STAT_ACTIVE)
        {
            /* ����Ҫ����identify����,��Ҫ��pd��ʹ���߼�һ  */
            pPD->nUsr++;

            nPart   = pPD->Ops.identify(hDev);
            /* ���ڵ���identify��������,��Ҫ��pd��ʹ���߼�һ */
            pPD->nUsr--;
        }

        if (nPart)
        {
            break;
        }
    }

    esDEV_Close(hDev);

    if (!pPD)
    {
        __err("cant find pd.");
        res = EPDK_FAIL;
        goto out;
    }

    __log("nPart = %d.", nPart);

    /************************************************************/
    /* װ�ط���                                                 */
    /************************************************************/
    for (i = 0; i < nPart; i++)
    {
        __fsys_part_t   *pPart;
        int32_t         k;
        uint8_t         buf[12] = {0};

        /********************************************************/
        /* ����part���ݽṹ                              */
        /********************************************************/
        /* ��ȡ�������ݽṹ�ڴ� */
        pPart = calloc(1, sizeof(__fsys_part_t));
        if (!pPart)
        {
            __log("malloc fail!");
            fs_err  = -ENOMEM;
            res     = EPDK_FAIL;
            goto out;
        }

        pPart->last_lun     = last_lun;

        /* ���豸����þ�� */
        hDev    = esDEV_Open(hNode, 0);
        if (!hDev)
        {
            /* �ͷŷ�����ڴ�ռ� */
            free(pPart);

            __log("device cannot be opened!");
            fs_err  = -ENODEV;
            res     = EPDK_FAIL;
            goto out;
        }

        /* ���÷����� */
        strncpy(pPart->dname, devname, MAX_PART_NAME_LEN);

        k   = strlen(pPart->dname);
        melis_itoa(i, buf);

        strncpy(&pPart->dname[k], buf, MAX_PART_NAME_LEN - k - 1);

        /* ��¼�豸������ڵ�ͷ����� */
        pPart->hNode        = hNode;
        pPart->hDev         = hDev;
        pPart->Unit         = i;
        pPart->error        = 0;
        pPart->attr         = ((devattrib & DEV_NODE_ATTR_RD) ? FSYS_PARTATTR_DEVR : 0) | ((devattrib & DEV_NODE_ATTR_WR) ? FSYS_PARTATTR_DEVW : 0);
        pPart->updateflag   = EPDK_FALSE;

        /********************************************************/
        /* �ҽӷ�������                                         */
        /********************************************************/
        /* ����Ҫ����mount����,��Ҫ��pdʹ���߼�1 */
        pPD->nUsr++;

        /* mount���� */
        if (pPD->Ops.mount(pPart) == EPDK_FAIL)
        {
            /* ����mountʧ��,��Ҫ��pdʹ���߼�1 */
            if (pPD->nUsr)
            {
                pPD->nUsr--;
            }

            /* �ͷŷ�����ڴ�ռ� */
            free(pPart);
            esDEV_Close(hDev);
            __log("part mount fail!");
            res     = EPDK_FAIL;
            goto out;
        }
        pPart->pPD  = pPD;

        /********************************************************/
        /* �������ҽӵ���������                                 */
        /********************************************************/
        pPart->letter   = 0xff;

        if (dletter == PART_LETTER_FREE) /* ���ɷ���ķ��� */
        {
            for (j = 0; j < FSYS_MAX_FPARTS; j++)
            {
                if (!pPartFTbl[j])
                {
                    pPart->letter   = PART_LETTER_FREESTART + j;
                    pPartFTbl[j]    = pPart;
                    __log("%s: \"%s\\%s\" is linked to symbel \"%c\".", pPart->pPD->name, classname, pPart->dname, pPart->letter);
                    break;
                }
            }

            if (j == FSYS_MAX_FPARTS)
            {
                __log("too many parts!");
            }
        }
        else if (dletter == PART_LETTER_USER) /* �����û��Զ���ķ��� */
        {
            for (j = FSYS_MAX_UPARTS - 1; j >= 0; j--)
            {
                if (!pPartUTbl[j].part)
                {
                    pPart->letter       = pPartUTbl[j].letter;
                    pPartUTbl[j].part   = pPart;
                    __log("%s: \"%s\\%s\" is linked to symbel \"%c\".", pPart->pPD->name, classname, pPart->dname, pPart->letter);
                    break;
                }
            }
            if (j < 0)
            {
                __log("too many user define parts!");
            }
        }
        else /* ϵͳ�ڲ��̶�����ķ��� */
        {
            for (j = 0; j < FSYS_MAX_XPARTS; j++)
            {
                if (pPartXTbl[j].letter == dletter + i)
                {
                    if (!pPartXTbl[j].part)
                    {
                        pPart->letter       = dletter + i;
                        pPartXTbl[j].part   = pPart;
                        __log("%s: \"%s\\%s\" is linked to symbel \"%c\".", pPart->pPD->name, classname, pPart->dname, pPart->letter);
                    }
                    else
                    {
                        __log("part \"%c\" is already exist!", dletter + i);
                    }

                    break;
                }
            }
            if (j == FSYS_MAX_XPARTS)
            {
                __err("wanna invalid fixed part letter!");
            }
        }

        /* ��������̷�ʧ�� */
        if (pPart->letter == 0xff)
        {
            pPD->Ops.unmount(pPart, 1);
            free(pPart);
            /*esDEV_Close(hDev);    //device have been closed when part unmount*/
            pPD->nUsr--;
            fs_err  = -ENOSPC;
            res     = EPDK_FAIL;
            goto out;
        }

        /********************************************************/
        /* �ҽӷ����ļ�ϵͳ                                     */
        /********************************************************/
        pPart->status   = FSYS_PARTSTATUS_UNUSED;
        if (devattrib & DEV_NODE_ATTR_FS)
        {
            if (esFSYS_mntfs(pPart) == EPDK_OK)
            {
                pPart->status   = FSYS_PARTSTATUS_FSUSED;
                pPart->attr     |= FSYS_PARTATTR_FS;
            }
            else
            {
                __err("mount %c failure.", pPart->letter);
                pPart->attr     &= ~FSYS_PARTATTR_FS;
            }
        }
    }

    res     = EPDK_OK;

out:
    /* ���ط������� */
    CurhNode    = NULL;
    esKRNL_SemPost(pPartSem);
    return res;
}

/*
******************************************************************
*
*             esFSYS_blkdevreg
*
*  Description:
*   ע����豸���ļ�ϵͳ��
*
*  Parameters:
*   hNode       - �豸�ڵ���.
*
*  Return value:
*   EPDK_OK     - registered ok
*   EPDK_FAIL   - fail.
******************************************************************
*/
int32_t esFSYS_mntparts(__hdle hNode)
{
    __hdle          hDev;
    int32_t         devattrib;
	
    esKRNL_SemPend(pPartSem, 0, NULL);

    /************************************************************/
    /* ���豸����ȡ�豸����                                   */
    /************************************************************/
    hDev    = esDEV_Open(hNode, 0);
    if (!hDev)
    {
        __log("open device node failed");
        esKRNL_SemPost(pPartSem);
        return EPDK_FAIL;
    }

    esDEV_Ioctl(hDev, DEV_IOC_SYS_GET_ATTRIB, 0, &devattrib);
    esDEV_Close(hDev);
    esKRNL_SemPost(pPartSem);

    if (devattrib & DEV_NODE_ATTR_SYNMNT)
    {
        /************************************************************/
        /* ͬ�������豸�ڵ㣬ֱ�ӹ����豸����                       */
        /************************************************************/
        __mount_parts(hNode);
    }
    else
    {
        /************************************************************/
        /* �첽�����豸�ڵ㣬���ѷ��������̹߳����豸�ڵ����       */
        /************************************************************/
        CurhNode    = hNode;
        wakeup_mnt_thread();

    }

    return EPDK_OK;
}

/*
******************************************************************
*
*             esFSYS_blkdevunreg
*
*  Description:
*   �γ����豸
*
*  Parameters:
*   hNode       - �豸�ڵ�
*
*  Return value:
*   <0          - Unable to find the device.
*   >=0         - Index of the device in the device information
*                 table.
******************************************************************
*/
int32_t esFSYS_umntparts(__hdle hNode, uint32_t force)
{
    int32_t         ret, i, j, match_i;
    __fsys_part_t   *pPart;
    __fsys_part_t   *pParts[FSYS_MAX_PARTS], **ppParts[FSYS_MAX_PARTS];

    esKRNL_SemPend(pPartSem, 0, NULL);
	if (esFSYS_vfslock())
	{
		__log("esFSYS_umntparts err when enter vfs mutex\n");
		ret =  EPDK_FAIL;
		goto out1;
	}

    /************************************************************/
    /* ���Է����ϵ��ļ�ϵͳ�Ƿ���Զ��Ტʵʩ����               */
    /************************************************************/
    match_i = 0;
    for (i = 0; i < FSYS_MAX_FPARTS; i++)
    {
        pPart   = pPartFTbl[i];
        if (pPart && (pPart->hNode == hNode))
        {
            pParts[match_i] = pPart;
            ppParts[match_i] = &pPartFTbl[i];
            match_i++;
        }
    }

    for (i = 0; i < FSYS_MAX_XPARTS; i++)
    {
        pPart   = pPartXTbl[i].part;
        if (pPart && (pPart->hNode == hNode))
        {
            pParts[match_i] = pPart;
            ppParts[match_i] = (__fsys_part_t **)&pPartXTbl[i];
            match_i++;
        }
    }

    for (i = 0; i < FSYS_MAX_UPARTS; i++)
    {
        pPart   = pPartUTbl[i].part;
        if (pPart && (pPart->hNode == hNode))
        {
            pParts[match_i] = pPart;
            ppParts[match_i] = (__fsys_part_t **)&pPartUTbl[i];
            match_i++;
        }
    }

    for (i = 0; i < match_i; i ++)
    {
        if (test_and_freeze_partfs(pParts[i]->hFSPrivate) && !force)
        {
            /* ����æ���޷����ᣬж�ؿ��豸���������ж�     */
            for (j = 0; j < i; j++)
            {
                unfreezepart(pParts[j]->hFSPrivate);
            }
            fs_err = -EBUSY;
            ret = EPDK_FAIL;
            goto out;
        }
    }

    /************************************************************/
    /* ж���豸�ϵ����еķ������ļ�ϵͳ                 */
    /************************************************************/
    for (i = 0; i < match_i; i++)
    {
        pPart   = pParts[i];
        if (pPart->status != FSYS_PARTSTATUS_FSUSED)
        {
            continue;
        }

        /* ж�ط����ϵ��ļ�ϵͳ                                 */
        ret = esFSYS_umntfs(pPart, force);
        if (ret != EPDK_OK)
            __log("unexpect error when unload partfs, force==%d, part:%c", force, pPart->letter);
        else
        {
            pPart->status = FSYS_PARTSTATUS_UNUSED;
        }
    }

    /************************************************************/
    /* �ͷ��豸�ϵ����еķ����ķ�������                         */
    /************************************************************/
    for (i = 0; i < match_i; i++)
    {
        uint32_t    busy = 0;

        pPart = pParts[i];

        /* �ӷ���������unmount����                              */
        if (pPart->pPD && pPart->pPD->Ops.unmount)
        {
            busy |= !!pPart->pPD->Ops.unmount(pPart, force);

            /* ��ɷ���ж�������ٷ�������������ɷ���ռ�������ٷ�
               �����                                           */
            if (busy && force)
            {
                __log("unexpect error when unmount part%c, force==%d", pPart->letter, force);
            }
        }
        *ppParts[i] = 0;
    }

    ret = EPDK_OK;

out:
	esFSYS_vfsunlock();
out1:
    esKRNL_SemPost(pPartSem);
    return ret;
}
