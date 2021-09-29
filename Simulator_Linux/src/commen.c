//
// Created by xshx on 2021/9/29.
//

#include "commen.h"
static const char *logtag ="[commen]-";

void SendMessageToUart8(int ID, char *data){
    if( Uart8MsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.ID=ID;
        strcpy(message.sender, logtag);
        strcpy(message.Data, data);
        portBASE_TYPE xStatus = xQueueSend( Uart8MsgQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \r\n");

        }else{
            printf("%s send [ %s ID =0x%x  DATA=%s]\r\n",logtag, message.sender, message.ID, message.Data );
        }
        taskYIELD();
    }else{
        printf("create uart8 message first \n");
    }

}


void SendMessageToUart5(int ID,char *data){
    if( Uart5MsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.ID=ID;
        strcpy(message.sender, logtag);
        strcpy(message.Data, data);
        portBASE_TYPE xStatus = xQueueSend( Uart5MsgQueue, (void *)&message, 0 );
        if(xStatus != pdPASS ){
            printf("could not send to the queue \r\n");

        }else{
            printf("%s send [ %s ID =0x%x  DATA=%s]\r\n",logtag, message.sender, message.ID, message.Data );
        }
        taskYIELD();
    }else{
        printf("create uart8 message first \n");
    }
}