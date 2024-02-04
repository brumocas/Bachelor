#include <stdio.h>
#include <math.h>

float parabola(float a,float x)
{ float fx;
    fx=a*x*x;
    return fx;
}


int main()
{   float a,Min,Max,inc,i;
    printf(" Qual o valor de a ? \n ");
    scanf("%f",&a);
    printf(" Qual o intervalo ? \n");
    scanf("%f%f",&Min,&Max);
    printf(" Introduza qual o incremento \n ");
    scanf("%f",&inc);

    printf(" f(%.2f)=%.1f \n",Min,parabola(a,Min));
    printf(" f(%.2f)=%.1f \n",Min+inc,parabola(a,Min+inc));
    printf(" f(%.2f)=%.1f \n",Max,parabola(a,Max));
    




}