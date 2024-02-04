#include <stdio.h>
#include <string.h>
# define MAX 100
typedef struct artigo
{   
    char tipo_artigo[MAX];
    char marca[MAX];
    char modelo[MAX];
    float preço;
    int armazem;

}artigo;

int leArtigos(artigo armazem[], char *nomeFicheiro);
int totalArtigos(artigo armazem[], int n, char *marca, char *modelo);
void alertaArtigos(artigo armazem[], int n);
void ordenaArtigos(artigo armazem[], int n);

int main()
{   artigo armazem[MAX]={0};
    char *modelo="S-40";
    char *marca="SuperTV";
    char *nome_ficheiro="artigos.txt";
    int artigos_disponiveis;
    artigos_disponiveis=leArtigos(armazem,nome_ficheiro);
    printf("\n");
    printf("O numero de artigos disponiveis no armazem sao: %d \n",artigos_disponiveis);
    printf("\n");
    printf("O numero total de artigos disponiveis da marca %s modelo %s e %d \n",marca,modelo,totalArtigos(armazem,artigos_disponiveis,marca,modelo));
    printf("\n");
    alertaArtigos(armazem,artigos_disponiveis);
    printf("\n");
    ordenaArtigos(armazem,artigos_disponiveis);
}


int leArtigos(  artigo armazem[] , char *nomeFicheiro)
{  int i,j,artigos_lidos=0;
    
    
    FILE *f;
   f=fopen(nomeFicheiro,"r");
  
   while (fgets(armazem[artigos_lidos].tipo_artigo,MAX,f)!=NULL)    
   {
       artigos_lidos++;
   }
    fclose(f);
   for ( j = 0; j < artigos_lidos; j++)
    {   
    printf("%s",armazem[j].tipo_artigo);
   
    }
return artigos_lidos;
}


int totalArtigos(artigo armazem[], int n, char *marca, char *modelo){
    int i,j, artigos_disponiveis=0;
     FILE *f;
     f=fopen("artigos.txt","r");

    for ( i = 0; i < MAX ; i++)
    {
        fscanf(f,"%s %s %s %f %d \n",armazem[i].tipo_artigo,armazem[i].marca,armazem[i].modelo,&armazem[i].preço,&armazem[i].armazem);
    }
    
    for ( i = 0; i < MAX; i++)
    {
        if ((strcmp(armazem[i].modelo,modelo)==0) && (strcmp((armazem[i].marca),marca)==0))
        {
            artigos_disponiveis=armazem[i].armazem;
                  
        }  
    }   
    fclose(f);
return artigos_disponiveis;
}


void alertaArtigos(artigo armazem[], int n)
{ int i ;

    for(i=0 ; i < n ; i++)
    {
        if (armazem[i].armazem < 10)
        {
            printf("O produto : %s %s %s  tem menos de 10 unidades de stock\n",armazem[i].tipo_artigo,armazem[i].marca,armazem[i].modelo);
        }
    }
}

void ordenaArtigos(artigo armazem[], int n)
{   int i;
    int pos =0, max=0;
    artigo aux;

            while (pos < n - 1)
            {
                max = pos;
                i = pos + 1;
                
                while (i < n)
                {                                       /* procura maior */
                if (armazem[i].armazem > armazem[max].armazem) max = i;
                i++; 
                }
                if (max != pos)
                {                       /* troca elementos */
                aux = armazem[pos];
                armazem[pos] = armazem[max];
                armazem[max] = aux;
                }
                pos ++;
            }   
    FILE *f;
    f=fopen("artigos.txt","w"); 
    
    for ( i = 0; i < n; i++)
        {
         fprintf(f,"%s %s %s %.1f %d \n",armazem[i].tipo_artigo,armazem[i].marca,armazem[i].modelo,armazem[i].preço,armazem[i].armazem);
        }
    fclose(f);
}

    
