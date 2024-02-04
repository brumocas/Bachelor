/*****************************************************************/
/*          Biblioteca plantas | PROG2 | MIEEC | 2020/21         */
/*****************************************************************/

#include "plantas.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//---------------------------- Nossas funcoes ------------------------------------------------------------------------------------------------------------------------------------//
/*funcao que ordena as plantas de acordo com o tipo de ordem 
*parametro: c- colecao a ordenar
*parametro: tipo de  ordem pela qual e necessario ordenar
*/
void colecao_ordena(colecao *c ,const char *tipo_de_ordem){
	int i,j;
	planta *tmp;

	if((tipo_de_ordem==NULL) || (strcmp(tipo_de_ordem,"nome")!=0 && strcmp(tipo_de_ordem,"id")!=0) || (c==NULL) )	//verifica parametros
	{
			printf("Erro na passagem do tipo de ordem || tipo de ordem incorreta \n");
			return ;
	}

	//baseado na funcao vetor ordena da aula PL2

	if (strcmp(tipo_de_ordem,"nome")==0)					//verifica se tipo de ordem igual a nome
	{
															// ordenacao por insercao por tipo de nome cientifico se o if se verificar
		
		for (i = 1; i < c->tamanho ; i++)					//percorrer as varias plantas da colecao
		{
			tmp = c->plantas[i];							//usar auxiliar para comparar com as plantas da colecao

			for (j = i; j > 0 && strcmp(tmp->nome_cientifico, c->plantas[j-1]->nome_cientifico) < 0; j--)	//comparar tmp com a planta anterior 
			{	
				c->plantas[j]= c->plantas[j-1];				//se tmp  menor alfabeticamente troca(retorno de strcmp negativo,resultado obtido na comparacao diferença entre codigos ASCCI)
				
			}
			c->plantas[j]=tmp;								//completar a troca 
		}	
			
			strcpy(c->tipo_ordem,tipo_de_ordem);			//Atualiza parametro de tipo de ordenamento na colecao
			
	}
	else//igual ao anterior mas para caso de ser para ordenar segundo id
	{	//ordenacao por insercao
				for (i = 1; i < c->tamanho ; i++)
				{
				tmp = c->plantas[i];
				for (j = i; j > 0 && strcmp(tmp->ID, c->plantas[j-1]->ID) < 0; j--)
				{
					c->plantas[j]= c->plantas[j-1];
				}
				c->plantas[j]=tmp;
				}

				strcpy(c->tipo_ordem,tipo_de_ordem);
	}
}

