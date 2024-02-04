#include <stdio.h>

int compacta(char orig[],int N,char dst[]);

int main()
{
    int N=25,i;
    char orig[N], dst[N];

    printf("Escreva uma frase: ");
    for(i=0;i<N;i++)
    {
        scanf(" %c",&orig[i]);
        if (orig[i]=='.')   
            break;
    }

    if (compacta(orig,N,dst))
        {
            for(i=0;i<N;i++)
            {
                if (dst[i]=='.')
                    break;
                printf("%c",dst[i]);
            } 
            printf("\n");
        }
}

int compacta(char orig[],int N,char dst[])
{
    int i,j,k=1;
    dst[0]=orig[0];
    for(i=0;i<N;i++)
    {
        for(j=i+1;j<N;j++)
        {
            if(orig[i]!=orig[j])
            {
                dst[k]=orig[j];
                k++;
                i=j;
            }
        }
    }
    return 1;
}