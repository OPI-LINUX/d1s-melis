/*
**************************************************************************************************************
*                                                    ePDK
*                                   the Easy Portable/Player Develop Kits
*                                              desktop system
*
* File      :   FileList
*                 ��������ṹ��Ϊ�����һ��ʼ�����ļ����ٶ�.
* By        :   Libaiao
* Func  :
* Version   :   v1.0
* ============================================================================================================
* 2010-6-5  15:24:52  Libaiao  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#include <log.h>
#include "beetles_app.h"

#include "FileList.h"

#define     FileListMalloc  eLIBs_malloc

/*#define KARAOKE_LYRIC_USE_MALLOC 1


#if KARAOKE_LYRIC_USE_MALLOC
#define esMEMS_Balloc(x) esMEMS_Malloc(0, x)
#define esMEMS_Bfree(x, s) esMEMS_Mfree(0, x)
#endif
*/
//�ݹ麯���е��ã����ں����еĻ����������ջ���
//char            fullpath[RAT_MAX_FULL_PATH_LEN];


/*
************************************************************************************************************************
*Function   :
*
*Description    :
*
*Arguments      :
*
*Return         :
*
************************************************************************************************************************
*/
//��Сд��ĸת���ɴ�С��ĸ
static __s32  LowerCase_2_UpperCase(char *str)
{
    char *p_str = NULL ;

    if (!str)
    {
        __msg("string is null , return \n");
        return EPDK_FAIL ;
    }

    p_str = str ;

    while (*p_str != '\0')
    {
        if ((*p_str <= 'z') && (*p_str >= 'a'))
        {
            *p_str =  *p_str - 'a' + 'A' ;
        }

        p_str++ ;
    }

    return EPDK_OK ;
}

// �ļ��б������������ļ�������������
static file_item_t  *file_list_sort(file_item_t  *p_file_item_list, __s32 file_item_total_num)
{
    __u32   i,  j ;
    file_item_t *p_cur_item = NULL, *p_next_item = NULL, *p_head_item = NULL ;
    __s32   ret =  0 ;
    __bool  is_swap = EPDK_FALSE ;

    if (!p_file_item_list)
    {
        __msg(" file list is null , return \n ") ;
        return NULL;
    }

    if (file_item_total_num <= 0)
    {
        __msg(" file item total num is invalue return \n ") ;
        return NULL ;
    }

    for (i = 0 ; i < file_item_total_num - 1 ; i++)
    {
        p_head_item = p_file_item_list ;
        p_cur_item = p_head_item ;
        is_swap = EPDK_FALSE ;

        for (j = 0 ; j < file_item_total_num - i - 1 ; j++)
        {
            char temp_str1[RAT_MAX_FULL_PATH_LEN] = {0}, temp_str2[RAT_MAX_FULL_PATH_LEN] = {0} ;
            p_next_item = p_cur_item->next ;
            eLIBs_strcpy(temp_str1, p_cur_item->name);  //�ȱ�����ļ���
            eLIBs_strcpy(temp_str2, p_next_item->name);
            LowerCase_2_UpperCase(p_cur_item->name);    //���ļ����е�Сд��ĸ����ͳһת���ɴ�д
            LowerCase_2_UpperCase(p_next_item->name);
            ret = eLIBs_strcmp((const char *)(p_cur_item->name), (const char *)(p_next_item->name)) ;    //�Ƚ��ļ���
            //__msg(" ret = %d\n " , ret );
            eLIBs_strcpy(p_cur_item->name, temp_str1);
            eLIBs_strcpy(p_next_item->name, temp_str2);

            if (ret < 0)
            {
                //__msg( " not need to change file pointer \n " ) ;
                p_cur_item = p_cur_item->next ; //û�н���ָ�룬��ǰ���ָ����һ��
            }
            else if (ret > 0)
            {
                file_item_t *nod1, *nod4 ;

                // __msg( " start to swap file pointer \n" );
                if (p_cur_item == p_head_item)   //��ǰָ��Ϊͷָ��
                {
                    p_head_item = p_next_item ;
                    p_file_item_list = p_head_item ;
                }

                // ����ָ��
                nod1 = p_cur_item->previous ;
                nod4 = p_next_item->next ;
                nod1->next = p_next_item ;
                nod4->previous = p_cur_item ;
                p_cur_item->next = nod4 ;
                p_cur_item->previous = p_next_item ;
                p_next_item->previous = nod1 ;
                p_next_item->next = p_cur_item ;
                is_swap =  EPDK_TRUE ;      //�ñ�־λ����ʾ�н�����λ��
            }
            else
            {
                __msg(" unvaluable value \n ");
            }
        }

        if (EPDK_FALSE == is_swap)
        {
            __msg("file has been sorted finished , break \n");
            break ;
        }
    }

    /*
    {       // for test
        file_item_t* temp ;
        temp = p_file_item_list ;
        while( temp->next != p_file_item_list )
        {
            __msg("temp->name = %s\n " , temp->name);
            temp = temp->next ;
        }

        __msg("temp->name = %s\n " , temp->name);
    }
    */
    return p_file_item_list ;
}
static __s32 FileList(void)
{
    file_item_t *item;
    file_list_t *list;
    return EPDK_OK;
}

