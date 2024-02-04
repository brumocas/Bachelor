#include <stdio.h> 
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    srand(time(NULL));
    int  lançamentos[30],i,valor,cont[5],p,s;

     for(i=0;i<30;i++)
        {
            valor=rand()%6+1;
            cont[valor-1]++;
           /*if(valor==1)
            cont[0]++;
            if(valor==2)
            cont[1]++;
            if(valor==3)
            cont[2]++;
            if(valor==4)
            cont[3]++;
            if(valor==5)
            cont[4]++;
            if(valor==6)
            cont[5]++;
            */
        }

    for ( p=0;p<6;p++)
    {   for ( s=0;s<cont[p]; s++)
        printf(" *");
        printf("\n");    
    }
}








