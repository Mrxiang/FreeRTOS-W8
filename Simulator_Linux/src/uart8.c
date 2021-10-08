//
// Created by xshx on 2021/9/29.
//

#include "uart8.h"
#include "commen.h"



//extern  xQueueHandle  Uart5MsgQueue;
static const char *logtag ="[UART8]-";


static void Uart8InitTask( void *pvParameters){
    printf("%s 初始化 \n", logtag);

    MqttStatus = MQTT_ONLINE;
    vTaskDelete(NULL);

}

static void Uart8MainTask( void *pvParameters){

    printf("%s 创建 Main Task \n", logtag);
    while(MqttStatus != MQTT_OFFLINE ){
        vTaskDelay(pdMS_TO_TICKS(1000));

        if( MqttStatus == MQTT_ONLINE ){
            printf("%s MQTT 已连接 \n", logtag);
            MqttStatus = MQTT_ONLINE;
//            SendMessageToUart5()
        } else{
            printf("%s offline \n", logtag);
            char *data ="下电 \n";
            SendMessageToUart5FromUart8(CMD_CLOSE_FACEBOARD, data);
        }
    }
}


static void Uart8ReceiverTask(void *pvParamters) {
    printf("%s 创建接收TASK \n", logtag);

    Message message;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

    for (;;) {
        while (uxQueueMessagesWaiting(Uart8MsgQueue)) {

            if (xQueueReceive(Uart8MsgQueue, &message, xTicksToWait) == pdPASS) {
                printf("%s receive[SenderID  %d,  ID = %d, DATA = %s]\n", logtag, message.SenderID, message.MessageID,
                       message.Data);
            } else {
                printf("%s receive error %d\n", logtag, xStatus);
            }
        }
//        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int uart8_task_start(void ){

//    Uart8MsgQueue = xQueueCreate(5, sizeof(Message));


    printf("%s 创建 uart8 task \n", logtag);
    if (xTaskCreate(Uart8InitTask, "uart8 init", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 init error \n");

    }

    if (xTaskCreate(Uart8MainTask, "uart8 Sender", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");

    }
    if (xTaskCreate(Uart8ReceiverTask, "uart8 receiver", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");
    }

    return 0;

}