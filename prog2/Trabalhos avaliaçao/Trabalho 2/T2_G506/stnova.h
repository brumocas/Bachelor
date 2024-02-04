/*****************************************************************/
/*    Estrutura nova a implementar | PROG2 | MIEEC | 2020/21     */
/*****************************************************************/

#ifndef _STNOVA_H_
#define _STNOVA_H_

#include "grafo.h"
#include "tabdispersao.h"   

/* baratucho termo utilizado para definir viagens baratas*/

typedef struct baratucho
{   
    char    *partida;            /* string com nome da cidade de partida */
    char    *destino;            /* string com nome da cidade de chegada */
    double   preço;              /* Preço da viagem baratucha */
    char    *codigo;             /*codigo de voo da viagem mais baratucha */
	struct baratucho * proximo;  /* util para lista ligada a utilizar */
} voo;

typedef struct
{
    voo **low_cost;	              /* vetor de apontadores para viagens low_cost */
    int tamanho;			      /* Numero de viagens low cost */
    int capacidade;               /*capacidade maxima de cidades a aramzenar*/
} estrutura;

/*
*  cria e inicializa a estrutura criada
*  retorna a estrutura se bem-sucedido ou NULL caso contrário
*/
estrutura *st_nova();

/*
*  importa todo o conteúdo do grafo g para o novo formato de acesso
*  parametro st - apontador para a estrutura criada 
*  parametro g - apontador para o grafo
*  retorna 0 se bem-sucedido ou -1 em contrário
*/
int st_importa_grafo(estrutura *st, grafo *g);

/* obtém o código de voo do par origem-destino com menor preço 
*  a instância retornada deverá ser mantida, i.e., deverá ficar uma cópia dela no respetivo elemento de ‘st’
*  NOTA: esta função será avaliada pelo tempo de execução!
*  parametro st - apontador para a estrutura
*  parametro origem - nome da cidade de origem dos voos a pesquisar
*  parametro destino - nome da cidade de destino dos voos a pesquisar
*  retorna o código do voo ou em caso de insucesso, retorna NULL
*/
char *st_pesquisa(estrutura *st, char *origem, char *destino);

/*
*  elimina todas as instâncias presentes na estrutura st e desaloca toda a memória da mesma
*  NOTA: esta função será avaliada pelo tempo de execução!
*  parametro st - apontador para a estrutura 
*  retorna 0 se bem-sucedido e -1 se ocorrer algum erro
*/
int st_apaga(estrutura *st);

/**
 * Funcao utilizada para dispersar viagens mais baratas pela tabela de dispersao
 * obs:usa metodo que concatena a partida e a chegada e depois usa esse valor como chave
 * */
unsigned long hash_djbm( char* partida, char* chegada, int tamanho);

/**
 *  Verifica se determinado numero e numero primo
 * */
int checkPrimeNumber2(int n);

/**
 * Calcula o numero primo mais proximo por excesso
 * */
int next_prime2(int n);

/**
 *  cria viagem a adicionar a estrutura
 *  parametro: aresta - aresta a copiar 
 *  parametro: partida -local de partida da viagem a considerar
 *  return NULL se erro e apontador para a viagem se correto
 **/
voo * cria_voo(aresta_grafo *aresta , char *cidade);

/**
 * adiciona uma determinada low_cost a tabela de dispersao atualiza preço e codigo se repetido 
 * paramreto : td  tabela de dispersao a adicionar 
 * parametro : voo  low_cost a possivelmente  adicionar á tabela
 * return : 1 se ok ; 0 se ja existe  ; -1 se erro   
 * */
int tabela_adiciona_baratucho(estrutura *td, voo *voo );

/*
*   Imprime todo o conteudo associado a um voo 
*   parametro : voo a imprimir
**/
void imprime_voo(voo *voo); 

/**
 *  Mostra o conteudo da estrutura em forma partida->destino
 *  parametro : td estrutura a mostrar 
 * */
void mostraTabela(estrutura *td);

#endif
