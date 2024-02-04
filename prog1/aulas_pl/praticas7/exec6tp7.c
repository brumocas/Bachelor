#include <stdio.h>
int main()
{
int var1 = 5;
char var2 ='a';
int *ptr1 = &var1;
char *ptr2;
ptr2 =&var2;
printf("var1 tem o endereco %p e o valor %d\n", ptr1, *ptr1);
printf("var2 tem o endereco %p e o valor %d\n", ptr2, *ptr2);
}
