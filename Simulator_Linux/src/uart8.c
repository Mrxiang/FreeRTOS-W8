//
// Created by xshx on 2021/9/29.
//

#include "uart8.h"
#include "middle/commen_middle.h"



//extern  xQueueHandle  Uart5MsgQueue;
static const char *logtag ="[UART8]-";

MQTT_STATUS   MqttStatus=MQTT_OFFLINE;

static void Uart8InitTask( void *pvParameters){
    printf("%s 初始化 \n", logtag);

    MqttStatus = MQTT_ONLINE;
    vTaskDelete(NULL);

}

static void Uart8MainTask( void *pvParameters){

    printf("%s 创建 Main Task \n", logtag);
    while( 1 ){

        switch (MqttStatus){
            case MQTT_ONLINE:
                printf("%s MQTT 已连接, 转入下载模式 \n", logtag);
                MqttStatus = MQTT_DOWNLOAD;
                break;
            case MQTT_DOWNLOAD:
                printf("%s MQTT 下载完成, 转入上传模式 \n", logtag);
                MqttStatus = MQTT_UPLOAD;
                break;
            case MQTT_UPLOAD:
                printf("%s MQTT 上传完成, 转入上传实时模式 \n", logtag);
                MqttStatus = MQTT_UPLOADING;
                break;
            case MQTT_UPLOADING:
                printf("%s MQTT 上传完成, 转入空闲模式 \n", logtag);
                MqttStatus = MQTT_FREE;
                break;
            case MQTT_FREE:
                printf("%s MQTT 无任务,请求下电 \n", logtag);
//                char *data ="23140100EZ4D";
//                SendMessageToUart5FromUart8( data);
                MqttStatus = MQTT_OFFLINE;
                break;
            case MQTT_OFFLINE:
                printf("MQTT 离线 \n");
                vTaskDelay(pdMS_TO_TICKS(1000));
                break;
            default:
                break;
        }

    }

}


static void Uart8ReceiveUart5Task(void *pvParamters) {
    printf("%s 创建接收TASK \n", logtag);

    Message message;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

    for (;;) {
        while (uxQueueMessagesWaiting(Uart8FromUart5MsgQueue)) {

            if (xQueueReceive(Uart8FromUart5MsgQueue, &message, xTicksToWait) == pdPASS) {
                printf("%s receive[SenderID  %d,  ID = %d, DATA = %s]\n", logtag, message.SenderID, message.MessageID,message.Data);

            } else {
                printf("%s receive error %d\n", logtag, xStatus);
            }
        }
//        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void Uart8ReceiveMqttTask(void *pvParamters) {
    printf("%s 创建接收TASK \n", logtag);

    Message message;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

    for (;;) {
        while (uxQueueMessagesWaiting(Uart8FromUart5MsgQueue)) {

            if (xQueueReceive(Uart8FromUart5MsgQueue, &message, xTicksToWait) == pdPASS) {
                printf("%s receive[SenderID  %d,  ID = %d, DATA = %s]\n", logtag, message.SenderID, message.MessageID,message.Data);

            } else {
                printf("%s receive error %d\n", logtag, xStatus);
            }
        }
//        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int uart8_task_start(void ){

    Uart8FromUart5MsgQueue = xQueueCreate(5, sizeof(Message));
    Uart8FromMqttMsgQueue = xQueueCreate(5, sizeof(Message));

    printf("%s 创建 uart8 task \n", logtag);
    if (xTaskCreate(Uart8InitTask, "uart8 init", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 init error \n");

    }

    if (xTaskCreate(Uart8MainTask, "uart8 main", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");

    }
    if (xTaskCreate(Uart8ReceiveUart5Task, "uart8 receive uart5", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");
    }

    if (xTaskCreate(Uart8ReceiveMqttTask, "uart8 receive mqtt server", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task \n");
    }
    return 0;

}