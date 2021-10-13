//
// Created by xshx on 2021/10/13.
//
#include "uart5_middle.h"

//主控接收指令:  开机同步响应
int cmdSysInitOKSyncRsp(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("接收开机同步响应 \r\n");
//    unsigned char szBuffer[32] = {0};
//
//    memcpy(szBuffer, pszMessage, nMessageLen);
//
//    uint8_t year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0, i = 0;
//    year = StrGetUInt8(pszMessage + i);
//    i++;
//    month = StrGetUInt8(pszMessage + i);
//    i++;
//    day = StrGetUInt8(pszMessage + i);
//    i++;
//    hour = StrGetUInt8(pszMessage + i);
//    i++;
//    min = StrGetUInt8(pszMessage + i);
//    i++;
//    sec = StrGetUInt8(pszMessage + i);
//    i++;
//    printf("setTime:%04d-%02d-%02d %02d:%02d:%02d \r\n", 2000 + year, month, day, hour, min, sec);
//
//    if ((month == 0 || month > 12) || (day == 0 || day > 31) || hour > 24 || min > 60 || sec > 60) {
//        printf("Set Time value error!\r\n");
//    } else {
//        /*系统时间设置*/
//        SysTimeSet(year, month, day, hour, min, sec);
//    }
//
//    bSysTimeSynProc = true;
//
//    //解析设置参数
//    battery_level = StrGetUInt8(pszMessage + i);
//    i++;
//    printf("battery_level:<%d>.\r\n", battery_level);
//    bInitSyncInfo = true;
//
//
//    {
//        char verbuf[4] = {0};
//        char ver_tmp[32] = {0};
//
//        /* 保存x7 信息到系统配置文件 */
//        // 获取字符串格式的版本号
//        memset(verbuf, 0, sizeof(verbuf));
//        memset(ver_tmp, 0, sizeof(ver_tmp));
//        sprintf(ver_tmp, "%s", FIRMWARE_VERSION);
//        update_sys_info(ver_tmp);
//        //printf("SYS_VERSION :<%s>.\n", ver_tmp);
//
//        // 保存设置到系统配置文件
//        memset(ver_tmp, 0, sizeof(ver_tmp));
//        sprintf(ver_tmp, "%s", PROJECT_VERSION);
//        update_project_info(ver_tmp);
//        //printf("OASIS_VERSION :<%s>.\n", ver_tmp);
//
//
//        /* 保存MCU 信息到系统配置文件 */
//        // 获取字符串格式的版本号
//        memset(verbuf, 0, sizeof(verbuf));
//        memset(ver_tmp, 0, sizeof(ver_tmp));
//        //sprintf(verbuf, "%03d", stInitSyncInfo.Mcu_Ver);
//        //sprintf(ver_tmp, "W8_HC130_106F_V%c.%c.%c", verbuf[0], verbuf[1], verbuf[2]);
//        verbuf[1] = StrGetUInt8(pszMessage + i);
//        i++;
//        verbuf[0] = StrGetUInt8(pszMessage + i);
//        i++;
//        sprintf(ver_tmp, "W8_HC130_106F_V%d.%d.%d", verbuf[1], verbuf[0] >> 4, verbuf[0] & 0x0f);
//        printf("MCU_VERSION:<%s>.\r\n", ver_tmp);
//
//        // 保存设置到系统配置文件
//        update_mcu_info(ver_tmp);
//        //read_config("./config.ini");
//        //printf("MCU_VERSION:<%s>.\n", ver_tmp);
//
//        //system("sync");
//    }
//
//    boot_mode = StrGetUInt8(pszMessage + i);
//    if (boot_mode > BOOT_MODE_MECHANICAL_LOCK) {
//        boot_mode = BOOT_MODE_INVALID;
//    }
//    printf("boot_mode: %d\r\n", boot_mode);
//    receive_boot_mode = 1;
//    if ((boot_mode == BOOT_MODE_RECOGNIZE) || (boot_mode == BOOT_MODE_REG)) {
//        if (oasis_task_start == false) {
//            oasis_task_start = true;
//            OpenLcdBackground();
//            Display_Start();
//            Oasis_Start();
//
//            //Display_Update((uint32_t)wave_logo_v3);
//        }
//    } else if (boot_mode == BOOT_MODE_REMOTE) {
//        cmdRemoteRequestRsp(HEAD_MARK, CMD_BOOT_MODE, 1);
//    }

    return 0;
}


