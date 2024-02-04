#include <stdio.h>

void quociresto(int dividendo, int divisor, int *quociente, int *resto);
int soma(int n);
int par(int n);

int main()
{   int x,y,quociente,resto,n;
    printf(" Introduza o dividendo e o divisor \n");
    scanf("%d %d",&x,&y);
    quociresto(x,y,&quociente,&resto);
    printf("Quociente:%d resto:%d\n",quociente,resto);
    printf("Introduza um numero para ver quantos digitos apresenta e se e par ou impar\n");
    scanf("%d",&n);
    printf("%d e a soma dos digitos de %d\n",soma(n),n);
    if (par(n))
    {
        printf("O numero é par \n");   
    }
    else
    {
        printf("O numero é impar \n");
    }
}

void quociresto(int dividendo, int divisor, int *quociente, int *resto)
{   int i,aux=0;
    for ( i = 0; dividendo>=divisor ; i++)
    {
        dividendo-=divisor;
        aux++;       
    }
    *resto=dividendo;
    *quociente=aux;
}

int soma(int n)
{   int i,sum=0;
    for ( i = 0; i <n; i++)
    {
        sum+=n%10;
        n=n/10;
    }
    return sum;
}
int par(int n)
{
    if (n%2==0)
    {
        return 1;
    }
    else return 0;
}
