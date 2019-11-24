/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 threadfxn for part2/3

This is my own work.
 */

#ifndef TFXN_C
#define TFXN_C

#include "threadfxn.h"

void init() {
    pthread_mutex_init(&plock, NULL);
    pthread_mutex_init(&slock, NULL);
}

void* publisher(void* param) {
    /* wait for main thread's flag change. BUSY WAITING */
    while (!flag) {
        sleep(1);
    }

    pthread_mutex_lock(&plock);
    thread_info* t_info = (thread_info *) param;
    int num = t_info->thread_num;
    printf("publisher thread %d reading from %s\n", num, t_info->str);
    pthread_mutex_unlock(&plock);

    free(t_info->str);
    return NULL;
}

void* subscriber(void* param) {
    /* wait for main thread's flag change. BUSY WAITING */
    while(!flag) {
        sleep(1);
    }
    pthread_mutex_lock(&slock);
    thread_info* t_info = (thread_info *) param;
    int num = t_info->thread_num;

    printf("subscriber thread %d reading from %s\n", num, t_info->str);
    free(t_info->str);
    pthread_mutex_unlock(&slock);

    return NULL;
}

void* cleanup(void* param) {
    pthread_t tid = pthread_self();
    printf("dequeue thread created!\n");
    while (!done) {
        sleep(1);
    }
    printf("Current Cleanup Thread ID: %d\n", (int) tid);
    return NULL;
}

#endif