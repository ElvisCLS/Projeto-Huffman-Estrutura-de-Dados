#ifndef tree_H
#define tree_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"
#include "tree.h"
#include "hash.h"
#include "heap.h"
#include "huff.h"
void print_pre_order_file(Node* raiz, FILE* out);
Node *new_node(void *item, int freq, Node *esquerda, Node *direita);
int tree_size(Node* raiz);
Node *create_tree(Heap *heap);
Node* recreate_tree(FILE* in, int *a);

#endif