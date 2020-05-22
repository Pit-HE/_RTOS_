

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
    
    
//    /* 对传入的栈指针做对齐处理 */
//    stk  = stack_addr + sizeof(unsigned long);
//    stk  = (unsigned char *) RT_ALIGN_DOWN ((unsigned long)stk, 8);
//    stk -= sizeof(struct stack_frame);

//    /* 得到上下文的栈帧的指针 */
//    stack_frame = (struct stack_frame *)stk;
    stack_frame = (struct stack_frame *)stack_addr;
    
    
    /* 把所有寄存器的默认值设置为 0xdeadbeef */
    for (i=0; i<(sizeof(struct stack_frame)) / sizeof(unsigned long); i++)
    {
        ((unsigned long *)stack_frame)[i] = 0xdeadbeef;
    }
    
    /* 根据 ARM APCS 调用标准， 将第一个参数保存在 r0 寄存器 */
    stack_frame->exception_stack_frame.r0 = (unsigned long)parameter;
    
    /* 将剩下的参数寄存器都设置为0 */
    stack_frame->exception_stack_frame.r1 = 0;
    stack_frame->exception_stack_frame.r2 = 0;
    stack_frame->exception_stack_frame.r3 = 0;
    
    /* 将 IP（Intra-Procedure-call scratch register) 设置为 0 */
    stack_frame->exception_stack_frame.r12 = 0;
    
    /* 将线程退出函数的地址保存在 lr 寄存器 */
    stack_frame->exception_stack_frame.lr = (unsigned long)texit;
    
    /* 将线程入口函数的地址保存在 pc 寄存器 */
    stack_frame->exception_stack_frame.pc = (unsigned long)tentry;
    
    /* 设置 psr 的值为 0x01000000L, 表示默认切换过去是 Thumb 模式 */
    stack_frame->exception_stack_frame.psr = 0x01000000L;


    return 0;
}





























