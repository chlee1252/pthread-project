/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 part1

This is my own work except the test cases is from lab.
 */

#ifndef PART1_C
#define PART1_C

#include "topicqueue.h"
#include "thread_safe_bounded_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char* argv[]) {
    topicQ* myQueue;
    long long id = 0;
    myQueue = tq_create(id, MAXTOPICS);

    printf("Initialized New Topic Queue id: %llu, size: %d\n", id, MAXTOPICS);
    printf("Full Queue Test\n");
    printf("Expected = 0, Actual = %d\n", is_full(myQueue));
    printf("Empty Queue Test\n");
    printf("Expected = 1, Actual = %d\n", is_empty(myQueue));
    
    printf("Fill up Queue\n");
    fill_up(myQueue);
    printf("Full Queue Test\n");
    printf("Expected = 1, Actual = %d\n", is_full(myQueue));
    printf("Empty Queue Test\n");
    printf("Expected = 0, Actual = %d\n", is_empty(myQueue));

    tq_entry* ent = (tq_entry *) malloc (sizeof(tq_entry));
    strcpy(ent->photoURL, "ABC");
    strcpy(ent->photoCaption, "XYZ");
    printf("Enqueue into a full queue Test!\n");
    printf("Expected = -1, Actual = %d\n", (int) tq_enqueue(myQueue, ent));
    free(ent);

    printf("Printing Entry\n");
    print(myQueue);

    printf("Empty Queue!\n");
    empty_queue(myQueue);
    printf("FULL QUEUE?? Expected = 0, Actual = %d\n", is_full(myQueue));
    printf("EMPTY QUEUE? Expected = 1, Actual = %d\n", is_empty(myQueue));
    printf("Dequeue on Empty Queue!\n");
    printf("Expected: -1 Actual = %d\n", tq_dequeue(myQueue));

    long long last = fill_up(myQueue);

    printf("Dequeue Three items\n");
    tq_dequeue(myQueue);
    tq_dequeue(myQueue);
    tq_dequeue(myQueue);

    print(myQueue);

    printf("FULL QUEUE?? Expected = 0, Actual = %d\n", is_full(myQueue));
    printf("EMPTY QUEUE? Expected = 0, Actual = %d\n", is_empty(myQueue));
    tq_entry* new_ent;
    tq_getentry(myQueue, last, (void **) &new_ent);

    printf("Get Entry Test\n");
    printf("Printing last entry!\n");

    print_entry(new_ent);

    printf("Try to Get Entry with bad id\n");
    long long bad_id = 999;
    if (tq_getentry(myQueue, bad_id, (void **) &new_ent) < 0) {
        printf("Failed to Get Bad ID %llu\n", bad_id);
    } else {
        print_entry(new_ent);
    }

    free_queue(myQueue);

    printf("Program Terminates Successfully!\n");
    
    
    return 0;
}

#endif