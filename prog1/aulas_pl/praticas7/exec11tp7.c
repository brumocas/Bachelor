#include <stdio.h>
/*por terminar*/
int converte_codigo_cores(int cor, int pos, float *ret)
{
    if (12==pos)
    {   
        switch (cor)
        {
        case 0:
            *ret+=1;
            break;
        case 1:
            *ret+=10;
            break;
        case 2:
            *ret+=100;
            break;
        case 3:
            *ret+=1000;
            break;
        case 4:
            *ret+=10000;
            break;
        case 5:
            *ret+=100000;
            break;
        case 6:
            *ret+=1000000;
            break;
        case 7:
            *ret+=10000000;
            break;
        case 8:
            *ret+=100000000;
            break;
        case 9:
            *ret+=1000000000;
            break;    
        }
    }
}

int main()
{
    int cor, i;
    float resistor = 0.0;
    printf("Cores:\n 0 - Black \n 1 - Brown \n 2 - Red \n 3 - Orange \n 4 - Yellow \n 5 - Green \n 6 - Blue \n 7 - Violet \n 8 - Grey \n 9 - White\n\n");
    for(i = 1; i < 4; i++)
    {
        do{ 
            printf("Introduza a cor da banda %d: ", i);
            scanf("%d", &cor);
        }while((cor < -1) || (cor > 12));
    }
    
    converte_codigo_cores(cor,i,&resistor);
    
    printf("Valor da resistencia: %.2f Ohms\n", resistor);

}