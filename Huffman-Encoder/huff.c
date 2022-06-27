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
void header(int trash_size, int tree_size, Node *tree, FILE* out)
{
	unsigned char bytes[3];// = (unsigned char*)malloc(3 * sizeof(unsigned char));
	bytes[0] = trash_size << 5 | tree_size >> 8;   //Joga o trash_size para os 3 primeiros bits
	bytes[1] = tree_size;                          //Salva o tree_size

	fputc(bytes[0], out);
	fputc(bytes[1], out);
	print_pre_order_file(tree, out);  //Salva arvore no arquivo
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
	int bb;
	if (in == NULL)
	{
		printf("The file couldn't be opened, or the file doesn't exist. Do you want to try again?\n");
		printf("1 - Yes\n2 - No");
		scanf("%d",&bb);
		if (bb == 1)
		{
			compact();
			return;
		}
		else 
			return;
	}
	printf("\nDigite o nome do arquivo após ser compactado:\n");
	scanf("%s", outputname);
	printf("\n");
	strcat(outputname,".zap");
	out = fopen(outputname, "wb");

	int i, size, internal_node = 1;
	unsigned char s;
	unsigned int shift_bit = 0;
	long long int freq[257] = {0}; 
	while (fscanf(in, "%c",&s) != EOF) //Preenche array freq
	{
		freq[s]++;
	}
	fclose(in);
	Heap *heap = create_heap();
	Hash *ht = create_hash();

	for(i = 0; i < 257; i++)
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
		printf("%hu ",s);
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
	int bg;
	printf("Press 0 to continue.\n");
	scanf("%d",&bg);
}

int bit_set(unsigned char byte, int i)
{
    return (byte & (1 << i));
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
	
	unsigned long long int bytes = Tamanho_arquivo(in);
	printf("%lld\n",bytes);
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
	printf("%hu\n",tamanho_arvore);
	Node* raiz;
	int array[5] = {0};

	raiz = recreate_tree(in, array);

	bytes -= array[0];
	printf("%lld\n",bytes);


	//Impressão
	int i, j;

	Node* aux =  raiz;
	printf("%lld\n",bytes);
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
				//printf("%hu ",*(unsigned char*)raiz -> item);

				fputc(*(unsigned char*)raiz -> item, out);
				raiz = aux;
			}
		}
	}
	fclose(out);

	int nb;
	printf("File decompressed!\nPress 0 to continue.");
	scanf("%d",&nb);
	system("clear");	
}