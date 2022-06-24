#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define DYN_SIZE_RUN 0 //usar reruns como tamanho (1) ou tamanho fixo (0)
#define RERUNS 100// caso {DYN_SIZE_RUN} seja 1, dá o tamanho crescente do dataset. caso contrario, repete com {SIZE_MAX}
#define SIZE_MAX 2000 //define o tamanho da pool do sorteio caso {DYN_SIZE_RUN} seja (0)

//estruturas aqui
struct list  {
    #if DYN_SIZE_RUN
        int values[RERUNS];
    #else
        int values[SIZE_MAX]; 
    #endif
};
typedef struct list list_t; //lista

struct BT  {
    int val; //(==)
    struct BT* left; //(<=)
    struct BT* right; //(>)
};
typedef struct BT binaryt_t; //binary tree


//assinaturas das funções aqui 
void fill_list(list_t* l, int size); //preenche a lista "aleatoriamente"

void add_bst_node(binaryt_t* root, int n); //add uma nova arvore no branch respectivo da arvore

int search_list(list_t* l, int size, int n); //busca o numero escolhido na lista (linearmente), retorna o numero de passos

int search_tree(binaryt_t* bst, int size, int n); //busca o numero escolhido na arvore binaria (bst), retorna o numero de passos

void print_list(list_t* l, int size); 

void print_tree(binaryt_t* root, int level);

//main
int main()    {

    int data[RERUNS][3]; //guarda a simulação
    srand(time(NULL)); //seed pra gerar os nums aleatorios

    printf("se os passos estiverem negativos, entao nao foi achado o valor, e os passos representam a busca completa.\n");

    for (int turn = 1; turn <= RERUNS; turn++) { //repete
        
        int choosen_num;
        list_t lista;
        #if DYN_SIZE_RUN 
            choosen_num = rand()%turn;
            fill_list(&lista, turn); //preenchendo a lista aleatoriamente
        #else
            choosen_num = rand()%SIZE_MAX;
            fill_list(&lista, SIZE_MAX); 
        #endif
        //printf("\n----------------------\nchoosen: %d\n", choosen_num);

        //inicializando a arvore binaria
        binaryt_t bst;
        bst.val = lista.values[0];
        bst.left = NULL;
        bst.right = NULL;
        #if DYN_SIZE_RUN
            for (int i = 1; i < turn; i++)  { //preenchendo ela de acordo com a regra para ser uma BST, com os mesmos valores da lista.
                add_bst_node(&bst, lista.values[i]);
            }

            int bst_results = search_tree(&bst, turn, choosen_num);
            int lista_results = search_list(&lista, turn, choosen_num);
            data[turn-1][2] = turn;
        #else
            for (int i = 1; i < SIZE_MAX; i++)  { 
                add_bst_node(&bst, lista.values[i]);
            }

            int bst_results = search_tree(&bst, SIZE_MAX, choosen_num);
            int lista_results = search_list(&lista, SIZE_MAX, choosen_num);
            data[turn-1][2] = SIZE_MAX;
        #endif
        data[turn-1][0] = bst_results;
        data[turn-1][1] = lista_results;

        // descomentar caso queira verificar os resultados na mão
        //print_list(&lista, DYN_SIZE_RUN?turn:SIZE_MAX); 
        //print_tree(&bst, 1);

        //printf("\nbusca na BST: %d steps\nbusca na lista: %d steps\n", bst_results, lista_results); //resultados
    }

    FILE *fp;
    #if DYN_SIZE_RUN
        fp = fopen("./results/dynamic_size_set.csv", "w+");
    #else
        fp = fopen("./results/fixed_size_set.csv", "w+");
    #endif

    fprintf(fp, "bst,lista,search_pool_size\n");
    for (int i = 0; i < RERUNS; i++)  { // resumo
        printf("turn %d - bst:%d, lista:%d\n", i+1, data[i][0], data[i][1]);
        fprintf(fp, "%d,%d,%d\n", data[i][0], data[i][1], data[i][2]);
    }
    fclose(fp);

    return data;
}


//declaração das funcs
void fill_list(list_t* l, int size)   {
    for (int i = 0; i < size; i++)    {
        l->values[i] = rand()%size;
        //printf("%dst choosen: %d\n", i+1, rand()); //mostra os sorteados pra formar a lista
    }
}

void print_list(list_t* l, int size)  {
    
    printf("lista:\n[ ");
    for (int i = 0; i < size; i++)  {
        printf("%d - ", l->values[i]);
    }
    printf("\b\b]\n");

    return;
}

void print_tree(binaryt_t* root, int level) { //print recursivo da arvore com niveis (preordem)
    if (!root) {
        return;
    }
    for (int i = 0; i < level-1; i++) {
        printf(i!=0?"|   ":"    ");
    }


    printf("|--{%d\n", root->val);
    print_tree(root->left, level + 1);
    print_tree(root->right, level + 1);

    
}

void add_bst_node(binaryt_t* root, int n)  {
    //cria o node
    binaryt_t* new_node = (binaryt_t*) malloc(sizeof(binaryt_t));
    new_node->val = n;
    new_node->left = NULL;
    new_node->right = NULL;

    int is_left = 0; //decide qual lado vai jogar o node
    binaryt_t* previous_node;
    while (root != NULL) { //ate chegar no fim de um node
        previous_node = root; // lembra o ultimo node caminhado
        if (root->val >= new_node->val) { //o valor do node atual é maior que do novo
            root = root->left; //vai pra esquerda
            is_left = 1;
        }
        else { //direita
            root = root->right;
            is_left = 0;
        }
        
    }
    if (is_left) previous_node->left = new_node; //se for pra esquerda no final, coloca o novo node lá
    else previous_node->right = new_node;

    return;
}

int search_list(list_t* l, int size, int n)   {
    int i = 0;
    for (; i < size; i++)  {
        if (l->values[i] == n) return i+1;  //achou, retorna os passos
    }
    return -i; //não achou, retorna o negativo dos passos
}

int search_tree(binaryt_t* bst, int size, int n)  {
    for (int i = 0; i < size; i++)    {
        if (bst == NULL) return -(i+1); //esgotou a busca sem achar
        else if (bst->val == n) return i+1; //achou aqui
        else if (n > bst->val) bst = bst->right; //num é maior, vai pra direita
        else bst = bst->left; //num é menor ou igual, vai pra esquerda
    }

    return 0; //deu ruim, em teoria isso aqui nunca pode acontecer
}