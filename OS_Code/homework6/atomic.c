#define __USE_GNU
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXNUM 10000000

int data[MAXNUM];
int INDEX = 0;

void* write_number(void *arg)
{
    int num = *(int*)arg;
    for(int i=num; i<MAXNUM; i+=2) {
        int idx = __sync_fetch_and_add(&INDEX, 1);
        data[idx] = i;
    }
    return NULL;
}

int main(void)
{
    pthread_t thread1, thread2;
    int max_diff = INT_MIN;
    struct timeval START, END;
    long long int elapsed_time;
    int even = 0;
    int odd = 1;

    gettimeofday(&START, NULL);
    pthread_create(&thread1, NULL, write_number, &even);
    pthread_create(&thread2, NULL, write_number, &odd);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    gettimeofday(&END, NULL);
    elapsed_time = (END.tv_sec - START.tv_sec) * 1000000 + (END.tv_usec - START.tv_usec);
    printf("The elapsed time is %lld us in total\n", elapsed_time);
     // 计算数组相邻两个数的最大绝对差值
     for (int i = 0; i < MAXNUM-1; i++) {
     int diff1 = abs(data[i + 1] - data[i]);
     int diff2 = abs(data[i] - data[i + 1]);
     int diff = diff1 > diff2 ? diff1 : diff2;
     if (diff > max_diff) {
         max_diff = diff;
     }
    }
    printf("The max_diff is: %d\n", max_diff);
    printf("\n");

    return 0;
}
