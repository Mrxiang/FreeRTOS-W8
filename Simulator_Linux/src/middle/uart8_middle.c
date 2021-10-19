//
// Created by xshx on 2021/10/13.
//
#include "uart8_middle.h"

int uploadHistoryRecords() {
    printf("上传历史记录 \r\n");

}
int uploadRealTimeRecord( ){
    printf("上传实时记录 \n");
}
int SendHeartBeat(){
    printf("发送心跳 \n");
}

// 连接WIFI
int connectWifi(const char* ssid, const char* password) {
    // int cmd_len = CMD_EXTRA_LEN + strlen(ssid) + strlen(password);

    // char *cmd = (char*)malloc(cmd_len);
    char cmd[MQTT_MAX_LEN];
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, password);

    printf("--- send AT CMD %s\n", cmd);
    int res = run_at_cmd(cmd, 2, 10000);
    // free(cmd);
    return res;
}


int InitMqttConnect( ){
    int result = AT_CMD_RESULT_OK;
    result = run_at_cmd("ATE0", 2, 1200);
}

int run_at_cmd(char const *cmd, int retry_times, int cmd_timeout_usec) {

    return AT_CMD_RESULT_TIMEOUT;

}

