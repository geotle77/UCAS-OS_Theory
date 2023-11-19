#include <stdio.h>
#define __USE_GNU  
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>


#define ARRAY_SIZE 1000000
#define THREADS_NUM 5 
int array[ARRAY_SIZE];
int cur = 0;
long long int P_sum=0;
void *sum(void* arg){
    int core_id = *(int*)arg;
    cpu_set_t cpuset;   //CPU核的位图
    CPU_ZERO(&cpuset);  //将位图清零
    CPU_SET(core_id, &cpuset);  //设置位图第N位为1，表示与core N绑定。N从0开始计数
    sched_setaffinity(0, sizeof(cpuset), &cpuset);  //将当前线程和cpuset位图中指定的核绑定运行
    printf("bound to core %d\n", core_id);
    while(cur<ARRAY_SIZE)    
        P_sum+=array[cur++];
    
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[THREADS_NUM];
    int state[THREADS_NUM];
    struct timeval START,END;
    int core_id[]={0, 1, 2, 3, 4};
    long elapse_time;
    int i;
    for(i=0; i<ARRAY_SIZE; array[i++]=i+1);
     gettimeofday(&START, NULL);
    for ( i = 0; i < THREADS_NUM; i++) {
        pthread_create(&threads[i], NULL, &sum,&core_id[i]);
    }
    for(i=0; i<THREADS_NUM; i++)
        state[i]=pthread_join(threads[i], NULL);
    gettimeofday(&END, NULL);
    for(i=0; i<THREADS_NUM; i++){
        if(state[i]){
            printf("Fail to wait thread %d to finish.\n", i);
            return -1;
        }}
    elapse_time = (END.tv_sec - START.tv_sec) * 1000000 + (END.tv_usec - START.tv_usec);
    printf("sum=%lld, elapse_time=%ld\n", P_sum, elapse_time);
    return 0;
}