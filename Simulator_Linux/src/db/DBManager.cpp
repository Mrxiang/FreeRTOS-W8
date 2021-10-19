//
// Created by xshx on 2021/4/8.
//

#include <cstring>
#include "DBManager.h"
#include "FreeRTOS.h"
#include "semphr.h"
//#include "../cjson/cJSON_Utils.h"
#include "../cjson/cJSON.h"
//#include "../fatfs/fatfs_op.h"

DBManager* DBManager::m_instance = NULL;
list<Record*> DBManager::recordList ;

typedef enum {
    RECORD_MGMT_OK      = 0x00,
	RECORD_MGMT_FAILED  = -0x01,
	RECORD_MGMT_NODB    = -0x02,
	RECORD_MGMT_ENOLOCK = -0x03,
	RECORD_MGMT_ERETRY  = -0x04,
	RECORD_MGMT_NOSPACE = -0x05,

} RECORD_LOCK_STATUS;

static SemaphoreHandle_t s_Record_Lock = NULL;
static int Record_Lock()
{
    if (NULL == s_Record_Lock)
    {
        return RECORD_MGMT_ENOLOCK;
    }

    if (pdTRUE != xSemaphoreTake(s_Record_Lock, portMAX_DELAY))
    {
        return RECORD_MGMT_ERETRY;
    }

    return RECORD_MGMT_OK;
}

static void Record_UnLock()
{
    xSemaphoreGive(s_Record_Lock);
}

DBManager::DBManager() {
    // Create a database lock semaphore
    if (NULL == s_Record_Lock)
    {
    	s_Record_Lock = xSemaphoreCreateMutex();
        if (NULL == s_Record_Lock)
        {
            printf("[ERROR]:Create DB lock semaphore\r\n");
        }
    }

    initDB();
}

DBManager* DBManager::getInstance()
{
    if(NULL == m_instance)
    {
        m_instance = new DBManager();
    }
    return m_instance;
}