// 主控接收指令:开门响应
int cmdOpenDoorRsp(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("x7 收到mcu 的开门响应 \r\n");
//    uint8_t ret = -1;
//    unsigned char *pop = NULL;
//    unsigned char szBuffer[32] = {0};
//
//    // MCU到face_loop，0代表开锁成功，1代表开锁失败
//    memcpy(szBuffer, pszMessage, nMessageLen);
//
//    pop = szBuffer;
//
//    // MCU到face_loop，0代表开锁成功，1代表开锁失败
//    ret = StrGetUInt8(pszMessage);
//    // 如果开锁成功 更新数据库状态 ,请求mqtt上传本次操作记录。
//    if (ret == 0) {
//        //					xshx add
//        //char power_msg[32] = {0};
//        pop += 1;
//        uint8_t power = StrGetUInt8(pop);
//        pop += 1;
//        uint8_t power2 = StrGetUInt8(pop);
//
//        Record *record = (Record *) pvPortMalloc(sizeof(Record));
//        HexToStr(username, g_uu_id.UID, sizeof(g_uu_id.UID));
//        strcpy(record->UUID, username);
//        record->action = FACE_UNLOCK;//  操作类型：0代表注册 1: 一键开锁 2：钥匙开锁  3 人脸识别开锁
//        char image_path[16];
//        //record->status = 0; // 0,操作成功 1,操作失败.
//        record->time_stamp = ws_systime; //时间戳 从1970年开始的秒数
////        record->power = power * 256 + power2;
//        record->data[0]=power;
//        record->data[1]=power2;
//        //sprintf(power_msg, "{\\\"batteryA\\\":%d\\,\\\"batteryB\\\":%d}", record->power, record->power2);
//        //printf("power_msg is %s \r\n", power_msg);
//
//        record->upload = BOTH_UNUPLOAD; //   0代表没上传 1代表记录上传图片未上传 2代表均已
////        record->action_upload = 0x300;
////        memset(image_path, 0, sizeof(image_path)); // 对注册成功的用户保存一张压缩过的jpeg图片
//        //snprintf(image_path, sizeof(image_path), "REC_%d_%d_%s.jpg", 0, record->time_stamp, record->UUID);
////#if    SUPPORT_PRESSURE_TEST != 0
////        snprintf(image_path, sizeof(image_path), "%x21.jpg", record->time_stamp & 0x00FFFFFF);
////#else
////        snprintf(image_path, sizeof(image_path), "%x.jpg", record->time_stamp);
////#endif
////        memcpy(record->image_path, image_path, sizeof(image_path));//image_path
//        record->image_path = IMAGE_VALID;
////        DBManager::getInstance()->addRecord(record);
//        int id =RecordManager::getInstance()->addRecord(record);
////        Oasis_SetOasisFileName(record->image_path);
//
////        Oasis_WriteJpeg();
//        char *imageBuffer = getOasisBuffer();
//        RecordManager::getInstance()->addImage(id, (uint8_t*)imageBuffer);
////        int ID = DBManager::getInstance()->getLastRecordID();
//        int ID = RecordManager::getInstance()->getLastRecordID();
//        printf("开锁成功, 更新数据库状态.请求MQTT上传本次开门的记录 \r\n");
//        cmdRequestMqttUpload(ID);
//    } else {
////    	g_command_executed = 1;
//        printf("开锁失败,不更新数据库状态. 不上传记录,请求下电\r\n");
//        cmdCloseFaceBoardReq();
//
//    }

    return 0;
}

