//
// Created by xshx on 2021/9/29.
//

#include "uart5.h"
#include "commen.h"


//extern  xQueueHandle  Uart8MsgQueue;
static const char *logtag ="[UART5]-";


int ProcessMessage(int nCommandID,unsigned char nMessageLen, char *Data) {
    ProcMessageByHead(HEAD_MARK, nCommandID, nMessageLen, Data);
    return 0;
}
int ProcMessageByHead(unsigned char nHead,int  nCommandID,unsigned char nMessageLen,char *Data) {
    printf("%s ======Command[0x%x], nMessageLen<%d>, Data<%s>\n", logtag, nCommandID, nMessageLen, Data);
    switch (nCommandID) {
        case CMD_INITOK_SYNC: {
            printf("%s 收到初始化消息\n", logtag);
            SendMessageToMCU(CMD_INITOK_SYNC,UART5_TASK, Data);
            break;
        }
        default:
            SendMessageToUart8(nCommandID,UART5_TASK, Data );
            break;
    }
    return 0;
}

static void Uart5MainTask( void *pvParameters){

    uint8_t recv_buffer[64] = {0};

    printf("%s 创建 Main Task  \n", logtag);
    LPUART_RTOS_Init();
    while(1){
        //接收
        printf("%s 接收串口消息\n", logtag);
        LPUART_RTOS_Receive(recv_buffer, 64);
        //处理消息
        printf("%s 处理串口消息\n", logtag);
        ProcessMessage(CMD_INITOK_SYNC , 64, recv_buffer );

        vTaskDelay(pdMS_TO_TICKS(1000));

    };


}


static void Uart5ReceiverTask(void *pvParamters){
    printf("%s 创建接收TASK \n", logtag);

    Message message;
    portBASE_TYPE  xStatus;
    const portTickType xTicksToWait = 100/portTICK_RATE_MS;

    for (;  ;) {

        if( xQueuePeek(MessageQueue, &message, xTicksToWait)==pdPASS ){
            if( message.ReceiverID == UART5_TASK && (message.SenderID == UART8_TASK) ){
                if(  xQueueReceive(MessageQueue, &message, xTicksToWait)==pdPASS){
                    printf("%s receive[SenderID %d  ID = %d DATA = %s]\n",logtag, message.SenderID, message.MessageID, message.Data );

                } else {
                    printf("receive error %d \n", xStatus);
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));

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