#define __USE_GNU  
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#include  <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXNUM 10000
#define MAXTHREAD 2
#define TRUE  1
#define FALSE 0
int INDEX = 0;
int data[MAXNUM];
int flag[MAXTHREAD];
int turn;
int start[MAXTHREAD]={0,0};
int end[MAXTHREAD]={400,400};
//peterson pseudocode
/*
flag[i] = True;
turn = j;
while(flag[j] && turn == j);
critical section;
flag[i] = False;
remainder section;
*/
void *work(void *args){
    int thread_id = *(int *)args;
    int other_thread = 1 - thread_id;
    while (INDEX < MAXNUM) {
        flag[thread_id] = 1;
        turn = other_thread; // 转让控制权
        while (flag[other_thread] && turn == other_thread) {
            //  忙等阶段
        }
 //---------------------------------临界区----------------------------------
            if (INDEX < MAXNUM ) { 
                int i;
                for (i = start[thread_id]; i < end[thread_id]; i+=2) {
                        data[INDEX] = i + thread_id;
                        INDEX ++;
                }
                start[thread_id] = end[thread_id];
                end[thread_id] =  start[thread_id]+400;
            } 
            else {
                pthread_exit(NULL);
            }
        
 //---------------------------------临界区----------------------------------
        flag[thread_id] = 0;
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t t1,t2;
    struct timeval START,END;
    int max_diff = INT_MIN;
    long long int elapsed_time;
    int id[] = {0, 1};
    gettimeofday(&START, NULL);
    pthread_create(&t1, NULL, work, &id[0]);
    pthread_create(&t2, NULL, work, &id[1]);
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