/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 threadfxn for part2/3

This is my own work except the structure of struct is 
from pthread_create man page
 */

#ifndef TFXN_H
#define TFXN_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define NUMPROXIES 3

/* locks and global variables */
pthread_mutex_t plock;
pthread_mutex_t slock;
int flag, done;

typedef struct info {
    pthread_t thread_id;
    int       thread_num;
    char      *str;
} thread_info;

/* init locks and variable */
void init();

/* Publisher Thread */
void* publisher(void* args);

/* Subscriber Thread */
void* subscriber(void* args);

/* cleanup thread */
void* cleanup(void* args);

#endif