//�������������ļ���Ϣ, �½�һ���ļ���Ŀ,�洢�ļ����ļ�����ص���Ϣ
file_item_t *new_file_item(ES_DIRENT   *pDirent)
{
    file_item_t *file_item = NULL;
    __inf("Enter %s\r\n", __FUNCTION__);

    if (pDirent == NULL)
    {
        return NULL;
    }

    //file_item = (file_item_t*)esMEMS_Balloc(sizeof(file_item_t));
    __wrn("new_file_item 1\n");
    file_item = (file_item_t *)eLIBs_malloc(sizeof(file_item_t));

    if (file_item  == NULL)
    {
        __wrn("memory is not enough for new a file list\n");
        return NULL;
    }

    __wrn("new_file_item 2\n");
    eLIBs_memset(file_item, 0, sizeof(file_item_t));
    __wrn("new_file_item 3\n");
    file_item->name_len = eLIBs_strlen(pDirent->d_name);

    if (file_item->name_len != 0)
    {
        file_item->name_len += 5;       //�������һ��byte
        __wrn("new_file_item 4\n");
        file_item->name = (char *)eLIBs_malloc(file_item->name_len);

        if (file_item->name == NULL)
        {
            return NULL;
        }

        eLIBs_memset(file_item->name, 0, file_item->name_len);
        eLIBs_strcpy(file_item->name, pDirent->d_name);
    }

    file_item->file_size = pDirent->d_size;
    file_item->fatdirattr = pDirent->fatdirattr;
    __wrn("new_file_item 5\n");
    return file_item;
}

//ɾ��һ���ļ���Ŀ,�ͷ���ص��ڴ�
__s32 delete_file_item(file_item_t *file_item)
{
    if (file_item == NULL)
    {
        return EPDK_FAIL;
    }

    if (file_item->name_len != 0)
    {
        if (file_item->name != NULL)
        {
            //esMEMS_Bfree(file_item->name, file_item->name_len);
            eLIBs_free(file_item->name);
            file_item->name = NULL;
        }
    }

    //esMEMS_Bfree(file_item, sizeof(file_item));
    eLIBs_free(file_item);
    return EPDK_OK;
}

