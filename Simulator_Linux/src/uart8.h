//
// Created by xshx on 2021/9/29.
//

#ifndef SIMULATOR_LINUX_UART8_H
#define SIMULATOR_LINUX_UART8_H

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef enum {
    MQTT_OFFLINE = 0,		/* (0) Succeeded */
    MQTT_ONLINE,			/* (1) A hard error occurred in the low level disk I/O layer */
    MQTT_DOWNLOAD,			/* (2) Assertion failed */
    MQTT_UPLOAD,			/* (3) The physical drive cannot work */
    MQTT_UPLOADING,			/* (4) Could not find the file */
    MQTT_FREE	            /* (5) Given parameter is invalid */
} MQTT_STATUS;

MQTT_STATUS   MqttStatus;
int uart8_task_start(void );

#endif //SIMULATOR_LINUX_UART8_H
