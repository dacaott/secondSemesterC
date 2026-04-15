#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int city;
    int cost;
    int owner;
} Edge;

typedef struct {
    Edge* items;
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue* createPriorityQueue();
void freePriorityQueue(PriorityQueue* pq);
void enqueue(PriorityQueue* pq, Edge value);
Edge dequeue(PriorityQueue* pq);
Edge peek(PriorityQueue* pq);
int isEmpty(PriorityQueue* pq);