// 主控接收指令:测温响应
int cmdTemperRsp(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("x7 收到mcu 的测温响应 \r\n");
//    uint16_t ret = -1;
////    unsigned char *pop = NULL;
//    char szBuffer[128]={0};
//
//    // MCU到face_loop，0代表开锁成功，1代表开锁失败
//    memcpy(szBuffer, pszMessage, nMessageLen);
//    printf("收到的测温数据  \r\n");
//    for(int i=0; i<nMessageLen; i++){
//        printf( "%d  0x%02x \r\n", i, szBuffer[i]);
//    }
//    //    pop = pszMessage;
//
//    // MCU到face_loop，0代表开锁成功，1代表开锁失败
////    ret = StrGetUInt16(pszMessage);
//    // 如果开锁成功 更新数据库状态 ,请求mqtt上传本次操作记录。
//    if (nMessageLen != 0) {
//        //					xshx add
////        char power_msg[32] = {0};
////        pop += 1;
////        uint8_t power = StrGetUInt8(pop);
////        pop += 1;
////        uint8_t  power2 = StrGetUInt8(pop);
//
//        Record *record = (Record *) pvPortMalloc(sizeof(Record));
//        memset( record, 0, sizeof(Record));
//        HexToStr(username, g_uu_id.UID, sizeof(g_uu_id.UID));
//        strcpy(record->UUID, username);
//        record->action = FACE_TEMPER;//  操作类型：0代表注册 1: 一键开锁 2：钥匙开锁  3 人脸识别开锁 10 测温
//        char image_path[16];
//        //record->status = 0; // 0,操作成功 1,操作失败.
//        record->time_stamp = ws_systime; //时间戳 从1970年开始的秒数
//
////        record->power = power * 256 + power2;
//        //sprintf(power_msg, "{\\\"batteryA\\\":%d\\,\\\"batteryB\\\":%d}", record->power, record->power2);
//        //printf("power_msg is %s \r\n", power_msg);
//
//        record->upload = BOTH_UNUPLOAD; //   0代表没上传 1代表记录上传图片未上传 2代表均已
////        record->action_upload = 0x1000;
////        memset(image_path, 0, sizeof(image_path)); // 对注册成功的用户保存一张压缩过的jpeg图片
////        snprintf(image_path, sizeof(image_path), "%x.jpg", record->time_stamp);
////        memcpy(record->image_path, image_path, sizeof(image_path));//image_path
//
////        蓝牙测温
//        char* ret = base64_encode( szBuffer, record->data, nMessageLen );
//        printf(" record->data %s \r\n",  record->data);
////        DBManager::getInstance()->addRecord(record);
//        RecordManager::getInstance()->addRecord(record);
//
////        Oasis_SetOasisFileName(record->image_path);
////        Oasis_WriteJpeg();
//
////        WriteTemper(record->image_path, (char *)szBuffer, sizeof(szBuffer) );//写温度文件
//
////        int ID = DBManager::getInstance()->getLastRecordID();
//        int ID = RecordManager::getInstance()->getLastRecordID();
//        printf("测温成功, 更新数据库状态.请求MQTT上传本次测温的记录 \r\n");
//        cmdRequestMqttUpload(ID);
//    } else {
//        printf("测温失败,不更新数据库状态. 不上传记录,请求下电 \r\n");
//        cmdCloseFaceBoardReq();
//
//    }

    return 0;
}
// 接收mcu发来的下电应答指令
int cmdPowerDownRsp(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("收到MCU发来的下电回复 \r\n");
//    for (int i = 0; i < nMessageLen; i++) {
//        printf("0x%02x	\r\n", pszMessage[i]);
//    }
    return 0;
}

