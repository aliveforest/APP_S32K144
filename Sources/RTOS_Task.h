/*
 * RTOS_Task.h
 *
 *  Created on: 2023年9月8日
 *      Author: dengtongbei
 */

#ifndef RTOS_TASK_H_
#define RTOS_TASK_H_
#include "FreeRTOS.h"
#include "task.h"

/* 1 字 = 4 字节 (1 word = 2 bytes) 32位
 * 1字节 =8 比特 (1 byte = 8bits)
 * 1 Byte = 8 Bits
 * 1 KB = 1024 Bytes
 * 1 MB = 1024 KB
 * 1 GB = 1024 MB */

/***************** FreeRTOS 配置 ****************/
/* 任务优先级越大越高 */
/* START_TASK 任务配置
 * 包含：任务句柄、任务优先级、堆栈大小、创建任务 */
TaskHandle_t start_task_handler;
#define START_TASK_PRIO 			(1u)
#define START_TASK_STACK_SIZE 		(128u) /* 设置的最小堆栈大小 128*4 bytes */
void start_task(void);

/* KEY_TASK 任务配置
 * 包含：任务句柄、任务优先级、堆栈大小、创建任务 */
TaskHandle_t key_task_handler;
#define KEY_TASK_PRIO               (8u)
#define KEY_TASK_STACK_SIZE         (128U) /* 设置的最小堆栈大小 128*4 bytes */
void key_task(void);

/* SHOW_TASK 任务配置
 * 包含：任务句柄、任务优先级、堆栈大小、创建任务 */
TaskHandle_t show_task_handler;
#define SHOW_TASK_PRIO            (2u)
#define SHOW_TASK_STACK_SIZE     (128u)
void show_task(void);

/* CPU_TASK 任务配置
 * 包含：任务句柄、任务优先级、堆栈大小、创建任务 */
TaskHandle_t cpu_task_handler;
#define CPU_TASK_PRIO           (4u)
#define CPU_TASK_STACK_SIZE     (512u)
void cpu_task(void);

/************************************************/

void init_fcn(void); /* 外设初始化函数 */
void rtos_start(void);

#endif /* RTOS_TASK_H_ */
