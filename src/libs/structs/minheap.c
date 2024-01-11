#include "minheap.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>

struct min_heap {
        int size;
        int capacity;
        int *array;
};

int is_empty(min_heap *heap) { return heap->size == 0; }

int is_full(min_heap *heap) { return heap->size == heap->capacity; }

int get_parent(int index) { return (index - 1) / 2; }

int get_left_index(int index) { return (2 * index) + 1; }

int get_right_index(int index) { return (2 * index) + 2; }

min_heap *create_minheap(int capacity) {
        min_heap *heap = malloc(sizeof *heap);

        heap->size = 0;
        heap->capacity = capacity;
        heap->array = (int *)malloc(sizeof(int) * capacity);

        return heap;
}
void min_heapify(min_heap *heap, int i) {
        int lowest;
        int left_index = get_left_index(i);
        int right_index = get_right_index(i);

        if (left_index <= heap->size - 1 &&
            heap->array[left_index] < heap->array[i]) {
                lowest = left_index;
        } else {
                lowest = i;
        }

        if (right_index <= heap->size - 1 &&
            heap->array[right_index] < heap->array[lowest]) {
                lowest = right_index;
        }

        if (heap->array[i] != heap->array[lowest]) {
                SWAP(heap->array[i], heap->array[lowest]);
                min_heapify(heap, lowest);
        }
}

void enqueue(min_heap *heap, int value) {
        if (is_full(heap)) {
                printf("Heap is full\n");
                return;
        }

        int i = heap->size;
        heap->array[i] = value;
        heap->size++;

        while (i != 0 && heap->array[get_parent(i)] > heap->array[i]) {
                SWAP(heap->array[i], heap->array[get_parent(i)]);
                i = get_parent(i);
        }
}

int dequeue(min_heap *heap) {
        if (is_empty(heap)) {
                printf("Heap is empty\n");
                return -1;
        }

        int root = heap->array[0];
        heap->array[0] = heap->array[heap->size - 1];
        heap->size--;
        min_heapify(heap, 0);

        return root;
}

int peek(min_heap *heap) { return heap->array[0]; }
