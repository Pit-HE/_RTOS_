
#include "InitPro.h"
#include <stdio.h>
#include "NUC123.h"



void SYS_Init(void)
{
    SYS->GPF_MFP |= SYS_GPF_MFP_PF0_XT1_OUT | SYS_GPF_MFP_PF1_XT1_IN;   /* Enable XT1_OUT (PF.0) and XT1_IN (PF.1) */
/* Init System Clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);                         /* Enable Internal RC 22.1184 MHz clock */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);                   /* Waiting for Internal RC clock ready */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));           /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);                         /* Enable external XTAL 12 MHz clock */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);                   /* Waiting for external XTAL clock ready */
    CLK_SetCoreClock(72000000);                                         /* Set core clock */
    CLK_EnableModuleClock(TMR0_MODULE);                                 /* Enable module clock */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);         /* Select module clock source */
    
    NVIC_EnableIRQ (PendSV_IRQn);
}



void Timer0_Init(void)
{
    /* Open Timer0 in periodic mode, enable interrupt and 1 interrupt tick per second */
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000); 
    TIMER_EnableInt(TIMER0);
    TIMER_Start(TIMER0);
    
    NVIC_EnableIRQ(TMR0_IRQn);
    NVIC_SetPriority (TMR0_IRQn, 0);
}



void IO_Init(void)
{
//将LED灯的IO口设置为普通IO口
    SYS->GPD_MFP &= ~(1 << 8);    //PD8       
    SYS->GPD_MFP &= ~(1 << 9);    //PD9
    SYS->GPD_MFP &= ~(1 << 10);   //PD10
    SYS->GPD_MFP &= ~(1 << 11);   //PD11
    SYS->ALT_MFP &= ~(1 << 15);   
    SYS->GPB_MFP &= ~(1 << 4);    //PB4
    SYS->ALT_MFP &= ~(1 << 18);
    SYS->GPB_MFP &= ~(1 << 5);    //PB5
    SYS->ALT_MFP &= ~(1 << 17);
    SYS->GPB_MFP &= ~(1 << 6);    //PB6
    SYS->ALT_MFP &= ~(1 << 16);
    SYS->GPB_MFP &= ~(1 << 7);    //PB7

//将LED灯的IO口设置为输出
    GPIO_SetMode (PD, BIT8,  GPIO_PMD_OUTPUT);
    GPIO_SetMode (PD, BIT9,  GPIO_PMD_OUTPUT);
    GPIO_SetMode (PD, BIT10, GPIO_PMD_OUTPUT);
    GPIO_SetMode (PD, BIT11, GPIO_PMD_OUTPUT);   
    GPIO_SetMode (PB, BIT4,  GPIO_PMD_OUTPUT);
    GPIO_SetMode (PB, BIT5,  GPIO_PMD_OUTPUT);
    GPIO_SetMode (PB, BIT6,  GPIO_PMD_OUTPUT);
    GPIO_SetMode (PB, BIT7,  GPIO_PMD_OUTPUT);

//OFF ALL LED 
    PD8 = PD9 = PD10 = PD11 = PB4 = PB5 = PB6 = PB7 = 0;       
}



//初始化板子上的四个按键引脚
void KeyIO_Pro (void)
{
    SYS->GPC_MFP &= ~(1<< 8);                   // PC8  设置为普通引脚
    SYS->GPC_MFP &= ~(1<< 9);                   // PC9  设置为普通引脚
    SYS->GPC_MFP &= ~(1<<10);                   // PC10 设置为普通引脚
    SYS->GPC_MFP &= ~(1<<11);                   // PC11 设置为普通引脚
    GPIO_SetMode (PC, BIT8,  GPIO_PMD_QUASI);   // PC8  设置成类51模式
    GPIO_SetMode (PC, BIT9,  GPIO_PMD_QUASI);   // PC9  设置成类51模式
    GPIO_SetMode (PC, BIT10, GPIO_PMD_QUASI);   // PC10 设置成类51模式
    GPIO_SetMode (PC, BIT11, GPIO_PMD_QUASI);   // PC11 设置成类51模式
}







