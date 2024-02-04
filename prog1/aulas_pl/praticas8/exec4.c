#include <stdio.h>
/*resolvido*/
void ordena(float notas[][2], int N)
{   int i,aux;
    for ( i = 0; i < N; i++)
 {
     if (notas[i][1]<notas[i+1][1])
    {   aux=notas[i][1];
        notas[i][1]=notas[i+1][1];
        notas[i+1][1]=aux;
    } 
 }
}
int main()
{   int n,i,m,k,j;
    printf("Introduza quantos alunos pretende \n");
    scanf("%d",&n);
    float notas[n][2];
    printf("Introduza os dados de cada aluno ");
    for ( i = 0; i < n; i++)
    {
        for ( j = 0; j < 2; j++)
        {
        scanf("%f",&notas[i][j]);
        }
        
    }
    ordena(notas,n);
     
    
    printf("Numero\tMédia\n");
    
    for ( m = 0; m < n; m++)
    {   
        for ( k = 0; k < 2; k++)
        {
            printf("%.1f\t",notas[m][k]);
           
        }
        printf("\n");
    }
}