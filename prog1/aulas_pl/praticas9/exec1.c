#include <stdio.h>
#include <string.h> 

/*ao escrever ola aparece @/como retirar o espaço na inversao/string aprasenta mais um valor que esperado/ersolvido inicializar a zero */

void inverte(char *strOriginal, char *strInvertida,int n);

int main()
{   char so[100]={0},si[100]={0},n=0;
    printf("Introduza uma frase para ser invertida \n");
    fgets(so,100,stdin);
    n=strlen(so);
    printf("Comprimento da string é %d\n",n);
    inverte(so,si,n);
    puts(si);

}

void inverte(char *strOriginal, char *strInvertida,int n)
{ int i;
    
    for ( i = 0; i <n; i++)
    {
        strInvertida[i]=strOriginal[n-i-1];
    }
    
}