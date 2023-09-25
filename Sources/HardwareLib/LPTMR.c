/*
 * LPTMR.c
 *
 *  Created on: 2023年8月8日
 *      Author: dengtongbei
 */

#include "LPTMR.h"
#include "LPUART.h"


/* LPTMR 初始化*/
void LPTMR_init(void)
{
	PTD->PDDR |= 1<<1;
	PORTD->PCR[1] = PORT_PCR_MUX(1);
	PCC->PCCn[PCC_LPTMR0_INDEX] = PCC_PCCn_CGC_MASK; 	/* 时钟	*/
//    PCC->PCCn[PCC_LPTMR0_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* 时钟	*/
//    PCC->PCCn[PCC_LPTMR0_INDEX] |= PCC_PCCn_PCS(0b01)	/* Clock Src=1, 8MHz SOSCDIV2_CLK */
//                                |  PCC_PCCn_PCD(0)	/* 外设时钟分频器选择 0~7: 1~8*/
//                                |  PCC_PCCn_CGC_MASK;   /* 时钟使能  	 	*/
    LPTMR0->CSR = 0x00000000U;                          /* 禁用定时器 */
    LPTMR0->PSR |= LPTMR_PSR_PCS(1)      				/* LPTMR clk src: LP01KCLC 1KHz	 */
    			| LPTMR_PSR_PBYP(0); 					/* Bypass Prescaler 1:预分频器/故障滤波器被旁路 */
//				| LPTMR_PSR_PRESCALE(6); 				/* 6: 128分频 */
    LPTMR0->CMR = 10000-1;       /* 1000-1: 1 Hz*/     /* CMR 为16位计数器   		*/
    LPTMR0->CSR |= LPTMR_CSR_TIE_MASK; 				/* Timer interrupt enabled 启用定时器中断 */

//    LPTMR0->CSR |= LPTMR_CSR_TFC(1);                  /* 启动 定时器自由运行计数器 */
   LPTMR0->CSR |= LPTMR_CSR_TEN(1);                    /* Enable Timer 启动定时器			*/


///* 1KHz*/
//    PCC->PCCn[PCC_LPTMR0_INDEX] = PCC_PCCn_CGC_MASK;	/* Enable LPTMR Clock 		*/
//    LPTMR0->CSR = 0x00000000U;                          /* 禁用定时器 */
//    LPTMR0->PSR |= LPTMR_PSR_PCS(0b01)                  /* LPTMR clk src: 1KHz LPO */
//                  |LPTMR_PSR_PBYP_MASK;               	/* Bypass Prescaler 		*/
//    LPTMR0->CMR = 500-1;                                  /* 500 ms interrupt 		*/
//    LPTMR0->CSR |= LPTMR_CSR_TIE_MASK; 					/* Timer interrupt enabled */
//    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;                  /* Enable Timer 			*/


     Enable_Interrupt(LPTMR0_IRQn, 0x06); 						/* 中断使能 */
}

/* 中断配置 */
void Enable_Interrupt (uint32_t vector_number, uint32_t priority) {
	uint8_t shift = (uint8_t) (8U - FEATURE_NVIC_PRIO_BITS);
	/* 清除任何挂起的 IRQ */
	S32_NVIC->ICPR[(uint32_t)(vector_number) >> 5U] = (uint32_t)(1U << ((uint32_t)(vector_number) & (uint32_t)0x1FU));
	/* 使能 IRQ */
	S32_NVIC->ISER[(uint32_t)(vector_number) >> 5U] = (uint32_t)(1U << ((uint32_t)(vector_number) & (uint32_t)0x1FU));
	/* 优先级设置 */
	S32_NVIC->IP[(uint32_t)vector_number] = (uint8_t)(((((uint32_t)priority) << shift)) & 0xFFUL);
}

void TMR_delay_us(volatile uint32_t nus) {
    LPTMR0->CNR = 1u; // 读CNR之前需要先写入一次任意值
	uint16_t told = LPTMR0->CNR ;      	//读取刚进入时的计数器值
	uint16_t tcnt=0;
	while(tcnt < nus) {  
        LPTMR0->CNR = 1u; // 读CNR之前需要先写入一次任意值
		tcnt = LPTMR0->CNR-told;
	}
}


void LPTMR0_IRQHandler(void) {   
	if(0!=(LPTMR0->CSR & LPTMR_CSR_TCF_MASK))
	{
		/* Check if TCF flag is set */
		LPTMR0->CSR |= LPTMR_CSR_TCF_MASK; /*	通过写入逻辑 1 清除 TCF 标志 */
//		LPUART1_printf("LPTMR0 Interrupt\r\n");
//		PTD->PTOR |= 1 << 1| 1<<15;				 /* 切换PTD1 */

		/* SPI_OLED重新上电初始化 */
		PTA->PSOR |= (1<<17); // 输出高
		PTA->PCOR |= (1<<11); // 输出低
		PTD->PSOR |= ((1 << 3)|(1 << 5)|(1 << 12)|(1 << 11)|(1 << 10)); // 输出高电平
	}

}
