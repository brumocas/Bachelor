#include <stdio.h>                 
#include <stdlib.h>
#define NAME_SIZE 20
#define DIAS_MAX 30
#define TAREFAS_MAX 30

typedef struct tarefa{

    char nome[NAME_SIZE];
    int trabalhadoresPorDia[DIAS_MAX];
    int diasPorTarefa;

}tarefa;

int maxVetorIndex(int d[], int n);
int maisTrabalhadores(tarefa taref[], int n);

    int main(){

        tarefa tarefas[30];
        int numTarefas = 0;

        FILE *inputFile;
        inputFile = fopen("ficheiro.txt", "r");
        
        int dias[DIAS_MAX] = {0};
        int fimTarefa[DIAS_MAX] = {0};

    for (numTarefas = 0; numTarefas < TAREFAS_MAX; numTarefas++)
    {
        int j = 0;
        if(fscanf(inputFile, "%s", tarefas[numTarefas].nome) == EOF) break;
        while(1)
        {
            fscanf(inputFile, "%d", &tarefas[numTarefas].trabalhadoresPorDia[j]);
            if(tarefas[numTarefas].trabalhadoresPorDia[j] == -1) {
                fimTarefa[j-1]++;
                break;
            }
            dias[j]++;
            j++;
        }
        tarefas[numTarefas].diasPorTarefa = j + 1;
        dias[j]++;
    }

    for (int i = 0; i < numTarefas; i++)
    {
        printf("tarefa %s:", tarefas[i].nome);
        
        for (int j = 0; j < tarefas[i].diasPorTarefa; j++)
        {
            printf(" %d", tarefas[i].trabalhadoresPorDia[j]);
        }
        printf("\n");
    }    

    printf("\n");
    
    printf("Dia com mais tarefas: %d\n", maxVetorIndex(dias, DIAS_MAX));
    printf("Dia com mais tarefas terminadas: %d\n", maxVetorIndex(fimTarefa, DIAS_MAX));
    printf("Tarefa com maior numero de trabalhadores: %s\n", tarefas[maisTrabalhadores(tarefas, numTarefas)].nome);

return 0;
}

int maisTrabalhadores(tarefa taref[], int n){
    int max = 0, sum = 0;
    int returnVAl = 0;
    
    for (int i = 0; i < n; i++)
    {
        sum = 0;
        for (int j = 0; j < taref[i].diasPorTarefa - 1; j++)
        {
            sum += taref[i].trabalhadoresPorDia[j];
        }
        if(sum > max){
            max = sum;
            returnVAl = i;
        }
    }
    return returnVAl;
}

int maxVetorIndex(int d[], int n){
    int max = 0, dmax = 0;

    for (int i = 0; i < n; i++)
    {
        if (d[i] > max){
            max = d[i];
            dmax = i;
        }    
    }

return dmax + 1;
}
