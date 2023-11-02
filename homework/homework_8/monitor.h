#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <syscall.h>
#include <stdbool.h>
#include <sys/time.h>


typedef struct {
    int a, b;
    bool status;
} Pair;

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t can_reduce;

    int remind_task;//记录还有多少个任务

    int array_point_front;
    int array_point_last;
    int current_num;//记录当前队列中还有多少个数，是否够相加

    void (*getTask)(Pair*);
    void (*putResult)(int);
    void (*array_print)(void);
} Monitor;

Monitor task_monitor;
void Monitor_init();