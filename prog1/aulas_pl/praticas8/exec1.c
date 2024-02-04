#include <stdio.h>

int procuraTodos(int *v, int N, int x, int posicoes[],int *nPosicoes);

int main()
{
    int N, x, nPosicoes=0,i;

    printf("Qual o tamanho do vetor que pretende? ");
    scanf("%d",&N);
    int v[N], posicoes[N];
    
    printf("Insira o vetor");
    for (i=0;i<N;i++)
    {
        scanf("%d",&v[i]);
    }
    printf("Qual o valor que pretende procurar? ");
    scanf("%d",&x);
    if (procuraTodos(v,N,x,posicoes,&nPosicoes))
    {
        printf("O valor %d ocorre %d vezes nas posicoes: ",x,nPosicoes);
        for(i=0;i<nPosicoes;i++)
        {
            printf("%d ",posicoes[i]);
        }
        printf("\n");
    }
    else
        printf("O valor %d nao ocorre\n",x);
}

int procuraTodos(int *v, int N, int x, int posicoes[],int *nPosicoes)
{
    int i,j=0;
    for(i=0;i<N;i++)
    {
        if (v[i]==x)
        {
            posicoes[j]=i;
            j++;
            *nPosicoes+=1;
        }
    }
    if (*nPosicoes==0)
        return 0;
    return 1;
}