//主控接收指令:  用户注册请求
int cmdUserRegReqProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("用户注册请求 \r\n");
//    uint8_t ret = SUCCESS, len = 0;
//    unsigned char szBuffer[32] = {0};
//    unsigned char *pos = szBuffer;
//    memcpy(szBuffer, pszMessage, nMessageLen);
//
//    //解析指令
//    if ((nMessageLen < sizeof(szBuffer)) && nMessageLen == 8) {
//        memcpy(&g_uu_id, pos + len, 8);
//        len += 8;
//    }
//
//    g_reging_flg = REG_STATUS_ING;
//    boot_mode = BOOT_MODE_REG;
//    if (lcd_back_ground == false) {
//        OpenLcdBackground();
//    }
//
//    //printf("reg uuid<len=%d> : L<0x%08x>, H<0x%08x>.\n", sizeof(g_uu_id), g_uu_id.tUID.L, g_uu_id.tUID.H);
//
//    memset(username, 0, sizeof(username));
//    HexToStr(username, g_uu_id.UID, sizeof(g_uu_id.UID));
//    username[16] = '\0';//NXP的人脸注册API的username最大只能16byte
//    printf("=====UUID<len:%d>:%s.\r\n", sizeof(username), username);
//
//
//    vizn_api_status_t status;
//    status = VIZN_AddUser(NULL, username);
//    switch (status) {
//        case kStatus_API_Layer_AddUser_NoAddCommand:
//            printf("No add command registered\r\n");
//            break;
//        case kStatus_API_Layer_AddUser_AddCommandStopped:
//            printf("Stopped adding %s \r\n");
//            break;
//        case kStatus_API_Layer_AddUser_InvalidUserName:
//            printf("Invalid User Name\r\n");
//            break;
//        case kStatus_API_Layer_AddUser_MultipleAddCommand:
//            printf("Add command already in pending. Please complete the pending registration or stop the adding\r\n");
//            break;
//        case kStatus_API_Layer_Success:
//            printf("Start registering...\r\n");
//            break;
//        default:
//            printf("ERROR API ENGINE, status<%d>.\r\n", status);
//            break;
//    }
//
//    if (kStatus_API_Layer_Success != status) {
//        ret = FAILED;
//    }
//
//    //返回响应消息
//    cmdUserRegRsp(ret);
    return 0;
}


//主控接收指令:  时间同步请求
int cmdSetSysTimeSynProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("时间同步请求 \r\n");
//    uint8_t year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0, i = 0;
//    uint8_t ret = SUCCESS;
//    if (nMessageLen == 6) {
//        year = StrGetUInt8(pszMessage + i);
//        i++;
//        month = StrGetUInt8(pszMessage + i);
//        i++;
//        day = StrGetUInt8(pszMessage + i);
//        i++;
//        hour = StrGetUInt8(pszMessage + i);
//        i++;
//        min = StrGetUInt8(pszMessage + i);
//        i++;
//        sec = StrGetUInt8(pszMessage + i);
//        i++;
//    } else {
//        ret = FAILED;
//    }
//    printf("setTime:%04d-%02d-%02d %02d:%02d:%02d \r\n", 2000 + year, month, day, hour, min, sec);
//
//    if ((month == 0 || month > 12) || (day == 0 || day > 31) || hour > 24 || min > 60 || sec > 60) {
//        ret = FAILED;
//        printf("Set Time value error!\n");
//    } else {
//        /*系统时间设置*/
//        SysTimeSet(year, month, day, hour, min, sec);
//    }
//
//    bSysTimeSynProc = true;
//
//    cmdSetSysTimeSynRsp(ret);

    return 0;
}

