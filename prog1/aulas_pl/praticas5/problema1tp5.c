#include <stdio.h>
#include <stdlib.h>
int main()
{int nl/*numero de lançamentos*/,i,v6=0/*numero de vezes que 6 aparece*/;
printf("Quantos lançamentos \n");
scanf("%d",&nl);

for (  i= 0; i <nl; i++)
{  
    if((rand()%6)==5)
    {v6++;}

}
printf(" A face 6 saiu %d vezes ",v6);

}