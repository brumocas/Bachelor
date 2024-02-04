#include <stdio.h>
/*rever exercicio*/

void imp_v(float v[],int n);
void ordena_v(float v[],int n);


int main ()
{   float v[15];
    int i,n=0;
    for ( i = 0; i < 15; i++)
    {   
        printf("Introduza o elemento %d: \n",i+1);
        scanf("%f",&v[i]);
        ordena_v(v,n);
        imp_v(v,n);
        n++;
        printf(" Tamanho do vetor:%d\n",n);
    }
}

void imp_v(float v[],int n)
{   int i;
    printf(" Vetor:{");
    for ( i = 0; i <= n; i++)
    {
        printf("%.1f ",v[i]);
    }
    printf("} \n");
}

void ordena_v(float v[], int n)
{
    float aux;
    int i, ordenado;
    do {
        ordenado = 1;
        for (i=0; i<n-1; i++)
        {
            if (v[i] > v[i+1])
            {
                aux=v[i+1];
                v[i+1]=v[i];
                v[i]=aux;
                ordenado = 0;
            }
        }
        n--;
    } while(ordenado != 1);

}
