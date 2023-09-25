/*
 * GPIO_init.h
 *
 *  Created on: 2023年8月10日
 *      Author: dengtongbei
 */

#ifndef GPIO_INIT_H_
#define GPIO_INIT_H_
#include "S32K144.h"
#include <stdbool.h>

/* 例如                 A           8             1(输出) ：GPIO_init('A', 8, 1);  */
void GPIO_init(uint8_t _x, uint32_t _pin, bool in_out);
void GPIO_out(uint8_t _x, uint32_t _pin, bool out_bit);
void GPIO_toggle(uint8_t _x, uint32_t _pin);
#endif /* GPIO_INIT_H_ */
