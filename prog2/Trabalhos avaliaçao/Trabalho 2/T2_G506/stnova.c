/*****************************************************************/
/*    Estrutura nova a implementar | PROG2 | MIEEC | 2020/21     */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stnova.h"

#define MAX_SIZE 100500

/*-----------------------------------Começa aqui-----------------------------------------------------*/

estrutura *st_nova()
{   
    int capacidade=next_prime2(MAX_SIZE);

    /* aloca memoria para a estrutura tabela_dispersao */

    estrutura *t = (estrutura*) malloc(sizeof (estrutura));
    if (t == NULL)
    return NULL;

    /* aloca memoria para os elementos */
    t->low_cost = (voo **) calloc(capacidade, sizeof (voo *));
    if (t->low_cost == NULL) 
    {
        free(t);
        return NULL;
    }
    
    for (int i = 0; i < capacidade; i++)
    {
        t->low_cost[i]=NULL; 
    }

    t->capacidade = capacidade;
    t->tamanho=0;

    return t;
}

int st_importa_grafo(estrutura *st, grafo *g)
{   if(!st || !g)
    return  -1;

    int ret;
    voo *voo;

    for (int i = 0; i <  g->tamanho; i++)
    {   
        for(int k=0 ; k < g->nos[i]->tamanho ; k++)
        {   
            voo=cria_voo(g->nos[i]->arestas[k],g->nos[i]->cidade);
            if(!voo)
            return -1;
        
            ret=tabela_adiciona_baratucho(st,voo);
            
            if (ret==-1)
            {
                printf("Erro ao inserir\n");
                return -1;
            }
            
        }   
    }    //mostraTabela(st);
    return 0;
}

char *st_pesquisa(estrutura *st, char *origem, char *destino)//observar pequeno pormenor
{   if(!st || !origem || !destino)
    return NULL;

    int index=hash_djbm(origem,destino,st->capacidade);
    
    voo *elem = st->low_cost[index];
    while (elem != NULL && strcmp(elem->partida,origem) != 0 && strcmp(elem->destino,destino)!=0)
        elem = elem->proximo;

    if (elem == NULL)//nao existe na lista ligada
    return NULL;
    
    //caso contrario existe e devolvemos NOTA PARA REVER AQUI

    return elem->codigo ;
}

int st_apaga(estrutura *st)
{   if(!st)
    return -1;
    
    int i;
    voo * elem, * aux;

    /* para cada entrada na tabela */
    for (i = 0; i < st->capacidade; i++)
    {
        /* apaga todos os elementos da entrada */       
            elem = st->low_cost[i]; 
            while(elem != NULL)
            {
                aux = elem->proximo;
                free(elem->destino);
                free(elem->partida);
                free(elem->codigo);
                free(elem);
                elem = aux;
    
            }
            
    }   
    //apagar estrtura da tabela
    free(st->low_cost);
    free(st);

    return 0;
}


//--------------------------Funcoes auxiliares--------------------------------------------------------//

unsigned long hash_djbm( char* partida, char* chegada, int tamanho)
{   
    if(!partida || !chegada || tamanho < 1)
    return -1;

    char chave[strlen(partida)+strlen(chegada)+1];
    strcpy(chave,partida);
    strcat(chave,chegada);

    unsigned long h, i;
    i = 0;
    h = 5347;

    while(chave[i])
    {
        h *= (31);
        h ^= chave[i++];
        h &= 0xffffffff; // consideram-se apenas 32 bits 
    }

    return h % tamanho;
}


int checkPrimeNumber2(int n)
{
    int j, flag = 1;
    for (j = 2; j <= n / 2; ++j) {
        if (n % j == 0) {
            flag = 0;
            break;
        }
    }
    return flag;
}


int next_prime2(int n)
{   int i,k;

    if(checkPrimeNumber2(n))
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

//-----------------------------------------------------------------------------------------------------//

voo * cria_voo(aresta_grafo *aresta , char *cidade)
{   if(!aresta || !cidade)
    return NULL;

    //mem pa viagem
    voo *v = (voo*)malloc(sizeof(voo));
    if(!v)
    return NULL;

    //mem para string de partida
    v->partida=(char *)malloc((strlen(cidade)+1)*sizeof(char));
    if(!v->partida)
    return NULL;
    
    
    //mem para a string destino
    v->destino=(char*)malloc((strlen(aresta->destino->cidade)+1)*sizeof(char));
    if(!v->destino)
    return NULL;
    
    
    //mem para o codigo da viagem
    v->codigo=(char *)malloc((strlen(aresta->codigo)+1)*sizeof(char));
    if(!v->codigo)
    return NULL;
    

    //atualizar campos
    v->preço = aresta->preco;
    strcpy(v->codigo,aresta->codigo);
    strcpy(v->destino,aresta->destino->cidade);
    strcpy(v->partida,cidade);
    
    return v;
}

int tabela_adiciona_baratucho(estrutura *td, voo *v )
{   if (!td || !v) return -1;

    int index;
    voo * elem; 

    // calcula hash para low_cost a adicionar 
    
    index = hash_djbm(v->partida,v->destino,td->capacidade);

    // verifica se viagem ja' existe na lista 
    elem = td->low_cost[index];
    while (elem != NULL && strcmp(elem->partida,v->partida) != 0 &&  strcmp(elem->destino,v->destino) !=0)    
    elem = elem->proximo;

    if (elem == NULL)//nao existe na lista ligada
    {   
        // insere no inicio da lista 
        v->proximo = td->low_cost[index];
        td->low_cost[index] = v;
        td->tamanho++;
        return 1;
    }   
    else//cidade ja existe atualizar parametros
        {   
            if (elem->preço > v->preço)
            {   
                elem->preço=v->preço;
                elem->codigo=(char *)realloc(elem->codigo,(strlen(v->codigo)+1)*sizeof(char));
                strcpy(elem->codigo,v->codigo);

                //apagar viagem que fui utilizada para atualizar
                free(v->codigo);
                free(v->destino);
                free(v->partida);
                free(v); 
                return 0;  
            }
            else //preço e mais alto nao interessa ,logo descartar viagem
            {           
                        free(v->codigo);
                        free(v->destino);
                        free(v->partida);
                        free(v); 
                        return 0;
            }
        }
    return -1;
}

void imprime_voo(voo *c)
{   if(!c)
    return ;

    printf("Viagem mais barata :\nPartida : %s -> Destino %s\nPreço %.3f $\nCodigo -> %s\n\n",c->partida,c->destino, c->preço,c->codigo);

}

void mostraTabela(estrutura *td)
{   
    if(!td) 
    return ;

    printf("TABELA DE DISPERSAO (%d capacidade)\n",td->capacidade);
    int i ;
    voo * elem;
    for (i = 0; i < td->capacidade; i++)
    {   
        if (td->low_cost[i])
        {
            printf("\t[%2d]", i);
            elem = td->low_cost[i];
            while (elem)
            {
                printf(" : [\"%s\" :: \"%s\"]", elem->partida,elem->destino);
                elem = elem->proximo;
            }
            printf("\n");
        }
    }
    printf("TABELA DE DISPERSAO (%d elementos)\n",td->tamanho);
}