// 主控接收指令: 删除用户请求
int cmdDeleteUserReqProcByHead(unsigned char nHead, unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf(" 删除用户请求 \r\n");
//    uint8_t ret = FAILED;
//    char szBuffer[32] = {0};
//    uUID uu_id;
//
//    // 解析指令
//    if ((nMessageLen < sizeof(szBuffer)) && nMessageLen == UUID_LEN) {
//        memcpy(&uu_id, pszMessage, nMessageLen);
//    }
//
//    printf("delete uuid : <0x%08x>, <0x%08x>.\r\n", uu_id.tUID.H, uu_id.tUID.L);
//    if (uu_id.tUID.H == 0xFFFFFFFF && uu_id.tUID.L == 0xFFFFFFFF) {
//        // 清空所有用户 和操作记录
//        char strUUID[20] = {0};
//        HexToStr(strUUID, uu_id.UID, UUID_LEN);
//        // 清空用户
//        //ret =DBManager::getInstance()->clearUser();
//        // 清空操作记录
//        //ret = DBManager::getInstance()->clearRecord();
//        ret = RecordManager::getInstance()->clearRecords();
//        vizn_api_status_t status;
//        status = VIZN_DelUser(NULL);
//        printf("cmdDeleteUserReqProcByHead VIZN_DelUser status is %d\r\n", status);
//        if (kStatus_API_Layer_Success == status) {
//            ret = SUCCESS;
//        } else {
//            ret = FAILED;
//        }
//        //DB_Save(0);
//    } else {
//        // 删除单个用户 和其操作记录
//        printf("delete single user start");
//        //删除单个用户
//        char strUUID[20] = {0};
//        HexToStr(strUUID, uu_id.UID, UUID_LEN);
//        //ret = DBManager::getInstance()->deleteUserByUUID( strUUID );
//        //删除用户的操作记录
////        ret = DBManager::getInstance()->deleteRecordByUUID(strUUID);
//        ret = RecordManager::getInstance()->clearRecordByUUID(strUUID);
//
//        memset(username, 0, sizeof(username));
//        HexToStr(username, uu_id.UID, sizeof(uu_id.UID));
//        username[16] = '\0';//NXP的人脸注册API的username最大只能16byte
//        printf("=====UUID<len:%d>:%s.\r\n", sizeof(username), username);
//
//        vizn_api_status_t status;
//        status = VIZN_DelUser(NULL, username);
//        if (kStatus_API_Layer_Success == status) {
//            ret = SUCCESS;
//        } else {
//            ret = FAILED;
//        }
//        //DB_Save(0);
//    }
////    DBManager::getInstance()->flushRecordList();//写回 记录文件
//    RecordManager::getInstance()->flushRecordList();//写回 记录文件
//    printf("delete uuid : <0x%08x>, <0x%08x> result %d nHead 0x%2x.\r\n", uu_id.tUID.H, uu_id.tUID.L, ret, nHead);
//    if (nHead == HEAD_MARK_MQTT) {
//        cmdCommRsp2MqttByHead(HEAD_MARK_MQTT, CMD_FACE_DELETE_USER, ret);
//    } else {
//        cmdCommRsp2MCU(CMD_FACE_DELETE_USER, ret);
//    }

    return 0;
}


//主控接收指令:  请求恢复出厂设置请求
int cmdReqResumeFactoryProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("请求恢复出厂设置请求 \r\n");

//    uint8_t ret = SUCCESS;
//
//    CloseLcdBackground();
//    //返回响应
//    cmdReqResumeFactoryRsp(ret);
//
//    vTaskDelay(pdMS_TO_TICKS(10));
//
//
//
//    //消息处理
//    SetSysToFactory();
//    /*关机 */
//
//    cmdCloseFaceBoardReqExt(false);
    return 0;
}

//主控接收指令: 手机APP请求注册激活请求
int cmdReqActiveByPhoneProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
//    uint8_t ret = SUCCESS;

//    if (REG_STATUS_OK == g_reging_flg) {
//        //返回响应
//        cmdReqActiveByPhoneRsp(ret);
//        vTaskDelay(pdMS_TO_TICKS(20));
//
//        /*注册激活后关机*/
//        cmdCloseFaceBoardReq();
//    } else {//借助APP激活按钮无注册时触发，显示IR图像
//        vizn_api_status_t status;
//        status = VIZN_SetDispMode(NULL, DISPLAY_MODE_IR);
//        if (kStatus_API_Layer_Success != status) {
//            ret = FAILED;
//        }
//    }

    return 0;
}


//串口接收消息处理
// 主控接收指令: WIFI SSID 设置请求
int cmdWifiSSIDProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("接收指令: WIFI SSID 设置 \r\n");

