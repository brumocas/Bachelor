#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define LOOPLENGTH 8
#define PI  3.14159265358979323846

int main()
{ 

int rx_sample_L;
int rx_sample_R;
int tx_sample_L;
int tx_sample_R;
int sine_table[LOOPLENGTH] = {0, 7071, 10000, 7071, 0, -7071, -10000, -7071};
int sine_ptr_L = 0; int sine_ptr_R = LOOPLENGTH/4; // pointers into lookup table

printf("LEFT\n");

for  (int i = 0; i < 8 ; i++)
{
    tx_sample_L = sine_table[sine_ptr_L];
    tx_sample_R = sine_table[sine_ptr_R];
    sine_ptr_L = (sine_ptr_L+1)%LOOPLENGTH;
    sine_ptr_R = (sine_ptr_R+1)%LOOPLENGTH;
    tx_sample_R = tx_sample_R * tx_sample_L;
    printf("%i\n",tx_sample_L);
    //printf("tx_sample_R =  %i\n",tx_sample_R);

}

printf("\nRIGHT\n");

for  (int i = 0; i < 8 ; i++)
{
    tx_sample_L = sine_table[sine_ptr_L];
    tx_sample_R = sine_table[sine_ptr_R];
    sine_ptr_L = (sine_ptr_L+1)%LOOPLENGTH;
    sine_ptr_R = (sine_ptr_R+1)%LOOPLENGTH;
    tx_sample_R = tx_sample_R * tx_sample_L;
    //printf("tx_sample_L =  %i\n",tx_sample_L);
    printf("%i\n",tx_sample_R);

}

printf("\nRIGHT ANALYTIC\n");

for (int i =0 ; i < 8 ; i++)
{

    printf("%f\n",10000 * sin(PI*i/4) * 10000 * sin ((PI*i/4) +  PI/2));

}

}
