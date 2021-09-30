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

#define CMD_INITOK_SYNC          		0x01		// 开机同步
#define CMD_HEART_BEAT		          	0x02		// 心跳
#define CMD_OPEN_DOOR                   0x03		// 开门
#define CMD_CLOSE_FACEBOARD				0x04		// 关机
#define CMD_FACE_REG					0x05		// 注册请求
#define CMD_REG_STATUS_NOTIFY			0x06		// 注册状态通知
#define CMD_FACE_REG_RLT				0x07		//注册结果
#define CMD_REG_ACTIVE_BY_PHONE			0x08		// 注册激活
#define CMD_FACE_DELETE_USER			0x09		// 删除用户
#define CMD_NTP_SYS_TIME				0x0A		// 网络对时
#define CMD_REQ_RESUME_FACTORY			0x0B		// 请求恢复出厂设置， MCU发起
#define CMD_UPGRADE_CTRL				0x0C		// 升级控制
#define CMD_UPGRADE_PACKAGE				0x0D		// 升级单包传输
#define CMD_FACE_RECOGNIZE				0x0E		// MCU主动请求人脸识别
#define CMD_WIFI_SSID					0x0F		// 设置wifi的SSID
#define CMD_WIFI_PWD					0x10		// 设置wifi的PWD
#define CMD_WIFI_MQTT					0x11		// 设置MQTT参数
#define CMD_WIFI_CONN_STATUS			0x12		// 上报wifi连接状态
#define CMD_DEV_DEBUG					0x13		// 上报设备故障状态
#define CMD_OPEN_LOCK_REC				0x14		// 上报开门记录
#define CMD_ORDER_TIME_SYNC				0x15		// 远程订单时间同步
#define CMD_BT_INFO						0x16		// 上报蓝牙模块信息
#define CMD_WIFI_MQTTSER_URL			0x17		// 设置wifi的MQTT server登录URL（可能是IP+Port，可能是域名+Port）
#define CMD_GETNETWORK_OPTVER			0x18		// 主控获取MCU中flash存储的网络参数设置版本号
#define CMD_SETNETWORK_OPTVER			0x19		// 设置网络参数版本号,每次设置后都自加1，存入flash
#define CMD_NETWORK_OPT					0x1A		// 网络参数设置
#define CMD_MQTT_UPLOAD					0x1B		// 请求mqtt 上传记录

#define CMD_TEMPER_DATA                  0x30        //蓝牙测温

#define CMD_BOOT_MODE                   0x82  //上报开机模式 (与其它命令有区别 06.21)
#define CMD_WIFI_OPEN_DOOR				0x83	// 远程wifi开门
#define CMD_WIFI_TIME_SYNC				0x8A	// 通过wifi设置系统时间
#define CMD_MECHANICAL_LOCK				0x93	//机械开锁
#define CMD_IRLIGHT_PWM_Req				0xE0	// 设置IR补光灯


#define HEAD_MARK			0x23     /*  ‘#’*/
#define HEAD_MARK_MQTT		0x24     /*  ‘for mqtt’*/



typedef  enum {
    MCU_TASK,
    UART5_TASK,
    UART8_TASK
}TASKID;

typedef  struct
{
    int MessageID;
    int SenderID;
    int ReceiverID;
    char Data[ 100 ];
} Message, *pMessage;

//xQueueHandle  Uart5MsgQueue;
//xQueueHandle  Uart8MsgQueue;
//xQueueHandle  McuMsgQueue;


//只定义一个MessageQueue
xQueueHandle  MessageQueue;


void SendMessageToUart5(int ID,TASKID SenderID, char *data);
void SendMessageToUart8(int ID,TASKID SenderID,char *data);
void SendMessageToMCU(int ID,TASKID SenderID,char *data);

int ProcessMessage( int nCommandID,unsigned char nMessageLen, char *Data);

int ProcMessageByHead(unsigned char nHead,int nCommandID,unsigned char nMessageLen,char *Data);

#endif //FREERTOS_W8_COMMEN_H