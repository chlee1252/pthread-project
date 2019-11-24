/*
Author: Marc Lee (Changhwan)
duckid: clee3
Title: CIS415 project2 bounded_queue header file

This is my own work
 */

#ifndef BOUNDED_QUEUE_H
#define BOUNDED_QUEUE_H

typedef struct bounded_queue BoundedQueue;

BoundedQueue *BB_MallocBoundedQueue(long size);

long long BB_TryEnqueue(BoundedQueue *queue,void *item); 

/* if it is not empty
   if id is valid
   Id == tail
   null what tail is indexing move tail to next id return success */
int BB_TryDequeue(BoundedQueue *queue,long long id);

/* Head points to the next if not empty: (Head -1) else -1 */
long long BB_GetFront(BoundedQueue *queue);

/* If not empty: Tail else -1 */
long long BB_GetBack(BoundedQueue *queue);

/* Head - Tail */
int BB_GetCount(BoundedQueue *queue);

/* Head > id >= Tail */
int BB_IsIdValid(BoundedQueue *queue,long long id);

/* if IsIdValid() index = ConvertIdtoIndex(id); return Buffer[index]
else null */
void *BB_GetItem(BoundedQueue *queue,long long id);

/* if Head - Tail == size */
int BB_IsFull(BoundedQueue *queue);

/* if Head == Tail */
int BB_IsEmpty(BoundedQueue *queue);

/* Cleanup and Free */
void BB_FreeBoundedQueue(BoundedQueue *queue);

#endif
