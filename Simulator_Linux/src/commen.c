//
// Created by xshx on 2021/9/29.
//

#include "commen.h"
static const char *logtag ="[commen]-";

void SendMessageToUart8(int ID, TASKID SenderID, char *data){
    if( MessageQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.MessageID = ID;
        message.SenderID  = SenderID;
        message.ReceiverID = UART8_TASK;
        strcpy(message.Data, data);
        portBASE_TYPE xStatus = xQueueSend( MessageQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \n");

        }else{
            printf("%s send [SenderID %d, ID 0x%x,  DATA=%s] \n",logtag, message.SenderID, message.MessageID, message.Data );
        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }

}


void SendMessageToUart5(int ID, TASKID SenderID, char *data){
    if( MessageQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.MessageID=ID;
        message.SenderID = SenderID;
        message.ReceiverID = UART5_TASK;
        strcpy(message.Data, data);
        portBASE_TYPE xStatus = xQueueSend(MessageQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \n");

        }else{
            printf("%s send [SenderID %d, ID =0x%x,  DATA=%s] \n",logtag, message.SenderID, message.MessageID, message.Data );
        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }
}

void SendMessageToMCU(int ID,TASKID SenderID, char *data){
    if( MessageQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.MessageID= ID;
        message.SenderID = SenderID;
        message.ReceiverID = MCU_TASK;
        strcpy(message.Data, data);
        portBASE_TYPE xStatus = xQueueSend( MessageQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \n");

        }else{
            printf("%s send [SenderID %d, ID =0x%x,  DATA=%s]\n",logtag, message.SenderID, message.MessageID, message.Data );

        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }
}