//-------------------------------------------------------------------------------
//function      :
//descriptions      : ����һ���µ��ļ��б��ú�������ָ��Ŀ¼�������ļ��к��ļ�
//               �ú���ִ��ʱ�����ļ���������
//               �ļ��е����γ�һ������
//              �ļ������γ�һ������
//              ������������ϲ����ļ�����ǰ��
//arguments     :   ext_para, Ϊ��չ����
//return            : �����������ļ��ĸ���,
//-------------------------------------------------------------------------------
__s32  new_file_list(const char *path, file_item_t **pp_item_list, void *ext_para)
{
    ES_DIRENT   *pDirent = NULL;
    ES_DIR      *pDir = NULL;
    file_item_t *list = NULL;
    file_item_t *first = NULL;
    file_item_t *next = NULL;
    file_item_t *folder_first = NULL;           //�ļ����б�
    file_item_t *folder_list = NULL;
    file_item_t *folder_next = NULL;
    __bool  no_folder = 0, no_file = 0 ;
    __u32 count = 0, folder_count = 0;
    __s32 ret = 0;
    pDir = eLIBs_opendir(path);

    if (pDir == NULL)
    {
        return 0;
    }

    eLIBs_rewinddir(pDir);

    /*
        pDirent = eLIBs_readdir(pDir);              //��õ�һ���ļ����ļ���
        if(pDirent == NULL)
        {
            return 0;
        }

        if(pDirent->fatdirattr & FSYS_ATTR_DIRECTORY)
        {
            folder_first = new_file_item(pDirent);
            if(folder_first != NULL)
            {
                folder_first->fatdirattr = FSYS_ATTR_DIRECTORY;
                folder_list = folder_first;
            }
        }
        else
        {
            first = new_file_item(pDirent);
            if(first == NULL)
            {
                return 0;
            }
            list = first;
            //pp_item_list = first;                     //�б��׵�ַ���ظ�������
            count++;
        }
    */
    while (1)
    {
        pDirent = eLIBs_readdir(pDir);

        if (pDirent == NULL)
        {
            break;
        }

        /* skip "." and ".." sub directory */
        if (0 == eLIBs_strcmp(".", pDirent->d_name)
            || 0 == eLIBs_strcmp("..", pDirent->d_name))
        {
            continue;
        }

        /*
        ret = eLIBs_strcmp(pDirent->d_name, ".");
        if(ret == 0)
        {
            continue;
        }
        ret = eLIBs_strcmp(pDirent->d_name, "..");
        if(ret == 0)
        {
            continue;
        }
        */
        if (pDirent->fatdirattr & FSYS_ATTR_DIRECTORY)      //�ļ��е����γ�һ������
        {
            if (folder_first == NULL)
            {
                folder_first = new_file_item(pDirent);

                if (folder_first != NULL)
                {
                    folder_first->fatdirattr = FSYS_ATTR_DIRECTORY;
                    folder_list = folder_first;
                    folder_count++;
                }
                else
                {
                    break;
                }
            }
            else
            {
                folder_next = new_file_item(pDirent);

                if (folder_next != NULL)
                {
                    folder_list->next = folder_next;
                    folder_next->previous = folder_list;
                    folder_list = folder_next;                  //folder_list Ϊ��������һ����
                    folder_count++;
                }
                else
                {
                    break;
                }
            }
        }
        else                                        //�ļ������γ�һ������
        {
            if (first == NULL)
            {
                first = new_file_item(pDirent);

                if (first == NULL)
                {
                    break;
                }

                list = first;
                count++;
            }
            else
            {
                next = new_file_item(pDirent);

                if (next != NULL)
                {
                    list->next = next;
                    next->previous = list;
                    list = next;                                //list Ϊ��������һ����
                    count++;
                }
                else
                {
                    break;
                }
            }
        }
    }

    __msg("get %d folders & %d files totally\n", folder_count, count);

    if (folder_first == NULL)       //�ļ�������Ϊ�գ���ʾû���ļ�����no_folder��1����ʾû���ļ���
    {
        no_folder = 1 ;
    }

    if (first == NULL)
    {
        no_file = 1 ;
    }

    if (!no_file)    //���ļ��γ�һ��˫��ѭ������
    {
        list->next = first;
        first->previous = list;     //�γ�˫������
        first = file_list_sort(first, count) ;       //���ļ����ļ�����������, �����ź���������׵�ַ
        list = first->previous ;         //���������ļ�������е�����ʹlist ����ָ���ļ�����Ľ�β
    }

    if (!no_folder)  //���ļ����γ�һ��˫��ѭ������
    {
        folder_list->next = folder_first;
        folder_first->previous = folder_list;
        folder_first = file_list_sort(folder_first, folder_count) ;
        folder_list = folder_first->previous ;   //���������ļ�������е�����ʹfolder_list  ����ָ������Ľ�β
    }

    //����������ϲ�����,�ļ���������ǰ
    if (!no_folder)
    {
        if (!no_file)
        {
            folder_list->next = first;                      //�ļ������ļ���������
            first->previous = folder_list;                  //�����ļ���
            list->next = folder_first;                      //�ļ��б�Ľ�βָ���ļ����б�ʼ�ĵط�
            folder_first->previous = list;
        }

        *pp_item_list = folder_first;                       //�б��׵�ַ���ظ�������
    }
    else
    {
        if (!no_file)                                     //ֻ���ļ�
        {
            *pp_item_list = first;                      //�б��׵�ַ���ظ�������
        }
        else
        {
            __msg("nothing in this folder\n");
        }
    }

    eLIBs_closedir(pDir);
    return count + folder_count;
}
//ɾ��һ���ļ��б�(ʹ������ɾ��)
//item_list = �б��׵�ַ
//total = ��������
__s32 delete_file_list(file_item_t  *item_list, __u32 total)
{
    __u32 i = 0;
    file_item_t *list = NULL;
    file_item_t *temp = NULL;

    if (item_list == NULL)
    {
        return EPDK_FAIL;
    }

    list = item_list;

    for (i = 0; i < total; i++)
    {
        if (list == NULL)
        {
            break;
        }

        if (list->next == NULL)
        {
            break;
        }

        temp = list->next;              //�ȱ�����һ����Ŀ��ַ

        if (temp != NULL)
        {
            delete_file_item(list);
            list = temp;
        }
        else
        {
            break;
        }
    }

    return EPDK_OK;
}

