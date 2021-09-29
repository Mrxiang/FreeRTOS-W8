//
// Created by xshx on 2021/9/22.
//
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define START_TASK_PRIO 1 //任务优先级
#define START_STK_SIZE 128 //任务堆栈大小
TaskHandle_t StartTask_Handler; //任务句柄
void start_task(void *pvParameters); //任务函数

#define TASK1_TASK_PRIO 2 //任务优先级
#define TASK1_STK_SIZE 128 //任务堆栈大小
TaskHandle_t Task1Task_Handler; //任务句柄
void task1_task(void *pvParameters); //任务函数

#define TASK2_TASK_PRIO 3 //任务优先级
#define TASK2_STK_SIZE 128 //任务堆栈大小
TaskHandle_t Task2Task_Handler; //任务句柄
void task2_task(void *pvParameters); //任务函数


/* 开始任务任务函数 */
void start_task(void *pvParameters) {
    taskENTER_CRITICAL(); //进入临界区
    //创建 TASK1 任务
    xTaskCreate((TaskFunction_t) task1_task,
                (const char *) "task1_task",
                (uint16_t) TASK1_STK_SIZE,
                (void *) NULL,
                (UBaseType_t) TASK1_TASK_PRIO,
                (TaskHandle_t * ) & Task1Task_Handler);
    //创建 TASK2 任务
    xTaskCreate((TaskFunction_t) task2_task,
                (const char *) "task2_task",
                (uint16_t) TASK2_STK_SIZE,
                (void *) NULL,
                (UBaseType_t) TASK2_TASK_PRIO,
                (TaskHandle_t * ) & Task2Task_Handler);
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL(); //退出临界区
}

//task1 任务函数
void task1_task(void *pvParameters) {
    uint8_t task1_num = 0;
    while (1) {
        task1_num++; //任务1执行次数加1
//        LED0 = !LED0;
        printf("任务1已经执行： %d 次\r\n", task1_num);
        if (task1_num == 5) {
//            vTaskDelete(Task2Task_Handler);//任务 1 执行 5 次删除任务 2 (4)
            vTaskList()
//            printf("任务 1 删除了任务 2!\r\n");
        }
        vTaskDelay(1000); //延时 1s，也就是 1000 个时钟节拍
    }
}

//task2 任务函数
void task2_task(void *pvParameters) {
    uint8_t task2_num = 0;
    while (1) {
        task2_num++; //任务2执行次数加1
//        LED1 = !LED1;
        printf("任务 2 已经执行： %d 次\r\n", task2_num);
        vTaskDelay(1000); //延时 1s，也就是 1000 个时钟节拍
    }
}


int main(void) {
//    ......
    //创建开始任务
    xTaskCreate((TaskFunction_t) start_task,    //任务函数
                (const char *) "start_task",    //任务名称
                (uint16_t) START_STK_SIZE,        //任务堆栈大小
                (void *) NULL,                    //传递给任务函数的参数
                (UBaseType_t) START_TASK_PRIO,    //任务优先级
                (TaskHandle_t * ) & StartTask_Handler); //任务句柄
    vTaskStartScheduler();                        //开启任务调度
}
/********************************************************/
/* This is a stub function for FreeRTOS_Kernel */
void vMainQueueSendPassed( void )
{
    return;
}

/* This is a stub function for FreeRTOS_Kernel */
void vApplicationIdleHook( void )
{
    return;
}