bool DBManager::saveRecordToFile(list<Record*> recordList,char *filePath){

    printf("----- 保存记录文件-----\r\n");

    printf("-----0.写回文件-----\r\n");


    printf("-----1.将链表数据组织成json格式-----\r\n");
    list <Record*>::iterator it;
    char 	*cjson_str = NULL;
    cJSON 	*jsonroot = NULL;
    //新建一个JSON项目：jsonroot
    jsonroot = cJSON_CreateObject();
    if(NULL == jsonroot)
    {
        return false;
    }
    //4.1 先生成每个对象
    Record_Lock();

    cJSON * ObjArr = cJSON_CreateArray();
    for ( it = recordList.begin( ); it != recordList.end( ); it++ ){
        Record  *tmpRecord = (Record*)*it;
        //保存未上传的记录
//        if( (tmpRecord->action_upload & 0xFF) != 2 ){
        if( tmpRecord->upload != BOTH_UPLOAD ){
            cJSON * cObj = cJSON_CreateObject();
            cJSON_AddNumberToObject(cObj, DB_KEY_ID, tmpRecord->ID);
            cJSON_AddStringToObject(cObj, DB_KEY_UUID, tmpRecord->UUID);
            cJSON_AddNumberToObject(cObj, DB_KEY_ACTION, tmpRecord->action);
            cJSON_AddNumberToObject(cObj, DB_KEY_TIME_STAMP, tmpRecord->time_stamp);
            //cJSON_AddNumberToObject(cObj, DB_KEY_STATUS, tmpRecord->status);
            cJSON_AddStringToObject(cObj, DB_KEY_IMAGE_PATH, tmpRecord->image_path);
//            cJSON_AddNumberToObject(cObj, DB_KEY_POWER,  tmpRecord->power);
            cJSON_AddStringToObject(cObj, DB_KEY_DATA, tmpRecord->data);// 蓝牙测温
            //cJSON_AddNumberToObject(cObj, DB_KEY_POWER1,  tmpRecord->power);
            //cJSON_AddNumberToObject(cObj, DB_KEY_POWER2, tmpRecord->power2);
            cJSON_AddNumberToObject(cObj, DB_KEY_UPLOAD, tmpRecord->upload);
//            cJSON_AddNumberToObject(cObj, DB_KEY_ACTION_UPLOAD, tmpRecord->action_upload);
            //4.2 把对象添加到数组
            cJSON_AddItemToArray(ObjArr, cObj);
        }
    }
    Record_UnLock();
    //4.3 ObjArr加入到jsonroot
    cJSON_AddItemToObject(jsonroot, DB_KEY_INFO, ObjArr);
    //cjson_str = cJSON_Print(jsonroot);
    cjson_str = cJSON_PrintUnformatted(jsonroot);
    if(cjson_str!=NULL){
		printf("-----2.将json格式数据存文件----%d 条记录--\r\n", cJSON_GetArraySize(ObjArr));

		printf("%d , max size %d \r\n", strlen(cjson_str), MAX_BYTE);
//		fatfs_write( filePath, cjson_str, 0, strlen(cjson_str));
        FILE *file = fopen( filePath , "w" );
        fwrite( cjson_str, strlen(cjson_str), 1, file);
		vPortFree(cjson_str);
    }
    cJSON_Delete(jsonroot);
    printf("----- 保存记录文件结束-----\r\n");
    return true;
}
list<Record*> DBManager::readRecordFromFile(char *filePath){
    printf("-----1.从文件中读出json格式的记录-----\r\n");
    int ArrLen = 0;
    int fsize=0;

//    fsize = fatfs_getsize(filePath);
    FILE *file = fopen( filePath, "r");
    fseek(file, 0, SEEK_END);
    fsize = ftell( file );
    if (fsize == -1) {
        return recordList;
    } else {
        memset(buf, 0, fsize);
        fseek(file, 0, SEEK_SET);
        int ret = fread(buf, 1, fsize, file);
        //printf("status %d file size %d ,%s \r\n", status, fsize, buf);
        printf("ret %d file size %d\r\n", ret, fsize);

        if (ret == 0)
        {
			cJSON *jsonroot = cJSON_Parse(buf);
			//4. 解析数组成员是json对象的数组ObjArr
			printf("------2.将json 格式数据组织成链表---------\r\n");
			cJSON *ObjArr = cJSON_GetObjectItem(jsonroot, DB_KEY_INFO);
			if (cJSON_IsArray(ObjArr)) {
				ArrLen = cJSON_GetArraySize(ObjArr);
				printf("ObjArr Len: %d\r\n", ArrLen);
				for (int i = 0; i < ArrLen; i++) {
					cJSON *SubObj = cJSON_GetArrayItem(ObjArr, i);
					if (NULL == SubObj) {
						continue;
					}

					Record *record = (Record *) pvPortMalloc(sizeof(Record));
					record->ID = i;
					strcpy(record->UUID, cJSON_GetObjectItem(SubObj, DB_KEY_UUID)->valuestring);
					record->action = cJSON_GetObjectItem(SubObj, DB_KEY_ACTION)->valueint;
					record->time_stamp = cJSON_GetObjectItem(SubObj, DB_KEY_TIME_STAMP)->valuedouble;
					//record->status = cJSON_GetObjectItem(SubObj, DB_KEY_STATUS)->valueint;
					strcpy(record->image_path , cJSON_GetObjectItem(SubObj, DB_KEY_IMAGE_PATH)->valuestring);
//					record->power = cJSON_GetObjectItem(SubObj, DB_KEY_POWER)->valueint;
					strcpy(record->data,  cJSON_GetObjectItem(SubObj, DB_KEY_DATA)->valuestring);//蓝牙测温
					//record->power1 = cJSON_GetObjectItem(SubObj, DB_KEY_POWER1)->valueint;
					//record->power2 = cJSON_GetObjectItem(SubObj, DB_KEY_POWER2)->valueint;
					record->upload = cJSON_GetObjectItem(SubObj, DB_KEY_UPLOAD)->valueint;
//					record->action_upload = cJSON_GetObjectItem(SubObj, DB_KEY_ACTION_UPLOAD)->valueint;
					
				    Record_Lock();
					recordList.push_back(record);
    				Record_UnLock();
					//printf("i: [%d] id:%d, uuid:%s, action %d, time_stamp:%d, upload:%d\r\n", i, record->ID, record->UUID, record->action, record->time_stamp, record->upload);
					//printf("i: [%d] id:%d, uuid:%s, time_stamp:%d, action_upload:%d\r\n", i, record->ID, record->UUID, record->time_stamp, record->action_upload);
					//printf("i: [%d] uuid:%s, time_stamp:%d, action_upload:%d\r\n", i, record->UUID, record->time_stamp, record->action_upload);
					//printf("[%d] %s, %d, %d\r\n", i, record->UUID, record->time_stamp, record->action_upload);
					//printf("[%d] %d, %d\r\n", i, record->time_stamp, record->action_upload);
				}
			}

			cJSON_Delete(jsonroot);
        }else {
        	 printf("readRecordFromFile ret = %d \r\n", ret);
        }
    }

    printf("%s end\r\n", __FUNCTION__);
    return  recordList;
}
// 初始化DB
void DBManager::initDB()
{

    printf("------初始化------ \r\n");
    readRecordFromFile(RECORD_PATH);
}

list<Record*> DBManager::getRecord() {
	return recordList;
}

