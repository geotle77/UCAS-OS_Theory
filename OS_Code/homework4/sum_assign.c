#define __USE_GNU  
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#include  <stdio.h>
#define MAXNUM 1000000
#define THREAD_NUM 2
#define STRIDE  MAXNUM/THREAD_NUM
int array[MAXNUM];
int cur;

//线程执行的函数
void *assignment(void *arg){
    int low, max;
    low = *(int*)arg;
    max = low + STRIDE;
    while(low<max){
        array[low++]= low;
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[THREAD_NUM];
    struct timeval START, END;
    int i;
    long elapse_time;
    int state[THREAD_NUM];
    long long int sum=0;
    int low[THREAD_NUM];
    // 确定每个线程赋值的起始位置
    for(i=0;i<THREAD_NUM;i++)
        low[i]= i*STRIDE;
    // 创建N个线程，并对数组赋值
    gettimeofday(&START, NULL);
    for(i=0; i<THREAD_NUM; i++){
        pthread_create(&threads[i], NULL, &assignment, &low[i]);
    }
    //等待所有线程结束
    for(i=0; i<THREAD_NUM; i++)
        state[i]=pthread_join(threads[i], NULL);
    gettimeofday(&END, NULL);
    for(i=0; i<THREAD_NUM; i++){
        if(state[i]){   // 若失败返回值为0
            printf("Fail to wait thread %d to finish.\n", i);
            return -1;
        }
    }
    for(i=0;i<MAXNUM;i++)
        sum+=array[i];
    elapse_time = (END.tv_sec - START.tv_sec) * 1000000 + (END.tv_usec - START.tv_usec);
    // 打印当前线程数、计算结果、总用时
    printf("sum=%lld, elapse_time=%ld\n", sum, elapse_time);
}