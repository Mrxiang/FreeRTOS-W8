//
// Created by xshx on 2021/10/13.
//
#include "mqtt_server.h"
#include "middle/commen_middle.h"
static const char *logtag ="[MQTT-SERVER]-";

static void MQTTServerMainTask(void *pvParamters){
    printf("%s 创建  MQTT Server Main Task \n", logtag);

    int count=0;
    for (;  ;) {
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}
static int ProcessMessage(
        unsigned char nCommand,
        unsigned char nMessageLen,
        const unsigned char *pszMessage) {
    printf("%s Command 0x%x,MessageLen %d Message %s \n",logtag, nCommand, nMessageLen,pszMessage);
    switch (nCommand) {
        case CMD_INITOK_SYNC:
            printf("%s mcu接到初始化请求 \n",logtag);


            break;
        default:
            break;
    }

    return  0;
}


static void MQTTServerReceiveTask(void *pvParameters) {
    printf("%s MCU 接收消息的TASK \n", logtag);
    Message message;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
    for (;;) {
        int           msglen = 0;
        unsigned char HeadMark;
        unsigned char CmdId = 0;
        unsigned char datalen = 0;
        uint8_t recv_buffer[64] = {0};
        const unsigned char *pszMsgInfo = NULL;

        while (uxQueueMessagesWaiting(MqttServerMsgQueue)) {
            memset(&message, 0, sizeof(Message));
            if (xQueueReceive(MqttServerMsgQueue, &message, xTicksToWait) == pdPASS) {
                printf("%s receive[ SenderID %d,  ID = %d, DATA = %s]\n", logtag, message.SenderID, message.MessageID,
                       message.Data);
                StrToHex(recv_buffer, message.Data, sizeof(recv_buffer));
                pszMsgInfo = MsgHead_Unpacket(recv_buffer, msglen, &HeadMark, &CmdId, &datalen);
                printf("%s %s \n", logtag, pszMsgInfo);
                ProcessMessage(CmdId,
                                    datalen,
                                    pszMsgInfo);

            } else {
                printf("%s could not receive from the queue \n", logtag);
            }
//            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

}

int mqtt_server_start(void){

    MqttServerMsgQueue     = xQueueCreate(5, sizeof(Message));

    printf("%s 创建 MCU  Task \n", logtag);
    if (xTaskCreate(MQTTServerMainTask, "MQTT Server Main Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("MQTT Server Main Task Error\n");

    }
    if (xTaskCreate(MQTTServerReceiveTask, "MQTT Server receiver Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("MQTT Server Main Task Error\n");

    }
    return 0;
}