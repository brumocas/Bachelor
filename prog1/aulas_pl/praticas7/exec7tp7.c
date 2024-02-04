#include <stdio.h>
int main()
{
int i, j, *p_1, *p_2, **p_p_1, **p_p_2;
i = 4;
j = 5;
p_1 = &i;
p_2 = &j;
p_p_1 = &p_2;
p_p_2 = &p_1;
}


/*  Variavel i     j     p_1    p_2      p_p_1     p_p_2
    Value     4     5
    Endereco 1000 1007 1030   1053     1071        1079/


Express~ao      i    *p_2       &i    &p_2    **p_p_1     *p_p_2  &(*p_1)     j   *p_1    *(&p_1)
Resultado       4      5        1000  1053                         1030       5    4         4                */