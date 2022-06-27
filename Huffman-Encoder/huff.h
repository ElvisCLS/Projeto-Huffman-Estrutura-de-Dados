#ifndef huff_H
#define huff_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"
#include "tree.h"
#include "hash.h"
#include "heap.h"
#include "huff.h"
void create_encoding(Node *raiz, Hash *ht, int internal_node, unsigned int shift_bit, int level);
void header(int trash_size, int tree_size, Node *tree, FILE* out);
void compact();
int bit_set(unsigned char byte, int i);
long long int Tamanho_arquivo(FILE *in);
void decompact();

#endif