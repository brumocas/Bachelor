#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXFILMES 4000
#define TSIZE 150
#define CSIZE 30
#define MAXPAISES 20
#define DECADA_PESQUISA 1970
#define PAIS_PESQUISA "Poland"

typedef struct{
    unsigned int id;
    char titulo[TSIZE];
    char origem[MAXPAISES][CSIZE];
    unsigned int ano, duracao;
} filme;

/****************************************************/
/*              Funcoes a implementar               */
/****************************************************/

int maisLongo(filme catalogo[], int n){
    int i,max,pos=0;

    max=catalogo[0].duracao;

    for ( i = 0; i <MAXFILMES; i++)
    {
        if(catalogo[i].duracao > max){
            max=catalogo[i].duracao;
            pos=i;
        }
    }
    return pos;
}

int gravaDecada(filme catalogo[], int n, char *nomeFicheiro, int decada){
    FILE *fout;
    int i,j,num_filmes=0,decada_aux,decada_out_aux;
    int anos_dec[10]={0};
   
    fout=fopen("stats.txt","w");

    for ( i = 0; i < n; i++)
    {   decada_aux=catalogo[i].ano/10;
        decada_out_aux=decada/10;
        if ((decada_aux%10)==decada_out_aux%10)
        {   
            num_filmes++;    
            switch (catalogo[i].ano%10)
                {
                case 0:
                anos_dec[0]++;
                    break;
                case 1:
                anos_dec[1]++;
                    break;
                case 2:
                anos_dec[2]++;
                    break;
                case 3:
                anos_dec[3]++;
                    break;
                case 4:
                anos_dec[4]++;
                    break;
                case 5:
                anos_dec[5]++;
                    break;
                case 6:
                anos_dec[6]++;
                    break;
                case 7:
                anos_dec[7]++;
                    break;
                case 8:
                anos_dec[8]++;
                    break;
                case 9:
                anos_dec[9]++;
                    break;
                }
        }
    }

    for ( i = 0; i < 10; i++)
    {
       fprintf(fout,"%d - %d \n",decada+i,anos_dec[i]);
    }

    fclose(fout);
    
return num_filmes;
}

int pesquisaPorPais(filme catalogo[], int n, char *pais, filme resultado[]){
    int i,j=0,k=0,filmes_pais=0;
    
    for ( i = 0; i < n; i++)
    {   for ( k = 0; k <20 ; k++){       
            if(strcmp((catalogo[i].origem[k]),pais)==0)
            {   filmes_pais++;
                resultado[j]=catalogo[i];
                j++;
            }
        }
    }   
    {   int pos =0, max, aux;
            while (pos < n - 1)
            {
                max = pos;
                i = pos + 1;
                
                while (i < n)
                {                                       /* procura maior */
                if (resultado[i].duracao > resultado[max].duracao) max = i;
                i++; 
                }
                if (max != pos) {                       /* troca elementos */
                aux = resultado[pos].duracao;
                resultado[pos].duracao = resultado[max].duracao;
                resultado[max].duracao = aux;
                }
                pos ++;
                }   
            }
    
      return filmes_pais;
}

/****************************************************/
/*     Funcoes ja implementadas (nao modificar)     */
/****************************************************/ 

int ler(char *filename, filme catalogo[]);

void listar(filme catalogo[], int n);

void imprimir(filme f);

int main(){
    filme catalogo[MAXFILMES] = {0}, resultado[MAXFILMES];
	int nfilmes, nres, duracao;
    int posMax;

	nfilmes = ler("catalogo.txt", catalogo);
	/*printf("\n*** Filmes (%d) ***\n", nfilmes);
	listar(catalogo, nfilmes);
	printf("\n");*/

    posMax = maisLongo(catalogo, nfilmes);
    if(posMax != -1)
    {    
        printf("\n*** Filme mais longo (duracao %d minutos) ***\n\n", catalogo[posMax].duracao);
        imprimir(catalogo[posMax]);
    }
    else
    	printf("\n*** Filme mais longo (duracao -- minutos) ***\n\n---");    
	printf("\n");

    printf("\n*** A gravar estatisticas por ano para ficheiro stats.txt ***\n");
    printf("\nInformacao da decada %d gravada com sucesso (total = %d filmes)\n", DECADA_PESQUISA, gravaDecada(catalogo, nfilmes, "stats.txt", DECADA_PESQUISA));
	printf("\n");

	nres = pesquisaPorPais(catalogo, nfilmes, PAIS_PESQUISA, resultado);
	printf("\n*** Filmes com origem %s (%d) ***\n\n", PAIS_PESQUISA, nres);
    listar(resultado, nres);
	printf("\n");

	return 0;
}

int ler(char *filename, filme catalogo[]){
	FILE * f;
	int n = 0;
    char linha[1000];
    char paises[MAXPAISES*CSIZE];

    f = fopen(filename, "r");
    if(f == NULL)
		return 0;

	while((n < MAXFILMES) && fgets(linha, 1000, f) != NULL){
        linha[strlen(linha)-1]='\0';
        catalogo[n].id = atoi(strtok(linha, "\t"));
        strcpy(catalogo[n].titulo, strtok(NULL, "\t"));
        strcpy(paises, strtok(NULL, "\t"));
        catalogo[n].ano = atoi(strtok(NULL, "\t"));
        catalogo[n].duracao = atoi(strtok(NULL, "\t"));
        
        int npaises=0;
        char *pais;
        strcpy(catalogo[n].origem[0], strtok(paises, ","));
        while((pais = strtok(NULL, ",")) != NULL)
            strcpy(catalogo[n].origem[npaises++], pais+1);
        n++;
    }

    fclose(f);
    return n;
}

void listar(filme catalogo[], int n){
    int i, j;
    
    for(i=0; i<3 && i<n; i++){
        imprimir(catalogo[i]);
    }
    if(n > 3)
    {
        if(n > 6)
            puts("...");
        j = i;
        if(n-3 > j)
            j = n-3;
        for (i=j; i<n; i++){
            imprimir(catalogo[i]);
        }
    }
}

void imprimir(filme f){
    int i=1;
    printf("%d - %s ", f.id, f.titulo);
    printf("| %s", f.origem[0]);
    while(strlen(f.origem[i]) != 0)
    {
        printf(", %s", f.origem[i]);
        i++;
    }
    printf(" | %d | %d\n", f.ano, f.duracao);
}
