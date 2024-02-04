#include <stdio.h>
#include <math.h>

void sum_v(float x[], float vret[],int n);
float imp(float x[],int n);
float range_v(float x[],int n);
void diff_v(float x[],float vret[], int n);
int main()
{   float x[15],vret[15];
    int i,n=0;
    for ( i = 0; i < 15; i++)
    {   
        printf("Introduza o elemento %d: \n",i+1);
        scanf("%f",&x[i]);
        if (x[i]==-1)
        break;    
    }    
printf("Tamanho do vetor: %d",i);printf("\n");
sum_v(x,vret,i);
printf("Vetor inicial :"); imp(x,i);printf("\n");
printf("Vetor Soma :");imp(vret,i/2);printf("\n");
printf("Gama :%.1f\n",range_v(x,i));

}



float imp(float x[],int n)
{   int i;
    printf(" Vetor:{");
    for ( i = 0; i < n; i++)
    {
        printf("%.1f ",x[i]);
    }
    printf("} \n");
 return 0;
}


void sum_v(float x[], float vret[],int n)
{   int i,j;
    for ( i = 0,j=0; i < n; i+=2,j++)
    {
        vret[j]=x[i]+x[i+1];
    }
}

float range_v(float x[],int n)
{   int i,j;
    float max,min;
    min=x[0];
    max=x[0];
    for ( i = 1; i <n; i++)
    {
        if (x[i]>max)
        {
            max=x[i];
        }
        
    }
    for ( j = 0; j <n; j++)
    {
        if (x[j]<min)
        {
            min=x[j];
        }   
    }
    printf("Maximo:%.2f \nMinimo:%.2f\n",max,min);
    return  max-min;
}

/*void diff_v(float x[],float vret[],int n) por concluir muito trabalhoso :)
{ int i;
    float media[n/2];
    for ( i = 0; i < n; i+=2)
    {
        media[i]=sqrt(x[i]+x[i+1]);
    }


    




}*/





