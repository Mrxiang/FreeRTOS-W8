//
// Created by xshx on 2021/9/29.
//
#include "uart_driver.h"
#include "interlayer.h"

const char *logtag = "[UART DRIVER]-";
unsigned char Init_Request[128]="230105302E302E32DDEC";

int LPUART_RTOS_Init() {
    printf("%s 初始化 uart 驱动 \n", logtag);
//    char Init_dest[64];
//    memset(Init_dest, 0, sizeof(Init_dest));
//    StrToHex(Init_dest, Init_Request, sizeof(Init_dest));
//    for( int i=0; i< sizeof(Init_dest); i++){
//        printf("0x%x \n", Init_dest[i]);
//    }
//    SendMessageToMCU( Init_dest);
    SendMessageToMCU( Init_Request);
}

int LPUART_RTOS_Receive(char *buffer, int length) {
    printf("%s 接收来自mcu 的消息 \n", logtag);
    Message message;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
    if (pdPASS == xQueueReceive(Uart5FromMcuMsgQueue, &message, xTicksToWait)) {
        printf("%s receive[SenderID %d,  ID = 0x%x, DATA = %s]\r\n", logtag, message.SenderID, message.MessageID,
               message.Data);
        strcpy(buffer, message.Data);
        return pdPASS;
    } else {
        printf("%s receive error %d \r\n", logtag, xStatus);
    }
    return  pdFALSE;
}


//int LPUART_RTOS_Send(char *buffer, int length){
//    int commandID;
//    SendMessageToMCU(commandID, UART5_TASK, buffer);
//
//}
