#include <stdio.h>
#include <math.h>

float distancia( float x1,float y1,float x2,float y2)
{   float dist;
    dist=(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
    return dist;
}

int main()
{   float x1,x2,y1,y2;
    printf(" Introduza as coordenadas do ponto 1 \n ");
    scanf("%f %f",&x1,&y1);
    printf(" Introduza as coordenadas do ponto 2 \n ");
    scanf("%f %f",&x2,&y2);
    printf(" A distancia entre 2 pontos é %.2f \n",distancia(x1,y1,x2,y2));

}