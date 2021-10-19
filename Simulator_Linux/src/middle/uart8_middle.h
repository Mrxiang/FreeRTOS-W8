//
// Created by xshx on 2021/10/13.
//

#ifndef FREERTOS_W8_UART8_MIDDLE_H
#define FREERTOS_W8_UART8_MIDDLE_H


#define AT_CMD_RESULT_OK 		0
#define AT_CMD_RESULT_ERROR 	1
#define AT_CMD_RESULT_TIMEOUT 	2
#define AT_CMD_RESULT_BUSY		3
#define AT_CMD_RESULT_UNDEF	    4

#define AT_CMD_MODE_INACTIVE	0
#define AT_CMD_MODE_ACTIVE 	    1
#define MQTT_MAX_LEN 128

int uploadHistoryRecords( );
int uploadRealTimeRecord( );
int SendHeartBeat( );

int connectWifi(const char* ssid, const char* password) ;
int InitMqttConnect( );

int run_at_cmd(char const *cmd, int retry_times, int cmd_timeout_usec);
#endif //FREERTOS_W8_UART8_MIDDLE_H
