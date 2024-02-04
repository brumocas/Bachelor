/*****************************************************************/
/*         Tabela de Dispersao | PROG2 | MIEEC | 2020/21         */
/*****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabdispersao.h"
#include "grafo.h"

//--------------------Funcoes auxiliares---------------------------------------------------//

int checkPrimeNumber(int n) {
    int j, flag = 1;
    for (j = 2; j <= n / 2; ++j) {
        if (n % j == 0) {
            flag = 0;
            break;
        }
    }
    return flag;
}

int next_prime(int n)
{   int i,k;

    if(checkPrimeNumber(n))
    return n;

    for ( i = n + 1 ;; i++)
    {
        for ( k  = 2 ; k < i ; k++)
        {
            if (i % k==0)
            break;
        }
        if (i==k)
        break;
        
    }
    return i;
}

void tabela_imprime(tabela_dispersao *td)
{   if(!td)
    return ;
   
    no_grafo * elem;
    printf("TABELA DE DISPERSAO (%d elementos)\n", td->tamanho);
    for (int i = 0; i < td->capacidade; i++)
    {
        if (td->estado_celulas[i]==1)
        {
            printf("\t[%2d]", i);
            
            elem = td->nos[i];
        
            printf(" : [\"%s\" :: \"%d\"]", elem->cidade, elem->tamanho);

            printf("\n");
                
        }   
           
    }           
        for (int i = 0; i < td->capacidade; i++)
            {
            printf("Estado das celulas  [%d] = %d \n",i,td->estado_celulas[i]);
            }

    printf("\n");
    
}

//-----------------------------------------------------------------------------------------//
tabela_dispersao *tabela_nova(int capacidade, hash_func *hfunc, sond_func *sfunc) //correto
{   if( capacidade < 1  || !hfunc || !sfunc )
    return NULL;

    tabela_dispersao *td =(tabela_dispersao *) malloc(sizeof( tabela_dispersao));
    if( td == NULL )
    return NULL; /* "Out of space!!! */

    td->capacidade = next_prime( capacidade );

    td->capacidade=capacidade;

    td->nos = (no_grafo **) calloc(td->capacidade,(sizeof(no_grafo*)) );
    if( td->nos == NULL ) 
    {
        free(td);
        return NULL; // "Out of space!!! 
    }
    
    td->estado_celulas=(int*)malloc(sizeof(int)*td->capacidade);
    if(!td->estado_celulas)
    { 
        free(td->nos);
        free(td);
        return NULL;//out of space
    }
    
    td->tamanho=0;
    td->hfunc=hfunc;
    td->sfunc=sfunc;
    
    for(int i=0; i< td->capacidade; i++ )
    { 
        td->estado_celulas[i] = 0;
    }
    return td;

}

int tabela_adiciona(tabela_dispersao *td, no_grafo *entrada)//correto
{   if(!td || !entrada)
    return -1;

    int indice= td->hfunc (entrada->cidade, td->capacidade),count=1;

    if(td->estado_celulas[indice]==0)//Nó ainda nao existe
    {   
        td->estado_celulas[indice]=1;
        td->nos[indice]=entrada;
        td->tamanho++;    
        return indice;
    } 
    else 
    {
        int aux = indice;
        
        while (td->estado_celulas[indice] !=0  && (td->nos[indice]!=entrada) )
        {
            if (td->estado_celulas[indice] == 1)
            {
               if (strcmp(entrada->cidade, td->nos[indice] ->cidade)==0)
                return -1;
            }
           
        indice = td-> sfunc(aux, count, td->capacidade);
        count++;
        }

        td-> estado_celulas[indice] = 1;
        td->nos[indice] = entrada;
        td->tamanho++;
        return indice;
    }
    return -1;
}


int tabela_remove(tabela_dispersao *td, no_grafo *saida)
{   if(!td || !saida)
    return -1;

    int indice= td->hfunc (saida->cidade, td->capacidade),count=1;

    if(td->estado_celulas[indice]==1)//Remover
    {   
        
        td->estado_celulas[indice]=-1;
        
        td->nos[indice]=NULL;
        
        td->tamanho--;
        
        return 0;
    } 
    else 
    {
        int aux = indice;
        
        while (td->estado_celulas[indice] !=0  && (td->nos[indice]!=saida) )
        {
            if (td->estado_celulas[indice] == 1)
            {
                if (strcmp(saida->cidade, td->nos[indice] ->cidade)==0)
                break;       
            }
        indice = td-> sfunc(aux, count, td->capacidade);
        count++;
        }
        
        td-> estado_celulas[indice] = -1;
        td->nos[indice] = NULL;
        td->tamanho--;
    }

    return 0;
}


void tabela_apaga(tabela_dispersao *td)
{   
    if (!td) return;

    for (int i=0; i< td->capacidade; i++)
    {   
        if(td->estado_celulas[i]!=0)
        tabela_remove(td,td->nos[i]);
    }
    free(td->estado_celulas);
    free(td->nos);
    free(td); 
 
}


int tabela_existe(tabela_dispersao *td, const char *cidade)
{
    if (!td || !cidade) 
    return -1;

    int indice=td->hfunc(cidade,td->capacidade);    

    if(td->estado_celulas[indice] == 1)
    {
      return indice;
    }
    
    return -1;
}


tabela_dispersao *tabela_carrega(grafo *g, int tamanho)
{ 
    if (!g || tamanho < 1) return NULL;

    tabela_dispersao *tabadd = tabela_nova(tamanho,hash_krm, sond_rh);
    if (!tabadd) return NULL;

    int pospreenchidas=0;
    
    for (int i=0; i< g->tamanho; i++)
    {
        
        if(tabela_adiciona(tabadd,g->nos[i])==-1)
        return NULL;

        pospreenchidas++;

    }
    tabadd->tamanho = pospreenchidas;

    return tabadd;
}


/*================================================================================*/
unsigned long sond_rh(int pos, int tentativas, int tamanho)//sondagem quadratica
{

    return (pos + tentativas * tentativas) % tamanho;
}

unsigned long hash_krm(const char *chave, int tamanho)
{
    int c, t = strlen(chave);
    unsigned long hash = 7;

    for (c = 0; c < t; c++)
    {
        hash += (int)chave[c];
    }

    return hash % tamanho;

    return 0;
}
