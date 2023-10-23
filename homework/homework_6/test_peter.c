#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 10000000
#define SIZE 200
int turn = 0;
int flag[2];
int Index = 0;
int data[MAX];
pthread_mutex_t mutex;
//thread1 

void* thread_fun_1(){
    int write_num = 0;
    int count;
    while(write_num < MAX) {
        flag[0] = 1;
        turn = 1;
        while(flag[1] && turn == 1)
            ;

        //printf("thread1 Index:%d\n",Index);
        //critical section
        for(count=0;count<SIZE;count++){
            data[Index] = write_num; //even
            Index++;
            write_num+=2;
        }
        //critical section

        flag[0] = 0;
    }

}
//thread2 
void* thread_fun_2(){
    int write_num = 0;
    int count;
    while(write_num < MAX){
        flag[1] = 1;
        turn = 0;

        while(flag[0] && turn == 0)
            ;

        //printf("thread2 Index:%d\n",Index);
        //critical section
        for(count=0;count<SIZE;count++){
            data[Index] = write_num+1; //odd 
            Index++;
            write_num+=2;
        }
        //critical section

        flag[1] = 0;
    }
}
int main()
{
    int result;
    int max = 0;
    int i;
    pthread_t pthread_1;
    pthread_t pthread_2;

    struct timespec time1;
    struct timespec time2;

    clock_gettime(CLOCK_REALTIME, &time1);
    pthread_create(&pthread_1,NULL,thread_fun_1,NULL);
    pthread_create(&pthread_2,NULL,thread_fun_2,NULL);

    pthread_join(pthread_1,NULL);
    pthread_join(pthread_2,NULL);
    clock_gettime(CLOCK_REALTIME, &time2);

    printf("Index is %d\n",Index);
    printf("write time = %ld ns\n", time2.tv_nsec - time1.tv_nsec);
    for(i = 1;i < MAX;i++){
        result = data[i] - data[i-1];
        if(result < 0)
            result = 0 - result;
        if(max < result)
            max = result;
    }

    printf("the maximum sub is %d\n",max);
}
