#define __USE_GNU  
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#define MAXNUM 1000000
int main(){
    long long int sum=0;
    int array[MAXNUM];
    struct timeval START,END;
    for(int i=0;i<MAXNUM;i++){array[i]=i+1;}
    gettimeofday(&START,NULL);
    for(int j=0; j<MAXNUM; sum+=array[j++]);
    gettimeofday(&END,NULL);
    printf("sum=%lld\n",sum);
    printf("the eplased time is :%ld\n",(END.tv_usec-START.tv_usec));
    return 0;
}