//���б���ɾ��һ���ڵ�
__s32 delete_file_item_from_list(file_item_t *item, file_list_t *list)
{
    file_item_t *previous = NULL;
    file_item_t *next = NULL;
    __s32 ret = 0;

    if (item == NULL)
    {
        return EPDK_FAIL;
    }

    if (list == NULL)
    {
        return EPDK_FAIL;
    }

    if (list->item_list == item)
    {
        list->item_list = item->next;       //ɾ�����ǵ�һ��ͷ�ڵ�,��Ҫ�ı�item_list��ֵ
    }

    if (item->next == item)                 //������ֻ��һ���ڵ�
    {
        __msg("=========================only one item\n");
        ret = delete_file_item(item);
        list->cur_item = NULL;
        list->item_list = NULL;
        return ret;
    }
    else
    {
        if (item->previous != NULL)
        {
            previous = item->previous;
        }

        if (item->next != NULL)
        {
            next = item->next;
        }

        if (previous == next)
        {
            //��ȱ�ʾֻ��������Ŀ
            __msg("=========================there are two items \n");
            next->next = next;
            next->previous = next;
            ret = delete_file_item(item);
        }
        else                        //��������
        {
            __msg("=========================more than two items \n");
            previous->next = next;
            next->previous = previous;
            ret = delete_file_item(item);
        }
    }

    return ret;
}

//cur_itemΪ��ǰ��Ŀ
//itemΪҪ�������Ŀ,���뵽cur_item�ĺ���
__s32 insert_file_item_to_list(file_item_t *cur_item, file_item_t *item)
{
    file_item_t *previous = NULL;
    file_item_t *next = NULL;

    if (cur_item == NULL)
    {
        return EPDK_FAIL;
    }

    if (item == NULL)
    {
        return EPDK_FAIL;
    }

    next = cur_item->next;
    cur_item->next = item;
    item->previous = cur_item;
    item->next = next;
    next->previous = item;
    return EPDK_OK;
}

//Ϊfilelist ��������һ���ڵ�
file_list_t *new_file_list_nod(const char *path, void *ext_para)
{
    file_list_t *nod = NULL;
    nod = (file_list_t *)eLIBs_malloc(sizeof(file_list_t));

    if (nod == NULL)
    {
        __err("memeor is not enough for file list\n");
    }

    eLIBs_memset((void *)nod, 0, sizeof(file_list_t));
    nod->total = new_file_list(path, &(nod->item_list), ext_para);

    if (nod->total != 0)
    {
        __msg("total = %d\n", nod->total);
    }

    eLIBs_strcpy(nod->file_path, path);
    return nod;
}

//ɾ��һ���ڵ�,��ɾ���ڵ��µ�itemlist
__s32 delete_file_list_nod(file_list_t *nod)
{
    __s32 ret = 0;

    if (nod == NULL)
    {
        return EPDK_FAIL;
    }

    if (nod->item_list != NULL)
    {
        ret = delete_file_list(nod->item_list, nod->total);
    }

    eLIBs_free(nod);
    return ret;
}

