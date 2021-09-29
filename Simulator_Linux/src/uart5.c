//
// Created by xshx on 2021/9/29.
//

#include "uart5.h"
#include "commen.h"


extern  xQueueHandle  Uart8MsgQueue;
static const char *logtag ="[UART5]-";


int ProcessMessage(int nCommandID,unsigned char nMessageLen, char *Data) {
    ProcMessageByHead(HEAD_MARK, nCommandID, nMessageLen, Data);
    return 0;
}
int ProcMessageByHead(unsigned char nHead,int  nCommandID,unsigned char nMessageLen,char *Data) {
    printf("%s ======Command[0x%X], nMessageLen<%d>, Data<%s>\r\n", logtag, nCommandID, nMessageLen, Data);
    switch (nCommandID) {
        case CMD_INITOK_SYNC: {
            SendMessageToUart5(CMD_INITOK_SYNC, Data);
            break;
        }
        default:
            SendMessageToUart8(nCommandID, Data );
            break;
    }
    return 0;
}

static void Uart5MainTask( void *pvParameters){

    uint8_t recv_buffer[64] = {0};

    printf("%s main task start \n", logtag);
    LPUART_RTOS_Init();
    do{
        //接收
        LPUART_RTOS_Receive(recv_buffer, 64);

        //处理消息
        ProcessMessage( CMD_TEMPER_DATA, 64, recv_buffer );
    }while (1);


}


static void Uart5ReceiverTask(void *pvParamters){
    printf("%s 创建接收TASK \n", logtag);

    Message message;
    portBASE_TYPE  xStatus;
    const portTickType xTicksToWait = 100/portTICK_RATE_MS;

    for (;  ;) {
        if (uxQueueMessagesWaiting(Uart5MsgQueue) != 0) {
            printf("%s queue should have been empty ! \r\n", logtag);
        }

        xStatus = xQueueReceive(Uart5MsgQueue, &message, xTicksToWait);
        if (xStatus == pdPASS) {
            printf("%s receive[  %s  ID = %d DATA = %s]\r\n",logtag, message.sender, message.ID, message.Data );

        } else {
            printf("could not receive from the queue \r\n");
        }
    }
}

int uart5_task_start(void ){

    Uart5MsgQueue = xQueueCreate(5, sizeof(Message));

    printf("%s 创建 uart5 task \n", logtag);
    if (xTaskCreate(Uart5MainTask, "uart5 Sender", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart5 task \n");

    }
    if (xTaskCreate(Uart5ReceiverTask, "uart5 receiver", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("创建uart5 task \n");
    }

    return 0;

}