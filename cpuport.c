

#include "cpuport.h"


unsigned long   rt_thread_switch_interrupt_flag;
unsigned long   rt_interrupt_from_thread;
unsigned long   rt_interrupt_to_thread;


#define RT_ALIGN_DOWN(size, align)  ((size) & ~((align) - 1))



struct exception_stack_frame
{
    unsigned long r0;
    unsigned long r1;
    unsigned long r2;
    unsigned long r3;
    unsigned long r12;
    unsigned long lr;
    unsigned long pc;
    unsigned long psr;
};


struct stack_frame
{
    unsigned long r4;
    unsigned long r5;
    unsigned long r6;
    unsigned long r7;
    unsigned long r8;
    unsigned long r9;
    unsigned long r10;
    unsigned long r11;
 
    struct exception_stack_frame exception_stack_frame;
};



unsigned char rt_hw_stack_init (void *tentry,   void* parameter,    unsigned char *stack_addr,  void* texit)
{
    struct stack_frame* stack_frame;
    unsigned char       *stk;
    unsigned long       i;
    
    
//    /* �Դ����ջָ�������봦�� */
//    stk  = stack_addr + sizeof(unsigned long);
//    stk  = (unsigned char *) RT_ALIGN_DOWN ((unsigned long)stk, 8);
//    stk -= sizeof(struct stack_frame);

//    /* �õ������ĵ�ջ֡��ָ�� */
//    stack_frame = (struct stack_frame *)stk;
    stack_frame = (struct stack_frame *)stack_addr;
    
    
    /* �����мĴ�����Ĭ��ֵ����Ϊ 0xdeadbeef */
    for (i=0; i<(sizeof(struct stack_frame)) / sizeof(unsigned long); i++)
    {
        ((unsigned long *)stack_frame)[i] = 0xdeadbeef;
    }
    
    /* ���� ARM APCS ���ñ�׼�� ����һ������������ r0 �Ĵ��� */
    stack_frame->exception_stack_frame.r0 = (unsigned long)parameter;
    
    /* ��ʣ�µĲ����Ĵ���������Ϊ0 */
    stack_frame->exception_stack_frame.r1 = 0;
    stack_frame->exception_stack_frame.r2 = 0;
    stack_frame->exception_stack_frame.r3 = 0;
    
    /* �� IP��Intra-Procedure-call scratch register) ����Ϊ 0 */
    stack_frame->exception_stack_frame.r12 = 0;
    
    /* ���߳��˳������ĵ�ַ������ lr �Ĵ��� */
    stack_frame->exception_stack_frame.lr = (unsigned long)texit;
    
    /* ���߳���ں����ĵ�ַ������ pc �Ĵ��� */
    stack_frame->exception_stack_frame.pc = (unsigned long)tentry;
    
    /* ���� psr ��ֵΪ 0x01000000L, ��ʾĬ���л���ȥ�� Thumb ģʽ */
    stack_frame->exception_stack_frame.psr = 0x01000000L;


    return 0;
}





























