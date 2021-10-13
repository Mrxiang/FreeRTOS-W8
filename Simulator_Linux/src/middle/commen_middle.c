//
// Created by xshx on 2021/10/9.
//
#include "commen_middle.h"

static const char *logtag ="[COMMEN_MIDDLE]-";

void SendMessageToUart8(  char *data){
    if( Uart8MsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.SenderID  = UART5_TASK;
        message.ReceiverID = UART8_TASK;
        strcpy(message.Data, data);
        if(xQueueSend( Uart8MsgQueue, (void *)&message, 0 ) == pdPASS ){
            printf("%s send [SenderID %d, ReceiverID %d,  DATA=%s] \n",logtag, message.SenderID, message.ReceiverID,  message.Data );

        }else{
            printf("could not send to the queue \n");

        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }

}


void SendMessageToUart5FromUart8( char *data){
    if( Uart5FromUart8MsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.SenderID = UART8_TASK;
        message.ReceiverID = UART5_TASK;
        strcpy(message.Data, data);
        if(xQueueSend(Uart5FromUart8MsgQueue, (void *)&message, 0 ) == pdPASS ){
            printf("%s send [SenderID %d, ReceiverID %d,  DATA=%s] \n",logtag, message.SenderID, message.ReceiverID,  message.Data );

        }else{
            printf("%s could not send to the queue \n",logtag);

        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }
}

void SendMessageToUart5FromMcu(  char *data){
    if( Uart5FromMcuMsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.SenderID = MCU_TASK;
        message.ReceiverID = UART5_TASK;
        strcpy(message.Data, data);
        if(xQueueSend( Uart5FromMcuMsgQueue, (void *)&message, 0 ) == pdPASS ){
            printf("%s send [SenderID %d, ReceiverID %d,  DATA=%s] \n",logtag, message.SenderID, message.ReceiverID,  message.Data );

        }else{
            printf("%s could not send to the queue \n",logtag);
        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }
}


void SendMessageToMCU( char *data){
    if( McuMsgQueue != NULL ){
        Message message;
        memset(&message, 0, sizeof(Message));
        message.SenderID = UART5_TASK;
        message.ReceiverID = MCU_TASK;
        strcpy(message.Data, data);
        if(xQueueSend( McuMsgQueue, (void *)&message, 0 ) == pdPASS ){
            printf("%s send [SenderID %d, ReceiverID %d,   DATA=%s] \n",logtag, message.SenderID, message.ReceiverID,  message.Data );

        }else{
            printf("could not send to the queue \n");

        }
        taskYIELD();
    }else{
        printf("create  message queue first \n");
    }
}

/****************************************************************************************
函数名称：MsgHead_Unpacket
函数功能：解包
入口参数：pszBuffer--数据
		iBufferSize--数据大小
		HeadMark--标记头
		CmdID--命令
		MsgLen--消息长度
出口参数：无
返回值：成功返回消息内容的指针，失败返回-1
****************************************************************************************/
const unsigned char *MsgHead_Unpacket(
        const unsigned char *pszBuffer,
        unsigned char iBufferSize,
        unsigned char *HeadMark,
        unsigned char *CmdId,
        unsigned char *MsgLen) {
    if (!pszBuffer) {
        printf("pszBuffer is NULL\n");
        return NULL;
    }
    const unsigned char *pTemp = pszBuffer;

    *HeadMark = StrGetUInt8(pTemp);
    pTemp += sizeof(uint8_t);
    *CmdId = StrGetUInt8(pTemp);
    pTemp += sizeof(uint8_t);
    *MsgLen = StrGetUInt8(pTemp);
    pTemp += sizeof(uint8_t);
    if (*HeadMark != HEAD_MARK && *HeadMark != HEAD_MARK_MQTT) {
        printf("byVersion[0x%x] != MESSAGE_VERSION[0x%x|0x%x]\n", \
            *HeadMark, HEAD_MARK, HEAD_MARK_MQTT);
        return NULL;
    }

//    if ((int) *MsgLen + sizeof(MESSAGE_HEAD) + CRC16_LEN > iBufferSize) {
//        printf("pstMessageHead->MsgLen  %d + sizeof(MESSAGE_HEAD) + CRC16_LEN > iBufferSize %d \n", (int)*MsgLen, iBufferSize);
//        return NULL;
//    }
    printf("%s解包完成 \n",logtag);
    return pszBuffer + sizeof(MESSAGE_HEAD);
}
