//
// Created by xshx on 2021/9/29.
//

#include "power.h"


static const char *logtag ="[POWER]-";

static void PowerMainTask(void *pvParamters){
    printf("%s创建POWER Main TasK \n", logtag);

    int count=0;
    for (;  ;) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("\r\n%s %d RUNNING...\r\n", logtag, count);
        count++;
    }
}
int power_task_start(void){


    printf("%s 创建 POWER monitor task \n", logtag);
    if (xTaskCreate(PowerMainTask, "Power Main Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        printf("POWER monitor task error \n");

    }

}