//���洢�豸�Ƿ�ready
//
file_list_t *new_file_root_list_nod(void *ext_para)
{
    ES_DIRENT dirent;
    file_item_t *item = NULL;
    file_list_t *root_nod = NULL;
    __s32 ret = 0, count = 0;
    char disk_name[RAT_MAX_PARTITION][4] = {{0},};
    __inf("Enter %s\r\n", __FUNCTION__);
    root_nod = (file_list_t *)eLIBs_malloc(sizeof(file_list_t));

    if (root_nod == NULL)
    {
        __err("memeor is not enough for file list\n");
        return NULL;
    }

    eLIBs_memset(root_nod, 0, sizeof(file_list_t));
    ret = rat_get_partition_name(RAT_USB_DISK, disk_name, 0);

    if ((ret == EPDK_OK) && (eLIBs_strlen(disk_name[0]) != 0))
    {
        //__here__ ;
        eLIBs_memset(&dirent, 0, sizeof(ES_DIRENT));
        dirent.d_size = 0;
        dirent.fatdirattr = RAT_MEDIA_TYPE_USB_DEVICE;
        //eLIBs_strcpy(root_nod->USB_path, disk_name[0]);       //�����̷�
        eLIBs_strcpy(dirent.d_name, disk_name[0]);
        // __wrn("new_file_root_list_nod 2\n");
        item = new_file_item(&dirent);

        if (item == NULL)
        {
            __err("%s %d error in file list!\r\n", __FILE__, __LINE__);
        }
        else
        {
            item->next = item;                  // add by linyaoshu 2011.9.1
            item->previous = item;              // add by linyaoshu 2011.9.1
            count++;
        }
    }

    eLIBs_memset(disk_name, 0, sizeof(disk_name));
    ret = rat_get_partition_name(RAT_SD_CARD, disk_name, 0);

    if ((ret == EPDK_OK) && (eLIBs_strlen(disk_name[0]) != 0))
    {
        char    disk_name_t[16] = {0}; //chengf  add
        //__here__ ;
        eLIBs_memset(&dirent, 0, sizeof(ES_DIRENT));
        dirent.d_size = 0;
        dirent.fatdirattr = RAT_MEDIA_TYPE_SD_DEVICE;
#if  TF_ENCRYPTION_EX

        if (rat_is_encryption_partition_Insert(disk_name_t, 1)) //���ڼ��ܷ���//chengf  add
        {
            eLIBs_strcpy(dirent.d_name, disk_name[1]);          //chengf  add
        }
        else
        {
            eLIBs_strcpy(dirent.d_name, disk_name[0]);
        }

#else
        eLIBs_strcpy(dirent.d_name, disk_name[0]);
#endif

        if (item == NULL)                               //ֻ��һ���̷�
        {
            // __wrn("new_file_root_list_nod 4\n");
            item = new_file_item(&dirent);

            //__wrn("new_file_root_list_nod item=%08x\n",item);
            if (item != NULL)
            {
                item->next = item;
                item->previous = item;
                count++;
            }
        }
        else
        {
            // __wrn("new_file_root_list_nod 5\n");
            item->next = new_file_item(&dirent);

            if (item->next != NULL)
            {
                item->next->previous = item;            //�������̷�
                item->next->next = item;
                item->previous  = item->next;
                count++;
            }
            else
            {
                item->next = item;                  //ֻ��һ���̷�
                item->previous = item;
            }
        }
    }

    if (item != NULL)
    {
        // __inf("new_file_root_list_nod 6\n");
        root_nod->item_list = item ;
        root_nod->total = count ;
    }
    else
    {
        //__wrn("new_file_root_list_nod 7\n");
        __msg(" item is null ,get new_file_root_list_nod fail \n");
        eLIBs_free(root_nod) ;   //һ���̷���û��
        root_nod = NULL ;
    }

    //__here__ ;
    return root_nod;
}

char root_device_str[2][20] =
{
    {"USB"},
    {"SD"}
};

char *get_root_device_disp_name(char *string, __u8 attr)
{
    char *str;

    if (attr == RAT_MEDIA_TYPE_USB_DEVICE)
    {
        str = root_device_str[0];
        eLIBs_strcpy(string, str);
        return str;
    }
    else if (attr == RAT_MEDIA_TYPE_SD_DEVICE)
    {
        str = root_device_str[1];
        eLIBs_strcpy(string, str);
        return str;
    }

    return NULL;
}


__s32 file_list_get_root_path(char *path, file_list_t *list, __u32 index)
{
    if (list == NULL)
    {
        return EPDK_FAIL;
    }

    if (index >= list->total)
    {
        return EPDK_FAIL;
    }

    return EPDK_FAIL;
}
__s32 update_file_root_list_nod(void *ext_para)
{
    return EPDK_OK;
}
//���������Ŵ�nod������
file_item_t *get_file_list_item(file_list_t *nod, __u32 index)
{
    __u32 i = 0, count = 0;
    file_item_t *temp = NULL;
    file_item_t *list = NULL;

    if (nod == NULL)
    {
        return NULL;
    }

    __msg("nod->total = %d  index = %d \n ", nod->total, index);

    if (nod->item_list == NULL)
    {
        __msg("nod->item_list is null \n");
        return NULL;
    }

    if (index >= nod->total)
    {
        __msg("index is bigger than total \n");
        return NULL;
    }

    list = nod->item_list;

    for (i = 0; i <= index; i++)
    {
        temp =  list;
        list = list->next;
    }

    return temp;
}


char *get_file_list_item_name(file_list_t *nod, __u32 index)
{
    file_item_t *temp = NULL;
    temp = get_file_list_item(nod, index);

    if (temp == NULL)
    {
        return NULL;
    }

    return temp->name;
}

