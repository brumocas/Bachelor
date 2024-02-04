#include <stdio.h>
#include <math.h>

int mdc(int x, int y);

int main()
{   int a,b;
    printf("Introduza o num a \n");
    scanf("%d",&a);
    printf("Introduza o num b \n");
    scanf("%d",&b);
    printf("o mdc entre %d e %d e %d\n",a,b,mdc(a,b));

}

int mdc(int x, int y)
{   if (y==0)
    return x;
    else
    return mdc(y,(x%y));
}