#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    static int array[10]={0};
    int sum=0;
    int status=0;
    int i=0;
    for(i=0;i<10;array[i++]=i){
    }
    int pid=fork();
    if(pid==0){
        i=0;
        while(i<10)
            sum+=array[i++];
        printf("I'm the son process,and the sum=%d\n",sum);
        execl("/bin/ls", "ls", "-l", "/home/solomon/CODES/C", NULL);
        exit(1);
    }
    else
    {   wait(&status);
        printf("parent process finishes\n");}
    return 0;
}