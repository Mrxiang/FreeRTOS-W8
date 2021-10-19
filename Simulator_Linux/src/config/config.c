#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"
#include "string.h"
//#include "log.h"

#include <fcntl.h>
#include <unistd.h>
#include "../cjson/cJSON.h"
#include "../middle/commen_middle.h"
#define CONFIG_USE_FATFS 1
//
#define CONFIG_FS_ADDR  (0xF20000U)


VERSIONCONFIG versionConfig;
BTWIFICONFIG btWifiConfig;
MQTTCONFIG mqttConfig;

static char buf[4096] = {0};
//char buf2[1024] = {0};
static bool config_json_changed = false;

int mysplit(char *src, char *first, char *last, char *separator) {
    if (src == NULL || strlen(src) <= 0 || separator == NULL || strlen(separator) <= 0) {
        return -1;
    }
    char *second=(char*)strstr(src, separator);
    int first_len = strlen(src);
    if (second != NULL) {
        first_len = strlen(src) - strlen(second);
        strncpy(first, src, first_len);
        first[first_len] = '\0';
        strncpy(last, "b", 1);
        last[1] = '\0';
        int last_len = strlen(second) - strlen(separator);
        if (last_len > 0) {
            strcpy(last, second + strlen(separator));
        } else {
            last[0] = '\0';
        }
        return 0;
    }
    return -1;
}

void init_config() {
    cJSON_Hooks hooks;

    /* Initialize cJSON library to use FreeRTOS heap memory management. */
    hooks.malloc_fn = malloc;
    hooks.free_fn   = free;
    cJSON_InitHooks(&hooks);

    //fatfs_mount_with_mkfs();

#if  CONFIG_USE_FATFS
    FILE *file = fopen(DEFAULT_CONFIG_FILE, "w+");
    int status = fread(buf, sizeof(buf), 1, sizeof(buf));
    fseek(file, 0, SEEK_END);
    int fsize = ftell( file );
    if (fsize == -1) {
        return ;
    } else {
        memset(buf, 0, fsize);
        fseek(file, 0, SEEK_SET);
        int ret = fread(buf, 1, fsize, file);
        printf("read fatfs, buf length is %d, size is %d, status is %d\r\n", strlen(buf), sizeof(buf), status);
    }
#else
    int status = SLN_Read_Flash_At_Address(CONFIG_FS_ADDR, buf, sizeof(buf));
    printf( "read flash , buf length is %d, size is %d , status id %d \r\n", strlen(buf), sizeof(buf), status );
#endif
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();
        if(root != NULL) {
            cJSON_AddItemToObject(root, CONFIG_KEY_SYS_VERSION, cJSON_CreateString((const char *)FIRMWARE_VERSION));
            cJSON_AddItemToObject(root, CONFIG_KEY_MCU_VERSION, cJSON_CreateString((const char *)""));
            cJSON_AddItemToObject(root, CONFIG_KEY_OASIS_VERSION, cJSON_CreateString((const char *)PROJECT_VERSION));

            cJSON_AddItemToObject(root, CONFIG_KEY_BT_VERSION, cJSON_CreateString((const char *)""));
            cJSON_AddItemToObject(root, CONFIG_KEY_BT_MAC, cJSON_CreateString((const char *)""));

            cJSON_AddItemToObject(root, CONFIG_KEY_WIFI_RESET, cJSON_CreateString((const char *)"true"));
            cJSON_AddItemToObject(root, CONFIG_KEY_WIFI_RECONNECT, cJSON_CreateString((const char *)"true"));
            cJSON_AddItemToObject(root, CONFIG_KEY_WIFI_MAC, cJSON_CreateString((const char *)""));
            cJSON_AddItemToObject(root, CONFIG_KEY_SSID, cJSON_CreateString((const char *)""));
            cJSON_AddItemToObject(root, CONFIG_KEY_WIFI_PWD, cJSON_CreateString((const char *)""));

            cJSON_AddItemToObject(root, CONFIG_KEY_MQTT_SERVER_IP, cJSON_CreateString((const char *)""));
            cJSON_AddItemToObject(root, CONFIG_KEY_MQTT_SERVER_PORT, cJSON_CreateString((const char *)""));
            cJSON_AddItemToObject(root, CONFIG_KEY_MQTT_SERVER_URL, cJSON_CreateString((const char *)""));

            cJSON_AddItemToObject(root, CONFIG_KEY_MQTT_USER, cJSON_CreateString((const char *)""));
            cJSON_AddItemToObject(root, CONFIG_KEY_MQTT_PASSWORD, cJSON_CreateString((const char *)""));

            char* tmp = NULL;

            tmp = cJSON_Print(root);
#if CONFIG_USE_FATFS
            cJSON *bufRoot = NULL;
            bufRoot = cJSON_Parse(buf);
            if( bufRoot ==NULL ){
				memset(buf, 0, sizeof(buf));
				memcpy(buf, tmp, strlen(tmp));
	            printf("%s tmp is %s\r\n", __FUNCTION__, tmp);

                FILE *file = fopen(DEFAULT_CONFIG_FILE,"w+");
                if( file != NULL ){
                    fwrite( buf, sizeof(buf), 1, file);
                }
//	            fatfs_write(DEFAULT_CONFIG_FILE, buf, 0, sizeof(buf));
            }

#else
            cJSON *bufRoot = NULL;
            bufRoot = cJSON_Parse(buf);
            if( bufRoot == NULL ) {
                memset(buf, 0, sizeof(buf));
                memcpy(buf, tmp, strlen(tmp));
                int status = SLN_Write_Sector(CONFIG_FS_ADDR, buf);
                if (status != 0) {
                    printf("write flash failed %d \r\n", status);
                }
            }
#endif
            printf("buf %s \r\n", buf);
            if(tmp != NULL) {
                vPortFree(tmp);
            }

            cJSON_Delete(root);
        }
    }

    read_config();

}

