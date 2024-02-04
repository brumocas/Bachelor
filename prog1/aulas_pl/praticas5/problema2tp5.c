#include <stdio.h>

float pesoideal(float altura,char sexo)
{   float pi;
    switch (sexo)
    {
    case 'M':
    { 
    pi=72.7*altura-58;
    break;
    }
    case 'F':
    { 
    pi=62.1*altura-44.7;
    break;
    }
    }
return pi;
}

int main()
{float a;
 char s;
printf(" introduza a sua altura \n");
scanf("%f",&a);
printf(" introduza o seu sexo \n");
scanf(" %c",&s);
printf("O seu peso ideal seria %.2f \n",pesoideal(a,s));

}