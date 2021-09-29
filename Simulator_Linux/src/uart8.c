//
// Created by xshx on 2021/9/29.
//

#include "uart8.h"

typedef struct
{
    int ID;
    char sender[20];
    char Data[ 100 ];
} Message, *pMessage;

//xQueueHandle MsgQueue;
extern  xQueueHandle  Uart5MsgQueue;
static const char *logtag ="[UART8]-";


static void Uart8SenderTask(void *pvParameters){
    printf("创建MainTASK \n");
    Message message;
    memset(&message, 0, sizeof(Message));
    message.ID=8;
    strcpy(message.sender, logtag);
    portBASE_TYPE  xStatus;

    for (;  ;) {
//        vTaskDelay( 2000/portTICK_RATE_MS );
        strcpy( message.Data, "from uart8 to uart5");
        xStatus = xQueueSend( Uart5MsgQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \r\n");

        }else{
            printf("%s send [ %s ID =%d  DATA=%s]\r\n",logtag, message.sender, message.ID, message.Data );
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
            printf("%s queue should have been empty ! \r\n", logtag);
        }

        xStatus = xQueueReceive(Uart8MsgQueue, &message, xTicksToWait);
        if (xStatus == pdPASS) {
            printf("%s receive[  %s  ID = %d DATA = %s]\r\n",logtag, message.sender, message.ID, message.Data );
        } else {
            printf("could not receive from the queue \r\n");
        }
    }
}

int uart8_task_start(void ){

    Uart8MsgQueue = xQueueCreate(5, sizeof(Message));

    printf("创建 uart8 task \n");
    if (xTaskCreate(Uart8SenderTask, "uart8 Sender", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");

    }
    if (xTaskCreate(Uart8ReceiverTask, "uart8 receiver", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");
    }

    return 0;

}