int DBManager::addRecord(Record *record){

	int id = getLastRecordID()+1;
    int count = recordList.size();
    printf("增加操作记录 %d  最大记录限制到 %d\r\n", id, max_size);
    if( count >= MAX_COLUMN ){
        record->ID=id;

	    Record_Lock();

		Record* firstOne = recordList.front();
		recordList.pop_front();
		remove(firstOne->image_path);
		vPortFree(firstOne);
        recordList.push_back( record );
    	Record_UnLock();
    } else{
        record->ID= id;
	    Record_Lock();
        recordList.push_back( record);
	    Record_UnLock();
    }

    int result= recordList.size();
    return  result;
}


int  DBManager::getAllUnuploadRecordCount()
{
    printf("获取操作成功但未上传记录总数 \r\n");
    if( recordList.empty()== true ){
        getAllUnuploadRecord();
    }
    int nrow = recordList.size();
    return  nrow;
}

int  DBManager::getRecordCount()
{
    return  recordList.size();
}

list<Record*>  DBManager::getAllUnuploadRecord()
{
    Record_Lock();

    printf("获取全部开门成功，但未上传的识别记录 \r\n");
    list <Record*>::iterator it;
    for ( it = recordList.begin( ); it != recordList.end( ); ) {
        Record *tmpRecord = (Record *) *it;
//        if( (tmpRecord->action_upload & 0xFF) == 2 ){
          if( tmpRecord->upload  == BOTH_UPLOAD ){
        	vPortFree(*it);
            it = recordList.erase(it);
        }else{
            it++;
        }
    }
    Record_UnLock();
    return  recordList;
}

Record*  DBManager::getLastRecord(   )
{
    Record_Lock();

    printf("获取最后一条记录 \r\n");
    Record *record= NULL;
    if( recordList.empty() != true ){
        record = recordList.back();

        //printf("%s \n", record->UUID);
        //printf("%ld\n", record->time_stamp);
        printf("%s %d\r\n", record->UUID, record->time_stamp);

    }
    Record_UnLock();

    return  record;
}

int DBManager::getLastRecordID(){
        printf("获取最后一条记录\r\n");
        Record_Lock();

        int ID=0;
        Record *record =NULL;
        if( recordList.empty() != true ){
            record = recordList.back();
            ID = record->ID;
        }
        Record_UnLock();
        return ID;
}




int DBManager::getRecordByID( int id, Record *record )
{
    printf("查找ID为 %d 的记录 \r\n", id);
    Record_Lock();

    int ret=-1;
    list <Record*>::iterator it;
    for ( it = recordList.begin( ); it != recordList.end( ); it++ ) {
        Record *tmpRecord = (Record *) *it;
        if( tmpRecord->ID == id ){
            memcpy( record, tmpRecord, sizeof(Record));
            ret = 0;
        }
    }
    Record_UnLock();
    return  ret;
}
bool  DBManager::updateRecordByID(Record *record)
{
    printf("更新ID匹配的识别记录\r\n");
    Record_Lock();

    list <Record*>::iterator it;
    for ( it = recordList.begin( ); it != recordList.end( ); it++ ) {
        Record *tmpRecord = (Record *) *it;
        if( tmpRecord->ID == record->ID ){
            memcpy( tmpRecord, record, sizeof(Record));
        }
    }
//    saveRecordToFile(recordList, RECORD_PATH);
    Record_UnLock();
    return  true;
}
bool  DBManager::updateLastRecordStatus(int status, long currTime)
{
    printf("更新最后一条记录\r\n");
    return  true;
}

int DBManager::clearRecord()
{
    printf("清空操作记录 \r\n");
    Record_Lock();

    int ret = 0;
    list <Record*>::iterator it;
    printf("clear records before vPortFree\r\n");

    for ( it = recordList.begin( ); it != recordList.end( );it++ ) {
    	vPortFree(*it);
    }
    printf("clear records after vPortFree\r\n");

    recordList.clear();
    Record_UnLock();
    printf("clear records before remove\r\n");

    ret = remove(RECORD_PATH);
    printf("clear records after remove %d\r\n", ret);

    return  ret;
}
bool DBManager::deleteRecordByUUID(char *UUID )
{
    printf("删除用户UUID %s 对应的操作记录 \r\n", UUID);
    Record_Lock();

    bool flag=false;
    list <Record*>::iterator it;
    for ( it = recordList.begin( ); it != recordList.end( ); ) {
        printf("%p\n", *it);
        Record *record = (Record *) *it;
        printf("%s \n", record->UUID);
        int ret= strcmp(record->UUID, UUID );
        if( ret == 0 ){
        	vPortFree(*it);
            it=recordList.erase(it);
            flag = true;
        }else{
            ++it;
        }
    }
    Record_UnLock();
//    saveRecordToFile( recordList, RECORD_PATH);
    return  flag;
}
void DBManager::flushRecordList(){
        bool flag=false;
        flag = saveRecordToFile( recordList, RECORD_PATH);
        if( flag ){
            printf("保存文件成功 \r\n");
        }else{
            printf("保存文件失败 \r\n");
        }
}
