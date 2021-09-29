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

//#define  TEST

#ifdef TEST
typedef struct
{
    int ID;
    portCHAR Data[ 20 ];
} Message, *pMessage;

xQueueHandle MsgQueue;
void TaskA( void *pvParameters );
void TaskB( void *pvParameters );
static void vSenderTask(void *pvParameters){
    printf("创建发送TASK \n");
    Message message;
    message.ID=1;
    strcpy(message.Data, "hello");
    portBASE_TYPE  xStatus;

    for (;  ;) {

        xStatus = xQueueSend( MsgQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \r\n");

        }else{
            printf("send ID =%d , DATA =%s\r\n", message.ID, message.Data );
        }
        taskYIELD();
        message.ID++;
    }

}
static void vReceiverTask(void *pvParamters){
    printf("创建接收TASK \n");

    Message message;
    portBASE_TYPE  xStatus;
    const portTickType xTicksToWait = 100/portTICK_RATE_MS;

    for (;  ;) {
        if (uxQueueMessagesWaiting(MsgQueue) != 0) {
            printf("queue should have been empty ! \r\n");
        }

        xStatus = xQueueReceive(MsgQueue, &message, xTicksToWait);
        if (xStatus == pdPASS) {
            printf("received ID = %d, DATA= %s \r\n", message.ID, message.Data);
        } else {
            printf("could not receive from the queue \r\n");
        }
    }
}

#else

void Uart5_Task(void *pvParameters) {

    uart5_task_start();
}


void Uart8_Task(void *pvParameters) {
    uart8_task_start();
}
#endif

int main(void ){
    printf("创建两个TASK \n");

#ifdef TEST
    MsgQueue = xQueueCreate( 5 , sizeof( Message ) );
    if (xTaskCreate(vSenderTask, "UART5", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("vSenderTask  ERROR\r\n");
    }
    if (xTaskCreate(vReceiverTask, "UART8", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("vSenderTask  ERROR\r\n");
    }

#else
     uart5_task_start( );
     uart8_task_start( );

#endif
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