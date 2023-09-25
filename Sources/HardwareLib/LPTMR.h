/*
 * LPTMR.h
 *
 *  Created on: 2023年8月8日
 *      Author: dengtongbei
 */

#ifndef LPTMR_H_
#define LPTMR_H_
#include "S32K144.h"
#include "device_registers.h"

void LPTMR_init(void);
void Enable_Interrupt (uint32_t vector_number, uint32_t priority) ;
void LPTMR0_IRQHandler (void);
void TMR_delay_us(volatile uint32_t nus);

#endif /* LPTMR_H_ */
