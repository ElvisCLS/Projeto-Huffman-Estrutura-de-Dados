#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"

#define MAX 257

Node *dequeue_heap(Heap *heap);

void enqueue_heap(Heap *heap, void *item, int freq, Node *esquerda, Node *direita);

void put_hash(Hash *ht, void *item, unsigned int shift_bit, int level);

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

Node* recreate_tree(FILE* in, int a[])	//Recria a arvore quando estiver descompactando
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

void create_encoding(Node *raiz, Hash *ht, int internal_node, unsigned int shift_bit, int level) //forma as novas representacoes dos bytes
{
	if(raiz->esquerda == NULL && raiz->direita == NULL)
	{
		void *item = raiz->item;
		put_hash(ht, item, shift_bit, level);
		return;
	}
	else
	{
		if(internal_node == 1)
		{
			create_encoding(raiz->esquerda, ht, 0, shift_bit, level + 1);
			create_encoding(raiz->direita, ht, 0, (shift_bit | 1), level + 1);
		}
		else
		{
			create_encoding(raiz->esquerda, ht, 0, (shift_bit<<1), level + 1);
			create_encoding(raiz->direita, ht, 0, ((shift_bit<<1) | 1), level + 1);
		}
	}
}

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

int fbytes(double bits)
{
    return ceil(bits/8);
}

void header(int trash_size, int tree_size, Node *tree, FILE* out)
{
	unsigned char* bytes = (unsigned char*)malloc(3 * sizeof(unsigned char));
	bytes[0] = trash_size << 5 | tree_size >> 8;   //Joga o trash_size para os 3 primeiros bits
	bytes[1] = tree_size;                          //Salva o tree_size

	fputc(bytes[0], out);
	fputc(bytes[1], out);
	print_pre_order_file(tree, out);  //Salva arvore no arquivo
}

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
		printf("Heap underflow");
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

void compact()
{
	system("clear");
	FILE *in;
	FILE *out;
	char filename[100] = "";
	char outputname[100] = "";
	printf("\nDigite o nome do arquivo para compactação:\n");
	scanf("%s", filename);
	in = fopen(filename, "rb");
	if (in == NULL)
	{
		compact();
		return;
	}
	printf("\nDigite o nome do arquivo após ser compactado:\n");
	scanf("%s", outputname);
	printf("\n");
	out = fopen(outputname, "wb");

	int i, size, internal_node = 1;
	unsigned char s;
	unsigned int shift_bit = 0;
	long long int freq[MAX] = {0}; 
	while (fscanf(in, "%c",&s) != EOF) //Preenche array freq
	{
		freq[s]++;
	}
	fclose(in);
	Heap *heap = create_heap();
	Hash *ht = create_hash();

	for(i = 0; i < MAX; i++)
	{
		if(freq[i] != 0)
		{
			enqueue_heap(heap, &i, freq[i], NULL, NULL);
		}
		ht->table[i]->freq = freq[i];
	}
	i = 0;
	Node *raiz;
	raiz = create_tree(heap);
	create_encoding(raiz, ht, internal_node, shift_bit, 0);

	int bits = fbits(ht);
	int bytes = fbytes(bits);
	int trash = (bytes*8) - bits; //A diferença entre os bytes utilizados *8 pela de bits utilizados é o lixo
	
	int size_of_tree = tree_size(raiz);

	header(trash, size_of_tree, raiz, out);// Calcula e coloca no cabeçalho do arquivo o tamanho do lixo e da arvore

	in = fopen(filename, "rb");

	int control = 0;
	int empty_bits = 8;

	unsigned int c;
	unsigned char aux = 0;

	//Impressão
	while (fscanf(in, "%c", &s) != EOF)
	{
		control = ht -> table[s] -> bits;
		empty_bits = empty_bits - control;
		c = ht -> table[s] -> shift_bit;
		if(empty_bits <= 0)
		{
			aux |= (c >> abs(empty_bits));
			fputc(aux, out);
			empty_bits =  8 + empty_bits;
			aux = 0;
			while(empty_bits < 0)
			{
				aux |= (c >> abs(empty_bits));
				fputc(aux, out);
				empty_bits =  8 + empty_bits;
				aux = 0;
			}
			aux |= (c << empty_bits);
		}
		else aux |= (c << empty_bits);
	}
	if(trash != 0)fputc(aux, out);
	fclose(in);
	fclose(out);	
}

int bit_set(unsigned char byte, int i)
{
    return (byte & (1 << i));
}

long long int Tamanho_arquivo(FILE *in)
{
	long long int bytes;
	if (in != NULL)
	{
		fseek(in, 0, SEEK_END);
		bytes = ftell(in);
	}
	return bytes;
}

void decompact()
{
	system("clear");
	FILE *in;
	FILE *out;

	char filename[100] = "";
	char outputname[100] = "";
	printf("\nDigite o nome do arquivo para descompactação:\n\n");
	scanf("%s", filename);
	in = fopen(filename, "rb");

	if (in == NULL)
	{
		decompact();
		return;
	}

	printf("\nDigite o nome do arquivo após ser descompactado com sua extensão:\n");
	scanf("%s", outputname);
	printf("\n");
	out = fopen(outputname, "wb");

	char s;
	long long int  bytes = Tamanho_arquivo(in);

	fclose(in);

	in = fopen(filename, "rb");

	unsigned char a, b, trash;
    fscanf(in, "%c%c", &a, &b);
    trash = a>>5;
    a = a<<3;  //Exclui os 3 bits de lixo
    a = a>>3;  //Retorna ao normal, com os 3 bits de lixo com 0
    short int tamanho_arvore = a;
    tamanho_arvore = tamanho_arvore<<8;
    tamanho_arvore |= b;

    bytes = bytes - (tamanho_arvore + 2);

	Node* raiz;
	int array[5] = {0};
	raiz = recreate_tree(in, array);

	bytes -= array[0];



	//Impressão
	int i, j;

	Node* aux =  raiz;

	for(i = 1; i <= bytes; i++)//bytes
	{
		fscanf(in, "%c", &s);
		for(j = 7; j >= 0; j--)//bits
		{
			if(j == (trash - 1) && (i == bytes)) break;
			if(bit_set(s, j)) raiz = raiz -> direita;//Percorre a arvore para salvar os dados no arquivo Out
			else raiz = raiz -> esquerda;

			if(raiz -> direita == NULL && raiz -> esquerda == NULL)
			{
				fputc(*(unsigned char*)raiz -> item, out);
				raiz = aux;
			}
		}
	}
	system("clear");	
}

void menu()
{
    int aux;
    printf("\n\n\tAlgorítmo de Huffman\n\n");
    printf("\tPaulo Santos, Elvis Correia e Luis Correia\n\n\n");
    printf("\tPressione '0' para encerrar o programa\n\tPressione '1' para compactar\n\tPressione '2' para descompactar\n\n");
    scanf("%d", &aux);
    if(aux == 0)
    {
        system("clear");
        return;
    }
	else if(aux == 1)
    {
        system("clear");
        compact();
    }
	else if(aux == 2)
    {
        system("clear");
        decompact();
    }
	else
    {
        printf("Opção inválida!\n");
        system("clear");
        menu();
    }
}
int main ()
{
	menu();
	return 0;
}