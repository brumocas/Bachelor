#include <stdio.h>
#include<stdlib.h>
#include <string.h>

#define DIM 100

int main()
{   char frase[DIM];
    char *aux,*palavra_maior;
    int media,max=0,comprimento,numero_palavras=1,i=0;
    float soma=0.0;
    
    printf("Introduza uma frase\n");
    fgets(frase,DIM,stdin);
    
    aux=strtok(frase," ");
    while (1)
    {   
        if (aux==NULL)break;
        else
        {   
            numero_palavras++;
            comprimento=strlen(aux);
            soma+=comprimento;
            if (comprimento>max)
            {
                max=comprimento;
                palavra_maior=aux;
            } 
    
         }
         aux=strtok(NULL," ");
    }     


    printf("Numero de palavras %d \n",numero_palavras);
    printf("Palavra maior %s \n",palavra_maior);
    printf("Comprimento medio %.1f \n",soma/numero_palavras);
}
