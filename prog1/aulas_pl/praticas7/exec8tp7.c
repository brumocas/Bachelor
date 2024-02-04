#include <stdio.h>

int contanum(int num,int dig)
{   int vezes=0,i=0;
    while(num!=0)
    {  /*printf("%d\n",num);*/
        if ((num%10)==dig)
        {
            vezes++;
        }
        num=num/10;
    }
    return vezes;
}

int main()
{   int num,dig,vezes=0;
    printf("Introduza um numero inteiro\n");
    scanf("%d",&num);
    printf("Introduza o digito a procurar\n");
    scanf("%d",&dig);
    printf("O numero %d aparece %d vezes no numero :%d ",dig,contanum(num,dig),num);
}