#define __USE_GNU
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_TELLERS 3 // 柜员数量
sem_t customer; // 顾客信号量
sem_t teller;   // 柜员信号量
int number = 0; // 号码
pthread_mutex_t lock; // 定义互斥锁
void* Customer_Service(void* data) {
    while (1) {
        sem_wait(&teller); // 等待柜员
        usleep(1000000); // 延时1秒
        pthread_mutex_lock(&lock); // 获取互斥锁
        number++; // 顾客取号
        printf("Customer took number %d\n", number);
        pthread_mutex_unlock(&lock); // 释放互斥锁
        sem_post(&customer); // 增加一个顾客信号量
    }
    return NULL;
}

void* Teller_Service(void* data) {
    int teller_number = *(int*)data; // 获取柜员编号
    while (1) {
        sem_wait(&customer); // 等待顾客
        usleep(1000000); // 延时1秒
        pthread_mutex_lock(&lock); // 获取互斥锁
        printf("Teller %d is serving customer %d\n", teller_number, number);
        number--; // 服务完毕，减少一个号码
        pthread_mutex_unlock(&lock); // 释放互斥锁
        sem_post(&teller); // 增加一个柜员信号量
    }
    return NULL;
}
int main() {
    pthread_t customers;
    pthread_t tellers[NUM_TELLERS];
    int teller_numbers[NUM_TELLERS]; // 存储柜员编号的数组


    sem_init(&customer, 0, 0);
    sem_init(&teller, 0, NUM_TELLERS); // 初始时所有的柜员都是空闲的
    pthread_mutex_init(&lock, NULL);

    pthread_create(&customers, NULL, Customer_Service, NULL);
    // 创建柜员线程
    for (int i = 0; i < NUM_TELLERS; i++) {
        teller_numbers[i] = i + 1; // 初始化柜员编号
        pthread_create(&tellers[i], NULL, Teller_Service, &teller_numbers[i]); // 创建柜员线程，传递柜员编号
    }

    pthread_join(customers, NULL);
    for (int i = 0; i < NUM_TELLERS; i++) {
        pthread_join(tellers[i], NULL);
    }

    return 0;
}