//����ļ���Ŀ�ļ�����
rat_media_type_t  get_file_list_item_file_type(file_item_t *item)
{
    rat_media_type_t media_type = RAT_MEDIA_TYPE_UNKNOWN;

    if (item == NULL)
    {
        return RAT_MEDIA_TYPE_UNKNOWN;
    }

    if (item->fatdirattr & FSYS_ATTR_DIRECTORY)
    {
        return RAT_MEDIA_TYPE_FOLDER;
    }

    if (item->fatdirattr == RAT_MEDIA_TYPE_SD_DEVICE)
    {
        return RAT_MEDIA_TYPE_SD_DEVICE;
    }

    if (item->fatdirattr == RAT_MEDIA_TYPE_USB_DEVICE)
    {
        return RAT_MEDIA_TYPE_USB_DEVICE;
    }

    media_type = rat_get_file_type(item->name);
    return media_type;
}

//���ݸ����Ķ����ڵ㣬ɾ������filelist ����
//ע��ֻ����ɾ������
__s32 delete_file_list_chain(file_list_t *top_nod)
{
    file_list_t *parent_nod = NULL;
    file_list_t *child_nod = NULL;

    if (top_nod == NULL)
    {
        return EPDK_FAIL;
    }

    parent_nod = top_nod;
    child_nod = parent_nod->child;              //�����ӽڵ��ַ

    if (child_nod == NULL)
    {
        delete_file_list_nod(parent_nod);       //ɾ�����ڵ�
        return EPDK_OK;
    }

    while (child_nod != NULL)
    {
        delete_file_list_nod(parent_nod);       //ɾ�����ڵ�
        parent_nod = child_nod;                 //ָ����һ���ڵ�
        child_nod = parent_nod->child;          //�����ӽڵ��ַ
    }

    return EPDK_OK;
}

//char            sub_dir[RAT_MAX_FULL_PATH_LEN];               //���ں����У��������ջ���
//���һ���ļ���Ŀ¼���ļ���Ŀ�����������ļ���
__s32 get_item_num_in_dir(const char *dirpath, __u32 *total)
{
    ES_DIR          *dirp;
    ES_DIRENT       *direntp;
    char            sub_dir[RAT_MAX_FULL_PATH_LEN];
    //char            *sub_dir;
    static __s32    dir_level = 0;

    if (dirpath == NULL)
    {
        return EPDK_FAIL;
    }
    else if (dir_level > C_MAX_DIR_SCAN_LEVEL)
    {
        //return EPDK_FAIL;
    }
    else
    {
        dir_level++;
        __msg("dir level is %d\n", dir_level);
    }

    //__here__;
    {
        __s32 len;
        len = eLIBs_strlen(dirpath);
        __msg("len = %d\n", len);
        __msg("dirpath=%s\n", dirpath);
    }
    dirp = (ES_DIR *)eLIBs_opendir(dirpath);

    //__here__;
    if (dirp == NULL)
    {
        //__here__;
        return EPDK_FAIL;
    }

    //__here__;
#if 0
    sub_dir = eLIBs_malloc(RAT_MAX_FULL_PATH_LEN);

    if (!sub_dir)
    {
        return EPDK_FAIL;
    }

#endif

    while ((direntp = (ES_DIRENT *)eLIBs_readdir(dirp)) != NULL)
    {
        //__here__;
        if (direntp->fatdirattr & FSYS_ATTR_DIRECTORY)
        {
            /* skip "." and ".." sub directory */
            if (0 == eLIBs_strcmp(".", direntp->d_name)
                || 0 == eLIBs_strcmp("..", direntp->d_name))
            {
                continue;
            }

            //__here__;
            eLIBs_memset(sub_dir, 0, RAT_MAX_FULL_PATH_LEN);
            eLIBs_strcpy(sub_dir, dirpath);
            eLIBs_strcat(sub_dir, "\\");
            //__here__;
            eLIBs_strcat(sub_dir, (const char *)direntp->d_name);

            //__here__;
            if (get_item_num_in_dir(sub_dir, total) == EPDK_FAIL)
            {
                return EPDK_FAIL;
            }

            if (dir_level > 0)
            {
                dir_level--;
                __msg("dir level is %d\n", dir_level);
            }

            __msg("---total: %d, dirpath: %s, sub_dir: %s\n", *total, dirpath, sub_dir);
        }
        else
        {
            (*total)++;
            __msg("---total: %d, dirpath: %s, file: %s\n", *total, dirpath, (const char *)direntp->d_name);
        }

        //__here__;
    }

#if 0

    if (sub_dir)
    {
        eLIBs_free(sub_dir);
        sub_dir = NULL;
    }

#endif

    //__here__;
    if (eLIBs_closedir(dirp) == EPDK_FAIL)
    {
        //__here__;
        return EPDK_FAIL;
    }

    (*total)++;
    __msg("----total: %d\n", *total);
    return EPDK_OK;
}


