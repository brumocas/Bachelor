#include <stdio.h>

int *ordena(int* valorA, int* valorB)
{  int aux;
    if(*valorA>*valorB)
    return valorA;
    else return valorB;
}

int main()
{   int valorA,valorB;
    
    printf(" INTRODUZA DOIS VALORES : ");
    scanf("%d%d",&valorA,&valorB);
    int* x;
    x=ordena(&valorA,&valorB);
    printf(" O endereço do maior : %p \n",x);

    printf("Valor %d \n",* x);




}   