
#include "TaskPro.h"
#include "ledandkey_pro.h"
#include "cpuport.h"



uint8_t        TaskStack[TASK_COUNT][512];                          //任务堆栈
Task_Struct     TCB      [TASK_COUNT  ];                            //任务控制块
uint8_t         BitMap   [TASK_COUNT/8];                            //任务优先级位图
OS_REDY_LIST    OSRdyList[TASK_COUNT  ];                            //定义任务就绪表
Task_Struct     *OSPrioCur;                                         //永远指向当前任务的指针



/*将任务的优先级写入位图*/
void OS_PrioInsert (uint8_t prio)
{
    uint8_t ix, bit;
  
    ix             = prio / 8;
    bit            = 1u;
    bit          <<= 7 - (prio - (ix*8));
    BitMap[ix]    |= bit;                                      //将任务优先级写入位图
}

/*清任务在位图的优先级*/
void OS_PrioRemove (uint8_t prio)
{
    uint8_t ix, bit;

    ix             = prio / 8;
    bit            = 1u;
    bit          <<= 7 - (prio - (ix*8));
    BitMap[ix]    &= ~bit;                                     //清任务优先在位图的位
}

/*获取位图中的最高优先级*/
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

/*任务挂起*/
void OSTaskSuspend (uint8_t prio)
{
    OSRdyList[prio].TaskStructPtr ->TaskDlyCNT = 0;     //清任务延时计数
    OS_PrioRemove (prio);                               //删除任务优先级
}

/*任务恢复*/
void OSTaskResume (uint8_t prio)
{
    OSRdyList[prio].TaskStructPtr ->TaskDlyCNT = 0;     //清任务延迟计数
    OS_PrioInsert (prio);                               //注册任务优先级
}

/*任务延迟阻塞*/
void OSTaskDly (uint8_t time)
{
    OSPrioCur->TaskDlyCNT = time;        //设置任务延迟计数
    OS_PrioRemove (OSPrioCur->TaskPrio); //删除任务优先级
    OSTask_Schedule ();
}



/*系统的空闲任务*/
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

/*系统数据初始化*/
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
        OSRdyList[i].TaskStructPtr  = &TCB[i];              // 将任务控制块写入就绪列表
        BitMap[i/8]                 = (uint8_t)0;           // 初始化任务位图              
    }

    TaskCreateInit (TASK_COUNT - 1, OS_IdleProcess);
}



/*创建任务*/
void TaskCreateInit (unsigned short Prio, void (*TaskEntry)(void* parameter))
{
    TCB[Prio].TaskPrio = Prio;
    TCB[Prio].TaskRun  = TaskEntry;
    
    OSRdyList[Prio].flag = (uint8_t)0xFF;
    
    OS_PrioInsert       (Prio);               //记录任务的优先级到位图
    rt_hw_stack_init    (TCB[Prio].TaskRun, (void*)0, (void*)((char *)TCB[Prio].TaskSP), (void*)0);     //初始化任务堆栈
}


/* 任务睡眠时间处理 */
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
            OS_PrioInsert (i);                              //置起任务的优先级，使任务重新运行
        }
    }
}


/* 任务调度器 */
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



/* 任务调度 */
void Start_System_Schedule (void)
{    
    Task_Struct*    to_thread;
    
    to_thread = OSRdyList[OS_GetHighestPrio()].TaskStructPtr;
    OSPrioCur = to_thread;
    
    rt_hw_context_switch_to((unsigned long)&to_thread->TaskSP);         // 开始调度第一个任务
}




































