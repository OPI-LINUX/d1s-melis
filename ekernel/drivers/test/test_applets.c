#include <rtthread.h>
#include <finsh_api.h>
#include <mod_defs.h>
#include <list.h>
#include "msh.h"
#include <finsh.h>
#include <finsh_api.h>
#include <shell.h>
extern int32_t console_LKeyDevEvent(__input_dev_t *dev, uint32_t type, uint32_t code, int32_t value);


#if 1 //auto test
static __krnl_event_t  *key_lock = NULL;
static int32_t is_running = 0;
static uint32_t task_id    = 0;
static int32_t all_test   = 0;

static int key_sim(uint32_t key_code,uint32_t delay_time)
{
    __u8 err;
    esKRNL_SemPend(key_lock, 0, &err);
    console_LKeyDevEvent(NULL,  EV_KEY,     key_code,  1); //1 is down;   0 is up;
    console_LKeyDevEvent(NULL,  EV_SYN,     0,      0   ); //1 is down;   0 is up;
    rt_thread_delay(2);
    console_LKeyDevEvent(NULL,  EV_KEY,     key_code,  0); //1 is down;   0 is up;
    console_LKeyDevEvent(NULL,  EV_SYN,     0,      0   ); //1 is down;   0 is up;
    esKRNL_SemPost(key_lock);
    
    if(delay_time<100 || delay_time>1000)
    {
        esKRNL_TimeDly(delay_time*100);//second
    }
    else
    {
        esKRNL_TimeDly(delay_time/10);//ms
    }
    return 0;
}


static void auto_movie_test(void*arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
    //@0
    while(test_cnt --)
    {
        key_sim(KPAD_ENTER,3);//�������
        key_sim(KPAD_UP,1);//��ҳ
        key_sim(KPAD_UP,1);//��ҳ
        key_sim(KPAD_DOWN,1);//��ҳ
        key_sim(KPAD_DOWN,1);//��ҳ
        
		key_sim(KPAD_ENTER,60*60);//����Ӱ,����1h

        key_sim(KPAD_RETURN,4);//���������
        key_sim(KPAD_RETURN,3);//����������
    }
	rt_kprintf("auto_test movie complete!\n");
	
	if(all_test == 1)
	{
		return ;//������ͨ������
	}
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);
}


static void auto_music_test(void* arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
    key_sim(KPAD_RIGHT,300);//���ص�һ��Ԫ��
    while(test_cnt --)
    {
        key_sim(KPAD_ENTER,3);//�������
        key_sim(KPAD_UP,6);//��ҳ
        key_sim(KPAD_UP,6);//��ҳ
        key_sim(KPAD_DOWN,6);//��ҳ
        key_sim(KPAD_DOWN,6);//��ҳ
        
        key_sim(KPAD_DOWN,60*60);//������,����1h

        key_sim(KPAD_RETURN,4);//���������
        key_sim(KPAD_RETURN,3);//����������
    }
	key_sim(KPAD_LEFT,300);//���ص�һ��Ԫ��
	rt_kprintf("auto_test music complete!\n");
	if(all_test == 1)
	{
		return ;//������ͨ������
	}
	
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);
}
static void auto_power_test(void *arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
	
    key_sim(KPAD_RIGHT,300);//@2
    key_sim(KPAD_RIGHT,300);
    while(test_cnt --)
    {
        key_sim(KPAD_ENTER,3);//�������
        key_sim(KPAD_ENTER,1);//�Զ�����
        key_sim(KPAD_ENTER,1);
        key_sim(KPAD_ENTER,1);
        key_sim(KPAD_ENTER,1);
        key_sim(KPAD_ENTER,1);
        key_sim(KPAD_ENTER,1);
        
        key_sim(KPAD_DOWN,1);
        key_sim(KPAD_ENTER,1);//��ʱ�ػ�
        key_sim(KPAD_ENTER,1);
        key_sim(KPAD_ENTER,1);
        key_sim(KPAD_ENTER,1);
        key_sim(KPAD_ENTER,1);
        
        key_sim(KPAD_RETURN,2);
    }
    key_sim(KPAD_LEFT,300);//���ص�һ��Ԫ��
    key_sim(KPAD_LEFT,300);
	rt_kprintf("auto_test power complete!\n");
	if(all_test == 1)
	{
		return ;//������ͨ������
	}
	
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);

}
static void auto_fm_test(void* arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
	
    key_sim(KPAD_RIGHT,300);//@3
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    
    while(test_cnt --)
    {
        key_sim(KPAD_ENTER,4);//����Ӧ��
        key_sim(KPAD_DOWN,2);//��̨
        key_sim(KPAD_DOWN,2);//��̨
        key_sim(KPAD_DOWN,2);//��̨
        key_sim(KPAD_DOWN,2);//��̨
        
        key_sim(KPAD_DOWN,60*60);//����1h
        
        key_sim(KPAD_RETURN,4);//�˻�������
    }
	
    key_sim(KPAD_LEFT,300);//���ص�һ��Ԫ��
    key_sim(KPAD_LEFT,300);
    key_sim(KPAD_LEFT,300);
	rt_kprintf("auto_test fm complete!\n");
	if(all_test == 1)
	{
		return ;//������ͨ������
	}
	
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);
}


