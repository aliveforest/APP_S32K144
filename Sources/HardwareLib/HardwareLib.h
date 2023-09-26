/*
 * HardwareLib.h
 *
 *  Created on: 2023年8月18日
 *      Author: dengtongbei
 */

#ifndef HARDWARELIB_H_
#define HARDWARELIB_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "S32K144.h"
#include "device_registers.h"
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
/* User includes. */
#include "RGB_LED.h"
#include "latency.h"
#include "LPUART.h"
#include "GPIO_init.h"
#include "LPTMR.h"
#include "LP_IT_timer.h"
#include "FlashWriteRead.h"
#include "SPI_OLED.h"
#include "PowerSwitch.h"
#include "Ymodem.h"

#endif /* HARDWARELIB_H_ */
