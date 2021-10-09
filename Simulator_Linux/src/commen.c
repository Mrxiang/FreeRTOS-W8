//
// Created by xshx on 2021/9/29.
//

#include "commen.h"
static const char *logtag ="[commen]-";

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
    printf("解包完成 \n");
    return pszBuffer + sizeof(MESSAGE_HEAD);
}

uint8_t StrGetUInt8( const uint8_t * i_pSrc )
{
    uint8_t u8Rtn = 0;
    memcpy(&u8Rtn, i_pSrc, 1);
    return u8Rtn;
}

uint16_t StrGetUInt16( const uint8_t * i_pSrc )
{
    uint16_t u16Rtn = 0;
    for ( uint8_t i=0; i<2; i++ )
    {
        uint16_t u16Temp = 0;
        memcpy(&u16Temp, i_pSrc, 1);
        u16Rtn += u16Temp << (2-1-i)*8;
        i_pSrc++;
    }
    return u16Rtn;
}

uint32_t StrGetUInt32( const uint8_t * i_pSrc )
{
    uint32_t u32Rtn = 0;
    for ( uint8_t i=0; i<4; i++ )
    {
        uint32_t u32Temp = 0;
        memcpy(&u32Temp, i_pSrc, 1);
        u32Rtn += u32Temp << (4-1-i)*8;
        i_pSrc++;
    }
    return u32Rtn;
}
void StrSetUInt8( uint8_t * io_pDst, const uint8_t i_u8Src )
{
    memcpy(io_pDst, &i_u8Src, 1);
    io_pDst++;
}

void StrSetUInt16( uint8_t * io_pDst, const uint16_t i_u16Src )
{
    for ( uint8_t i=0; i<2; i++ )
    {
        uint8_t u8Temp = (i_u16Src >> (2-1-i)*8) & 0xFF;
        memcpy(io_pDst, &u8Temp, 1);
        io_pDst++;
    }
}

void StrSetUInt32( uint8_t * io_pDst, const uint32_t i_u32Src )
{
    for ( uint8_t i=0; i<4; i++ )
    {
        uint8_t u8Temp = (i_u32Src >> (4-1-i)*8) & 0xFF;
        memcpy(io_pDst, &u8Temp, 1);
        io_pDst++;
    }
}
/*
// C prototype : void StrToHex(byte *pbDest, char *pszSrc, int nLen)
// parameter(s): [OUT] pbDest - 输出缓冲区
//	[IN] pszSrc - 字符串
//	[IN] nLen - 16进制数的字节数(字符串的长度/2)
// return value:
// remarks : 将字符串转化为16进制数
*/
void StrToHex(unsigned char *pbDest, char *pszSrc, int nLen)
{
    int i;
    char h1, h2;
    unsigned char s1, s2;
    for (i = 0; i < nLen; i++)
    {
        h1 = pszSrc[2 * i];
        h2 = pszSrc[2 * i + 1];

        s1 = toupper(h1) - 0x30;
        if (s1 > 9)
            s1 -= 7;

        s2 = toupper(h2) - 0x30;
        if (s2 > 9)
            s2 -= 7;

        pbDest[i] = s1 * 16 + s2;
    }
}

/*
// C prototype : void HexToStr(char *pszDest, byte *pbSrc, int nLen)
// parameter(s): [OUT] pszDest - 存放目标字符串
//	[IN] pbSrc - 输入16进制数的起始地址
//	[IN] nLen - 16进制数的字节数
// return value:
// remarks : 将16进制数转化为字符串
*/
void HexToStr(char *pszDest, unsigned char *pbSrc, int nLen)
{
    char	ddl, ddh;
    int i;
    for (i = 0; i < nLen; i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 7;
        if (ddl > 57) ddl = ddl + 7;
        pszDest[i * 2] = ddh;
        pszDest[i * 2 + 1] = ddl;
    }

    pszDest[nLen * 2] = '\0';

}