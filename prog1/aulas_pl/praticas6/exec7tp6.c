#include <stdio.h>
/*duvida no exercicio*/
#define DIM 100
void multiEspecial(int n, int v1[], int v2[], int vRet[]);
void imp_v(int v[],int n);




int main ()
{ int v1[DIM],v2[DIM],vret[DIM],i,j,n=0;
    printf("Multiplicação especial\n");
    printf("Introduza o vetor 1:\n");
    
    for ( i = 0; i <DIM; i++)
    {   scanf("%d",&v1[i]);
        n=i+1;
        if (v1[i]==0)
        {   n--;
            break;
        }   
    }
    
    printf("Tamanho do vetor:%d",n);
    printf("\nIntroduza o vetor 2:\n");
    for ( j = 0; j <n; j++)
    {
        
        scanf("%d",&v2[j]);
    }
    imp_v(v1,n);
    imp_v(v2,n);
    multiEspecial(n,v1,v2,vret);
    imp_v(vret,n);
    
}   
void multiEspecial(int n, int v1[], int v2[], int vRet[])
{   int j;
    
    for ( j = 0; j < n; j++)
    {
        vRet[j]=v1[j]*v2[n-j-1];
    }
    
}
void imp_v(int v[],int n)
{   int i;
    printf(" Vetor:{");
    for ( i = 0; i < n; i++)
    {
        printf("%d ",v[i]);
    }
    printf("} \n");
}