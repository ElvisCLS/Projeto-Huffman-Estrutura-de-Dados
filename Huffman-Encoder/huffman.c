#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Structs.h"
#include "tree.h"
#include "hash.h"
#include "heap.h"
#include "huff.h"

//#define MAX 257





void menu()
{
    
    int aux = 90;
    while(aux != 0)
    {
    	
    	system("clear");
    	printf("\n\n\tAlgorítmo de Huffman\n\n");
        printf("\tPaulo Santos, Elvis Correia e Luis Correia\n\n\n");
        printf("\tPressione '0' para encerrar o programa\n\tPressione '1' para compactar\n\tPressione '2' para descompactar\n\n");
       // getchar();
        scanf("%d", &aux);
    	
    	if(aux == 1)
        {
            system("clear");
            compact();
        }
    	else if(aux == 2)
        {
            system("clear");
            decompact();
        }
    	else if (aux != 2 && aux != 1 && aux != 0)
        {
            printf("Opção inválida!\n");
            system("clear");
            menu();
        }
        else
        	return;
    }
}
int main ()
{
	
	menu();
	printf("The real compression are the friends we made along the way!\n");
	//return 0;
}