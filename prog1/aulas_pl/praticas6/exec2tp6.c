#include <stdio.h>
#include <math.h>

int contaRepeticao(int v[], int tamanhoVetor, int numero)
{   
    int p=1,max=0,i,x=0;
    for (i=0; i<tamanhoVetor;i++)
    {   
        if(v[i]==numero)    
            {   
                if(v[i]==v[i+1])
                p++;
                if(p>max)
                max=p;
            }          
        else p=1;
    }
    return max;
}




int main()
{ int vetor[100],i,tamanhoVetor,numero;
    
    for ( i=0; i<100; i++)
    {
        printf("Introduza um numero:\n");
        scanf("%d",&vetor[i]);
        if (vetor[i]<0)
        {
            break;
        }
        tamanhoVetor=i+1;
    }
printf("Introduza o numero a pesquisar no vetor : \n");
scanf("%d",&numero);
printf("Maior sequencia com numeros %d tem tamanho %d \n",numero,contaRepeticao(vetor,tamanhoVetor,numero));





}