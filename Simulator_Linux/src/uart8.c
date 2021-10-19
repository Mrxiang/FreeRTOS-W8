//
// Created by xshx on 2021/9/29.
//

#include "uart8.h"
#include "middle/uart8_middle.h"
#include "middle/commen_middle.h"

#define MAX_MSG_LINES           20
#define MAX_MSG_LEN_OF_LINE     256

//extern  xQueueHandle  Uart5MsgQueue;
static const char *logtag ="[UART8]-";

#define	MQTT_CWJAP	"+CWJAP:"
#define	MQTT_CWJAP_SIZE	(sizeof(MQTT_CWJAP) - 1)

MQTT_STATUS   MqttStatus=MQTT_OFFLINE;


uint8_t recv_msg_lines[MAX_MSG_LINES][MAX_MSG_LEN_OF_LINE];

static int handle_line(char *data,int length){

    printf("len %d ,data %s \n", length, data);

    if( strncmp(data, MQTT_CWJAP, MQTT_CWJAP_SIZE) == 0 ){

    }else if( strncmp( data, "+MQTTSUBRECV:", 13 ) == 0 ){
        int ret = analyzeMQTTMsgInternal( data );
    }else{
        printf("other data %s \n", data);
    }
    return  0;
}
int analyzeMQTTMsgInternal(char *msg) {
    printf("%s %s \n",logtag, msg);
}
static int ProcessMessage(
        unsigned char nCommand,
        unsigned char nMessageLen,
        const unsigned char *pszMessage) {
    printf("%s Command 0x%x,MessageLen %d Message %s \n",logtag, nCommand, nMessageLen,pszMessage);
    switch (nCommand) {
//        case :
//            uploadRealTimeRecord();
        default:
            break;
    }

    return  0;
}


static void Uart8InitTask( void *pvParameters){
    printf("%s 初始化 \n", logtag);
    for (int i = 0; i < MAX_MSG_LINES; i++) {
        memset(recv_msg_lines[i], '\0', MAX_MSG_LEN_OF_LINE);
    }
// 若mqtt 连接成功, 将Mqtt 状态设置为已连接
    if( InitMqttConnect() == 0 ){
        MqttStatus = MQTT_CONNECTED;
    }
    vTaskDelete(NULL);

}

static void Uart8ConnectedTask( void *pvParameters){
    for (int i = 0; i < MAX_MSG_LINES; i++) {
        printf("%s MQTT 已连接 \n", logtag);
    }

    MqttStatus = MQTT_DOWNLOAD;
    vTaskDelete(NULL);

}

static void Uart8MainTask( void *pvParameters){

    printf("%s 创建 Main Task \n", logtag);
    while( 1 ){

        switch (MqttStatus){

            case MQTT_CONNECTED:
                printf("%s MQTT 已连接, 转入下载模式 \n", logtag);
                if (xTaskCreate(Uart8ConnectedTask, "uart8  connected", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
                    printf("创建uart8 connected error \n");
                }
                break;
            case MQTT_DOWNLOAD:
                uploadRealTimeRecord();
                printf("%s MQTT 下载完成, 转入上传实时模式 \n", logtag);
                MqttStatus = MQTT_UPLOADING;
                break;
            case MQTT_UPLOADING:
                uploadHistoryRecords();
                printf("%s MQTT 上传完成, 转入上传模式 \n", logtag);
                MqttStatus = MQTT_UPLOAD;
                break;
            case MQTT_UPLOAD:
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
        SendHeartBeat();
    }

}


static void Uart8ReceiveUart5Task(void *pvParamters) {
    printf("%s 创建接收TASK \n", logtag);

    Message message;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

    for (;;) {
        while (uxQueueMessagesWaiting(Uart8FromUart5MsgQueue)) {
            unsigned char HeadMark;
            unsigned char CmdId = 0;
            unsigned char datalen = 0;
            const unsigned char *pszMsgInfo = NULL;
            int           msglen = 0;
            if (xQueueReceive(Uart8FromUart5MsgQueue, &message, xTicksToWait) == pdPASS) {
                printf("%s receive[SenderID  %d,  ID = %d, DATA = %s]\n", logtag, message.SenderID, message.MessageID,message.Data);
                pszMsgInfo = MsgHead_Unpacket(message.Data, msglen, &HeadMark, &CmdId, &datalen);
                ProcessMessage(CmdId,
                               datalen,
                               pszMsgInfo);
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
        while (uxQueueMessagesWaiting(Uart8FromMqttMsgQueue)) {

            if (xQueueReceive(Uart8FromMqttMsgQueue, &message, xTicksToWait) == pdPASS) {
                printf("%s receive[SenderID  %d,  ID = %d, DATA = %s]\n", logtag, message.SenderID, message.MessageID,message.Data);
                handle_line(message.Data, strlen(message.Data));
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
        printf("创建uart8 task  error\n");

    }
    if (xTaskCreate(Uart8ReceiveUart5Task, "uart8 receive uart5", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task error\n");
    }

    if (xTaskCreate(Uart8ReceiveMqttTask, "uart8 receive mqtt server", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart8 task error\n");
    }
    return 0;

}