#include <stdio.h>
#include <math.h>
 
 float distancia( float x1,float y1,float x2,float y2)
{   float dist;
    dist=(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
    return dist;
}
 
 float colisao(float x1,float y1, float x2 , float y2, float r1 , float r2 )
 {   int dc/*distancia ente centros*/,sr/* soma dos raios*/;
    dc=distancia(x1,y1,x2,y2);
    sr=r1+r2;
    if(dc<=sr)
    return 1;
    else return 0;
}

int main()
{   float x1,x2,y1,y2,r1,r2;
    printf(" Introduza o valor do centro da bola 1 e seu respetivo raio \n ");
    scanf("%f%f%f",&x1,&y1,&r1);
    printf(" Introduza o valor do centro da bola 2 e seu respetivo raio \n ");
    scanf("%f%f%f",&x2,&y2,&r2);
    if(colisao(x1,y1,x2,y2,r1,r2)==1)
    { 
        printf(" As duas bolas estão em colisão \n");
    }
    else
    { 
     printf(" As duas bolas não estão em colisão \n");   
    }
}