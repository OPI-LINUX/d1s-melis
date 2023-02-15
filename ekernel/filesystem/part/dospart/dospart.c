/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                                  File System
*
*                                    (c) Copyright 2011-2014, Sunny China
*                                             All Rights Reserved
*
* File    : dospart.c
* By      : Sunny
* Version : v1.0
* Date    : 2011-3-15
* Descript: dos partition system.
* Update  : date                auther      ver     notes
*           2011-3-15 15:04:00  Sunny       1.0     Create this file.
*********************************************************************************************************
*/

#include "dospart.h"
#include "smbr.h"
#include "fsys_libs.h"
#include "fsys_debug.h"

/*
**********************************************************************************************************************
*
*             part_read
*
  Description:
  FS internal function. Read sector from device.

  Parameters:
  pDriver     - Pointer to a device driver structure.
  Unit        - Unit number.
  Sector      - Sector to be read from the device.
  pBuffer     - Pointer to buffer for storing the data.

  Return value:
  ==0         - Sector has been read and copied to pBuffer.
  <0          - An error has occured.
**********************************************************************************************************************
*/
uint32_t dospart_read(void *pData, uint32_t Sector, uint32_t N, __fsys_part_t *pPart)
{
    int32_t             x;
    __hdle              hDev;
    __fsys_dospart_p_t  *pPDPrivate = ((__fsys_dospart_p_t *)(pPart->hPDPrivate));

    if ((Sector + N) > pPDPrivate->partseccnt)
    {
        fs_log_error("read range over flow: Sector(%x)+N(%x)>partseccnt(%x)\n", Sector, N, pPDPrivate->partseccnt);
        return 0;
    }

    hDev    = pPart->hDev;
    if (hDev)
    {
        esDEV_Lock(pPart->hNode);
        x = esDEV_Read(pData, (Sector + pPDPrivate->partaddr), N, hDev);
        esDEV_Unlock(pPart->hNode);

        return  x;
    }
    return 0;
}


/*
**********************************************************************************************************************
*
*             dospart_write
*
*  Description:
*   FS internal function. Write sector to device.
*
*  Parameters:
*   pDriver     - Pointer to a device driver structure.
*   Sector      - Sector to be written to the device.
*   pBuffer     - Pointer to data to be stored.
*
*  Return value:
*   ==0         - Sector has been written to the device.
*   <0          - An error has occured.
**********************************************************************************************************************
*/
uint32_t dospart_write(const void *pData, uint32_t Sector, uint32_t N, __fsys_part_t *pPart)
{
    int32_t             x;
    __hdle              hDev;
    __fsys_dospart_p_t  *pPDPrivate = ((__fsys_dospart_p_t *)(pPart->hPDPrivate));

    if ((Sector + N) > pPDPrivate->partseccnt)
    {
        fs_log_error("write range over flow: Sector(%x)+N(%x)>partseccnt(%x)\n", Sector, N, pPDPrivate->partseccnt);
        return 0;
    }

    hDev = pPart->hDev;
    if (hDev)
    {
        esDEV_Lock(pPart->hNode);
        x   = esDEV_Write(pData, (Sector + pPDPrivate->partaddr), N, hDev);
        esDEV_Unlock(pPart->hNode);
        return  x;
    }
    return 0;
}

/*
**********************************************************************************************************************
*
*             dospart_ioctl
*
  Description:
  FS internal function. Execute device command.

  Parameters:
  pPart       - Pointer to a partition driver structure.
  Cmd         - Command to be executed.
  Aux         - Parameter depending on command.
  pBuffer     - Pointer to a buffer used for the command.

  Return value:
  Command specific. In general a negative value means an error.
**********************************************************************************************************************
*/
int32_t dospart_ioctl(__fsys_part_t *pPart, uint32_t Cmd, long Aux, void *pBuffer)
{
    int32_t             x;
    __hdle              hDev;
    __fsys_dospart_p_t  *pPDPrivate = ((__fsys_dospart_p_t *)(pPart->hPDPrivate));

    hDev = pPart->hDev;
    if (!hDev)
    {
        return EPDK_FAIL;
    }

    switch (Cmd)
    {
        case PART_IOC_USR_FLUSHCACHE:
            esDEV_Lock(pPart->hNode);
            esDEV_Ioctl(pPart->hDev, DEV_IOC_USR_FLUSH_CACHE, 0, NULL);
            esDEV_Unlock(pPart->hNode);
            break;

        case PART_IOC_USR_GETPARTSIZE:
            *((uint32_t *)pBuffer)  = pPDPrivate->partseccnt;
            break;

        case PART_IOC_USR_GETSCTSIZE:
            *((uint32_t *)pBuffer)  = pPDPrivate->partsecsize;
            break;

        /*********************oldddddddddddddddd************************************/
        case FSYS_PART_CMD_GET_STATUS:              /* ��ȡ״̬��Ϣ                 */
            return 0;

        case FSYS_PART_CMD_GET_INFO:                /* ��ȡ��Ϣ                     */
            esDEV_Lock(pPart->hNode);
            x = esDEV_Ioctl(hDev, DEV_CMD_GET_INFO, Aux, pBuffer);
            esDEV_Unlock(pPart->hNode);
            return x;

        case FSYS_PART_CMD_INC_BUSYCNT:             /*  */
            break;

        case FSYS_PART_CMD_DEC_BUSYCNT:             /*  */
            break;

        case FSYS_PART_CMD_GET_PARTSIZE:
            esDEV_Lock(pPart->hNode);
            *((uint32_t *)pBuffer)   = pPDPrivate->partseccnt;
            esDEV_Unlock(pPart->hNode);
            break;
        /*********************oldddddddddddddddd************************************/

        default:
            return EPDK_FAIL;
    }
    return EPDK_OK;

}

