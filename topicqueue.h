/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 topicqueue header file

This is my own work.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sched.h>
#include <string.h>
#include "thread_safe_bounded_queue.h"

#ifndef TQ_H
#define TQ_H

#define QUACKSIZE 256
#define CAPTIONSIZE 256
#define MAXTOPICS 10

/* struct for topic queue */
typedef struct tq topicQ;

/* sturct for entry */
typedef struct entry {
    int entrynum;
    struct timeval timestamp;
    int pubID;
    char photoURL[QUACKSIZE];
    char photoCaption[CAPTIONSIZE];
} tq_entry;

/* Create topic queue */
topicQ *tq_create(long long id, int size);

/* enqueue to topic queue */
long long tq_enqueue(topicQ* q, void* param);

/* dequeue from topic queue */
int tq_dequeue(topicQ *q);

/* Get Entry function */
int tq_getentry(topicQ *queue, long long id, void** entry);

/* 
 * Below Functions are only for testing purposes. 
 * Threads are not using these functions.
 */

/* Full Queue Fucntion */
int is_full(topicQ *queue);

/* Empty Queue Function */
int is_empty(topicQ *queue);

/* Fill Up Queue */
long long fill_up(topicQ *queue);

/* Print all elem in the Queue */
void print(topicQ *queue);

/* Empty the FULL QUEUE*/
void empty_queue(topicQ *queue);

/* Free all elems and queue */
void free_queue(topicQ *queue);

/* Print certain entry */
void print_entry(tq_entry* entry);

/* 
 * End Testing Functions 
 */


#endif