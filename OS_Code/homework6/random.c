#define __USE_GNU
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 5
#define MAX_RANDOM_VALUE 20

int data[ARRAY_SIZE];
pthread_mutex_t mutex;
pthread_cond_t cond_writer, cond_reader;
int written = 0;

void *write_to_array(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        while (written) {
            pthread_cond_wait(&cond_writer, &mutex);
        }
        for (int j = 0; j < ARRAY_SIZE; j++) {
            data[j] = rand() % MAX_RANDOM_VALUE + 1;
        }
        written = 1;
        pthread_cond_signal(&cond_reader);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *read_and_sum_array(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        while (!written) {
            pthread_cond_wait(&cond_reader, &mutex);
        }
        int sum = 0;
        for (int j = 0; j < ARRAY_SIZE; j++) {
            sum += data[j];
        }
        printf("Sum: %d\n", sum);
        written = 0;
        pthread_cond_signal(&cond_writer);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t writer, reader;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_writer, NULL);
    pthread_cond_init(&cond_reader, NULL);

    pthread_create(&writer, NULL, write_to_array, NULL);
    pthread_create(&reader, NULL, read_and_sum_array, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    pthread_cond_destroy(&cond_writer);
    pthread_cond_destroy(&cond_reader);
    pthread_mutex_destroy(&mutex);

    return 0;
}
