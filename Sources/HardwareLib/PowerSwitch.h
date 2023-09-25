/*
 * PowerSwitch.h
 *
 *  Created on: 2023年9月13日
 *      Author: dengtongbei
 */

#ifndef POWERSWITCH_H_
#define POWERSWITCH_H_

#include "Cpu.h"
#include "HardwareLib.h"

#define MENU_MESSAGE "Enter:\r\n\
\t0 > for HSRUN\r\n\
\t1 > for RUN\r\n\
\t2 > for VLPR\r\n\
\t3 > for STOP1\r\n\
\t4 > for STOP2\r\n\
\t5 > for VLPS\r\n"

#define ADD_MESSAGE "-->Press SW3 to wake up the CPU from STOP1,STOP2 or VLPS mode\r\n\
        Enter your input:\r\n"
#define SEPARATOR "----------------------------------------------------------------------------\r\n"

#define HSRUN (0u) /* High speed run      */
#define RUN   (1u) /* Run                 */
#define VLPR  (2u) /* Very low power run  */
#define STOP1 (3u) /* Stop option 1       */
#define STOP2 (4u) /* Stop option 2       */
#define VLPS  (5u) /* Very low power stop */



void Power_Man_Init(void);


#endif /* POWERSWITCH_H_ */
