//
// Created by xshx on 2021/9/29.
//
#include "uart_driver.h"
#include "commen.h"
const char *logtag ="[UART DRIVER]-";
int LPUART_RTOS_Init(){
    printf("%s 初始化 uart 驱动 \n", logtag);
}
int LPUART_RTOS_Receive( char *buffer, int length){
    printf("%s 接收来自mcu 的消息 \n", logtag);
    Message message;
    portBASE_TYPE  xStatus;
    const portTickType xTicksToWait = 100/portTICK_RATE_MS;
    xStatus = xQueuePeek(MessageQueue, &message, xTicksToWait);
    if (xStatus == pdPASS) {
            if(message.ReceiverID==UART5_TASK && message.SenderID==MCU_TASK )
            xStatus = xQueueReceive(MessageQueue, &message, xTicksToWait);
            if (xStatus == pdPASS  ) {
                printf("%s receive[SenderID %d,  ID = 0x%x, DATA = %s]\r\n",logtag, message.SenderID, message.MessageID, message.Data );
                strcpy( buffer, message.Data);
            } else {
                printf("receive error %d \r\n",xStatus);
            }
    }

}


//int LPUART_RTOS_Send(char *buffer, int length){
//    int commandID;
//    SendMessageToMCU(commandID, UART5_TASK, buffer);
//
//}
