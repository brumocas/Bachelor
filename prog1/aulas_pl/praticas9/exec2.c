#include <stdio.h>
#include <string.h>
#define DIM 100
int conta(char *frase, char *palavra);


int main()
{ char so[DIM],procura[DIM];
printf("Introduza a frase a ser pesquisada\n");
fgets(so,DIM,stdin);
printf("Introduza a palavra a procurar\n");
scanf("%s",procura);
printf("A palavra %s aparece %d vezes na frase ",procura,conta(so,procura));
}


int conta(char *frase, char *palavra)
{   int vezes=0;
   while (1)
   {   
        {   frase=(strstr(frase,palavra));
                if (frase==NULL)
                break;  
            frase++;
            vezes++;
        }    
   }
return vezes; 
}

/*int conta(char *frase, char *palavra)
{   int vezes=0;char *fraseorig;
   while (1)
   {    fraseorig=frase; 
        frase=strstr(frase,palavra);
        if (frase==fraseorig)
        {   
            frase++;
            vezes++;
        }
        else
        {   frase=strstr(frase,palavra);
            if (*(frase-1)&&*(frase+1)==" ")
            {   
                frase++;
                vezes++;

            }
        }
        if (frase==NULL)
        {
            break;
        }   
        frase++;
        vezes++;
   }
return vezes; 
} */