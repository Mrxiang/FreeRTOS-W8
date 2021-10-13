//
// Created by xshx on 2021/9/29.
//

#include "uart5.h"
#include "middle/commen_middle.h"
#include "middle/uart5_middle.h"

//extern  xQueueHandle  Uart8MsgQueue;
static const char *logtag ="[UART5]-";


int ProcessMessage(unsigned char nCommand,unsigned char nMessageLen, unsigned  char *pszMessage) {
    ProcMessageByHead(HEAD_MARK, nCommand, nMessageLen, pszMessage);
    return 0;
}
int ProcMessageByHead(unsigned char nHead,unsigned char  nCommand,unsigned char nMessageLen,unsigned  char *pszMessage) {
    printf("%s ======Head[0x%x],Command[0x%x], nMessageLen<%d>, Message<%s>\n", logtag, nHead,nCommand, nMessageLen, pszMessage);
    switch (nCommand) {

        case CMD_INITOK_SYNC: {
            printf("%s 初始化完成\n", logtag);
            cmdSysInitOKSyncRsp(nMessageLen, pszMessage);
            break;
        }
        case CMD_OPEN_DOOR: {
            cmdOpenDoorRsp(nMessageLen, pszMessage);

            break;
        }
        case CMD_TEMPER_DATA:{//测温请求回复
            cmdTemperRsp( nMessageLen, pszMessage );
            break;
        }
        case CMD_CLOSE_FACEBOARD: {
            cmdPowerDownRsp(nMessageLen, pszMessage);
            break;
        }
        case CMD_FACE_REG: {
            cmdUserRegReqProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_FACE_REG_RLT: {
            break;
        }
        case CMD_NTP_SYS_TIME: {
            cmdSetSysTimeSynProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_FACE_DELETE_USER: {
            cmdDeleteUserReqProcByHead(/*HEAD_MARK*/nHead, nMessageLen, pszMessage);
            break;
        }
        case CMD_REQ_RESUME_FACTORY: {
            cmdReqResumeFactoryProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_REG_ACTIVE_BY_PHONE: {
            cmdReqActiveByPhoneProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_WIFI_SSID: {
            cmdWifiSSIDProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_WIFI_PWD: {
            cmdWifiPwdProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_WIFI_MQTT: {
            cmdMqttParamSetProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_WIFI_TIME_SYNC: {
            cmdWifiTimeSyncRsp(nMessageLen, pszMessage);
            break;
        }
        case CMD_ORDER_TIME_SYNC: {
            cmdWifiOrderTimeSyncRsp(nMessageLen, pszMessage);
            break;
        }
        case CMD_WIFI_OPEN_DOOR: {
            cmdWifiOpenDoorRsp(nMessageLen, pszMessage);
            break;
        }

        case CMD_BT_INFO: {
            printf("%s 收到蓝牙消息\n",logtag);
            cmdBTInfoRptProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_WIFI_MQTTSER_URL: {
            cmdMqttSvrURLProc(nMessageLen, pszMessage);
            break;
        }
        case CMD_MECHANICAL_LOCK: {
//            cmdMechicalLockRsp(nMessageLen, pszMessage);
            break;
        }
        default:
            printf("%s 收到其他消息\n",logtag);
            break;
    }
    return 0;
}



static void Uart5MainTask( void *pvParameters){

    uint8_t recv_buffer[128] = {0};

    printf("%s 创建 Main Task  \n", logtag);
    LPUART_RTOS_Init();
    for (;;) {
        //查询消息
        while (uxQueueMessagesWaiting(Uart5FromMcuMsgQueue)) {
            printf("%s 接收串口消息\n", logtag);
            int           msglen = 0;
            unsigned char HeadMark;
            unsigned char CmdId = 0;
            unsigned char datalen = 0;
            uint8_t       recv_buffer[64] = {0};
            uint8_t       buffer[128] = {0};
            const unsigned char *pszMsgInfo = NULL;

            if (LPUART_RTOS_Receive(buffer, sizeof(buffer)) == pdPASS) {
                //处理消息
                printf("%s 处理串口消息\n", logtag);
                StrToHex(recv_buffer, buffer, sizeof(recv_buffer));
                pszMsgInfo = MsgHead_Unpacket(recv_buffer, msglen, &HeadMark, &CmdId, &datalen);
                printf("%s %s \n", logtag, pszMsgInfo);
                ProcessMessage(CmdId,
                            datalen,
                            pszMsgInfo);

            }
//            vTaskDelay(pdMS_TO_TICKS(1000));

        }
    }


}


static void Uart5ReceiverTask(void *pvParamters) {
    printf("%s 创建接收Uart8 消息TASK \n", logtag);

    Message message;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

    for (;;) {
        while (uxQueueMessagesWaiting(Uart5FromUart8MsgQueue)) {

            if (xQueueReceive(Uart5FromUart8MsgQueue, &message, xTicksToWait) == pdPASS) {
                printf("%s receive[SenderID %d  ID = %d DATA = %s]\n", logtag, message.SenderID, message.MessageID,
                       message.Data);

            } else {
                printf("%s receive error %d \n", logtag, xStatus);
            }
//            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

int uart5_task_start(void ){

//    Uart5MsgQueue = xQueueCreate(5, sizeof(Message));

    printf("%s 创建 uart5 task \n", logtag);

    if (xTaskCreate(Uart5MainTask, "uart5 Sender", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart5 task \n");

    }
    if (xTaskCreate(Uart5ReceiverTask, "uart5 receiver", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart5 task \n");
    }

    return 0;

}