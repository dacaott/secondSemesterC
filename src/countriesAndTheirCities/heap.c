#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

static int edgeCompare(Edge a, Edge b)
{
    return a.cost - b.cost;
}

static int resize(PriorityQueue* pq)
{
    pq->capacity *= 2;
    Edge* temp = (Edge*)realloc(pq->items, pq->capacity * sizeof(Edge));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    pq->items = temp;
    return 0;
}
static void swap(Edge* a, Edge* b)
{
    Edge temp = *a;
    *a = *b;
    *b = temp;
}

static void heapifyUp(PriorityQueue* pq, int index)
{
    if (index
        && edgeCompare(pq->items[(index - 1) / 2], pq->items[index]) > 0) {
        swap(&pq->items[(index - 1) / 2],
            &pq->items[index]);
        heapifyUp(pq, (index - 1) / 2);
    }
}
static void heapifyDown(PriorityQueue* pq, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size
        && edgeCompare(pq->items[left], pq->items[smallest]) < 0)
        smallest = left;

    if (right < pq->size
        && edgeCompare(pq->items[right], pq->items[smallest]) < 0)
        smallest = right;

    if (smallest != index) {
        swap(&pq->items[index], &pq->items[smallest]);
        heapifyDown(pq, smallest);
    }
}

PriorityQueue* createPriorityQueue()
{
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    pq->capacity = 100;
    pq->items = (Edge*)malloc(pq->capacity * sizeof(Edge));
    if (pq->items == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(pq);
        return NULL;
    }
    return pq;
}

void freePriorityQueue(PriorityQueue* pq)
{
    free(pq->items);
    free(pq);
}

void enqueue(PriorityQueue* pq, Edge value)
{
    if (pq->size == pq->capacity) {
        int err = resize(pq);
        if (err) {
            fprintf(stderr, "Failed to resize priority queue\n");
            return;
        }
    }

    pq->items[pq->size++] = value;
    heapifyUp(pq, pq->size - 1);
}

Edge dequeue(PriorityQueue* pq)
{
    if (!pq->size) {
        printf("Priority queue is empty\n");
        return (Edge) { -1, -1, -1 };
    }

    Edge item = pq->items[0];
    pq->items[0] = pq->items[--pq->size];
    heapifyDown(pq, 0);
    return item;
}

Edge peek(PriorityQueue* pq)
{
    if (!pq->size) {
        printf("Priority queue is empty\n");
        return (Edge) { -1, -1, -1 };
    }
    return pq->items[0];
}

int isEmpty(PriorityQueue* pq)
{
    return pq->size == 0;
}