//
// Created by xshx on 2021/9/29.
//

#ifndef SIMULATOR_LINUX_UART5_H
#define SIMULATOR_LINUX_UART5_H

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef enum {
    UART5_FREE,
    UART5_WORK,			/* (1) A hard error occurred in the low level disk I/O layer */
} UART5_STATUS;

int uart5_task_start(void );

#endif //SIMULATOR_LINUX_UART5_H
