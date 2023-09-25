/*
 * GPIO_init.c
 *
 *  Created on: 2023年8月10日
 *      Author: dengtongbei
 */
#include "GPIO_init.h"

/*
 ************引脚初始化函数************
 *GPIO_init(uint8_t _x, uint32_t _pin, uint8_t in_out)
 *   例如    A           8             1(输出) ：GPIO_init('A', 8, 1); 
*/
void GPIO_init(uint8_t _x, uint32_t _pin, bool in_out) {
    GPIO_Type * PTx;
    PORT_Type * PORTx;
    uint16_t PCC_PORTx_INDEX;
    switch (_x) { 
    case 'A':PTx = PTA; PORTx = PORTA; PCC_PORTx_INDEX = PCC_PORTA_INDEX; break;
    case 'B':PTx = PTB; PORTx = PORTB; PCC_PORTx_INDEX = PCC_PORTB_INDEX; break;
    case 'C':PTx = PTC; PORTx = PORTC; PCC_PORTx_INDEX = PCC_PORTC_INDEX; break;
    case 'D':PTx = PTD; PORTx = PORTD; PCC_PORTx_INDEX = PCC_PORTD_INDEX; break;
    case 'E':PTx = PTE; PORTx = PORTE; PCC_PORTx_INDEX = PCC_PORTE_INDEX; break;
    }
    PCC-> PCCn[PCC_PORTx_INDEX] |= PCC_PCCn_CGC_MASK; // 开启时钟模块
    PORTx->PCR[_pin] = PORT_PCR_MUX(1) | PORT_PCR_DSE(1);   // 配置SDA引脚为GPIO，并设置驱动能力 1->强驱动, 0->普通驱动
    if(in_out) PTx->PDDR |= (1 << _pin);   /* 设置_pin为输出 */
    else PTx->PDDR &= ~(1 << _pin);        /* 或者输入模式 */

    // PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
	// PORTD->PCR[2] = PORT_PCR_MUX(1);
	// PTD->PDDR |= (1 << 2);
}
//设置引脚输出0/1
void GPIO_out(uint8_t _x, uint32_t _pin, bool out_bit){
    GPIO_Type * PTx;
    switch (_x) { 
    case 'A':PTx = PTA; break;
    case 'B':PTx = PTB; break;
    case 'C':PTx = PTC; break;
    case 'D':PTx = PTD; break;
    case 'E':PTx = PTE; break;
    }
    // PTx->PDOR |= (out_bit << _pin);   //设置引脚输出0/1
    if(out_bit) PTx->PSOR |= (1 << _pin);   
    else PTx->PCOR |= (1 << _pin);
}
//引脚输出切换 
void GPIO_toggle(uint8_t _x, uint32_t _pin){
    GPIO_Type * PTx;
    switch (_x) { 
    case 'A':PTx = PTA; break;
    case 'B':PTx = PTB; break;
    case 'C':PTx = PTC; break;
    case 'D':PTx = PTD; break;
    case 'E':PTx = PTE; break;
    }
    PTx->PTOR |= (1 << _pin);   //引脚输出切换    
}

