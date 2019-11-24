/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 part3

This is my own work.
 */

#ifndef PART3_C
#define PART3_C

#include "threadfxn.h"
#include "topicqueue.h"
#include "p1fxns.h"

#define MAX 256

/* Helper function for query command "query topic" */
void query_topic(topicQ** tq, int num);
/* Helper function for query command "query publisher" */
void query_pub(thread_info* pub, int pub_num);
/* Helper function for query command "query subscriber" */
void query_sub(thread_info* sub, int sub_num);
/* Helper function for free up allocated memeory when error occured */
void freeup(char** w, topicQ **tq, thread_info *pub_info, thread_info *sub_info, int tn, int tp, int pub, int sub);

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

void query_topic(topicQ** tq, int num) {
    int i;
    if (num == 0) {
        fprintf(stderr, "No items in topics queue!\n");
        return;
    }
    for (i = 0; i < num; i++)
        printf("topic %d %d\n", tq[i]->id, tq[i]->lines);

}

void query_pub(thread_info *pub, int pub_num) {
    int i;

    if (pub_num == 0) {
        fprintf(stderr, "No threads in Publisher thread pool!\n");
        return;
    }
    printf("\n"); //Just for formatting
    for (i = 0; i < pub_num; i++)
        printf("publisher thread %d %s\n", pub[i].thread_num, pub[i].str);
}

void query_sub(thread_info *sub, int sub_num) {
    int i;
    
    if (sub_num == 0) {
        fprintf(stderr, "No threads in Subscriber thread pool!\n");
        return;
    }
    printf("\n");        // Just for formatting
    for (i = 0; i < sub_num; i++)
        printf("subscriber thread %d %s\n", sub[i].thread_num, sub[i].str);
}

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

