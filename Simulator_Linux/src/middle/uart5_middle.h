//
// Created by xshx on 2021/10/13.
//

#ifndef FREERTOS_W8_UART5_MIDDLE_H
#define FREERTOS_W8_UART5_MIDDLE_H
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

int cmdSysInitOKSyncRsp(unsigned char nMessageLen, const unsigned char *pszMessage);

int cmdOpenDoorRsp(unsigned char nMessageLen, const unsigned char *pszMessage);

int cmdTemperRsp(unsigned char nMessageLen, const unsigned char *pszMessage);

int cmdPowerDownRsp(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdUserRegReqProc(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdSetSysTimeSynProc(unsigned char nMessageLen, const unsigned char *pszMessage);

int cmdDeleteUserReqProcByHead(unsigned char nHead, unsigned char nMessageLen, const unsigned char *pszMessage);

int cmdReqResumeFactoryProc(unsigned char nMessageLen, const unsigned char *pszMessage);

int cmdReqActiveByPhoneProc(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdWifiSSIDProc(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdWifiPwdProc(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdMqttParamSetProc(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdWifiTimeSyncRsp(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdWifiOrderTimeSyncRsp(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdWifiOpenDoorRsp(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdBTInfoRptProc(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdMqttSvrURLProc(unsigned char nMessageLen, const unsigned char *pszMessage) ;

int cmdMechicalLockRsp(unsigned char nMessageLen, const unsigned char *pszMessage) ;


#endif //FREERTOS_W8_UART5_MIDDLE_H
