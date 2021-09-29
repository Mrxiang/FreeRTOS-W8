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
#include "power.h"



int main(void ){
    printf("创建两个TASK \n");

    uart5_task_start( );
    uart8_task_start( );
    power_task_start();

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