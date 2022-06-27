#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"
#include "tree.h"
#include "hash.h"
#include "heap.h"
#include "huff.h"
#define MAX 257
void swap(Node **x, Node **y)
{
   Node *temp;
   temp = *x;
   *x = *y;
   *y = temp;
}

Heap* create_heap()
{
	int i;
	Heap *new_heap = (Heap*) malloc(sizeof(Heap));
	new_heap->size = 0;
	for(i = 1; i < 257; i++) 
		new_heap->data[i] = NULL;
	return new_heap;
}

int get_parent_index(Heap *heap, int i)
{
    return i/2;
}

int get_left_index(Heap *heap, int i)
{
    return 2*i;
}

int get_right_index(Heap *heap, int i)
{
    return 2*i + 1;
}

Node *item_of(Heap *heap, int i)
{
    return heap->data[i];
}

void min_heapify(Heap *heap, int i)
{
	int menor;
	int left_index = get_left_index(heap, i);	
	int right_index = get_right_index(heap, i);
	if (left_index <= heap->size && (heap->data[left_index]->freq < heap->data[i]->freq))
		menor = left_index;
	else menor = i;
	if (right_index <= heap->size && (heap->data[right_index]->freq < heap->data[menor]->freq))
		menor = right_index;
	if (heap->data[i]->freq != heap->data[menor]->freq)
	{
		swap(&(heap->data[i]), &(heap->data[menor]));
		min_heapify(heap, menor);
	}
}

void enqueue_heap(Heap *heap, void *item, int freq, Node *esquerda, Node *direita)
{	
	if (heap->size >= MAX) printf("Heap overflow");
	else
	{
		heap->data[++heap->size] = new_node(item, freq, esquerda, direita);
		int key_index = heap->size;
		int parent_index = get_parent_index(heap, heap->size);
		while (parent_index >= 1 && heap->data[key_index]->freq < heap->data[parent_index]->freq)
		{
			swap(&heap->data[key_index], &heap->data[parent_index]);
			key_index = parent_index;
			parent_index = get_parent_index(heap, key_index);
		}
	}
}

Node *dequeue_heap(Heap *heap)
{
	if (heap == NULL || heap->size == 0)
	{
		printf("Heap underflow\n");
		return NULL;
	}
	else
	{
		Node *item = heap->data[1];
		heap->data[1] = heap->data[heap->size];
		heap->size--;
		min_heapify(heap,1);
		return item;
	}
}