static void auto_photo_test(void* arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
	
    key_sim(KPAD_RIGHT,300);//@4
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);

    while(test_cnt --)
    {
        key_sim(KPAD_ENTER,3);//�������
        key_sim(KPAD_DOWN,1);//���·�ҳ
        key_sim(KPAD_DOWN,1);//���·�ҳ
        key_sim(KPAD_UP,1);//���·�ҳ
        key_sim(KPAD_UP,1);//���·�ҳ
        
        key_sim(KPAD_ENTER,2);//��ͼƬ
        key_sim(KPAD_DOWN,4);//���·�ҳ
        key_sim(KPAD_DOWN,4);//���·�ҳ
        key_sim(KPAD_UP,4);//���·�ҳ
        key_sim(KPAD_UP,4);//���·�ҳ

        key_sim(KPAD_ENTER,60*60);//�Զ�����1h
        
        key_sim(KPAD_ENTER,5);//ֹͣ�Զ�����
        
        key_sim(KPAD_RETURN,2);//���������
        key_sim(KPAD_RETURN,3);//����������
    }
	
    key_sim(KPAD_RIGHT,300);//���ص�һ��Ԫ��
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
	rt_kprintf("auto_test photo complete!\n");
	if(all_test == 1)
	{
		return ;//������ͨ������
	}
	
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);
}

static void auto_explorer_test(void* arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
	
    key_sim(KPAD_RIGHT,300);//@5
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    
    while(test_cnt --)
    {
        key_sim(KPAD_ENTER,5);// SD/USB
        key_sim(KPAD_ENTER,3);//SD list
        key_sim(KPAD_ENTER,2);//enter node
        key_sim(KPAD_DOWN,500);//next
        key_sim(KPAD_RETURN,2);
        
        key_sim(KPAD_DOWN,800);//next
        key_sim(KPAD_DOWN,800);//next
        key_sim(KPAD_UP,800);//next
        key_sim(KPAD_UP,800);//next
        key_sim(KPAD_RETURN,3);//����SD list
        key_sim(KPAD_RETURN,3);//����������
    }
	
    key_sim(KPAD_RIGHT,300);//���ص�һ��Ԫ��
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
	rt_kprintf("auto_test explorer complete!\n");
	if(all_test == 1)
	{
		return ;//������ͨ������
	}
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);
}


static void auto_ebook_test(void*arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
	
    key_sim(KPAD_RIGHT,300);//@6
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);
    key_sim(KPAD_RIGHT,300);

    while(test_cnt --)
    {
        key_sim(KPAD_ENTER,3);//�������
        key_sim(KPAD_ENTER,3);//��������
        key_sim(KPAD_RIGHT,1);//��ҳ
        key_sim(KPAD_RIGHT,1);//��ҳ
        key_sim(KPAD_LEFT,1);//��ҳ
        key_sim(KPAD_LEFT,1);//��ҳ
        
        key_sim(KPAD_MENU,2);//����˵�
        key_sim(KPAD_ENTER,2);//������ɫѡ��
        key_sim(KPAD_DOWN,2);//�ƶ�����ɫ
        key_sim(KPAD_ENTER,2);//ѡ����ɫ
        key_sim(KPAD_DOWN,2);//�ƶ�����ɫ
        key_sim(KPAD_RETURN,2);//�˳�

        key_sim(KPAD_MENU,2);//����˵�
        key_sim(KPAD_DOWN,2);
        key_sim(KPAD_ENTER,2);//�����ٶ�ѡ��
        key_sim(KPAD_DOWN,2);//ѡ���
        key_sim(KPAD_DOWN,2);//ѡ���
        key_sim(KPAD_DOWN,2);//ѡ����
        key_sim(KPAD_DOWN,2);//ѡ����
        key_sim(KPAD_RETURN,2);//�˳�

		key_sim(KPAD_MENU,2);//����˵�
        key_sim(KPAD_DOWN,2);
        key_sim(KPAD_DOWN,2);
        key_sim(KPAD_ENTER,2);//������תҳ��
        key_sim(KPAD_ENTER,2);
		key_sim(KPAD_ENTER,2);
		key_sim(KPAD_ENTER,2);
        key_sim(KPAD_ENTER,2);//��ת����1111ҳ
        key_sim(KPAD_UP,2);
        key_sim(KPAD_UP,2);//�ƶ���ȷ��
        key_sim(KPAD_ENTER,2);//���ȷ��
        
        
        key_sim(KPAD_RETURN,3);//���������
        key_sim(KPAD_RETURN,2);//����������
    }

    key_sim(KPAD_RIGHT,300);//���ص�һ��Ԫ��
    key_sim(KPAD_RIGHT,300);
	rt_kprintf("auto_test ebook complete!\n");
	if(all_test == 1)
	{
		return ;//������ͨ������
	}
	
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);
}


