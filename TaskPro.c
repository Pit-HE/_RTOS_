
#include "TaskPro.h"
#include "ledandkey_pro.h"
#include "cpuport.h"



uint8_t        TaskStack[TASK_COUNT][512];                          //�����ջ
Task_Struct     TCB      [TASK_COUNT  ];                            //������ƿ�
uint8_t         BitMap   [TASK_COUNT/8];                            //�������ȼ�λͼ
OS_REDY_LIST    OSRdyList[TASK_COUNT  ];                            //�������������
Task_Struct     *OSPrioCur;                                         //��Զָ��ǰ�����ָ��



/*����������ȼ�д��λͼ*/
void OS_PrioInsert (uint8_t prio)
{
    uint8_t ix, bit;
  
    ix             = prio / 8;
    bit            = 1u;
    bit          <<= 7 - (prio - (ix*8));
    BitMap[ix]    |= bit;                                      //���������ȼ�д��λͼ
}

/*��������λͼ�����ȼ�*/
void OS_PrioRemove (uint8_t prio)
{
    uint8_t ix, bit;

    ix             = prio / 8;
    bit            = 1u;
    bit          <<= 7 - (prio - (ix*8));
    BitMap[ix]    &= ~bit;                                     //������������λͼ��λ
}

/*��ȡλͼ�е�������ȼ�*/
uint8_t OS_GetHighestPrio (void)
{
    uint8_t i, prio, ix;

    prio = i = 0;
    while (BitMap[i] == 0){
        i++;
        prio += 8;
    }
    ix = BitMap[i];
    while (!(ix & 0x80)){
        prio++;
        ix <<= 1;
    }
    return (prio);
}

/*�������*/
void OSTaskSuspend (uint8_t prio)
{
    OSRdyList[prio].TaskStructPtr ->TaskDlyCNT = 0;     //��������ʱ����
    OS_PrioRemove (prio);                               //ɾ���������ȼ�
}

/*����ָ�*/
void OSTaskResume (uint8_t prio)
{
    OSRdyList[prio].TaskStructPtr ->TaskDlyCNT = 0;     //�������ӳټ���
    OS_PrioInsert (prio);                               //ע���������ȼ�
}

/*�����ӳ�����*/
void OSTaskDly (uint8_t time)
{
    OSPrioCur->TaskDlyCNT = time;        //���������ӳټ���
    OS_PrioRemove (OSPrioCur->TaskPrio); //ɾ���������ȼ�
    OSTask_Schedule ();
}



/*ϵͳ�Ŀ�������*/
void OS_IdleProcess (void* parameter)
{
    while(1)
    {
        if (OS_GetHighestPrio() != (TASK_COUNT - 1))
        {
            OSTask_Schedule ();
        }
    }
}

/*ϵͳ���ݳ�ʼ��*/
void OS_TCB_init (void)
{
    uint8_t i;

    for (i=0; i<TASK_COUNT; i++)
    {
        TCB[i].TaskPrio             = (uint16_t)0;
        TCB[i].TaskDlyCNT           = (uint16_t)0;
        TCB[i].TaskRun              = (void   *)0;
        TCB[i].TaskSP               = &TaskStack[i][0];
        OSRdyList[i].flag           = (uint8_t)0;
        OSRdyList[i].TaskStructPtr  = &TCB[i];              // ��������ƿ�д������б�
        BitMap[i/8]                 = (uint8_t)0;           // ��ʼ������λͼ              
    }

    TaskCreateInit (TASK_COUNT - 1, OS_IdleProcess);
}



/*��������*/
void TaskCreateInit (unsigned short Prio, void (*TaskEntry)(void* parameter))
{
    TCB[Prio].TaskPrio = Prio;
    TCB[Prio].TaskRun  = TaskEntry;
    
    OSRdyList[Prio].flag = (uint8_t)0xFF;
    
    OS_PrioInsert       (Prio);               //��¼��������ȼ���λͼ
    rt_hw_stack_init    (TCB[Prio].TaskRun, (void*)0, (void*)((char *)TCB[Prio].TaskSP), (void*)0);     //��ʼ�������ջ
}


/* ����˯��ʱ�䴦�� */
void OSTask_TimeProcess (void)
{
    unsigned char i;

    for (i=1; i<TASK_COUNT; i++)
    {
        if (OSRdyList[i].flag == 0)
            continue;
        if (OSRdyList[i].TaskStructPtr ->TaskDlyCNT > 0)
        {
            OSRdyList[i].TaskStructPtr ->TaskDlyCNT --;
        }
        if (OSRdyList[i].TaskStructPtr ->TaskDlyCNT == 0)
        {
            OS_PrioInsert (i);                              //������������ȼ���ʹ������������
        }
    }
}


/* ��������� */
void OSTask_Schedule (void)
{
    long level;
    Task_Struct*    to_thread;
    Task_Struct*    from_thread;

    
    level = rt_hw_interrupt_disable();

    from_thread = OSPrioCur;
    to_thread   = OSRdyList[OS_GetHighestPrio()].TaskStructPtr;
    OSPrioCur   = to_thread;
    rt_hw_context_switch ((unsigned long)&from_thread->TaskSP, (unsigned long)&to_thread->TaskSP);

    rt_hw_interrupt_enable (level);
}



/* ������� */
void Start_System_Schedule (void)
{    
    Task_Struct*    to_thread;
    
    to_thread = OSRdyList[OS_GetHighestPrio()].TaskStructPtr;
    OSPrioCur = to_thread;
    
    rt_hw_context_switch_to((unsigned long)&to_thread->TaskSP);         // ��ʼ���ȵ�һ������
}




































