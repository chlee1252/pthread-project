/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 threadfxn_p4 header file

This is my own work
 */

#ifndef PART4_H
#define PART4_H

#include "topicqueue.h"
#include "p1fxns.h"

#define NUMPROXIES 3
#define MAX 256
#define MAXTRY 6

int flag, done;              /* flag variable to wait threads after create || done for cleanup thread */
pthread_mutex_t plock;       /* lock for the publisher thread */
pthread_mutex_t slock;       /* lock for the subscriber thread */

typedef struct info {
    pthread_t thread_id;
    int       thread_num;         /* thread number */
    char      *str;               /* thread name   */
    topicQ    **tq;               /* Topic queue   */
    int       topicCount;         /* topic count to avoid range error */
    int       lastentry_read;     /* position that thread read last time */
} thread_info;

/* Helper function initializing locks */
void init();

/* Helper function for query command "query topic" */
void query_topic(topicQ** tq, int num);

/* Helper function for query command "query publisher" */
void query_pub(thread_info* pub, int pub_num);

/* Helper function for query command "query subscriber" */
void query_sub(thread_info* sub, int sub_num);

/* Helper function for free up allocated memeory when error occured */
void freeup(char** w, topicQ **tq, thread_info *pub_info, thread_info *sub_info, int tn, int tp, int pub, int sub);

/* publisher thread */
void* publisher(void* param);

/* subscriber thread */
void* subscriber(void* param);

/* cleanup thread */
void* cleanup(void* param);

/* time difference function */
int timepassed(struct timeval then, struct timeval now);

#endif