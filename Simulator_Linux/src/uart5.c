//
// Created by xshx on 2021/9/29.
//

#include "uart5.h"

typedef struct
{
    int ID;
    portCHAR Data[ 20 ];
} Message, *pMessage;

//xQueueHandle MsgQueue;
xQueueHandle Uart5MsgQueue;

void TaskA( void *pvParameters );
void TaskB( void *pvParameters );
static const char *logtag ="[UART5]-";
static void Uart5SenderTask(void *pvParameters){
    printf("创建发送TASK \n");
    Message message;
    message.ID=1;
    strcpy(message.Data, logtag);
    portBASE_TYPE  xStatus;

    for (;  ;) {
//        vTaskDelay( 2000/portTICK_RATE_MS );

        xStatus = xQueueSend( Uart5MsgQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \r\n");

        }else{
            printf("%s ,send ID =%d\r\n",message.Data, message.ID  );
        }
        taskYIELD();
        message.ID++;
    }

}
static void Uart5ReceiverTask(void *pvParamters){
    printf("创建接收TASK \n");

    Message message;
    portBASE_TYPE  xStatus;
    const portTickType xTicksToWait = 100/portTICK_RATE_MS;

    for (;  ;) {
        if (uxQueueMessagesWaiting(Uart5MsgQueue) != 0) {
            printf("queue should have been empty ! \r\n");
        }

        xStatus = xQueueReceive(Uart5MsgQueue, &message, xTicksToWait);
        if (xStatus == pdPASS) {
            printf("%s received ID = %d  \r\n", message.Data, message.ID );
        } else {
            printf("could not receive from the queue \r\n");
        }
    }
}

int uart5_task_start(void ){

        Uart5MsgQueue = xQueueCreate( 5 , sizeof( Message ) );

        printf("创建uart5 task \n");
        if( xTaskCreate( Uart5SenderTask, "uart5 sender", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1 ,NULL) != pdPASS){
            printf("create uart5  task error \r\n");

        }
        if( xTaskCreate( Uart5ReceiverTask, "uart5 receiver", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1 ,NULL) !=pdPASS){
            printf("create uart5  task error \r\n");
        }

        return  0;
}