/*
* funcao que imprime planta 
* usada em testes
*/
void imprime_planta(planta * p){
int i;
	
	printf("/-----------Planta :--------------------------/\n"),			
	puts(p->ID);															
	puts(p->nome_cientifico);												//Pede as características da planta e
	printf("Numero de sementes da planta %d \n",p->n_sementes);				//coloca-as na respetíveis variáveis
	printf("Numero de alcunnhas da planta %d \n",p->n_alcunhas);

	for ( i = 0; i < p->n_alcunhas; i++)
	{	
		if (p->alcunhas[i]==NULL)
		{
			printf("A alcunha encontra-se vazia...Erro! Devia ter algo\n");
			return;
		}
		

		puts(p->alcunhas[i]);
	}
	printf("/------------------------------------------/\n");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

planta *planta_nova(const char *ID, const char *nome_cientifico, char **alcunhas, int n_alcunhas, int n_sementes)	
{	
	//------------------------ Alocar espaço para a nova planta  -----------------------------------------------------------------------------------------------------------------------// 
	planta *nova;
	
	nova=calloc(sizeof(planta),1);
	if (nova==NULL)
	{
		printf("Erro a alocar memoria\n");
		return NULL;
	}

	//------------------------ Colocar e verificar ID na nova planta -------------------------------------------------------------------------------------------------------------------//
	
	if ( ID==NULL)
	{
		printf("ID da planta com mais caracteres que o suportado ou nao encontrado \n");
		return NULL;
	}
	else
	{
		strcpy(nova->ID,ID);
	}

	//------------------------ Colocar e verificar nome cientifico na nova planta ------------------------------------------------------------------------------------------------------//

	if (nome_cientifico==NULL)
	{
		printf("Nome cientifico maior que o suportado ou nao encontrado\n");
		return NULL;
	}
	else
	{
		strcpy(nova->nome_cientifico,nome_cientifico);
	}	
	
	//------------------------ Inserir e verificar o numero de sementes na planta nova -------------------------------------------------------------------------------------------------//

	if (n_sementes<1)
	{
		printf("Erro no numero de sementes\n");
		return NULL;
	}
	else
	{
		nova->n_sementes=n_sementes;
	}

	//------------------------ Colocar e verificar alcunhas na nova planta ------------------------------------------------------------------------------------------------------------//
	
	if (alcunhas==NULL || n_alcunhas<1)
	{
		//printf("A planta criada nao apresenta alcunhas locais\n");
		nova->n_alcunhas=0;
		nova->alcunhas=NULL;

	} 
	else
	{	
		nova->n_alcunhas=n_alcunhas;														//atualizar num alcunhas da planta
		nova->alcunhas=malloc(sizeof(char*)*(1*nova->n_alcunhas)); 							//reservar espaço para o apontador das strings alcunhas

		for (int i = 0; i < n_alcunhas ; i++)												//percorrer as alcunhas
		{
			nova->alcunhas[i]=malloc(sizeof(char)*(strlen(alcunhas[i])+1));					//reservar espaço para as strings a adicionar a planta (+1 referente ao barra zero)
			strcpy(nova->alcunhas[i],alcunhas[i]);											//copiar as strings do vetor alcunhas para a planta
		}

		//printf("Planta criada corretamente\n");
	}		

	//------------------------ Retorno de um apontador para a estrutura planta criada --------------------------------------------------------------------------------------------------//
	
	return nova;
}


colecao *colecao_nova(const char *tipo_ordem)	
{	
	//------------------------ Alocar memoria para a nova coleçao --------------------------------------------------------------------------------------------------------------------// 
	colecao *nova;
	
	nova=calloc(sizeof(colecao),1);
	
	if (nova==NULL)
	{
		printf("Erro a alocar memoria\n");
		return NULL;
	}
	//------------------------ Colocar tamanho de colecao neste caso zero e NULL porque se encontra vazia ------------------------------------------------------------------------------//

	nova->tamanho=0;
	nova->plantas=NULL;

	//------------------------ Verificar e colocar tipo de ordem em coleçao nova --------------------------------------------------------------------------------------------------------//
		if((tipo_ordem==NULL) || (strcmp(tipo_ordem,"nome")!=0 && strcmp(tipo_ordem,"id")!=0))
		{
			printf("Erro na passagem do tipo de ordem / tipo de ordem incorreta \n");
			return NULL;
		}
		else
		{
			strcpy(nova->tipo_ordem,tipo_ordem);
		}
	
	//------------------------ Retorno de apontador para a estrutura coleçao criada -----------------------------------------------------------------------------------------------------// 

	return nova;
}


int planta_insere(colecao *c, planta *p)
{	 
	
//---------------------------- Verificar parametros passados para a funcao ---------------------------------------------------------------------------------------------------------------//	
	if (c==NULL || p==NULL || c->tamanho <0 )
	{
		printf("Erro na passagem dos parametros\n");
		return -1;
	}
	
//---------------------------- Verificar se colecao se encontra vazia---------------------------------------------------------------------------------------------------------------------//
	
	int i=0,j=0,tam=0,local_encontrada=0,existe=0,encontrada=0;
	

	if (c->tamanho==0)							//Caso a colecao esteja vazia			
	{	
		c->plantas=calloc(sizeof(planta *),1);					//alocar memoria para a nova planta
		c->plantas[0]=p;										//apontar para a nova planta
		c->tamanho++;											//aumentar a tamanho da colecao
		//printf("A colecao de plantas encontra-se vazia planta introduzida corretamente\n");
		return 0;												//retornar 0 se tudo correu bem
	}
	else if (c->tamanho>0)                                             					//Coleçao apresenta outras plantas
	{	
		for ( i = 0; i < c->tamanho; i++)												// Percorrer todas as plantas da colecao
		{	
			if(strcmp(c->plantas[i]->ID,p->ID)==0)										// comparar o id das plantas da colecao com a planta a inserir
			{	
				existe=1;																// flag para identificar que ja existe
			
				//printf("A planta ja existe, foi atualizado numero de sementes\n");	// Dar conhecimento ao utilizador que ja existia e foi atualizada(vai ser)
				
				c->plantas[i]->n_sementes+=p->n_sementes;								// Atualizar o numero de sementes da planta que ja existia (somar com as que existiam)
				
				local_encontrada=i;														//Guardar local onde a planta repetida foi encontrada
			}
																	
			if(existe==1)break;															//caso a planta tenha sido encontrada parar o ciclo
			
		}

	}
	
//---------------------------- Caso a planta nao existia previamente ------------------------------------------------------------------------------------------------------------------//

	if (existe==0)															// Flag que demonstra que a planta não existia previamente
	{	
		//printf("Planta introduzida corretamente\n");
		
		c->plantas=realloc(c->plantas,(sizeof(planta *)*(c->tamanho +1)));	//realocar memoria para o apontador para  plantas

		c->plantas[c->tamanho]=p;											// Acrescentar à coleção as características da planta no fim da mesma
		
		c->tamanho++;														// Acrescentar ao tamanho da coleção a planta nova
			
		colecao_ordena(c,c->tipo_ordem);									//tratar do ordenamento da planta inserida


		return 0;				//return zero de insercao de planta correu bem
	}
//---------------------------- Planta existe verificar se alcunha esta repetida ----------------------------------------------------------------------------------------------------------//

	

	
	if(existe==1)
	{ 	for ( i = 0; i< p->n_alcunhas; i++)				// Percorrer as alcunhas da planta nova 
		{		
				
				encontrada=0;																//atualizar flag porque vamos compararcom alcunha diferente
			
				for ( j = 0; j <c->plantas[local_encontrada]->n_alcunhas; j++)				// Percorrer as alcunhas da planta antiga 
				{	
					if(strcmp(c->plantas[local_encontrada]->alcunhas[j],p->alcunhas[i])==0)			// Verificar se as alcunhas da planta antiga no sistema coincidem com a nova a ser carregada
					{	
						//printf("Alcunha igual encontrada\n");
						encontrada=1;																//flag que diz se alcunha da pos x da nova planta foi encontrada na colecao			
					}
				
				}
			
			if (encontrada==0)																		//caso alcunha nao exista no vetor alcuhas da planta da colecao
			{																		
					c->plantas[local_encontrada]->alcunhas=realloc(c->plantas[local_encontrada]->alcunhas,(c->plantas[local_encontrada]->n_alcunhas)*sizeof(char*));		//alocar memoria para o apontador de strings alcunha 
					
					c->plantas[local_encontrada]->alcunhas[c->plantas[local_encontrada]->n_alcunhas]=malloc(sizeof(char)*(strlen(p->alcunhas[i])+1));						//alocar memoria para a string alcunha																					
					
					strcpy(c->plantas[local_encontrada]->alcunhas[c->plantas[local_encontrada]->n_alcunhas],p->alcunhas[i]);												// Caso sejam diferentes as alcunhas copiar para a planta antiga e coloca-las no fim do vetor alcunhas 
				
					c->plantas[local_encontrada]->n_alcunhas++;																												// Aumentar o tamanho do vetor de alcunhas se alcunha diferente das outra
						

			return 1;		//return 1 se atualizacao de planta correu bem 
			}	
		}
	}
	

	
	//retorna -1 se nada aconteceu (Ocorreu erro)//

	return -1;									
}


int colecao_tamanho(colecao *c) 
{	
	//------------------------ Verificar parametros passados para a funcao ---------------------------------------------------------------------------------------------------------------//
	if (c==NULL)
	{
		printf("Erro na passagem de colecao \n");
		return -1;
	}
	
	if (c->tamanho<0)
	{
		printf("Erro na passagem do tamanho da colecao\n");
		return -1;
	}

	//------------------------ Indicacao do tamanho da colecao ---------------------------------------------------------------------------------------------------------------//		

	return c->tamanho;						// retorna o tamanho da colecao
}


colecao *colecao_importa(const char *nome_ficheiro, const char *tipo_ordem)
{	
	colecao *new;

	FILE *f;
	f=fopen(nome_ficheiro,"r");		    // Abertura do ficheiro

	//------------------------ Verificar parametros passados para a funcao ---------------------------------------------------------------------------------------------------------------//	

	if (f==NULL)						// Mensagem caso haja erro na abertura do ficheiro
	{
		printf("Erro na abertura do ficheiro\n");
		return NULL;
	}
	
	new=colecao_nova(tipo_ordem);		// Mensagem caso o apontador nao aponte para o local correto
	
	if (new==NULL)
	{
		printf("Erro na criacao da nova colecao\n");
	}

	//------------------------ Extracao dos dados do ficheiro ---------------------------------------------------------------------------------------------------------------------------//	

	
	int num_alcunhas=0;
	char linha[400],id[10],nome[200],**alcunhas,sementes[30],*tokken;
	
	alcunhas=malloc(1*sizeof(char*));
	
	while (fgets(linha,400,f)!=NULL)		// Importacao dos dados do ficheiro para o cofre
	{		

			tokken=strtok(linha,",");		//Importaçao do ID
			strcpy(id,tokken);
			
			
			tokken=strtok(NULL,",");		//Importaçao do Nome
			strcpy(nome,tokken);
		
			
			tokken=strtok(NULL,",");		//Importaçao das Sementes
			strcpy(sementes,tokken);
			
			num_alcunhas=0;					//atualizar contador de alcunhas porque e uma planta diferente
			
			while (1)
			{ 	
				
				tokken=strtok(NULL,",");	
				if(tokken==NULL)			//Caso não sejam localizadas as alcunhas
				{
					break;
				}
				
				*alcunhas=realloc(*alcunhas,(sizeof(char*)*(num_alcunhas+1)));		//realocar memoria para o apontador das alcunhas novas a adicionar 
				
				alcunhas[num_alcunhas]=malloc(sizeof(char)*(strlen(tokken)+1));		//alocar memoria para as alcunhas strings a adicionar ao vetor 
				
				strcpy(alcunhas[num_alcunhas],tokken);								//copiar alcunhas para vetor na pos num_alcunhas
					
				num_alcunhas++;														//incrementar tamanho do vetor alcunhas
			
				
			}
		
		new->plantas=realloc(new->plantas,(new->tamanho+1)*sizeof(planta *));		//realocar memoria para o apontador para vetor de plantas

		planta_insere(new,planta_nova(id,nome,alcunhas,num_alcunhas,atoi(sementes)));		//Importar nova planta para a colecao recorrendo a planta insere que verifica muitos parametros

		for (int i = 0; i < num_alcunhas; i++)										//libertar memoria das strings do vetor de alcunhas
			{
				free(alcunhas[i]);
			}	
		
	}
		free(alcunhas);																//libertar memoria para o apontador para as strings alcunhas
		
		strcpy(new->tipo_ordem,tipo_ordem);											//atualizar tipo de ordenamento da colecao
		
		colecao_ordena(new,new->tipo_ordem);										//ordenar de acordo com o tipo de ordem atualizado anteriomente
		
		fclose (f);		 															// Encerramento do ficheiro								

	return new;
}


planta *planta_remove (colecao *c, const char *nomep)
{	//----------------------- Verificar parametros-----------------------------------------------------------------------------------------------------------------------------------//
	if (c==NULL || nomep==NULL)
	{
		printf("Erro na passagem de parametros\n");
		return NULL;
	}
	
	//----------------------- Pesquisa se existe a planta a remover -----------------------------------------------------------------------------------------------------------------//
	
	int i=0,existe=0,local_encontrada;
	
	planta *removida;
	
	for ( i = 0; i < c->tamanho; i++)									//percorrer plantas da colecao
	{
		if (strcmp(c->plantas[i]->nome_cientifico,nomep)==0)			//verifica se nome cientifico de planta a remover existe
		{
			existe=1;													//flag que identfica e existencia de planta a remover
		}
		else
		{
			existe=0;													//caso a planta n exista
		}
		if (existe==1)													//terminar ciclo de planta a remover foi encontrada
		{	
			removida=c->plantas[i];										//copiar planta a remover							
			local_encontrada=i;											//guardar o local onde planta foi copiada
			break;
		}
	}
	
	//----------------------- Proceder a remocao da planta caso exista na colecao------------------------------------------------------------------------------------------------------//

	if(existe==1)
	{
	/* copia todos os elementos a partir da posicao pos+1 ate' ao fim do vetor para pos */
	for(i=local_encontrada+1; i<c->tamanho; i++)			//percorrer apartir de planta a remover
	{
		c->plantas[i-1] = c->plantas[i];					//copia todas a planta da posicao i para i-1 
	}

	c->tamanho--;											//decrementar tamanho da colecao

	return removida;
	}

	if (existe==0)											//caso planta nao exista na colecao
	{		
		printf("Planta a remover nao existe na colecao\n");
		return NULL;
	}
	

	return NULL;
}


int planta_apaga(planta *p)
{	
	//----------------------- Verificar parametros passados para a funcao ---------------------------------------------------------------------------------------------------------------//	
	
	if (p==NULL)
	{
		printf("Planta nao existe\n");
	
		return -1;
	}

	//----------------------- Libertação dos dados da planta guardada ---------------------------------------------------------------------------------------------------------------//	

		for (int i = 0; i < p->n_alcunhas; i++)
		{
			free(p->alcunhas[i]);			//liberta memoria das strings que contem as alcunhas//
		}
			free(p->alcunhas);				//liberta memoria dos apontadores para a string//	
		
			free(p);						//liberta memória da estrutura p//
		

	return 0;								//retorna zero caso concluida com sucesso//
}


int colecao_apaga(colecao *c)
{	
	//----------------------- Verificar parametros passados para a funcao ---------------------------------------------------------------------------------------------------------------//	
	if (c==NULL)
	{
		printf("Colecao nao existe");
		return -1;
	}
	
	//----------------------- Libertação dos dados da colecao guardada ------------------------------------------------------------------------------------------------------------------//	
	
	for (int i = 0; i <c->tamanho; i++)
	{
		planta_apaga(c->plantas[i]);			//liberta memoria de plantas
	}

	free(c->plantas);							//liberta memoria do apontador para vetor de plantas

	free(c);									//liberta memoria do apontador para colecao
	
	return 0;
}


int *colecao_pesquisa_nome(colecao *c, const char *nomep, int *tam)
{	
	//----------------------- Verificar parametros ---------------------------------------------------------------------------------------------------------------------------------//
	if (c==NULL && nomep==NULL )
	{
		printf("Erro na passagem de argumentos\n");
		return NULL;
	}	

	//----------------------- Percorrer plantas da colecao e comparar com nome cientifico ----------------------------------------------------------------------------------------//
	
	int i=0,j=0,*posicoes,tamanho=0,existe=0;
	
		posicoes=calloc(sizeof(int),1);		//alocar memoria para o vetor que contem as posicoes da pesqquisa efetuada
	

	for ( i = 0; i < c->tamanho; i++)	//percorrer plantas
		{	 
			
			if(strstr(c->plantas[i]->nome_cientifico,nomep)!=NULL)			//pesquisar em nome cientifico de plantas por substring nomep
			{	
				existe=1;													//flag que identifica existencia de substring
				
				posicoes=realloc(posicoes,(tamanho+1)*sizeof(int));			//realocar memoria para o vetor de posicoes						
				
				posicoes[tamanho]=i;										//guardar posicao encontrada no vetor
				
				tamanho++;													//incrementar tamanho do vetor	
			}
		}

	//----------------------- Percorrer plantas da colecao e comparar com alcunhas ----------------------------------------------------------------------------------------------//

		for ( i = 0; i < c->tamanho; i++)									//percorrer plantas				
		{ 	
			for ( j = 0; j < c->plantas[i]->n_alcunhas; j++)				//percorrer alcunhas
			{		
				if (strstr(c->plantas[i]->alcunhas[j],nomep)!=NULL)			//pesquisar em alcunhas de plantas por substring nomep
						{	

							existe=1;										//flag que identifica existencia de substring
							
							posicoes=realloc(posicoes,(tamanho+1)*sizeof(int));	//realocar memoria para o vetor de posicoes
							
							posicoes[tamanho]=i;								//guardar posicao encontrada no vetor
							
							tamanho++;							
								
						}
				
			}
		}
		
	//----------------------- Caso a planta a procurar nao exista -----------------------------------------------------------------------------------------------------------------//
		
		if (existe==0)
		{	
			printf("A planta nao existe\n");
			return NULL;	
		}
		
		*tam=tamanho;				//passar a apontar para o valor do tamanho do vetor posicoes

	//nota nao e possivel dar free ao vetor posicoes pois este tem de ser devolvido perde-se 2 frees

	return 	posicoes;				//retornar apontador para a 1 posicao do vetor 
}


int colecao_reordena(colecao *c, const char *tipo_ordem)
{	
	//----------------------- Verificar parametros ---------------------------------------------------------------------------------------------------------------------------//

	if((tipo_ordem==NULL) || (strcmp(tipo_ordem,"nome")!=0 && strcmp(tipo_ordem,"id")!=0) || (c==NULL) )
	{
			printf("Erro na passagem do tipo de ordem / tipo de ordem incorreta \n");
			return -1;
	}

	//----------------------- Verificar se ja se encontra ordenada ----------------------------------------------------------------------------------------------------------//
	
	if (strcmp(c->tipo_ordem,tipo_ordem)==0)
	{
		printf("Colecao ja se encontra ordenada ");
		return 0;
	}
	else//------------------- Ordenar de acordo com o tipo de ordem ------------------------------------------------------------------------------------------------------------//
	{
		colecao_ordena(c,tipo_ordem);
		return 1;
	}

return -1;
}