void check_config() {

	read_config();
}

int update_section_key(char *section, char *key) {
	printf("%s section:%s key:%s \r\n", __FUNCTION__, section, key);
	cJSON *root = NULL;
	//printf("buf is %s\n", buf);


	root = cJSON_Parse(buf);
	if(root != NULL) {
        cJSON *item = cJSON_GetObjectItem(root, section);
        if(item != NULL) {
            char *item_string;
            item_string = cJSON_GetStringValue(item);
            if( strcmp( item_string, key)==0 ){
                cJSON_Delete(root);
                return 0;
            }
            cJSON_ReplaceItemInObject(root, section, cJSON_CreateString((const char *)key));
            char *tmp = NULL;
            tmp = cJSON_Print(root);
            memset(buf, 0, sizeof(buf));
            memcpy(buf, tmp, strlen(tmp));

            if(tmp != NULL) {
                vPortFree(tmp);
            }

            cJSON_Delete(root);
            config_json_changed = true;

        }


	}else{
	    printf("ERROR  root  is NULL \r\n");
	}

	return 0;
}

int update_mac(char *mac) {
	//printf("%s\n", __FUNCTION__);
	
	//update_section_key(CONFIG_KEY_WIFI_MAC, mac);

	return 0;
}

int update_need_reset(char *need_reset) {
	//printf("%s\n", __FUNCTION__);
	
	update_section_key(CONFIG_KEY_WIFI_RESET, need_reset);

	return 0;
}

int update_need_reconnect(char *need_reconnect) {
	//printf("%s\n", __FUNCTION__);
	
	update_section_key(CONFIG_KEY_WIFI_RECONNECT, need_reconnect);

	return 0;
}


int update_wifi_ssid(char *ssid) {
	//printf("%s\n", __FUNCTION__);
	
	update_section_key(CONFIG_KEY_SSID, ssid);
    save_json_config_file();

	return 0;
}

int update_wifi_pwd(char *password) {
	//printf("%s\n", __FUNCTION__);
	
	update_section_key(CONFIG_KEY_WIFI_PWD, password);
    save_json_config_file();

	return 0;
}

