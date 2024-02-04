#include <stdio.h>
void preencher_vetor(float v[], int n)
{ 
    int i;       
    for ( i = 0; i< n; i++)
    {   printf(" Introduza o elemento %d : ",i+1);
        scanf("%f",&v[i]);
    }
}
float somatorio_vetor(float v[],int n)
{   int i;
    float soma=0; 
    for (i = 0; i < n; i++)
    {
        soma=soma +v[i];
    }
    return soma;
}
void imprimir_vetor(float v[],int n)
{ int i;
    printf("{");
    for (i = 0; i < n; i++)
    {
        printf(" %.2f ",v[i]);
    }
    printf("}");
    

}
int main()
{ float v[20];
    int n;
    printf(" Quantos elementos pretende armazenar ? \n");
    scanf("%d",&n);
    if (n>16)
    {
        printf(" O numero de elementoss está limitado a 15 ");
        printf(" Quantos elementos pretende armazenar ? \n");
        scanf("%d",&n);
    }
    else
    {   preencher_vetor(v,n);
        imprimir_vetor(v,n);
       printf(" O valor do sum é %.2f \n",somatorio_vetor(v,n));
    }
}