void* dequeue_td(void* param) {
    printf("Dequeue Thread Created!\n");
    while(!done) {
        sleep(1);
    }
    int i;
    struct clean_arg* args;
    args = (struct clean_arg*) param;
    topicQ** q = args->tq;
    int num = args->tq_num;
    for (i = 0; i < num; i++) {
        free(q[i]->title);
        free_queue(q[i]);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    FILE *file;
    topicQ *myQueue[MAX];              // Change it to MAXTOPICS
    thread_info *pub_info;
    thread_info *sub_info;
    struct clean_arg *clean_info;
    pthread_t clean;
    char word[MAX];
    char* buf = NULL;
    int loc, i, total_num, id, delta, pub = 0, sub = 0, tp = 0;
    size_t len = 0;
    ssize_t read;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [command_file]\n", *argv);
        exit(EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "No file to read, %s cannot open!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    pub_info = (thread_info *) calloc(NUMPROXIES, sizeof(thread_info));
    sub_info = (thread_info *) calloc(NUMPROXIES, sizeof(thread_info));
    clean_info = calloc(1, sizeof(struct clean_arg));

    if (pub_info == NULL) {
        fprintf(stderr, "Create Pub thread pool Failed\n");
        exit(EXIT_FAILURE);
    }
    if (sub_info == NULL) {
        fprintf(stderr, "Create Sub thread pool Failed\n");
        exit(EXIT_FAILURE);
    }

    init();

    while ((read = getline(&buf, &len, file)) != -1) {
        loc = 0;
        total_num = 0;
        char* word_list[MAX];
        if (buf[0] == '\n') {
            continue;
        }
        buf[strlen(buf)-1]='\0'; 
        while((loc = p1getword(buf, loc, word)) != -1) {
            word_list[total_num] = strdup(word);
            total_num++;
        }

        if (strcmp("create", word_list[0]) == 0) {
            if (strcmp("topic", word_list[1]) == 0) {
                id = atoi(word_list[2]);
                myQueue[id] = tq_create(id, MAXTOPICS);
                myQueue[id]->title = strdup(word_list[3]);
                myQueue[id]->lines = atoi(word_list[4]);
                tp++;
            } else if (strcmp("publisher", word_list[1]) == 0) {
                pub_info[pub].thread_num = pub+1;
                pub_info[pub].str = strdup(word_list[2]);
                if (pthread_create(&pub_info[pub].thread_id, NULL, &publisher, &pub_info[pub]) != 0) {
                    fprintf(stderr, "Create publisher thread Failed! index: %d\n", pub);
                    exit(EXIT_FAILURE);
                } 
                pub++;
            } else if (strcmp("subscriber", word_list[1]) == 0) {
                sub_info[sub].thread_num = sub+1;
                sub_info[sub].str = strdup(word_list[2]);
                if (pthread_create(&sub_info[sub].thread_id, NULL, &subscriber, &sub_info[sub]) != 0) {
                    fprintf(stderr, "Create subscriber thread Failed! index: %d\n", sub);
                    exit(EXIT_FAILURE);
                } 
                sub++;
            } else {
                fprintf(stderr, "Error wrong commands for create!\n");
                freeup(word_list, myQueue, pub_info, sub_info, total_num, tp, pub, sub);
                break;
            }
        } else if (strcmp("query", word_list[0]) == 0) {
            if (strcmp("topics", word_list[1]) == 0) {
                query_topic(myQueue, tp);
            } else if (strcmp("publishers", word_list[1]) == 0) {
                query_pub(pub_info, pub);
            } else if (strcmp("subscribers", word_list[1]) == 0) {
                query_sub(sub_info, sub);
            } else {
                fprintf(stderr, "Error Wrong Commands for query!\n");
                freeup(word_list, myQueue, pub_info, sub_info, total_num, tp, pub, sub);
                break;
            }
        } else if (strcmp("Delta", word_list[0]) == 0) {
            // Delta time 
            delta = atoi(word_list[1]);
        } else if (strcmp("start", word_list[0]) == 0) {
            // Start!
            flag = 1;
            printf("\n"); /*    formatting */
            clean_info[0].tq = myQueue;
            clean_info[0].tq_num = tp;
            clean_info[0].delta = delta;

            if (pthread_create(&clean, NULL, &dequeue_td, (void *) &clean_info[0]) != 0) {
                fprintf(stderr, "Create cleanup thread failed!\n");
                exit(EXIT_FAILURE);
            }
            for (i = 0; i < pub; i++) {
                /* If thread is done, wait for other threads to terminate */
                if (pthread_join(pub_info[i].thread_id, NULL) != 0) {
                    fprintf(stderr, "Join pub thread %d is failed! Thread pool index: %d\n", pub_info[i].thread_num, i);
                    exit(EXIT_FAILURE);
                }
                printf("publisher thread %d exited!\n", pub_info[i].thread_num);
            }
            
            for (i = 0; i < sub; i++) {
                if (pthread_join(sub_info[i].thread_id, NULL) != 0) {
                    fprintf(stderr, "Join sub thread %d is failed! Thread pool index: %d\n", sub_info[i].thread_num, i);
                    exit(EXIT_FAILURE);
                }
                printf("subscriber thread %d exited!\n", sub_info[i].thread_num);
            }

            done = 1;
            if (pthread_join(clean, NULL) != 0) {
                fprintf(stderr, "Join CleanUp Thread is failed!\n");
                exit(EXIT_FAILURE);
            }
            printf("dequeue thread exited!\n");
        } else {
            fprintf(stderr, "Error Wrong Commands!\n");
            freeup(word_list, myQueue, pub_info, sub_info, total_num, tp, pub, sub);
            break;
        }

        for (i = 0; i < total_num; i++) {
            free(word_list[i]);
        }
    }

    free(pub_info);
    free(sub_info);
    free(clean_info);
    free(buf);
    pthread_mutex_destroy(&plock);
    pthread_mutex_destroy(&slock);
    fclose(file);

    return 0;
}

#endif