int update_mqtt_opt(char *username, char *password)
{
    //printf("%s\n", __FUNCTION__);

    update_section_key(CONFIG_KEY_MQTT_USER, username);
    update_section_key(CONFIG_KEY_MQTT_PASSWORD, password);
    memcpy(mqttConfig.username, username, strlen(username));
    memcpy(mqttConfig.password, password, strlen(password));

    save_json_config_file();

    return 0;
}

int update_MqttSvr_opt(char *MqttSvrUrl)
{
    if (MqttSvrUrl == NULL || strchr(MqttSvrUrl, ':') == NULL) {
        printf("Failed to save MqttSvrUrl %s\n", MqttSvrUrl);
        return -1;
    }

    update_section_key(CONFIG_KEY_MQTT_SERVER_URL, MqttSvrUrl);

    mysplit(MqttSvrUrl, mqttConfig.server_ip, mqttConfig.server_port, ":");
    printf("%s server_ip is %s, server_port is %s\r\n", __FUNCTION__, mqttConfig.server_ip, mqttConfig.server_port);
    update_section_key(CONFIG_KEY_MQTT_SERVER_IP, mqttConfig.server_ip);
    update_section_key(CONFIG_KEY_MQTT_SERVER_PORT, mqttConfig.server_port);

    save_json_config_file();

    return 0;
}

int update_bt_info(char *version, char *mac)
{
	//printf("%s\n", __FUNCTION__);

	update_section_key(CONFIG_KEY_BT_VERSION, version);
	update_section_key(CONFIG_KEY_BT_MAC, mac);
    memcpy(mqttConfig.client_id, mac, strlen(mac));
    memcpy(btWifiConfig.bt_mac, mac, strlen(mac));
    //memcpy(mqttConfig.username, mac, strlen(mac));

	return 0;
}

int update_mcu_info(char *version)
{
	//printf("%s\n", __FUNCTION__);

	update_section_key(CONFIG_KEY_MCU_VERSION, version);

	return 0;
}

int update_sys_info(char *version)
{
	//printf("%s\n", __FUNCTION__);

	update_section_key(CONFIG_KEY_SYS_VERSION, version);

    return 0;
}


int update_project_info(char *version)
{
	//printf("%s\n", __FUNCTION__);

	update_section_key(CONFIG_KEY_OASIS_VERSION, version);

	return 0;
}

int read_config_value(char *dst, char *key) {
	cJSON *root = NULL;
    //printf("buf is %s\n", buf);
    root = cJSON_Parse(buf);

    if(root != NULL) {
        cJSON *item = cJSON_GetObjectItem(root, key);
        if(item != NULL) {
            char *item_string;
            item_string = cJSON_GetStringValue(item);

            //printf("key is %s, item_string is %s\r\n", key, item_string);
            if(item_string != NULL) {
                memcpy(dst, item_string, strlen(item_string));
            }
        }

        cJSON_Delete(root);
    }else{
        printf("Error root is NULL \r\n");
    }
    return 0;
}

