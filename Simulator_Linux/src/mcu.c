//
// Created by xshx on 2021/9/29.
//

#include "mcu.h"
#include "commen.h"

//MCU TASK 和 uart5 交互
static const char *logtag ="[MCU]-";


//int Init_CMD[] = {0x23, 0x01};
//int BLT_CMD[]={0x23, 0x05};


//int Recognize_Process[][]={Init_CMD, BLT_CMD};

static void McuMainTask(void *pvParamters){
    printf("%s创建  MCU Main TasK \n", logtag);

    int count=0;
    for (;  ;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("\r\n%s %d  Start A Process ...\r\n", logtag, count);

//        Recognize_Process
        char *data ="init";
        SendMessageToUart5( CMD_INITOK_SYNC, MCU_TASK, data);

        count++;
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}


static void  McuReceiverTask( void *pvParameters){
    printf("%s MCU 接收消息的TASK \n", logtag);
    Message message;
    portBASE_TYPE  xStatus;
    const portTickType xTicksToWait = 100/portTICK_RATE_MS;
    while(1){
        memset( &message, 0, sizeof(Message));
        if(  xQueuePeek(MessageQueue, &message, xTicksToWait) == pdPASS ){
            if( message.ReceiverID == MCU_TASK ){
                if(  xQueueReceive(MessageQueue, &message, xTicksToWait) == pdPASS ){
                    printf("%s receive[ SenderID %d,  ID = %d DATA = %s]\n",logtag, message.SenderID, message.MessageID, message.Data );

                } else {
                    printf("could not receive from the queue \n");
                }
            }

        }else{
            printf("%s could not peek from the queue %d \n",logtag, xStatus );
        }
        vTaskDelay(pdMS_TO_TICKS(1000));

    };

}

int mcu_task_start(void){

//    McuMsgQueue = xQueueCreate(5, sizeof(Message));
    printf("%s 创建 MCU  Task \n", logtag);
    if (xTaskCreate(McuMainTask, "MCU Main Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("MCU main Task error \n");

    }
    if (xTaskCreate(McuReceiverTask, "MCU receiver Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("MCU receiver task error \n");

    }
    return 0;
}
