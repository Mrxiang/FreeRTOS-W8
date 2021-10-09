//
// Created by xshx on 2021/9/29.
//

#include "uart5.h"
#include "interlayer.h"


//extern  xQueueHandle  Uart8MsgQueue;
static const char *logtag ="[UART5]-";


int ProcessMessage(int nCommandID,unsigned char nMessageLen, char *Data) {
    ProcMessageByHead(HEAD_MARK, nCommandID, nMessageLen, Data);
    return 0;
}
int ProcMessageByHead(unsigned char nHead,int  nCommandID,unsigned char nMessageLen,char *Data) {
    printf("%s ======Head[0x%x],Command[0x%x], nMessageLen<%d>, Data<%s>\n", logtag, nHead,nCommandID, nMessageLen, Data);
    switch (nCommandID) {
        case CMD_INITOK_SYNC: {
            printf("%s 初始化完成\n", logtag);
            break;
        }
        case CMD_BT_INFO:{
            printf("%s 收到蓝牙消息\n",logtag);
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