//ɾ��һ���ļ���
__s32 delete_dir(char *dir_name, file_op_info_t *op_info)
{
    ES_DIR          *dir = NULL;
    ES_DIRENT       *dirent = NULL;
    char            fullpath[RAT_MAX_FULL_PATH_LEN];
    __s32           ret = EPDK_OK;
    //static __s32    dir_level = 0;

    if (dir_name == NULL)
    {
        __msg("------------------------------------------input param error\n");
        return EPDK_FAIL;
    }
    else if (op_info->dir_level > C_MAX_DIR_SCAN_LEVEL)
    {
        if (op_info != NULL)
        {
            op_info->percent = 100;
        }

        __msg("------------------------------------------dir_level is over than MAX_DIR_SCAN_LEVEL, failed\n");
        //return EPDK_FAIL;
    }
    else
    {
        op_info->dir_level++;
        __msg("------------------------------------------dir_level is %d\n", op_info->dir_level);
    }

    /*
    if(_is_dir_exist(dir_name) == EPDK_FAIL)
    {
        info->percent = 101;
        return EPDK_FAIL;
    }
    */
    eLIBs_memset(fullpath, 0, RAT_MAX_FULL_PATH_LEN);
    /*open dir*/
    dir = eLIBs_opendir(dir_name);

    if (dir == NULL)
    {
        return EPDK_FAIL;
    }

    while (1)
    {
        /*read dir*/
        dirent = eLIBs_readdir(dir);

        if (dirent == NULL)
        {
            __msg("----read dir %s is empty\n", dir_name);
            break;
        }

        eLIBs_memset(fullpath, 0, RAT_MAX_FULL_PATH_LEN);
        eLIBs_strcpy(fullpath, dir_name);
        eLIBs_strcat(fullpath, "\\");
        eLIBs_strcat(fullpath, (const char *)dirent->d_name);
        __msg("----read dir %s ---fullpath: %s\n", (const char *)dirent->d_name, fullpath);

        if (dirent->fatdirattr & FSYS_ATTR_DIRECTORY)
        {
            if (0 == eLIBs_strcmp(".", dirent->d_name)
                || 0 == eLIBs_strcmp("..", dirent->d_name))
            {
                continue;
            }

            ret = delete_dir(fullpath, op_info);

            if (ret == EPDK_FAIL)
            {
                __msg("------------------------_inner_delete_dir failed---fullpath: %s\n", fullpath);
                ret = EPDK_FAIL;
                break;
            }
            else
            {
                __msg("------------------------_inner_delete_dir success----fullpath: %s\n", fullpath);
            }

            if (op_info->dir_level > 0)
            {
                op_info->dir_level--;
                __msg("------------------------------------------dir_level is %d\n", op_info->dir_level);
            }
        }
        else
        {
            if (eLIBs_remove(fullpath) == -1)
            {
                __msg("-------------------------remove file %s failed\n", fullpath);
                ret = EPDK_FAIL;
                break;
            }
            else
            {
                __msg("------------------------remove file %s successfully\n", fullpath);

                if (op_info != NULL)
                {
                    op_info->finish_num++;
                    op_info->percent = (op_info->finish_num * 100) / op_info->total_num;

                    if (op_info->OnDraw != NULL)
                    {
                        op_info->OnDraw(op_info);
                    }
                }
            }
        }
    }

    if (op_info != NULL)
    {
        op_info->finish_num++;
        op_info->percent = (op_info->finish_num * 100) / op_info->total_num;

        if (op_info->OnDraw != NULL)
        {
            op_info->OnDraw(op_info);
        }
    }

    eLIBs_closedir(dir);
    ret = eLIBs_rmdir(dir_name);
    __msg("------------------------eLIBs_rmdir %s--- ret: %d\n", dir_name, ret);

    if (ret == EPDK_FAIL)
    {
        if (op_info != NULL)
        {
            op_info->percent = 101;
        }
    }

    //dir_level = 0;
    return ret;
}