/*
**********************************************************************************************************************
*
*             dospart_mount
*
  Description:


  Parameters:

  Return value:
    EPDK_OK/EPDK_FAIL
**********************************************************************************************************************
*/

int32_t dospart_mount(__fsys_part_t *pPart)
{
    __fsys_dospart_p_t  *pPDPrivate = NULL;

    /* ����˽�е����ݿռ�       */
    pPart->hPDPrivate   = (__hdle)malloc(sizeof(__fsys_dospart_p_t));
    pPDPrivate          = ((__fsys_dospart_p_t *)(pPart->hPDPrivate));

    if (!(pPart->hPDPrivate))
    {
        fs_log_warning("mount part fail!\n");
        return EPDK_FAIL;
    }

    esDEV_Lock(pPart->hNode);

    //for analy patition
    {
        struct parsed_partitions    *state;
        struct part_geometry        *p_partgeo;
        __dev_blkinfo_t             blkinf;
        int32_t                     i, count;

        state   = check_partition(pPart->hDev);
        if (state == NULL)
        {
            return EPDK_FAIL;
        }

        p_partgeo = (struct part_geometry *)malloc(state->limit * sizeof(struct part_geometry));
        if (!p_partgeo)
        {
            return EPDK_FAIL;
        }
        memset(p_partgeo, 0, state->limit * sizeof(struct part_geometry));

        count = 0;
        for (i = 0; i < state->limit; i++)
        {
            if (state->parts[i].size > 2)
            {
                memcpy(&p_partgeo[count], &state->parts[i], sizeof(struct part_geometry));
                count++;
            }
        }
        pPDPrivate->partaddr    = p_partgeo[pPart->Unit].from;
        pPDPrivate->partseccnt  = p_partgeo[pPart->Unit].size;

        /* set part sector size */
        memset(&blkinf, 0x00, sizeof(blkinf));
        esDEV_Ioctl(pPart->hDev, DEV_CMD_GET_INFO, 0, &blkinf);

        pPDPrivate->partsecsize = blkinf.secsize;

        fs_log_info("part geometer: start sector: %x, size: %dm\n", (pPDPrivate->partaddr),
                    (pPDPrivate->partseccnt * pPDPrivate->partsecsize) >> 20);

        free(state);
        free(p_partgeo);
    }

    esDEV_Unlock(pPart->hNode);

    return EPDK_OK;
}

/*
**********************************************************************************************************************
*
*             npart_unmount
*
  Description:


  Parameters:

  Return value:
    EPDK_OK/EPDK_FAIL
**********************************************************************************************************************
*/
int32_t dospart_unmount(__fsys_part_t *pPart, int32_t force)
{
    int32_t     res;

    res     = deactivate_part(pPart, force);

    if (res == EPDK_OK || force)
    {
        pPart->status = FSYS_PARTSTATUS_DEAD;
        free((void *)pPart->hPDPrivate);
    }

    return res;
}
/*
**********************************************************************************************************************
*
*             ndskpart_identify
*
  Description:ʶ����������hDev��ָ��Ŀ��豸Ϊ��ͨ���̷�����ʽ����ô����EPDK_OK


  Parameters:

  Return value:
    EPDK_OK/EPDK_FAIL
**********************************************************************************************************************
*/
uint32_t dospart_identify(__hdle hDev)
{
    struct parsed_partitions    *state;
    struct part_geometry        *p_partgeo;
    int32_t                     i, count;

    /* ʶ��������������state��    */
    state       = check_partition(hDev);

    if (state == NULL)
    {
        return 0;
    }

    p_partgeo   = malloc(state->limit * sizeof(struct part_geometry));
    if (!p_partgeo)
    {
        return 0;
    }
    memset(p_partgeo, 0, state->limit * sizeof(struct part_geometry));

    count = 0;
    for (i = 0; i < state->limit; i++)
    {
        if (state->parts[i].size > 2)
        {
            memcpy(&p_partgeo[count], &state->parts[i], sizeof(struct part_geometry));
            count++;
        }
    }
    /* �ͷ�state��state������check_partition������    */
    free(state);
    free(p_partgeo);

    return count;
}

__fsys_pd_t dospart_type =
{
    .name   = "dospart",
    .Ops    = {
        .read       = dospart_read,
        .write      = dospart_write,
        .ioctl      = dospart_ioctl,
        .mount      = dospart_mount,
        .unmount    = dospart_unmount,
        .identify   = dospart_identify,
    }
};

/*
**********************************************************************************************************************
*
*             fsys_npart_init
*
  Description:


  Parameters:

  Return value:
    EPDK_OK/EPDK_FAIL
**********************************************************************************************************************
*/
int32_t fsys_dospart_init(void)
{
    return esFSYS_pdreg((__hdle)&dospart_type);
}

/*
**********************************************************************************************************************
*
*             fsys_npart_exit
*
  Description:


  Parameters:

  Return value:
    EPDK_OK/EPDK_FAIL
**********************************************************************************************************************
*/
int32_t fsys_dospart_exit(void)
{
    return esFSYS_pdunreg((__hdle)&dospart_type);
}
