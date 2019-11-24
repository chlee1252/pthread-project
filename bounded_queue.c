/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 bounded_queue

This is my own work
 */

#include <stdio.h>
#include <stdlib.h>
#include "bounded_queue.h"

struct bounded_queue
{
        int size;         // capacity
        int len;          // current size
        void **buffer;    // storage
        long long head;   // Point Newest element
        long long tail;   // Point Oldest element
};

int RoundIDToBufferIndex(int size, long long index)
{
        long long value = (index % ((long long)size));
        return (int)value;
}

BoundedQueue *BB_MallocBoundedQueue(long size)
{
        BoundedQueue *new_queue = (BoundedQueue *) malloc (sizeof(BoundedQueue));
        if (new_queue == NULL) {
                return NULL;
        }
        new_queue->buffer = (void **) malloc(size*sizeof(void *));
        if (new_queue->buffer == NULL) {
                free(new_queue);
                return NULL;
        }

        new_queue->size = size;
        new_queue->head = new_queue->tail = 0;
        new_queue->len = 0;
        return new_queue;
}

long long BB_TryEnqueue(struct bounded_queue *queue,void *item)
{
        long long returnValue = -1;

        if (!BB_IsFull(queue)) {
                returnValue = queue->head;
                queue->buffer[RoundIDToBufferIndex(queue->size, queue->head)] = item;
                queue->head++;
                queue->len++;

        }
        return returnValue;
}

int BB_TryDequeue(struct bounded_queue *queue, long long id)
{
        int  returnValue = 0;
        if (!BB_IsEmpty(queue)) {
                if (BB_IsIdValid(queue, id)) {
                        // queue->buffer[RoundIDToBufferIndex(queue->size, queue->tail)] = NULL;
                        free(queue->buffer[RoundIDToBufferIndex(queue->size, queue->tail)]);
                        queue->tail++;
                        returnValue = 1;
                        queue->len--;
                }

        }
        return returnValue;
}

long long BB_GetFront(struct bounded_queue *queue)
{
        long long returnValue = -1;
        if(!BB_IsEmpty(queue))
        {
                returnValue = queue->head;
        }
        return returnValue;
}

long long BB_GetBack(struct bounded_queue *queue)
{
        long long returnValue = -1;
        if (!BB_IsEmpty(queue)) {
                return queue->tail;
        }
        return returnValue;
}

int BB_GetCount(struct bounded_queue *queue)
{
        int returnValue = queue->len;
        return (int)returnValue;
}

int BB_IsIdValid(struct bounded_queue *queue,long long id)
{
        int returnValue = 0;
        //long long id2idx = (long long) RoundIDToBufferIndex(queue->size, id);
        //printf("head: %llu, tail: %llu, id: %llu\n", queue->head, queue->tail, id);
        if (queue->head > id && id >= queue->tail) {
                returnValue = 1;
        }

        return returnValue;
}

void *BB_GetItem(struct bounded_queue *queue,long long id)
{
        void *returnValue = NULL;
        if (BB_IsIdValid(queue, id)) {
                int index = RoundIDToBufferIndex(queue->size, id);
                return queue->buffer[index];
        }
        return returnValue;
}

int BB_IsFull(struct bounded_queue *queue)
{
        int returnValue = 0;
        if (queue->len == queue->size){
                returnValue = 1;
        }
        return returnValue;
}

int BB_IsEmpty(struct bounded_queue *queue)
{
        int returnValue = 0;
        if (queue->len == 0) {
                returnValue = 1;
        }
        return returnValue;
}

void BB_FreeBoundedQueue(struct bounded_queue *queue)
{
        int n, i;
        int tail = RoundIDToBufferIndex(queue->size, queue->tail);
        for (i = tail, n = queue->len; n > 0; i = (i+1) % queue->size, n--) {
                free(queue->buffer[i]);
        }
        free(queue->buffer);
        free(queue);       
}