static void auto_setting_test(void*arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
	
    key_sim(KPAD_LEFT,300);//@7
    while(test_cnt --)
    {
        key_sim(KPAD_ENTER,3);//������
        key_sim(KPAD_ENTER,1);//����
        key_sim(KPAD_ENTER,1);//
        
        key_sim(KPAD_DOWN,500);
        key_sim(KPAD_ENTER,500);//��������
        key_sim(KPAD_ENTER,500);//
        
        key_sim(KPAD_DOWN,500);
        key_sim(KPAD_ENTER,500);//����
        key_sim(KPAD_ENTER,500);//
        
        key_sim(KPAD_DOWN,500);
        key_sim(KPAD_ENTER,500);//���
        key_sim(KPAD_ENTER,500);//
        
        key_sim(KPAD_DOWN,500);
        key_sim(KPAD_ENTER,2);//�ָ�����
        key_sim(KPAD_ENTER,3);//��
        
        key_sim(KPAD_DOWN,500);//����汾
        key_sim(KPAD_RETURN,2);
    }

    key_sim(KPAD_RIGHT,300);//���ص�һ��Ԫ��
	rt_kprintf("auto_test setting complete!\n");
	if(all_test == 1)
	{
		return ;//������ͨ������
	}
	
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);
}

static void auto_all_test(void* arg);

void(*pfun[9])(void*arg) = 
{
    auto_movie_test,
    auto_music_test,
    auto_power_test,
    auto_fm_test,
    auto_photo_test,
    auto_explorer_test,
    auto_ebook_test,
    auto_setting_test,
    auto_all_test,
};
	
static void auto_all_test(void* arg)
{
    int32_t test_cnt= *(unsigned int*)arg;
	uint8_t err;
    int32_t id = 0;
	int32_t temp_cnt = 1;
	all_test = 1;
    while(test_cnt -- )
    {
        id = esKSRV_Random(7);
        pfun[id](&temp_cnt);
    }
	
	rt_kprintf("auto_test all complete! test time = %d\n",test_cnt);
	
	esKRNL_SemPend(key_lock, 0, &err);
	task_id = 0;
	is_running = 0;
	all_test = 0;
    esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err);
	key_lock = NULL;
    esKRNL_TDel(EXEC_prioself);

}

static int auto_test(int argc, const char **argv)
{
    static int32_t id = 0,cnt = 0;
    char*err;
    __u8 err1;
    long args[2];
    
    if(argc == 1 || argc == 2)
    {
        rt_kprintf("auto_test [id] [cnt]\r\n");
        rt_kprintf("\t   id: 0 : movie\n");
        rt_kprintf("\t   id: 1 : music\n");
        rt_kprintf("\t   id: 2 : power\n");
        rt_kprintf("\t   id: 3 : fm\n");
        rt_kprintf("\t   id: 4 : photo\n");
        rt_kprintf("\t   id: 5 : explorer\n");
        rt_kprintf("\t   id: 6 : ebook\n");
        rt_kprintf("\t   id: 7 : setting\n");
        rt_kprintf("\t   id: 8 : all test\n");
        rt_kprintf("\t   id: -1��stop test(cnt = -1 too)\n");
        rt_kprintf("\t   cnt: n time\n");
        return 0;
    }
    else if(argc == 3)
    {
        id  = strtol(argv[1], &err, 0);
        cnt = strtol(argv[2], &err, 0);
        rt_kprintf("id = %d cnt = %d\n",id,cnt);
        if(is_running == 0)
        {
            if(id >= 0 && id <=8 && cnt >0)
            {
                key_lock = esKRNL_SemCreate(1);
                task_id = esKRNL_TCreate(pfun[id],&cnt,0x1000,KRNL_priolevel1);
                is_running = 1;
            }
            else
            {
                rt_kprintf("please check para!!\n");
            }
        }
        else
        {
            if((id == -1 ) && (cnt == -1))
            {
            	if(key_lock != NULL && task_id != 0)
            	{
                	esKRNL_SemPend(key_lock, 0, &err1);
                	esKRNL_TDel(task_id);
					task_id = 0;
					is_running = 0;
                	esKRNL_SemDel(key_lock, OS_DEL_ALWAYS, &err1);
					key_lock = NULL;
            	}
            }
            else
            {
                rt_kprintf("please check para!!\n");
            }
        }
    }
    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(auto_test, __cmd_auto_test, auto test);

#endif
