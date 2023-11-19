#define __USE_GNU  
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#include  <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXNUM 10000000
#define MAXTHREAD 2
int data[MAXNUM];
int INDEX = 0;
pthread_mutex_t mutex ; //互斥锁
struct timespec delay ={0,1};
int start[MAXTHREAD]={0,0},end[MAXTHREAD]={400,400};
void* work(void* args){
    int thread_id = *(int*)args;
    while(INDEX<MAXNUM){
            pthread_mutex_lock(&mutex);  //加锁
            //---------------------------------临界区----------------------------------
       
                    if(INDEX<MAXNUM){
                    for(int i=start[thread_id];i<end[thread_id];i+=2) {
                        data[INDEX] = i + thread_id; //even ( i+1 for thread 2) 
                        INDEX++;
                    }
                    start[thread_id] = end[thread_id];
                    end[thread_id] =  start[thread_id]+400;
                    }
                
            //---------------------------------临界区----------------------------------
            pthread_mutex_unlock(&mutex); //解锁
        // }
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t t1,t2;
    struct timeval START, END;
    int delta=-1;
    int thread_id[] = {0, 1};
    long long int elapsed_time;
    int max_diff = INT_MIN;
    pthread_mutex_init(&mutex, NULL);
    gettimeofday(&START, NULL);
    pthread_create(&t1, NULL, work, &thread_id[0]);
    pthread_create(&t2, NULL, work, &thread_id[1]);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    gettimeofday(&END, NULL);
    elapsed_time = (END.tv_sec - START.tv_sec) * 1000000 + (END.tv_usec - START.tv_usec);
    printf("The elapsed time is %lld us in total\n", elapsed_time);
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
