//
// Created by xshx on 2021/9/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart5.h"
#include "uart8.h"
#include "mcu.h"

#include "commen.h"


int main(void ){
    printf("创建 3个 TASK \n");
    MessageQueue = xQueueCreate(5, sizeof(Message));

    uart5_task_start( );
    uart8_task_start( );
    mcu_task_start();

    vTaskStartScheduler();
    printf("START TASKS \r\n");
    return 0;

}


void vMainQueueSendPassed( void )
{
    return;
}

/* This is a stub function for FreeRTOS_Kernel */
void vApplicationIdleHook( void )
{
    return;
}