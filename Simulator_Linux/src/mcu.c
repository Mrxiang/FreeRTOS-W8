//
// Created by xshx on 2021/9/29.
//

#include "mcu.h"
#include "middle/commen_middle.h"

//MCU TASK 和 uart5 交互
static const char *logtag ="[MCU]-";



char Init_CMD[128]="23010B15091B00281044014000013B76";

char BLUETOOTH_CMD[128]="2316078DC12E332B5E8FB8A0";
MCU_STATUS   mcuStatus=MCU_SLEEP;

static void McuMainTask(void *pvParamters){
    printf("%s 创建  MCU Main TasK \n", logtag);

    int count=0;
    for (;  ;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        if( mcuStatus == MCU_WAKE ){
            printf("\r\n%s %d  Start A Process ...\r\n", logtag, count);
            SendMessageToUart5FromMcu( BLUETOOTH_CMD);

            count++;
        }

    }
}
int ProcessW8Message(
        unsigned char nCommand,
        unsigned char nMessageLen,
        const unsigned char *pszMessage) {
    printf("%s Command 0x%x,MessageLen %d Message %s \n",logtag, nCommand, nMessageLen,pszMessage);
    switch (nCommand) {
        case CMD_INITOK_SYNC:
            printf("%s mcu接到初始化请求 \n",logtag);
            SendMessageToUart5FromMcu(   Init_CMD );
            mcuStatus = MCU_WAKE;

            break;
        default:
            break;
    }

    return  0;
}


static void McuReceiverTask(void *pvParameters) {
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

        while (uxQueueMessagesWaiting(McuMsgQueue)) {
            memset(&message, 0, sizeof(Message));
            if (xQueueReceive(McuMsgQueue, &message, xTicksToWait) == pdPASS) {
                printf("%s receive[ SenderID %d,  ID = %d, DATA = %s]\n", logtag, message.SenderID, message.MessageID,
                       message.Data);
                StrToHex(recv_buffer, message.Data, sizeof(recv_buffer));
                pszMsgInfo = MsgHead_Unpacket(recv_buffer, msglen, &HeadMark, &CmdId, &datalen);
                printf("%s %s \n", logtag, pszMsgInfo);
                ProcessW8Message(CmdId,
                            datalen,
                            pszMsgInfo);

            } else {
                printf("%s could not receive from the queue \n", logtag);
            }
//            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

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
