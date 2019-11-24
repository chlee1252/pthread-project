/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 thread function for part4 or above

This is my own work.
 */

#ifndef THFXNP4_C
#define THFXNP4_C

#include "threadfxn_p4.h"

struct tq {
    pthread_mutex_t lock;
    int id;
    int lines;
    char* title;
    TSBoundedQueue *queue;
};

struct clean_arg {
    topicQ **tq;
    int tq_num;
    int delta;
};

void init() {
    pthread_mutex_init(&plock, NULL);
    pthread_mutex_init(&slock, NULL);
}

void query_topic(topicQ** tq, int num) {
    int i;

    if (num == 0) {
        fprintf(stderr, "No items in topics queue!\n");
        return;
    }
    for (i = 0; i < num; i++) {
        printf("topic %d %d\n", tq[i]->id, tq[i]->lines);
    }
}

void query_pub(thread_info *pub, int pub_num) {
    int i;

    if (pub_num == 0) {
        fprintf(stderr, "No threads in Publisher thread pool!\n");
        return;
    }
    printf("\n"); //Just for formatting
    for (i = 0; i < pub_num; i++) {
        printf("publisher thread %d %s\n", pub[i].thread_num, pub[i].str);
    }
}

void query_sub(thread_info *sub, int sub_num) {
    int i;
    
    if (sub_num == 0) {
        fprintf(stderr, "No threads in Subscriber thread pool!\n");
        return;
    }
    printf("\n");        // Just for formatting
    for (i = 0; i < sub_num; i++) {
        printf("subscriber thread %d %s\n", sub[i].thread_num, sub[i].str);
    }
}

/* Use this function only for error cases */
void freeup(char** w, topicQ **tq, thread_info *pub_info, thread_info *sub_info, int tn, int tp, int pub, int sub) {
    printf("Freeing all data and canceling threads if there is thread waiting...\n");
    int i;
    for (i = 0; i < tn; i++) {
        free(w[i]);
    }
    printf("Successfully freed word_list\n");
    for (i = 0; i < tp; i++) {
        free(tq[i]->title);
        free_queue(tq[i]);
    }
    printf("Successfully freed all the topic queues\n");
    for (i = 0; i < pub; i++) {
        free(pub_info[i].str);
        pthread_cancel(pub_info[i].thread_id);
        pthread_join(pub_info[i].thread_id, NULL);
    }
    printf("Successfully canceled and freed publisher threads\n");
    for (i = 0; i < sub; i++) {
        free(sub_info[i].str);
        pthread_cancel(sub_info[i].thread_id);
        pthread_join(sub_info[i].thread_id, NULL);
    }
    printf("Successfully canceled and freed subscriber threads\n");
}

int timepassed(struct timeval now, struct timeval then) {
    return (((now.tv_sec - then.tv_sec) * 1000000) + (now.tv_usec - then.tv_usec)) / 1000;
}

void* publisher(void* param) {
    while(!flag) {
        sleep(1);
    }
    thread_info* arg = (thread_info *) param;
    FILE *fp;
    fp = fopen((char *) arg->str, "r");
    if (fp == NULL) {
        fprintf(stderr, "File has no contents to read!\n");
        exit(EXIT_FAILURE);
    }
    topicQ **queue = arg->tq;
    tq_entry* ent = (tq_entry *) malloc(sizeof(tq_entry));
    int sleep_time;
    int topic_id = 0;
    long long i = 0;
    long long enqueue_result;

	while(fscanf(fp, "%d\n%s\n%[^\n]\n%d\n", &ent->pubID, ent->photoURL, ent->photoCaption, &sleep_time) != EOF) {
        pthread_mutex_lock(&plock);
        i++;
        int try = 0;
        topic_id = ent->pubID-1;
        tq_entry* copy = (tq_entry *) malloc(sizeof(tq_entry));
        memcpy(copy, ent, sizeof(tq_entry));
        if (topic_id >= arg->topicCount) {
            fprintf(stderr, "ID ERROR: %d\n", topic_id);
            break;
        }
        while (TS_BB_IsFull(queue[topic_id]->queue)) {
            printf("BB is FULL!... retry %d out of %d\n", try, MAXTRY);
            sleep(sleep_time);
            if (try == MAXTRY) break;
            try++;
        }
        if (try == MAXTRY) {
            pthread_mutex_unlock(&plock);
            break;
        }
        enqueue_result = tq_enqueue(queue[topic_id], (void *) copy);
        if (enqueue_result == -1) {
            fprintf(stderr, "Enqueue Failed! head: %llu tail: %llu\n", TS_BB_GetFront(queue[topic_id]->queue), TS_BB_GetBack(queue[topic_id]->queue));
            exit(EXIT_FAILURE);
        }
        pthread_mutex_unlock(&plock);
        sleep(sleep_time);
	}
    free(ent);
    free(arg->str);
    fclose(fp);
    return NULL;
}

void* subscriber(void* param) {
    while(!flag) {
        sleep(1);
    }

    thread_info *args = (thread_info *) param;
    FILE *fp;
    fp = fopen((char *) args->str, "r");
    topicQ **queue = args->tq;
    int sleeptime, id = 0, try=0;
    long long i;

    while (fscanf(fp, "%d\n%d\n", &id, &sleeptime) != EOF) {
        pthread_mutex_lock(&slock);
        i = (long long )args->lastentry_read;
        id = id-1;
        while (TS_BB_IsEmpty(queue[id]->queue)) {
            printf("BB is Empty!... retry %d out of %d\n", try, MAXTRY);
            args->lastentry_read = (int) i;
            sleep(sleeptime);
            if (try == MAXTRY) break;
            try++;
        }
        if (try == MAXTRY) {
            pthread_mutex_unlock(&slock);
            break;
        }
        tq_entry* item;

        while ((args->lastentry_read = tq_getentry(queue[id], i, (void **) &item)) == -1) {
            fprintf(stderr, "Get Item Failed!...retry %d out of %d\n", try, MAXTRY);
            sleep(sleeptime);
            if (try == MAXTRY) break;
            try++;
        }
        if (try == MAXTRY) {
            pthread_mutex_unlock(&slock);
            break;
        }

        tq_entry* entry = (tq_entry *) item;
    
        printf("%d\n%s\n%s\n", entry->pubID, entry->photoURL, entry->photoCaption);
        args->lastentry_read++;
        pthread_mutex_unlock(&slock);
        sleep(sleeptime);
    }
    free(args->str);
    fclose(fp);
    return NULL;
}

void* cleanup(void* param) {
    printf("Dequeue Thread Created!\n");
    int i, j;
    struct clean_arg* args;
    args = (struct clean_arg*) param;
    topicQ** q = args->tq;
    int num = args->tq_num;
    int delta = args->delta;
    while(!done) {
        sleep(4);                       /* give publisher and subscriber a time to do their job */
        for (i = 0; i < num; i++) {
            struct timeval time;
            gettimeofday(&time, NULL);
            for (j = 0; j < MAXTOPICS; j++) {
                tq_entry* item;
                if (tq_getentry(q[i], j, (void **) &item) != -1) {
                    if (timepassed(time, item->timestamp) > delta) {
                        tq_dequeue(q[i]);
                    }
                }
            }
        }
    }
    printf("publisher, subscriber done\n");
    for (i = 0; i < num; i++) {
        free(q[i]->title);
        free_queue(q[i]);
    }
    return NULL;
}

#endif