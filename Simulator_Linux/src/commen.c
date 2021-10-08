//
// Created by xshx on 2021/9/29.
//

#include "commen.h"
static const char *logtag ="[commen]-";

void SendMessageToUart8(int ID,  char *data){
    if( Uart8MsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.MessageID = ID;
        message.SenderID  = UART5_TASK;
        message.ReceiverID = UART8_TASK;
        strcpy(message.Data, data);
        if(xQueueSend( Uart8MsgQueue, (void *)&message, 0 ) == pdPASS ){
            printf("%s send [SenderID %d, ID 0x%x,  DATA=%s] \n",logtag, message.SenderID, message.MessageID, message.Data );

        }else{
            printf("could not send to the queue \n");

        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }

}


void SendMessageToUart5FromUart8(int ID,  char *data){
    if( Uart5FromUart8MsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.MessageID=ID;
        message.SenderID = UART8_TASK;
        message.ReceiverID = UART5_TASK;
        strcpy(message.Data, data);
        if(xQueueSend(Uart5FromUart8MsgQueue, (void *)&message, 0 ) == pdPASS ){
            printf("%s send [SenderID %d, ID =0x%x,  DATA=%s] \n",logtag, message.SenderID, message.MessageID, message.Data );

        }else{
            printf("%s could not send to the queue \n",logtag);

        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }
}

void SendMessageToUart5FromMcu(int ID,  char *data){
    if( Uart5FromMcuMsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.MessageID=ID;
        message.SenderID = MCU_TASK;
        message.ReceiverID = UART5_TASK;
        strcpy(message.Data, data);
        if(xQueueSend( Uart5FromMcuMsgQueue, (void *)&message, 0 ) == pdPASS ){
            printf("%s send [SenderID %d, ID =0x%x,  DATA=%s] \n",logtag, message.SenderID, message.MessageID, message.Data );
        }else{
            printf("%s could not send to the queue \n",logtag);
        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }
}


void SendMessageToMCU(int ID, char *data){
    if( McuMsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.MessageID= ID;
        message.SenderID = UART5_TASK;
        message.ReceiverID = MCU_TASK;
        strcpy(message.Data, data);
        if(xQueueSend( McuMsgQueue, (void *)&message, 0 ) == pdPASS ){
            printf("%s send [SenderID %d, ID =0x%x,  DATA=%s]\n",logtag, message.SenderID, message.MessageID, message.Data );

        }else{
            printf("could not send to the queue \n");

        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }
}