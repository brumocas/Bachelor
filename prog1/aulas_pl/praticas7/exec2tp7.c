#include <stdio.h>
    /*Se a>b && b>c, então introduzir a,b,c. 
    Se a>c && c>b, então introduzir a,c,b. 
    Se b>c && c>a, então introduzir b,c,a, 
    Se b>a && a>c, então introduzir b,a,c. 
    Se c>a && a>b, então introduzir c,a,b. 
    Se c>b && b>a, então introduzir c,b,a. */ 
void ordena(int *a, int *b, int *c)     
{ int aux,aux1;
    if (*a>*c && *c>*b)
    {
    aux=*c;
    *c=*b;
    *b=aux;
    }
    if (*b>*c && *c>*a)
    {
    aux=*a;
    *a=*b;
    *b=*c;
    *c=aux;
    }
    if(*b>*a && *a>*c)
    {
    aux=*a;    
    *a=*b;
    *b=aux;
    }
    if (*c>*a && *a>*b)
    {   aux=*a;
        aux1=*b;
        *a=*c;
        *b=aux;
        *c=aux1;
    }
    if (*c>*b && *b>*a)
    {
        aux=*a;
        *a=*c;
        *c=aux;
    }
}
int main()
{   int a,b,c;
    printf("Insira os valores a ordenar: ");
    scanf("%d%d%d",&a,&b,&c);
    ordena(&a,&b,&c);
    printf(" Os valores a,b,c por ordem crescente são %d %d %d ",c,b,a);
}