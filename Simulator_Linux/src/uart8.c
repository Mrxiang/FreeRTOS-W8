//
// Created by xshx on 2021/9/29.
//

#include "uart8.h"

typedef struct
{
    int ID;
    portCHAR Data[ 20 ];
} Message, *pMessage;

//xQueueHandle MsgQueue;
xQueueHandle  Uart8MsgQueue;
static const char *logtag ="[UART8]-";

void TaskA( void *pvParameters );
void TaskB( void *pvParameters );
static void Uart8SenderTask(void *pvParameters){
    printf("创建发送TASK \n");
    Message message;
    message.ID=1;
    strcpy(message.Data, logtag);
    portBASE_TYPE  xStatus;

    for (;  ;) {
//        vTaskDelay( 2000/portTICK_RATE_MS );

        xStatus = xQueueSend( Uart8MsgQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \r\n");

        }else{
            printf("%s send ID =%d \r\n", message.Data, message.ID  );
        }
        taskYIELD();
        message.ID++;
    }

}
static void Uart8ReceiverTask(void *pvParamters){
    printf("创建接收TASK \n");

    Message message;
    portBASE_TYPE  xStatus;
    const portTickType xTicksToWait = 100/portTICK_RATE_MS;

    for (;  ;) {
        if (uxQueueMessagesWaiting(Uart8MsgQueue) != 0) {
            printf("queue should have been empty ! \r\n");
        }

        xStatus = xQueueReceive(Uart8MsgQueue, &message, xTicksToWait);
        if (xStatus == pdPASS) {
            printf("%s received ID = %d \r\n",message.Data, message.ID );
        } else {
            printf("could not receive from the queue \r\n");
        }
    }
}

int uart8_task_start(void ){

    Uart8MsgQueue = xQueueCreate(5, sizeof(Message));

    printf("创建 uart8 task \n");
    if (xTaskCreate(Uart8SenderTask, "uart8 sender", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");

    }
    if (xTaskCreate(Uart8ReceiverTask, "uart8 receiver", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");
    }

    return 0;

}
