#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <syscall.h>
#include <stdbool.h>
#include <sys/time.h>
#include "monitor.h"

// int array_point_last;
// int array_point_front;


void * func_thread_reduce(void * arg) {
    Pair task;
    while (1) {
        task_monitor.getTask(&task);
        if (!task.status) {
            printf("A thread finished!\n");
            return NULL;
        }
        sleep((rand()%10)/1000); //random time delay: 1~10ms
        int res = task.a + task.b;
        task_monitor.putResult(res);
    }
    return NULL;
}

int main() {
    Monitor_init();
    printf("The initial 16 integers are:\n");
    task_monitor.array_print();
    printf("Processing...\n");

    pthread_t thread[8];
    for (int i = 0; i < 8; i++) {
        pthread_create(thread + i, NULL, func_thread_reduce, NULL);
    }
    for (int i = 0; i < 8; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("\nPass\nThe final result is: ");
    task_monitor.array_print();

    return 0;
}