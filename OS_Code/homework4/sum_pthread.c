#define __USE_GNU  
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#include  <stdio.h>

#define ARRAY_SIZE 1000000
#define THREADS_NUM 4 
int cur=0;
int array[ARRAY_SIZE];
long long int P_sum=0;

void *sum(void* arg){
    while(cur<ARRAY_SIZE)    
        P_sum+=array[cur++];
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[THREADS_NUM];
    int state[THREADS_NUM];
    struct timeval START,END;
    long elapse_time;
    int i;
    for(i=0; i<ARRAY_SIZE; array[i++]=i+1);
     gettimeofday(&START, NULL);
    for ( i = 0; i < THREADS_NUM; i++) {
        pthread_create(&threads[i], NULL, &sum, &i);
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