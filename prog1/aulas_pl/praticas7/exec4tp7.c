#include <stdio.h>
#include <math.h>

void cartesianas_polares(float x, float y, float *r, float *theta);
void polares_cartesianas(float r, float theta, float *x, float *y);

int main ()
{   float x,r,y,o;
    char tipo;
    printf("Programa que traduz coordenadas polares em cartesianas e vice-versa\n");
    printf("Que tipo de coordenadas pretende polares ou cartesianas?\nIntroduza p para polares e c para cartesianas\n");
    scanf("%c",&tipo);
    switch (tipo)
    {
    case 'p': 
        printf("Introduza as suas coordenadas polares na forma r o \n");
        scanf("%f %f",&r,&o);
        polares_cartesianas(r,o,&x,&y);
         printf("As coordenadas na forma cartesiana são x:%.2f y:%.2f",x,y);
        break;
    case 'c':
        printf("Introduza as suas coordenadas cartesianas na forma x y \n");
        scanf("%f %f",&x,&y);
        cartesianas_polares(x,y,&r,&o);
        printf("As coordenadas na forma polar são r:%.2f o:%.2f",r,o);
        break;
    }
}

void cartesianas_polares(float x, float y, float *r, float *theta)
{   
    *r=sqrt(x*x+y*y);
    *theta=atan2(x,y);


}
void polares_cartesianas(float r, float theta, float *x, float *y)
{
    *x=r*cos(theta);
    *y=r*sin(theta);
}