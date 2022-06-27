#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"
#include "tree.h"
#include "hash.h"
#include "heap.h"
#include "huff.h"
void print_pre_order_file(Node* raiz, FILE* out)
{
	if(raiz != NULL)
	{
		if(raiz->freq != 0)
		{
			if(((*(unsigned char*)raiz->item == 92) || *(unsigned char*)raiz->item == 42) && (raiz->esquerda == NULL && raiz->direita == NULL)) fputc(92, out);// Caso o nó seja '*' ou '\' imprime '\'
			fputc(*(int*)raiz->item, out);
			print_pre_order_file(raiz->esquerda, out);
			print_pre_order_file(raiz->direita, out);
		}
	}
}
Node *new_node(void *item, int freq, Node *esquerda, Node *direita)
{
	Node *node_queue = (Node*) malloc(sizeof(Node));
	node_queue->item = (unsigned char*) malloc(sizeof(unsigned char));
	*(unsigned char *)node_queue->item = *(unsigned char*)item;
	node_queue->freq = freq;
	node_queue->esquerda = esquerda;
	node_queue->direita = direita;
	return node_queue;
}

int tree_size(Node* raiz)
{
	if(raiz == NULL) return 0;
	else return  1 + tree_size(raiz -> esquerda) + tree_size(raiz -> direita);
}

Node *create_tree(Heap *heap)
{
	while(heap->size > 0)
	{
		Node *filho_esquerdo = dequeue_heap(heap);
		Node *filho_direito = dequeue_heap(heap);

		unsigned char asterisco = 42; // Código ASCII do asterisco
		if(heap->size == 0)
		{	
			return new_node(&asterisco, (filho_esquerdo->freq + filho_direito->freq), filho_esquerdo, filho_direito);
		}
		if((*(unsigned char*)filho_direito->item == '*' && *(unsigned char*)filho_esquerdo->item != '*') && (filho_direito->freq == filho_esquerdo->freq))
		{
			enqueue_heap(heap, &asterisco,filho_esquerdo->freq + filho_direito->freq, filho_direito, filho_esquerdo);
		}
		else
		{
			enqueue_heap(heap, &asterisco,filho_esquerdo->freq + filho_direito->freq, filho_esquerdo, filho_direito);
		}
	}
}

Node* recreate_tree(FILE* in, int *a)	//Recria a arvore quando estiver descompactando
{
	char c;
	short int internal_node = 1;
	fscanf(in, "%c", &c);
	if(c == 92) 	// ASCII da contrabarra
	{
		a[0]++;
		fscanf(in, "%c", &c);
		internal_node = 0;   // Indica que o nó com '*' é uma folha
	}
	Node *tree_node;
	tree_node = new_node(&c, 1, NULL, NULL);
	if (c == '*' && internal_node == 1) //Não é uma folha
	{
		tree_node -> esquerda = recreate_tree(in, a);
		tree_node -> direita = recreate_tree(in, a);
	}
	return tree_node;
}