//
// Created by xshx on 2021/9/29.
//

#ifndef FREERTOS_W8_POWER_H
#define FREERTOS_W8_POWER_H
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef enum {
    MCU_SLEEP = 0,		/* (0) 睡眠模式 */
    MCU_FREE,			/* (1) 空闲模式 */
    MCU_WORK            /*(2) 工作模式*/
} MCU_STATUS;


int mcu_task_start();
#endif //FREERTOS_W8_POWER_H
