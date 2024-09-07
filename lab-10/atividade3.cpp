#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_cond_t cond;
int stage = 0;

void* thread1(void* arg) {
    pthread_mutex_lock(&lock);
    printf("Thread 1: olá, tudo bem?\n");
    stage++;
    if (stage == 2) {
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* thread2(void* arg) {
    pthread_mutex_lock(&lock);
    printf("Thread 2: hello!\n");
    stage++;
    if (stage == 2) {
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* thread3(void* arg) {
    pthread_mutex_lock(&lock);
    while (stage < 2) {
        pthread_cond_wait(&cond, &lock);
    }
    printf("Thread 3: até mais tarde.\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* thread4(void* arg) {
    pthread_mutex_lock(&lock);
    while (stage < 2) {
        pthread_cond_wait(&cond, &lock);
    }
    printf("Thread 4: tchau!\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t1, t2, t3, t4;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_create(&t3, NULL, thread3, NULL);
    pthread_create(&t4, NULL, thread4, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}