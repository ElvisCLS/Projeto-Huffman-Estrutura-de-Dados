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
int fbytes(double bits)
{
    return ceil(bits/8);
}
Elemento *new_element()
{
	Elemento *node = (Elemento*) malloc(sizeof(Elemento));
	node->item = NULL;
	node->shift_bit = 0;
	node -> bits = 0;
	return node;
}

Hash *create_hash()
{
	int i;
	Hash *new_ht = (Hash*) malloc(sizeof(Hash));
	for(i = 0; i < 257; i++)
		new_ht->table[i] = new_element();
	return new_ht;
}

Elemento *create_data(Elemento *local, void *item, unsigned int shift_bit, int level)
{
	local -> item = item;
	local -> shift_bit = shift_bit;
	local -> bits = level;
	return local;
}

void put_hash(Hash *ht, void *item, unsigned int shift_bit, int level)
{
	int index;
	int item_aux = *(int*)item;
	index = item_aux % MAX;
	ht -> table[index] = create_data(ht -> table[index], item, shift_bit, level);
}

int fbits(Hash *ht)
{
	int bits = 0;
	for(int i = 0; i < MAX; i++)
	    if(ht->table[i]->freq != 0) 
	    	bits += (ht->table[i]->bits * ht->table[i]->freq);

	    return bits;
}