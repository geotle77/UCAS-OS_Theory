#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int max_test_times = 1000; 
int main() {
   // 定义运行测试的次数
    long int elapsed_time_glibc = 0, elapsed_time_syscall = 0, elapsed_time_asm = 0;
    struct timespec start, end;
    pid_t pid_glibc,pid_syscall,pid_asm;
    for (int i = 0; i < max_test_times; i++) {
        // 使用glibc库函数调用getpid
        clock_gettime(CLOCK_MONOTONIC, &start);
         pid_glibc = getpid();
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_time_glibc += (end.tv_nsec - start.tv_nsec) ;

        // 使用syscall函数直接调用getpid
        clock_gettime(CLOCK_MONOTONIC, &start);
         pid_syscall = syscall(SYS_getpid);
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_time_syscall +=(end.tv_nsec - start.tv_nsec);

        // 使用内联汇编调用getpid
        clock_gettime(CLOCK_MONOTONIC, &start);
        asm volatile (
            "syscall"
            : "=a" (pid_asm)
            : "0" (SYS_getpid)
        );
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_time_asm += (end.tv_nsec - start.tv_nsec)+(end.tv_sec-start.tv_sec)*1e9;
    }

    long int average_time_glibc = elapsed_time_glibc;
    long int average_time_syscall = elapsed_time_syscall ;
    long int average_time_asm = elapsed_time_asm ;

    printf("Average time taken by glibc: %ld ns\n", average_time_glibc);
    printf("Average time taken by syscall: %ld ns\n", average_time_syscall);
    printf("Average time taken by asm: %ld ns\n", average_time_asm);

    return 0;
}
