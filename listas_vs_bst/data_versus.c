#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define SIZE_MAX 1000


struct list  {
    int values[SIZE_MAX]; 
};
typedef struct list list_t; //lista

struct BT  {
    int val;
    struct BT* left;
    struct BT* right;
};
typedef struct BST binaryt_t; //binary tree

//assinaturas das funções aqui 

void fill_list(list_t* l); //preenche a lista "aleatoriamente"

void print_list(list_t* l); 

binaryt_t* make_binary_tree(list_t* l);

int search_list(list_t* l, int n); //busca o numero escolhido na lista (linearmente)

int search_tree(binaryt_t* bst, int n); //busca o numero escolhido na arvore binaria (bst)

//main
int main()    {
    srand(time(NULL));
    int choosen_num = rand()%SIZE_MAX;
    printf("choosen: %d\n", choosen_num);
    
    list_t lista;
    fill_list(&lista);
    print_list(&lista);

    return 0;
}

//declaração das funcs

void fill_list(list_t* l)   {
    for (int i = 0; i < SIZE_MAX; i++)    {
        l->values[i] = rand()%SIZE_MAX;
        //printf("%dst choosen: %d\n", i+1, rand()); //mostra os sorteados pra formar a lista
    }
}

void print_list(list_t* l)  {
    
    printf("lista:\n[ ");
    for (int i = 0; i < SIZE_MAX; i++)  {
        printf("%d - ", l->values[i]);
    }
    printf("\b\b]\n");

    return;
}

binaryt_t* make_binary_tree(list_t* l)  {
    
}