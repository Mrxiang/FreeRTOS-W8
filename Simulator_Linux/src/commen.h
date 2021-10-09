//
// Created by xshx on 2021/9/29.
//

#ifndef FREERTOS_W8_COMMEN_H
#define FREERTOS_W8_COMMEN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

uint8_t StrGetUInt8( const uint8_t * i_pSrc );
uint16_t StrGetUInt16( const uint8_t * i_pSrc );
uint32_t StrGetUInt32( const uint8_t * i_pSrc );
void StrSetUInt8( uint8_t * io_pDst, const uint8_t i_u8Src );
void StrSetUInt16( uint8_t * io_pDst, const uint16_t i_u16Src );
void StrSetUInt32( uint8_t * io_pDst, const uint32_t i_u32Src );

void StrToHex(unsigned char *pbDest, char *pszSrc, int nLen);
void HexToStr(char *pszDest, unsigned char *pbSrc, int nLen);



#endif //FREERTOS_W8_COMMEN_H