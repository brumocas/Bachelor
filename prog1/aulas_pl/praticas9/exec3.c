#include <stdio.h>

int multiplicacao(int primeiroNumero, int segundoNumero);

int main()
{   int a,b;
    printf("Calcula da multiplicaçao recursivamente\n");\
    printf("Introduza o 1 num :\n");
    scanf("%d",&a);
    printf("Introduza o 2 num :\n");
    scanf("%d",&b);
    printf("A multiplicacao e : %d\n",multiplicacao(a,b));
}

int multiplicacao(int primeiroNumero, int segundoNumero)
{   int aux=0;
    if (segundoNumero==0)
    return aux;
    else
    {   segundoNumero--;
        return aux=primeiroNumero+multiplicacao(primeiroNumero,segundoNumero);
    }  
}