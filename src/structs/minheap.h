#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct min_heap min_heap;

void min_heapify(min_heap *heap, int i);
void enqueue(min_heap *heap, int value);

int dequeue(min_heap *heap);
int peek(min_heap *heap);

min_heap *create_minheap(int capacity);

#endif