//����һ���ļ���
__s32 copy_dir(char *dst_dir, char *src_dir, file_op_info_t *op_info)
{
    ES_DIR          *dir = NULL;
    ES_DIRENT       *dirent = NULL;
    char            fullpath[RAT_MAX_FULL_PATH_LEN];
    char            dstpath[RAT_MAX_FULL_PATH_LEN];
    __s32           ret = EPDK_OK;
    ES_FILE *fp1, *fp2, *fp3;
    //static __s32    dir_level = 0;

    if (src_dir == NULL)
    {
        __msg("------------------------------------------input param error\n");
        return EPDK_FAIL;
    }
    else if (op_info->dir_level > C_MAX_DIR_SCAN_LEVEL)
    {
        if (op_info != NULL)
        {
            op_info->percent = 100;
        }

        __msg("------------------------------------------dir_level is over than MAX_DIR_SCAN_LEVEL, failed\n");
        //return EPDK_FAIL;
    }
    else
    {
        op_info->dir_level++;
        __msg("------------------------------------------dir_level is %d\n", op_info->dir_level);
    }

    eLIBs_memset(fullpath, 0, RAT_MAX_FULL_PATH_LEN);
    /*open dir*/
    dir = eLIBs_opendir(src_dir);

    if (dir == NULL)
    {
        return EPDK_FAIL;
    }

    while (1)
    {
        /*read dir*/
        dirent = eLIBs_readdir(dir);

        if (dirent == NULL)
        {
            __msg("----read dir %s is empty\n", src_dir);
            break;
        }

        eLIBs_memset(fullpath, 0, RAT_MAX_FULL_PATH_LEN);
        eLIBs_strcpy(fullpath, src_dir);
        eLIBs_strcat(fullpath, "\\");
        eLIBs_strcat(fullpath, (const char *)dirent->d_name);
        eLIBs_memset(dstpath, 0, RAT_MAX_FULL_PATH_LEN);
        eLIBs_strcpy(dstpath, dst_dir);
        eLIBs_strcat(dstpath, "\\");
        eLIBs_strcat(dstpath, (const char *)dirent->d_name);
        __msg("----read dir %s ---fullpath: %s\n", (const char *)dirent->d_name, fullpath);

        if (dirent->fatdirattr & FSYS_ATTR_DIRECTORY)
        {
            if (0 == eLIBs_strcmp(".", dirent->d_name)
                || 0 == eLIBs_strcmp("..", dirent->d_name))
            {
                continue;
            }

            fp1 = eLIBs_fopen(dstpath, "rb");

            if (NULL == fp1)
            {
                if (0 != eLIBs_mkdir(dstpath))
                {
                    __wrn("create dir [ %s ] fail\n", dstpath);
                    return EPDK_FAIL;
                }
            }
            else
            {
                eLIBs_fclose(fp1);
                fp1 = NULL;
            }

            ret = copy_dir(dstpath, fullpath, op_info);

            if (ret == EPDK_FAIL)
            {
                __msg("------------------------_inner_delete_dir failed---fullpath: %s\n", fullpath);
                ret = EPDK_FAIL;
                break;
            }
            else
            {
                __msg("------------------------_inner_delete_dir success----fullpath: %s\n", fullpath);
            }

            if (op_info->dir_level > 0)
            {
                op_info->dir_level--;
                __msg("------------------------------------------dir_level is %d\n", op_info->dir_level);
            }
        }
        else
        {
            __s32 len;
            __s64 total_size, file_size;
            char *pbuf = NULL;
            char path[4];
            eLIBs_strncpy(path, dstpath, 3);
            path[3] = '\0';
            total_size = eLIBs_GetVolFSpace(path);
            file_size = eLIBs_GetFileSize(fullpath);

            if (file_size > total_size)
            {
                return EPDK_FAIL;
            }

            fp1 = eLIBs_fopen(dstpath, "wb+");

            if (NULL == fp1)
            {
                return EPDK_FAIL;
            }

            fp2 = eLIBs_fopen(fullpath, "rb+");

            if (NULL == fp2)
            {
                __msg("parse file err!\n");
                eLIBs_fclose(fp1);
                return EPDK_FAIL;
            }

            pbuf = esMEMS_Palloc(CAT_READ_BUFFER_SIZE >> 10, 0);

            if (NULL == pbuf)
            {
                __wrn("palloc err\n");
                eLIBs_fclose(fp1);
                eLIBs_fclose(fp2);
                return EPDK_FAIL;
            }

            while (1)
            {
                len = eLIBs_fread(pbuf, 1, CAT_READ_BUFFER_SIZE, fp1);

                if (0 < len)
                {
                    eLIBs_fwrite(pbuf, 1, len, fp2);
                }
                else
                {
                    break;
                }

                esKRNL_TimeDly(5);
            };

            eLIBs_fclose(fp1);

            eLIBs_fclose(fp2);

            esMEMS_Pfree(pbuf, 1);
        }
    }

    eLIBs_closedir(dir);
    return ret;
}


