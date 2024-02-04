#include <stdio.h>

int hoursMin(int totalMins, int *hours, int *minutes)
{
    *hours = *hours / 60;
    *minutes = totalMins % 60;
    if (totalMins > 1440)
        return 1;
    else
        return 0;
}

int main()
{
    int totalMins, hours, minutes;
    printf(" Insira o total de minutes \n");
    scanf("%d", &totalMins);
    hoursMin(totalMins, &hours, &minutes);
    printf(" %d correspodem a %d : %d ", totalMins, hours, minutes);
    if (hoursMin(totalMins, &hours, &minutes))
    {
        printf(" superior a 1 dia \n ");
    }
    else
    {
        printf(" inferior a 1 dia \n");
    }
}