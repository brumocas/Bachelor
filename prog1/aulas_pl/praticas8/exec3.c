#include <stdio.h>
int main()
{   float num[4][4];
    int m,k,i,j;
    
    printf(" Introduza os dados do aluno\n");
    for ( i = 0; i <4; i++)
    {   for ( j = 0; j <3; j++)
        {
         scanf("%f",&num[i][j]);

        }
        num[i][3]=(num[i][1]+num[i][2])/2;    
    }
    printf("Numero\t\tNota1\tNota2\tMedia\n");
    for ( m = 0; m < 4; m++)
    {   
        for ( k = 0; k < 4; k++)
        {
            printf("%0.2f\t",num[m][k]);
           
        }
        printf("\n");
    }
}