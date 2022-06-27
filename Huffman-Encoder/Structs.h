#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX 257

struct node
{
	void *item;
	int freq;
	struct node *esquerda;
	struct node *direita;
};
typedef struct node Node;

struct heap
{
	int size;
	Node *data[MAX];
};
typedef struct heap Heap;

struct elemento
{
	void *item;
	unsigned int shift_bit;
	int bits;
	int freq;
};
typedef struct elemento Elemento;

struct hash
{
	Elemento *table[MAX];
};
typedef struct hash Hash;

#endif