//    uint8_t ret = FAILED;
//    char wifi_ssid[WIFI_SSID_LEN_MAX + 1] = {0};
//
//    CloseLcdBackground();
//    // 解析指令
//    if ((nMessageLen < sizeof(wifi_ssid)) && nMessageLen < WIFI_SSID_LEN_MAX) {
//        memset(wifi_ssid, 0, sizeof(wifi_ssid));
//        memcpy(wifi_ssid, pszMessage, nMessageLen);
//
//        // 保存设置到系统配置文件
//        printf("wifi ssid : <%s>.\r\n", wifi_ssid);
//        update_wifi_ssid(wifi_ssid);
//        update_need_reconnect("true");
//        //read_config("./config.ini");
//
//        ret = SUCCESS;
//    }
//
//    cmdCommRsp2MCU(CMD_WIFI_SSID, ret);
    return 0;
}

// 主控接收指令: WIFI 密码 设置请求
int cmdWifiPwdProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("接收指令: WIFI 密码 设置 \r\n");

//    uint8_t ret = FAILED;
//    char wifi_pwd[WIFI_PWD_LEN_MAX + 1] = {0};
//
//    CloseLcdBackground();
//    // 解析指令
//    if ((nMessageLen < sizeof(wifi_pwd)) && nMessageLen < WIFI_SSID_LEN_MAX) {
//        memset(wifi_pwd, 0, sizeof(wifi_pwd));
//        memcpy(wifi_pwd, pszMessage, nMessageLen);
//
//        // 保存设置到系统配置文件
//        printf("wifi pwd : <%s>.\r\n", wifi_pwd);
//        update_wifi_pwd(wifi_pwd);
//        update_need_reconnect("true");
//        //read_config("./config.ini");
//        ret = SUCCESS;
//    }
//
//    cmdCommRsp2MCU(CMD_WIFI_PWD, ret);
    return 0;
}

// 主控接收指令: 设置MQTT 参数
int cmdMqttParamSetProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("设置MQTT 参数  \r\n");
//    uint8_t ret = FAILED;
//    uint8_t mqtt_user[MQTT_USER_LEN + 1] = {0};
//    uint8_t mqtt_pwd[MQTT_PWD_LEN + 1] = {0};
//    char mqtt_user_tmp[32] = {0};
//    char mqtt_pwd_tmp[32] = {0};
//
//    // 解析指令
//    if (nMessageLen == MQTT_USER_LEN + MQTT_PWD_LEN) {
//        memset(mqtt_user, 0, sizeof(mqtt_user));
//        memcpy(mqtt_user, pszMessage, MQTT_USER_LEN);
//        memset(mqtt_pwd, 0, sizeof(mqtt_pwd));
//        memcpy(mqtt_pwd, pszMessage + MQTT_USER_LEN, MQTT_PWD_LEN);
//        printf("mqtt_user<0x%02x%02x%02x%02x%02x%02x>,mqtt_pwd<0x%02x%02x%02x%02x%02x%02x%02x%02x>!\r\n", \
//        mqtt_user[0], mqtt_user[1], mqtt_user[2], mqtt_user[3], mqtt_user[4], mqtt_user[5], \
//                mqtt_pwd[0], mqtt_pwd[1], mqtt_pwd[2], mqtt_pwd[3], mqtt_pwd[4], mqtt_pwd[5], mqtt_pwd[6], mqtt_pwd[7]);
//
//        // 保存设置到系统配置文件
//        memset(mqtt_pwd_tmp, 0, sizeof(mqtt_user_tmp));
//        HexToStr(mqtt_user_tmp, mqtt_user, MQTT_USER_LEN);
//        memset(mqtt_pwd_tmp, 0, sizeof(mqtt_pwd_tmp));
////        HexToStr(mqtt_pwd_tmp, mqtt_pwd, MQTT_PWD_LEN);
//        update_mqtt_opt(mqtt_user_tmp, (char *) mqtt_pwd);
//        //read_config("./config.ini");
//        printf("mqtt user :<%s>, mqtt_pwd:<%s>.\r\n", mqtt_user_tmp, mqtt_pwd_tmp);
//
//        //system("sync");
//        ret = SUCCESS;
//    }
//
//    // 重启MQTT 以新的参数启动
//    //system("killall mqtt");
//    //system("/opt/smartlocker/mqtt &");
//
//    cmdCommRsp2MCU(CMD_WIFI_MQTT, ret);
    return 0;
}


