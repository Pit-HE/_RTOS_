

#ifndef _TASKPRO_H_
#define _TASKPRO_H_



#include "NUC123.h"



#define   TASK_COUNT    32                // �����������


/*
-------------------------------
*   ������ƿ�ṹ��
---------------------------------
*/
typedef struct _Task_Struct	
{
  
  void*     TaskSP;
  uint16_t  TaskPrio;                     // �������ȼ�
  uint16_t  TaskDlyCNT;                   // ��������ʱ�������
  void (*TaskRun)(void* parameter);       // ����ĺ���ָ��
}Task_Struct,  *Task_Struct_Pointer; 


/*
-------------------------------
*   ���������ṹ��
---------------------------------
*/
typedef struct  os_rdy_list {
    unsigned char     flag;
    Task_Struct       *TaskStructPtr;            /* ������ƿ�ָ�� */
}OS_REDY_LIST, *OS_REDY_List;




extern Task_Struct      tasks [TASK_COUNT];      //������ƿ�����



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

