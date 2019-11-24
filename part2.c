/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 part2

This is my own work except the making multiple threads is from
pthread_create man page
 */

#ifndef PART2_C
#define PART2_C

#include "threadfxn.h"

int main(int argc, char* argv[]) {
    /* Pub and sub Thread Pool */
    thread_info *pub_info, *sub_info;
    pthread_t clean;
    int tnum, i = 0;
    char* string[NUMPROXIES + NUMPROXIES] = {"ABC", "DEF", "GHI", "JKL", "MNO", "PQR"};

    pub_info = (thread_info *) calloc(NUMPROXIES, sizeof(thread_info));
    sub_info = (thread_info *) calloc(NUMPROXIES, sizeof(thread_info));
    if (pub_info == NULL) {
        fprintf(stderr, "Create Pub thread pool Failed\n");
        exit(EXIT_FAILURE);
    }
    if (sub_info == NULL) {
        fprintf(stderr, "Create Sub thread pool Failed\n");
        exit(EXIT_FAILURE);
    }

    /* Create Publisher thread pool */
    for (tnum = 0; tnum < NUMPROXIES; tnum++) {
        pub_info[tnum].thread_num = tnum+1;
        pub_info[tnum].str = strdup(string[i]);
        i++;
        if (pthread_create(&pub_info[tnum].thread_id, NULL, &publisher, &pub_info[tnum]) != 0) {
            fprintf(stderr, "Create publisher thread Failed! index: %d\n", tnum);
            exit(EXIT_FAILURE);
        } 
    }

    /* Create Subscriber thread pool */
    for (tnum = 0; tnum < NUMPROXIES; tnum++) {
        sub_info[tnum].thread_num = tnum+1;
        sub_info[tnum].str = strdup(string[i]);
        i++;
        if (pthread_create(&sub_info[tnum].thread_id, NULL, &subscriber, &sub_info[tnum]) != 0) {
            fprintf(stderr, "Create subscriber thread Failed! index: %d\n", tnum);
            exit(EXIT_FAILURE);
        } 

    }

    if (pthread_create(&clean, NULL, &cleanup, NULL) != 0) {
        fprintf(stderr, "Create cleanup thread failed!\n");
        exit(EXIT_FAILURE);
    }
    init();

    printf("Start!\n");
    flag = 1;

    for (tnum = 0; tnum < NUMPROXIES; tnum++) {
        /* If thread is done, wait for other threads to terminate */
        if (pthread_join(pub_info[tnum].thread_id, NULL) != 0) {
            fprintf(stderr, "Join pub thread is failed! Thread pool index: %d\n", tnum);
            exit(EXIT_FAILURE);
        }
    }

    for (tnum = 0; tnum < NUMPROXIES; tnum++) {
        if (pthread_join(sub_info[tnum].thread_id, NULL) != 0) {
            fprintf(stderr, "Join sub thread is failed! Thread pool index: %d\n", tnum);
            exit(EXIT_FAILURE);
        }
    }

    done = 1;
    if (pthread_join(clean, NULL) != 0) {
        fprintf(stderr, "Join CleanUp Thread is failed!\n");
        exit(EXIT_FAILURE);
    }

    free(pub_info);
    free(sub_info);

    /* destroy mutex locks */
    pthread_mutex_destroy(&plock);
    pthread_mutex_destroy(&slock);
    
    return 0;
}

#endif