// 主控接收指令: wifi 网络时间同步响应
int cmdWifiTimeSyncRsp(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("网络时间同步响应 \r\n");
//    uint8_t ret = SUCCESS;
//
//    ret = StrGetUInt8(pszMessage);
//
//    {
//        cmdCommRsp2Mqtt(CMD_WIFI_TIME_SYNC, ret);
//    }

    return 0;
}

//MCU->106F->MQTT:  WIFI 同步订单时间响应
int cmdWifiOrderTimeSyncRsp(unsigned char nMessageLen, const unsigned char *pszMessage) {
    uint8_t ret = StrGetUInt8(pszMessage);
//    printf("远程同步订单请求  %d \r\n", (uint8_t) nMessageLen);
//
//    cmdCommRsp2Mqtt(CMD_ORDER_TIME_SYNC, ret);
    return 0;
}

// 主控接收指令:远程wifi开门响应
int cmdWifiOpenDoorRsp(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("远程开门回复 \r\n");

//    uint8_t ret = SUCCESS;
//
//    ret = StrGetUInt8(pszMessage);
//
//    if (1/*stSystemCfg.flag_wifi_enable*/) {
//        // 转发响应给 Mqtt 模块
//        cmdCommRsp2Mqtt(CMD_WIFI_OPEN_DOOR, ret);
//        //usleep(200);
//    }

    return 0;
}


// 主控接收指令: 蓝牙模块状态信息上报
int cmdBTInfoRptProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
    printf("主控接收指令: 蓝牙模块状态信息上报 \r\n");
//    uint8_t ret = FAILED;
//    uint8_t bt_ver = 0;
//    uint8_t bt_mac[6] = {0};
//    char bt_verbuf[4] = {0};
//    char bt_ver_tmp[32] = {0};
//    char bt_mac_tmp[16] = {0};
//
//    //解析指令
//    if (nMessageLen == 1 + 6) {
//        bt_ver = StrGetUInt8(pszMessage);
//        memset(bt_mac, 0, sizeof(bt_mac));
//        memcpy(bt_mac, pszMessage + 1, 6);
//
//        // 获取字符串格式的版本号
//        memset(bt_verbuf, 0, sizeof(bt_verbuf));
//        memset(bt_ver_tmp, 0, sizeof(bt_ver_tmp));
//        sprintf(bt_verbuf, "%03d", bt_ver);
//        sprintf(bt_ver_tmp, "W8_52832_V%c.%c.%c", bt_verbuf[0], bt_verbuf[1], bt_verbuf[2]);
//
//        // 转换为字符格式的BT mac
//        memset(bt_mac_tmp, 0, sizeof(bt_mac_tmp));
//        sprintf(bt_mac_tmp, "%02X%02X%02X%02X%02X%02X", \
//            bt_mac[0], bt_mac[1], bt_mac[2], bt_mac[3], bt_mac[4], bt_mac[5]);
//
//        // 保存设置到系统配置文件
//        update_bt_info(bt_ver_tmp, bt_mac_tmp);
//        //read_config("./config.ini");
//        printf("bt_ver :<%s>, bt_mac:<%s>.\r\n", bt_ver_tmp, bt_mac_tmp);
//
//        //system("sync");
//        ret = SUCCESS;
//    }

    //cmdCommRsp2MCU(CMD_BT_INFO, ret);
    return 0;
}

