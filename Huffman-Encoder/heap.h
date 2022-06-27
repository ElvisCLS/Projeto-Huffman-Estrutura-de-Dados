#ifndef heap_H
#define heap_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"
#include "tree.h"
#include "hash.h"
#include "heap.h"
#include "huff.h"


Node *dequeue_heap(Heap *heap);

void swap(Node **x, Node **y);

Heap* create_heap();

void enqueue_heap(Heap *heap, void *item, int freq, Node *esquerda, Node *direita);

int get_parent_index(Heap *heap, int i);

int get_left_index(Heap *heap, int i);

int get_right_index(Heap *heap, int i);


Node *item_of(Heap *heap, int i);


void min_heapify(Heap *heap, int i);

#endif