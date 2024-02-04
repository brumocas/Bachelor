#include <stdio.h>

float avg(float x[]);
float max(float x[]);
float min(float x[]);
float imp(float x[]);

int main()
{   float x[10];
    int i;
    for ( i = 0; i < 10; i++)
    {
        printf("Introduza o elemento %d: \n",i+1);
        scanf("%f",&x[i]);
    }
printf("Media:%.2f\n",avg(x));
printf("Maximo:%.2f\n",max(x));
printf("Minimo:%.2f\n",min(x));
imp(x);

}

float avg(float x[])
{   int i;
    float soma=0;
    for ( i = 0; i <10; i++)
    {   
        soma=soma+x[i];
    }
    return (soma/10);
}

float max(float x[])
{   int i;
    float maximo;
    maximo=x[0];
    for ( i = 0; i < 10; i++)
    {
        if(x[i]>maximo)
        maximo=x[i];
    }
    return maximo;
}
float min(float x[])
{   int i;
    float minimo;
    minimo=x[0];
    for ( i = 0; i < 10; i++)
    {
        if(x[i]<minimo)
        minimo=x[i];
    }
    return minimo;
}

float imp(float x[])
{   int i;
    printf(" Vetor:{");
    for ( i = 0; i < 10; i++)
    {
        printf("%.2f ",x[i]);
    }
    printf("} \n");
 return 0;
}



