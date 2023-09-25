/*

 * RGB_LED.c
 *
 *  Created on: 2023年8月1日
 *      Author: dengtongbei
 */
#include "RGB_LED.h"
#include "S32K144.h"
#include "HardwareLib.h"


// RGB_LED初始化
void RGB_LED_KEY_init(void){
	PCC-> PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
	PCC-> PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;/* Enable clock for PORTC */

	/* Configure port C12 as GPIO input (BTN 0 [SW2] on EVB) */
	PTC->PDDR &= ~(1<<SW2);   /* Port C12: Data Direction= input (default) */
	PORTC->PCR[SW2] = PORT_PCR_MUX(1) |PORT_PCR_PFE_MASK  /* Port C12: MUX = GPIO, input filter enabled */
									  | PORT_PCR_IRQC(0x0B); /* 上升沿中断0x09 下降沿中断0x0A */
	
	/* Configure port C13 as GPIO input (BTN 1 [SW3] on EVB) */
	PTC->PDDR &= ~(1<<SW3);   /* Port C12: Data Direction= input (default) */
	PORTC->PCR[SW3] = PORT_PCR_MUX(1) |PORT_PCR_PFE_MASK  /* Port C12: MUX = GPIO, input filter enabled */
									  | PORT_PCR_IRQC(0x0B); /* 上升沿中断0x09 下降沿中断0x0A */
	KEY_NVIC_EnableIRQ(PORTC_IRQn, 0x07); /* 使能中断，并设置优先级 */

	/* Configure port D0 as GPIO output (LED on EVB) */
	PTD->PDDR |= 1<<BlueLED;       /* Port D0: Data Direction= output */
	PORTD->PCR[BlueLED] = PORT_PCR_MUX(1); /* Port D0: MUX = GPIO */
	PTD->PDDR |= 1<<RedLED;       
	PORTD->PCR[RedLED] = PORT_PCR_MUX(1);
	PTD->PDDR |= 1<<GreenLED;       
	PORTD->PCR[GreenLED] = PORT_PCR_MUX(1);

	// 关闭RGB led
	PTD-> PSOR |= 1<<BlueLED;
	PTD-> PSOR |= 1<<GreenLED;
	PTD-> PSOR |= 1<<RedLED;
}
/* 中断配置 */
void KEY_NVIC_EnableIRQ (uint32_t vector_number, uint32_t priority) {
	uint8_t shift = (uint8_t) (8U - FEATURE_NVIC_PRIO_BITS);
	/* 清除任何挂起的 IRQ */
	S32_NVIC->ICPR[(uint32_t)(vector_number) >> 5U] = (uint32_t)(1U << ((uint32_t)(vector_number) & (uint32_t)0x1FU));
	/* 使能 IRQ */
	S32_NVIC->ISER[(uint32_t)(vector_number) >> 5U] = (uint32_t)(1U << ((uint32_t)(vector_number) & (uint32_t)0x1FU));
	/* 优先级设置 */
	S32_NVIC->IP[(uint32_t)vector_number] = (uint8_t)(((((uint32_t)priority) << shift)) & 0xFFUL);
}
/* LED控制开/关 */
void LED_Ctrl(uint32_t LED_pin, bool out_bit){
	if(out_bit) PTD->PCOR |= (1 << LED_pin);   
    else PTD->PSOR |= (1 << LED_pin); 
}

/* LED闪烁 */
void LED_Toggle(uint32_t LED_pin){
	PTD-> PTOR |= 1<<LED_pin; 
}


// SW2 按键扫描
bool SW2_key(void){
	return (PTC->PDIR & (1<<SW2));
}
// SW3 按键扫描
bool SW3_key(void){
	return (PTC->PDIR & (1<<SW3));
}

void PORTC_IRQHandler(void)
{
	// PORTC->ISFR = PORT_ISFR_ISF_MASK; //清除PORTC外部中断
	PORTC->PCR[SW2] |= PORT_PCR_ISF_MASK; //清除外部中断
	PORTC->PCR[SW3] |= PORT_PCR_ISF_MASK; //清除外部中断
	
	/* SPI_OLED重新上电初始化 */
	PTA->PSOR |= (1<<17); // 输出高
	PTA->PCOR |= (1<<11); // 输出低
	PTD->PSOR |= ((1 << 3)|(1 << 5)|(1 << 12)|(1 << 11)|(1 << 10)); // 输出高电平

}
