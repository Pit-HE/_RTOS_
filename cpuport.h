

#ifndef _CPUPORT_H_
#define _CPUPORT_H_



extern long rt_hw_interrupt_disable         (void);
extern void rt_hw_interrupt_enable          (long level);
extern void rt_hw_context_switch_to         (unsigned long to);
extern void rt_hw_context_switch            (unsigned long from, unsigned long to);
extern void rt_hw_context_switch_interrupt  (unsigned long from, unsigned long to);
extern void rt_hw_interrupt_thread_switch   (void);





unsigned char rt_hw_stack_init (void *tentry,   void* parameter,    unsigned char *stack_addr,  void* texit);





#endif

