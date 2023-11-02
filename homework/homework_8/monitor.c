#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <syscall.h>
#include <stdbool.h>
#include <sys/time.h>
#include "monitor.h"
#define INIT_NUM 16

int general_array[2*INIT_NUM];

void Monitor_getTask(Pair * result);
void Monitor_putResult(int res);
void array_print();
void Monitor_init();

void Monitor_getTask(Pair * result) {
    pthread_mutex_lock(&task_monitor.lock);
    
    while ((task_monitor.current_num < 2) && task_monitor.remind_task > 0) {
        pthread_cond_wait(&task_monitor.can_reduce, &task_monitor.lock);
    }//如果当前数组中的元素个数不大于2个，则等待

    if (task_monitor.remind_task <= 0)
        result->status = false;
    else
    {
        result->status = true;

        task_monitor.remind_task--;
        task_monitor.current_num -= 2;

        result->a = general_array[task_monitor.array_point_front++];
        result->b = general_array[task_monitor.array_point_front++];
        printf("get task: %d, %d\n", result->a, result->b);
    }

    pthread_mutex_unlock(&task_monitor.lock);
    return ;
}

void Monitor_putResult(int res) {
    pthread_mutex_lock(&task_monitor.lock);

    general_array[task_monitor.array_point_last++] = res;
    task_monitor.current_num++;
    printf("put result: %d\n", res);
    printf("The queue now is ");
    task_monitor.array_print();

    pthread_cond_broadcast(&task_monitor.can_reduce);//tell other threads

    pthread_mutex_unlock(&task_monitor.lock);
}

void array_print() {
    int i;
    for (i = task_monitor.array_point_front; i < task_monitor.array_point_last; i++) {
        printf("%d ", general_array[i]);
    }
    printf("\n");
}

void Monitor_init() {
    task_monitor.array_point_front = 0;
    task_monitor.array_point_last = 0;
    while(task_monitor.array_point_last < INIT_NUM)
        general_array[task_monitor.array_point_last++] = rand()%100;

    task_monitor.remind_task = 15;
    task_monitor.current_num = task_monitor.array_point_last - task_monitor.array_point_front;

    task_monitor.getTask = Monitor_getTask;
    task_monitor.putResult = Monitor_putResult;
    task_monitor.array_print = array_print;
    pthread_mutex_init(&task_monitor.lock, NULL);
}