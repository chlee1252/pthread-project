/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 topicqueue

This is my own work except the basic idea is from the lab
 */

#ifndef TQ_C
#define TQ_C
#include "topicqueue.h"

/* Global Variable */
int ent_num = 0;

struct tq {
    pthread_mutex_t lock;
    int id;
    int lines;
    char* title;
    TSBoundedQueue *queue;
};

topicQ *tq_create(long long id, int size) {
    topicQ *topic_queue = NULL;
    topic_queue = (topicQ *) malloc (sizeof(topicQ));

    if (topic_queue == NULL) {
        fprintf(stderr, "Topic Queue Malloc Failed!\n");
        return NULL;
    }
    /* Try Malloc topicqueue by calling malloc function in TS_BB */
    topic_queue->queue = TS_BB_MallocBoundedQueue(size);
    if (topic_queue->queue == NULL) {
        free(topic_queue);
        fprintf(stderr, "TS_BB_MallocBoundedQueue() Failed!\n");
        return NULL;
    }
    /* Set id for topic queue */
    topic_queue->id = id;
    topic_queue->lines = 0;
    topic_queue->title = NULL;
    pthread_mutex_init(&(topic_queue->lock), NULL);

    return topic_queue;

}

long long tq_enqueue(topicQ *q, void* param) {
    pthread_mutex_lock(&(q->lock));
    tq_entry *item = (tq_entry *) param;
    long long returnValue = -1;
    item->entrynum = ent_num++;

    /* Get Time stamp */
    struct timeval time;
    gettimeofday(&time, NULL);
    item->timestamp = time;
    /* Done Getting Time stamp */

    /* Enqueue Using TS_BB_TryEnqueue() */
    returnValue = TS_BB_TryEnqueue(q->queue, (void*) item);
    pthread_mutex_unlock(&(q->lock));
    return returnValue;
}

int tq_dequeue(topicQ *q) {
    pthread_mutex_lock(&(q->lock));
    int returnValue = -1;
    /* Dequeue Using TS_BB_TryDequeue() */
    long long id = TS_BB_GetBack(q->queue);
    returnValue = TS_BB_TryDequeue(q->queue, id);
    if (!returnValue) {
        // printf("head: %llu tail: %llu id: %llu\n", TS_BB_GetFront(q->queue), TS_BB_GetBack(q->queue), id);
        returnValue = -1;
    }
    pthread_mutex_unlock(&(q->lock));
    return returnValue;
}

int tq_getentry(topicQ *queue, long long id, void** entry) {
    pthread_mutex_lock(&(queue->lock));
    int returnValue = -1;
    long long tail = TS_BB_GetBack(queue->queue);
    if (TS_BB_IsIdValid(queue->queue, id)) {
        *entry =  TS_BB_GetItem(queue->queue, id);
        returnValue = (int) id;
    }
    else if (id < tail) {
        *entry =  TS_BB_GetItem(queue->queue, tail);
        returnValue = (int) tail;
    }
    else {
        // printf("id: %llu head: %llu, tail: %llu\n", id, TS_BB_GetFront(queue->queue), tail);
        returnValue = -1;   
    }
    pthread_mutex_unlock(&(queue->lock));
    return returnValue;
}

/* Below functions are for testing purpose! Thread not using below functions! */

int is_full(topicQ* queue) {
    return TS_BB_IsFull(queue->queue);
}

int is_empty(topicQ* queue) {
    return TS_BB_IsEmpty(queue->queue);
}

long long fill_up(topicQ* queue) {
    int i;
    for (i = 0; i < MAXTOPICS; i++) {
        tq_entry* item;
        item = (tq_entry *) malloc(sizeof(tq_entry));
        strcpy(item->photoURL, "ABC");
        strcpy(item->photoCaption, "XYZ");
        tq_enqueue(queue, (void *) item);
    }
    return (long long) ent_num-2;
}

void print(topicQ* queue) {
    long long i;
    long long tail = TS_BB_GetBack(queue->queue);
    printf("%llu ent_num: %d\n", tail, ent_num);
    for (i = tail; i < ent_num-1; i++) {
        printf("ID: %llu\n", i);
        void* entry = TS_BB_GetItem(queue->queue, i);
        if (entry == NULL) {
            printf("Trouble to get Entry! %llu\n", i);
            break;
        }
        tq_entry* item = (tq_entry *) entry;
        printf("%llu entry: \n", i);
        printf("\tID: \t\t%d\n", item->entrynum);
        printf("\tURL: \t\t%s\n", item->photoURL);
        printf("\tCaption: \t%s\n", item->photoCaption);
        printf("\tTimeStamp: \t%llu\n", (long long) item->timestamp.tv_sec);
    }
}

void empty_queue(topicQ* queue) {
    int i;
    for (i = 0; i < MAXTOPICS; i++) {
        tq_dequeue(queue);
        
    }
}

void print_entry(tq_entry* entry) {
    //tq_entry* item = (tq_entry *) entry;
    printf("\tID: \t\t%d\n", entry->entrynum);
    printf("\tURL: \t\t%s\n", entry->photoURL);
    printf("\tCaption: \t%s\n", entry->photoCaption);
    printf("\tTimeStamp: \t%llu\n", (long long) entry->timestamp.tv_sec);
}

void free_queue(topicQ* queue) {
    pthread_mutex_destroy(&(queue->lock));
    TS_BB_FreeBoundedQueue(queue->queue);
    free(queue);
}

/* end Testing Functions */

#endif
