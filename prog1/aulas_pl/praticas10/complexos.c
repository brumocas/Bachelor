#include "complexos.h"
#include <math.h>
#include <stdio.h>

 complexo leComplexo()
{   complexo c;
    printf("Introduza a parte real do num : ");
    scanf("%f",&c.real);
    printf("Introduza a parte imaginaria do num : ");
    scanf("%f",&c.imaginaria);
    return c;
}



void escreveComplexo(complexo c)
{       
    printf("z= %.2f + i%.2f \n",c.real,c.imaginaria);
}

complexo somaComplexo(complexo c1, complexo c2)
{   complexo soma;

    soma.real=c1.real+c2.real;
    soma.imaginaria=c1.imaginaria+c2.imaginaria;
return soma;
}

double modComplexo(complexo c)
{ double modulo;

    modulo=sqrt((c.real*c.real)+(c.imaginaria*c.imaginaria));
 return modulo;
}

double argComplexo(complexo c)
{   double arg;
 
    arg=atan(c.imaginaria/c.real);
    return arg;
}