#include <stdio.h>
#include "complexos.h"



int main()
{   complexo c1,c2,soma;

    c1=leComplexo();
    escreveComplexo(c1);
    c2=leComplexo();
    escreveComplexo(c2);
    soma=somaComplexo(c1,c2);
    printf("O modulo de C1 : %.2f \n", modComplexo(c1));
    printf("O modulo de C2 : %.2f \n", modComplexo(c2));
    printf("O argumento angular de c1 e %.2f \n",argComplexo(c1));
    printf("O argumento angular de c2 e %.2f \n",argComplexo(c2));

}