// 主控接收指令: 设置wifi的MQTT server 登录URL(可能是IP+PORT, 可能是域名+PORT)
int cmdMqttSvrURLProc(unsigned char nMessageLen, const unsigned char *pszMessage) {
//    uint8_t ret = FAILED;
//    char MqttSvr_Url[MQTT_SVR_URL_LEN_MAX + 1] = {0};
//
//    CloseLcdBackground();
//    //system("killall face_recgnize");
//    // 解析指令
//    if ((nMessageLen < sizeof(MqttSvr_Url)) && nMessageLen < MQTT_SVR_URL_LEN_MAX) {
//        memset(MqttSvr_Url, 0, sizeof(MqttSvr_Url));
//        memcpy(MqttSvr_Url, pszMessage, nMessageLen);
//
//        // 保存设置到系统配置文件
//        printf("MqttSvr_Url : <%s>.\r\n", MqttSvr_Url);
//        update_MqttSvr_opt(MqttSvr_Url);
//        //read_config("./config.ini");
//
//        ret = SUCCESS;
//    }
//    cmdCommRsp2MCU(CMD_WIFI_MQTTSER_URL, ret);
    return 0;
}

// 主控接收指令:机械开锁响应
int cmdMechicalLockRsp(unsigned char nMessageLen, const unsigned char *pszMessage) {
//    uint8_t ret = -1;
//    unsigned char *pop = NULL;
//    unsigned char szBuffer[32] = {0};
//
//    // MCU到face_loop，0代表开锁成功，1代表开锁失败
//    memcpy(szBuffer, pszMessage, nMessageLen);
//
//    pop = szBuffer;
//
//    // MCU到face_loop，0代表开锁成功，1代表开锁失败
//    ret = StrGetUInt8(pszMessage);
//    // 如果开锁成功 更新数据库状态 ,请求mqtt上传本次操作记录。
//    if (ret == 0) {
//        //					xshx add
//        //char power_msg[32] = {0};
//        pop += 1;
//        uint8_t power = StrGetUInt8(pop);
//        pop += 1;
//        uint8_t power2 = StrGetUInt8(pop);
//
//        Record *record = (Record *) pvPortMalloc(sizeof(Record));
//        HexToStr(username, g_uu_id.UID, sizeof(g_uu_id.UID));
//        strcpy(record->UUID, username);
//        record->action = MECH_UNLOCK;//  操作类型：0代表注册 1: 一键开锁 2：钥匙开锁  3 人脸识别开锁  机械开锁
//        char image_path[16];
//        //record->status = 0; // 0,操作成功 1,操作失败.
//        record->time_stamp = ws_systime; //时间戳 从1970年开始的秒数
////        record->power = power * 256 + power2;
//        record->data[0]=power;
//        record->data[1]=power2;
//        //sprintf(power_msg, "{\\\"batteryA\\\":%d\\,\\\"batteryB\\\":%d}", record->power, record->power2);
//        //printf("power_msg is %s \r\n", power_msg);
//
//        record->upload = BOTH_UNUPLOAD; //   0代表没上传 1代表记录上传图片未上传 2代表均已
////        record->action_upload = 0xB00;
//        memset(image_path, 0, sizeof(image_path)); // 对注册成功的用户保存一张压缩过的jpeg图片
//        //snprintf(image_path, sizeof(image_path), "REC_%d_%d_%s.jpg", 0, record->time_stamp, record->UUID);
//        //snprintf(image_path, sizeof(image_path), "C%d", record->time_stamp);
//        //memcpy(record->image_path, image_path, sizeof(image_path));//image_path
//
////        DBManager::getInstance()->addRecord(record);
//        RecordManager::getInstance()->addRecord(record);
//
//        //Oasis_SetOasisFileName(record->image_path);
//        //Oasis_WriteJpeg();
//
////        int ID = DBManager::getInstance()->getLastRecordID();
//        int ID = RecordManager::getInstance()->getLastRecordID();
//        printf("开锁成功, 更新数据库状态.请求MQTT上传本次开门的记录 \r\n");
//        cmdRequestMqttUpload(ID);
//    } else {
////    	g_command_executed = 1;
//        cmdCloseFaceBoardReq();
//        printf("开锁失败,不更新数据库状态. 不上传记录\r\n");
//    }

    return 0;
}