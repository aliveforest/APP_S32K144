/*
 * RGB_LED.h
 *
 *  Created on: 2023年8月1日
 *      Author: dengtongbei
 */

#ifndef RGB_LED_H_
#define RGB_LED_H_
#include "S32K144.h"
#include "device_registers.h"
#include "stdbool.h"

#define RedLED   15u
#define GreenLED 16u
#define BlueLED  0u  // Port PTD0, bit 0: FRDM EVB output to blue LED
#define SW2 12u 	 // Port PTC12, bit 12: FRDM EVB input from BTN0 [SW2]
#define SW3 13u 	 // Port PTC12, bit 12: FRDM EVB input from BTN1 [SW3]

void RGB_LED_KEY_init(void);
void LED_Ctrl(uint32_t LED_pin, bool out_bit);
void LED_Toggle(uint32_t LED_pin);
void KEY_NVIC_EnableIRQ (uint32_t vector_number, uint32_t priority);
bool SW2_key(void);
bool SW3_key(void);

#endif /* RGB_LED_H_ */
