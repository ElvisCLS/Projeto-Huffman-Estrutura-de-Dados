#ifndef hash_H
#define hash_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"
#include "tree.h"
#include "hash.h"
#include "heap.h"
#include "huff.h"
void put_hash(Hash *ht, void *item, unsigned int shift_bit, int level);
int fbytes(double bits);
Elemento *new_element();
Hash *create_hash();
Elemento *create_data(Elemento *local, void *item, unsigned int shift_bit, int level);
void put_hash(Hash *ht, void *item, unsigned int shift_bit, int level);
int fbits(Hash *ht);

#endif