#define __USE_GNU
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 8
#define NUM_INTS 16

/**
 * @brief 定义了一个监视器类型 monitor_t，包含了任务数组、任务索引、结果数组、结果索引、互斥锁和条件变量。
 * 
 */
typedef struct {
    int tasks[NUM_INTS];
    int task_index;
    int results[NUM_INTS];
    int result_index;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} monitor_t;

/**
 * @brief 初始化监视器
 * 
 * @param monitor 监视器指针
 */
void monitor_init(monitor_t* monitor) {
    for (int i = 0; i < NUM_INTS; i++) {
        monitor->tasks[i] = rand() % 100;
    }
    monitor->task_index = NUM_INTS;
    monitor->result_index = 0;
    pthread_mutex_init(&monitor->mutex, NULL);
    pthread_cond_init(&monitor->cond, NULL);
}

/**
 * @brief 销毁监视器
 * 
 * @param monitor 监视器指针
 */
void monitor_destroy(monitor_t* monitor) {
    pthread_mutex_destroy(&monitor->mutex);
    pthread_cond_destroy(&monitor->cond);
}

/**
 * 从监视器中获取任务
 * @param monitor 监视器
 * @param num1 存储第一个数字的指针
 * @param num2 存储第二个数字的指针
 * @param is_result 表明操作数是否为初值的指针
 */
void monitor_getTask(monitor_t* monitor, int* num1, int* num2, int* is_result) {
    pthread_mutex_lock(&monitor->mutex);
    while (monitor->task_index < 2 && monitor->result_index < 2) {
        if (monitor->task_index == 0 && monitor->result_index == 1) {
            *num1 = *num2 = 0;
            pthread_mutex_unlock(&monitor->mutex);
            return;
        }
        pthread_cond_wait(&monitor->cond, &monitor->mutex);
    }
    if (monitor->task_index >= 2) {
        *num1 = monitor->tasks[--monitor->task_index];
        *num2 = monitor->tasks[--monitor->task_index];
        *is_result = 0;
    } else if (monitor->result_index >= 2) {
        *num1 = monitor->results[--monitor->result_index];
        *num2 = monitor->results[--monitor->result_index];
        *is_result = 1;
    } else {
        *num1 = *num2 = 0;
    }
    pthread_mutex_unlock(&monitor->mutex);
}

/**
 * 将结果放入监视器的结果数组中，并唤醒所有等待的线程。
 * @param monitor 监视器指针
 * @param result 要放入结果数组的结果
 */
void monitor_putResult(monitor_t* monitor, int result) {
    pthread_mutex_lock(&monitor->mutex);
    monitor->results[monitor->result_index++] = result;
    pthread_cond_broadcast(&monitor->cond);
    pthread_mutex_unlock(&monitor->mutex);
}

/**
 * @brief 线程函数，用于执行加法任务并将结果放入共享缓冲区中
 * 
 * @param arg 监视器指针
 * @return void* 
 */
void* thread_func(void* arg) {
    monitor_t* monitor = (monitor_t*)arg;
    while (1) {
        int num1, num2, is_result;
        monitor_getTask(monitor, &num1, &num2, &is_result);
        if (num1 == 0 && num2 == 0) {
            break;
        }
        usleep((rand() % 10 + 1) * 1000); // 随机时间扰动
        int sum = num1 + num2;
        /**
         * pthread_self is  using the pthread_self function to get the thread ID of the current thread.
         */
        printf("Thread %ld added %d and %d to get %d. The numbers were %s.\n", (long)pthread_self(), num1, num2, sum, is_result ? "results from previous additions" : "original numbers");
        
        monitor_putResult(monitor, sum);
    }
    return NULL;
}

/**
 * @file guancheng.c
 * @brief 使用监视器模拟多线程计算
 * 
 * 该程序使用监视器模拟多线程计算，其中包括线程创建、同步等操作。
 * 
 */
int main() {
    srand(time(NULL));
    pthread_t threads[NUM_THREADS];
    monitor_t monitor;

    monitor_init(&monitor);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, &monitor);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final result: %d\n", monitor.results[0]);

    monitor_destroy(&monitor);

    return 0;
}