#include  <stdio.h>  
#define __USE_GNU 
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#define THREADS_NUM 5
#define ARRAY_SIZE 1000000

int array[ARRAY_SIZE];
long long int result_from_pthread[THREADS_NUM]={0};

void *sum(void* arg){
    int core_id = *(int*)arg;
    cpu_set_t cpuset;   //CPU核的位图
    CPU_ZERO(&cpuset);  //将位图清零
    CPU_SET(core_id, &cpuset);  //设置位图第N位为1，表示与core N绑定。N从0开始计数
    sched_setaffinity(0, sizeof(cpuset), &cpuset);  //将当前线程和cpuset位图中指定的核绑定运行
    int cur=*(int *)arg ;
    int i,low,max;
    low=cur*ARRAY_SIZE/THREADS_NUM;
    max=(cur+1)*ARRAY_SIZE/THREADS_NUM;
    for(i=low ; i<max;result_from_pthread[cur]+=array[i++]-1);
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[THREADS_NUM];
    int state[THREADS_NUM];
    struct timeval START,END;
    long elapse_time;
    long long int P_sum=0;
    int cpu_id[THREADS_NUM];
    int i;
    for(i=0; i<ARRAY_SIZE; array[i++]=i+1);
    for(i=0; i<THREADS_NUM; cpu_id[i++]=i-1);
    gettimeofday(&START, NULL);
    for ( i = 0; i < THREADS_NUM; i++) {
        pthread_create(&threads[i], NULL, &sum, &cpu_id[i]);
    }
    for(i=0; i<THREADS_NUM; i++)
        state[i]=pthread_join(threads[i],NULL);
    gettimeofday(&END, NULL);
    for(i=0; i<THREADS_NUM; i++){
        if(state[i]){
            printf("Fail to wait thread %d to finish.\n", i);
            return -1;
        }}
    for(i=0; i<THREADS_NUM; i++){
        P_sum+=result_from_pthread[i];
    }
    elapse_time = (END.tv_sec - START.tv_sec) * 1000000 + (END.tv_usec - START.tv_usec);
    printf("sum=%lld, elapse_time=%ld\n", P_sum, elapse_time);
    return 0;
}
