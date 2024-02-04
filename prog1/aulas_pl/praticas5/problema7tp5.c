#include <stdio.h>
int main()
{  int a,b,y=0,x,i;/*multiplicação entre 2 operandos sem usar o símbolo*/
    printf("Introduza um operando \n ");
    scanf("%d",&a); 
    printf("Introduza um operando base 2 : \n");
    scanf("%d",&b);
    for (i = 0; i <b; i++)
    {
        while((b%2==0) && b!=0)
        {   b=b/2;
            y++;
        }
    
    }
    x=a<<y;
    printf("O resultado da multiplicação é %d \n",x);
   
}