int read_config() {
    printf("read_config \r\n");
    memset(&versionConfig, '\0', sizeof(VERSIONCONFIG));
    memset(&btWifiConfig, '\0', sizeof(BTWIFICONFIG));
    memset(&mqttConfig, '\0', sizeof(MQTTCONFIG));

    memset(buf, 0, sizeof(buf));
#if  CONFIG_USE_FATFS
    FILE *file = fopen(DEFAULT_CONFIG_FILE, "w+");
    int status = fread(buf, sizeof(buf), 1, sizeof(buf));
    fseek(file, 0, SEEK_END);
    int fsize = ftell( file );
    if (fsize == -1) {
        return -1;
    } else {
        memset(buf, 0, fsize);
        fseek(file, 0, SEEK_SET);
        int ret = fread(buf, 1, fsize, file);
        printf("read fatfs, buf length is %d, size is %d, status is %d\r\n", strlen(buf), sizeof(buf), status);
    }
//    if(fatfs_read(DEFAULT_CONFIG_FILE, buf, 0, sizeof(buf)) != 0) {
//        printf("%s read error\r\n", __FUNCTION__);
//        return -1;
//    }
#else
    int status = SLN_Read_Flash_At_Address(CONFIG_FS_ADDR, buf, sizeof(buf));
    if( status != 0 ){
        printf("%s read error\r\n", __FUNCTION__);
        return -1;
    }
#endif
    //printf("buf is %s\r\n", buf);
    printf("buf %s  buf length is %d, size is %d\r\n",buf,  strlen(buf), sizeof(buf));

    printf("========= reading config =========\r\n");
    read_config_value(versionConfig.sys_ver, CONFIG_KEY_SYS_VERSION);
    read_config_value(versionConfig.bt_ver, CONFIG_KEY_BT_VERSION);
    read_config_value(versionConfig.oasis_ver, CONFIG_KEY_OASIS_VERSION);
    read_config_value(versionConfig.mcu_ver, CONFIG_KEY_MCU_VERSION);

    read_config_value(btWifiConfig.need_reset, CONFIG_KEY_WIFI_RESET);
    read_config_value(btWifiConfig.need_reconnect, CONFIG_KEY_WIFI_RECONNECT);
    read_config_value(btWifiConfig.bt_mac, CONFIG_KEY_BT_MAC);
    read_config_value(btWifiConfig.wifi_mac, CONFIG_KEY_WIFI_MAC);
    read_config_value(btWifiConfig.ssid, CONFIG_KEY_SSID);
    read_config_value(btWifiConfig.password, CONFIG_KEY_WIFI_PWD);

    char server_url[CONFIG_MQTT_ITEM_LEN * 2 + 1];
    memset(server_url, '\0', sizeof(server_url));
    read_config_value(server_url, CONFIG_KEY_MQTT_SERVER_URL);
    mysplit(server_url, mqttConfig.server_ip, mqttConfig.server_port, ":");

    read_config_value(mqttConfig.client_id, CONFIG_KEY_BT_MAC);
    read_config_value(mqttConfig.username, CONFIG_KEY_MQTT_USER);
    read_config_value(mqttConfig.password, CONFIG_KEY_MQTT_PASSWORD);

    print_project_config();

    return 0;
}

void print_project_config(void) {
    printf("version config:\r\n");
    printf("sys_ver: %s\r\n", versionConfig.sys_ver);
    printf("bt_ver: %s\r\n", versionConfig.bt_ver);
    printf("oasis_ver: %s\r\n", versionConfig.oasis_ver);
    printf("mcu_ver: %s\r\n", versionConfig.mcu_ver);

    printf("bt wifi config:\r\n");
    printf("reset: %s\r\n", btWifiConfig.need_reset);
    printf("reconnect: %s\r\n", btWifiConfig.need_reconnect);
    printf("bt_mac: %s\r\n", btWifiConfig.bt_mac);
    printf("wifi_mac: %s\r\n", btWifiConfig.wifi_mac);
    printf("ssid: %s\r\n", btWifiConfig.ssid);
    printf("wifi password: %s\r\n", btWifiConfig.password);

    printf("mqtt config:\r\n");
    printf("server_ip: %s\r\n", mqttConfig.server_ip);
    printf("server_port: %s\r\n", mqttConfig.server_port);
    printf("client_id: %s\r\n", mqttConfig.client_id);
    printf("username: %s\r\n", mqttConfig.username);
    printf("mqtt password: %s\r\n", mqttConfig.password);
}

int save_json_config_file() {
    printf("保存 config 文件 %d \r\n", config_json_changed);
    if (config_json_changed) {
#if CONFIG_USE_FATFS
        FILE *file = fopen(DEFAULT_CONFIG_FILE,"w+");
        if( file != NULL ){
            fwrite( buf, sizeof(buf), 1, file);
        }
//        fatfs_write(DEFAULT_CONFIG_FILE, buf, 0, sizeof(buf));
        printf("write fatfs %s \r\n", buf);

#else
        int status = SLN_Write_Sector(CONFIG_FS_ADDR, buf);
        if (status != 0)
        {
            printf("write flash  %s failed %d \r\n", buf, status);
        }
#endif
        config_json_changed = false;

    }
    printf("保存config 文件结束 \r\n");
    return 0;
}


