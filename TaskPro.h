

#ifndef _TASKPRO_H_
#define _TASKPRO_H_



#include "NUC123.h"



#define   TASK_COUNT    32                // 最大任务数量


/*
-------------------------------
*   任务控制块结构体
---------------------------------
*/
typedef struct _Task_Struct	
{
  
  void*     TaskSP;
  uint16_t  TaskPrio;                     // 任务优先级
  uint16_t  TaskDlyCNT;                   // 任务休眠时间计数器
  void (*TaskRun)(void* parameter);       // 任务的函数指针
}Task_Struct,  *Task_Struct_Pointer; 


/*
-------------------------------
*   任务就绪表结构体
---------------------------------
*/
typedef struct  os_rdy_list {
    unsigned char     flag;
    Task_Struct       *TaskStructPtr;            /* 任务控制块指针 */
}OS_REDY_LIST, *OS_REDY_List;




extern Task_Struct      tasks [TASK_COUNT];      //任务控制块数组



void TaskCreateInit (unsigned short Priority, void (*TaskEntry)(void* parameter));
void OS_TCB_init (void);
void        OS_PrioRemove          (uint8_t prio);
void        OS_PrioInsert          (uint8_t prio);
void        OS_IdleProcess         (void* parameter);
uint8_t     OS_GetHighestPrio      (void);
void        OSTaskSuspend          (uint8_t prio);
void        OSTaskResume           (uint8_t prio);
void        OSTaskDly              (uint8_t time);
void OSTask_Schedule (void);

void        OSTask_TimeProcess     (void);
void        Start_System_Schedule       (void);


#endif  

