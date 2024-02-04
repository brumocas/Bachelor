#include "IO.c"
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#define DEBUG
//#undef DEBUG_ESTADOS


//--------------Seccao do Tempo-------------------------------------//

//Definir um tipo para o temporizador
typedef struct{
	bool on;
	uint64_t time;	                   
} timerBlock;

// Tempo de ciclo
uint64_t scan_time = 100; // 1 milissegundo 

//Declarar Temporizadores	
timerBlock timer10,timer2,timer15;

//Declarar variaveis para calculo do tempo de ciclo
uint64_t start_time = 0, end_time = 0, cycle_time = 0;

//Funcoes de controlo de tempo

void update_timers(){

	//Calcula o tempo de ciclo
    end_time = get_time();
    
    if (start_time == 0)
    cycle_time = 0;
    else
    cycle_time = end_time - start_time;

	//o fim do ciclo atual e o inicio do proximo ciclo
    start_time = end_time;

	//Atualiza os timers todos
	if(timer10.on)
		timer10.time = timer10.time + cycle_time;

    if(timer2.on)
		timer2.time = timer2.time + cycle_time;

    if(timer15.on)
		timer15.time = timer15.time + cycle_time;

}

void start_timer(timerBlock* t){
    t->on = true;
    t->time = 0;

}

void stop_timer(timerBlock* t){
	t->on = false;
	t->time = 0;	
	
	// t->time pode ser comentado 
  	// se for importante guardar
  	// o valor do temporizador
}

//------------------------------------------------------------------//






//-----------------Maquinas de Estado------------------------------//

//Estados da maquina de estados modo de funcionamento
typedef enum {
	PARADO,
	PRESSIONADO_START,
	OPERAR,
	PRESSIONADO_STOP,
	A_PARAR,
	T1_LIVRE
} MODO_FUNCIONAMENTO;

//Estados da maquina de estados iluminacao
typedef enum{
	INICIO_ILUMINACAO,
	LIGADA,
	DESLIGADA
} ILUMINACAO;

//Estados da maquina de estados separacao de pecas
typedef enum{
	INICIO_SEPARACAO,                
	TAPETE_1_MOVE,
	SV_DETETA,
	INICIO_TRANSF,
	PECA_FORA_T1,
	ZONA_PUSHER,
	LEVAR_ATE_R1,
	LEVAR_ATE_R2,
	ATIVA_BRACO,
	PECA_NO_T3,
	ESVAZIA_TAPETES
} SEPARACAO_PECAS;

//Estados da maquina de estados Conta pecas
typedef enum{
	CONTA_AZUL,
	CONTA_VERDE
} CONTA_PECAS;

// Inicializacao de estados
MODO_FUNCIONAMENTO p_CurrentstatMF, CurrentstateMF, aux_CurrentstateMF;
ILUMINACAO  p_CurrentstateI, CurrentstateI, aux_CurrentstateI;
SEPARACAO_PECAS p_CurrentstateSP, CurrentstateSP, aux_CurrentstateSP;
CONTA_PECAS Currentstate4, Currentstate5;

//Variaveis globais
int conta_a = 0, conta_b = 0, cor = 0, cor_aux = 0;

//Guardar flancos anteriores
bool p_STOP = false, p_START = false, p_STR = false , p_SV = false, p_ST3 = false, p_ST2 = false;

//Variaveis auxiliar para guardar flancos
// re -> rising edge
// fe -> faling edge
bool re_STOP = false, re_START = false, re_STR = false, re_SV = false, fe_STOP = false, fe_START = false, fe_STR = false, fe_SV = false;
bool fe_ST3 = false, fe_ST2 = false, re_ST2 = false;

// Funcoes do tempo
void update_timers();
void start_timer( timerBlock *t);
void stop_timer(timerBlock *t);

//Funcoes para as transicoes das ME's
void ME_MODO_FUNCIONAMENTO();
void ME_ILUMINACAO();
void ME_SEPARACAO_PECAS();
void ME_CONTA_PECAS();
void sync_ME();
void init_ME();
void edge_detection();


void ME_MODO_FUNCIONAMENTO(){

	switch(CurrentstateMF){
		case PARADO:
			if(re_START == true)
				aux_CurrentstateMF = PRESSIONADO_START;
			break;
		case PRESSIONADO_START:
			if(fe_START == true)
				aux_CurrentstateMF = OPERAR;
			break;
		case OPERAR:
			if(fe_STOP == true)
				aux_CurrentstateMF = PRESSIONADO_STOP;
			break;
		case PRESSIONADO_STOP:
			if(re_STOP == true)
			{
				aux_CurrentstateMF = A_PARAR;
				start_timer(&timer10);
			
			}
			break;
		case A_PARAR:
			if((SV == false) && (timer10.time >= 10000))  
			{	
				aux_CurrentstateMF = T1_LIVRE;
				stop_timer(&timer10);

			}		
			else if( timer10.time >= 10000)
			{
				start_timer(&timer10);
			}

			break;
		case T1_LIVRE:
			if(CurrentstateSP == INICIO_SEPARACAO) 
			{
				aux_CurrentstateMF = PARADO;
			}
			break;
		}

}

void ME_ILUMINACAO(){

	switch (CurrentstateI){
		case INICIO_ILUMINACAO:
			if(CurrentstateMF == A_PARAR )
			{	
				aux_CurrentstateI = LIGADA;
				start_timer(&timer2);

			}
			break;
		
		case LIGADA:
			if(CurrentstateMF == OPERAR || CurrentstateMF == PARADO)
				aux_CurrentstateI = INICIO_ILUMINACAO;
			
			if(timer2.time >= 2000)
			{
				aux_CurrentstateI = DESLIGADA;
				start_timer(&timer2);
			}
			break;
		
		case DESLIGADA:
			if(CurrentstateMF == OPERAR || CurrentstateMF == PARADO)
				aux_CurrentstateI = INICIO_ILUMINACAO;	
			
			if(timer2.time >= 2000)
			{
				aux_CurrentstateI = LIGADA;
				start_timer(&timer2);
			}
			break;
		}

}
			
void ME_SEPARACAO_PECAS(){
	
	switch (CurrentstateSP){
		case  INICIO_SEPARACAO:
			if(CurrentstateMF == OPERAR || CurrentstateMF == A_PARAR)
				aux_CurrentstateSP = TAPETE_1_MOVE;
			break;
		
		case TAPETE_1_MOVE:
			if(re_SV == true )
			{	
				cor_aux = SV;
				aux_CurrentstateSP = SV_DETETA;
			}
			if (CurrentstateMF == T1_LIVRE)
			{
				aux_CurrentstateSP = ESVAZIA_TAPETES;
				start_timer(&timer15);
			}
			
			break;
		
		case SV_DETETA:
			if(re_STR == true)
				aux_CurrentstateSP = INICIO_TRANSF;
			break;
		
		case INICIO_TRANSF:
			if(fe_SV == true)
				aux_CurrentstateSP = PECA_FORA_T1;
			break;
		
		case PECA_FORA_T1:
			if( fe_STR == true){
				aux_CurrentstateSP = ZONA_PUSHER;
				cor = cor_aux;
			}
			break;
		
		case ZONA_PUSHER:
			if(cor == 1)
				aux_CurrentstateSP = LEVAR_ATE_R1;
			if(cor == 4) 
				aux_CurrentstateSP = ATIVA_BRACO;
			break;
		
		case LEVAR_ATE_R1:
			if(CurrentstateMF == OPERAR && fe_ST2 == true) 
				aux_CurrentstateSP = TAPETE_1_MOVE;

			if(CurrentstateMF == T1_LIVRE)
			{
				aux_CurrentstateSP = ESVAZIA_TAPETES;
				start_timer(&timer15);
			}

			if(CurrentstateMF == A_PARAR && fe_ST2 == true) 
				aux_CurrentstateSP = TAPETE_1_MOVE;

			break;
		
		case ATIVA_BRACO:
			if(SPE == 1)
				aux_CurrentstateSP = PECA_NO_T3;
			break;
		
		case PECA_NO_T3:
			if(SPR == 1)
				aux_CurrentstateSP = LEVAR_ATE_R2;
			break;
		
		case LEVAR_ATE_R2:
			if(CurrentstateMF == OPERAR && fe_ST3 == true)
				aux_CurrentstateSP = TAPETE_1_MOVE;
			
			if(CurrentstateMF == T1_LIVRE )
			{
				aux_CurrentstateSP = ESVAZIA_TAPETES;
				start_timer(&timer15);
			}	

			if(CurrentstateMF == A_PARAR && fe_ST3 == true) 
				aux_CurrentstateSP = TAPETE_1_MOVE;
		
		case ESVAZIA_TAPETES:
			if(timer15.time >= 15000)
			{
				aux_CurrentstateSP = INICIO_SEPARACAO;
				stop_timer(&timer15);
			}
			break;
		
		}


 }

void ME_CONTA_PECAS(){

    // Maquina Pecas Azuis
    if(Currentstate4 ==  CONTA_AZUL)
    {
        if(CurrentstateMF == OPERAR && p_CurrentstatMF != OPERAR)
        conta_a = 0;

        if(fe_ST2 == true)
        conta_a++;
        
    }

    // Maquina Pecas Verdes
    if(Currentstate5 ==  CONTA_VERDE)
    {
        if(CurrentstateMF == OPERAR && p_CurrentstatMF != OPERAR) 
        conta_b = 0;

        if(fe_ST3 == true)
        conta_b++;
    }
}

void sync_ME(){

	//MODO_FUNCIONAMENTO
	//Atualizar valor anterior do estado seguinte que e o estado atual
	p_CurrentstatMF = CurrentstateMF;
	CurrentstateMF = aux_CurrentstateMF;
	

	//ILUMINACAO
	p_CurrentstateI = CurrentstateI;
	CurrentstateI = aux_CurrentstateI;
	

	//SEPARACAO_PECAS	
	p_CurrentstateSP = CurrentstateSP;
	CurrentstateSP = aux_CurrentstateSP;
	


}

void edge_detection()
{	
	//Flancos Ascendentes START
	if( p_START == false && START == true)
		re_START = true;
	else
		re_START = false;

	//Flancos descendentes START
	if( p_START == true && START == false)
		fe_START = true;
	else
		fe_START = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_START = START;

	
	//Flancos Ascendentes STOP
	if( p_STOP == false && STOP == true)
		re_STOP = true;
	else
		re_STOP = false;

	//Flancos descendentes STOP
	if( p_STOP == true && STOP == false)
		fe_STOP = true;
	else
		fe_STOP = false;

 	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_STOP = STOP;

	//Flancos Ascendentes SV
	if( p_SV == false && SV != false) //true == 0 e as vezes SV = 4
		re_SV = true;
	else
		re_SV = false;

	//Flancos descendentes SV
	if( p_SV != false && SV == false)
		fe_SV = true;
	else
		fe_SV = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_SV = SV;

	//Flancos Ascendentes STR
	if( p_STR == false && STR == true)
		re_STR = true;
	else
		re_STR = false;

	//Flancos descendentes STR
	if( p_STR == true && STR == false)
		fe_STR = true;
	else
		fe_STR = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_STR = STR;

	//Flancos descendentes ST3
	if( p_ST3 == true && ST3 == false)
		fe_ST3 = true;
	else
		fe_ST3 = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_ST3 = ST3;

	//Flancos ascendentes ST2
	if( p_ST2 == false && ST2 == true)
		re_ST2 = true;
	else
		re_ST2 = false;

	//Flancos descendentes ST2
	if( p_ST2 == true && ST2 == false)
		fe_ST2 = true;
	else
		fe_ST2 = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_ST2 = ST2;

}

void init_ME(){

	CurrentstateMF = PARADO;
	CurrentstateI = INICIO_ILUMINACAO;
	CurrentstateSP = INICIO_SEPARACAO;
	Currentstate4 = CONTA_AZUL;
	Currentstate5 = CONTA_VERDE;

}


//---------------------------------------------------------------//


//-----------------------Main----------------------------------//
int main(){

	//start timer
	
	init_ME();

	while(1){

	//Atualiza Temporizadores
	update_timers();

	//Ler entradas 
	read_inputs();

	//Deteta flancos
	edge_detection();
	
	//Implemetar Maquinas de estado               
	
	ME_MODO_FUNCIONAMENTO();
	ME_ILUMINACAO();
	ME_SEPARACAO_PECAS();
	ME_CONTA_PECAS();

	//Atualizar estados apenas no fim do ciclo
	sync_ME();

	//Definir as saidas boleanas

	//MODO FUNCIONAMENTO
	LSTOP = (CurrentstateMF == PARADO ) || (CurrentstateMF == PRESSIONADO_START );
	LSTART = (CurrentstateMF == OPERAR) || (CurrentstateMF == PRESSIONADO_STOP);
	E1 = (CurrentstateMF == OPERAR ) || (CurrentstateMF == PRESSIONADO_STOP );
	

	//ILUMINACAO
	LWAIT = (CurrentstateI == LIGADA);

	//SEPARACAO DE PECAS
	T1A = (CurrentstateSP == TAPETE_1_MOVE) || (CurrentstateSP == SV_DETETA) || (CurrentstateSP == INICIO_TRANSF) || (CurrentstateSP == PECA_FORA_T1);
	T2A = (CurrentstateSP == INICIO_TRANSF) || (CurrentstateSP == LEVAR_ATE_R1) || (CurrentstateSP == PECA_FORA_T1) || (CurrentstateSP == ESVAZIA_TAPETES);
	T3A = (CurrentstateSP == LEVAR_ATE_R2) || (CurrentstateSP == ESVAZIA_TAPETES);
	PE = (CurrentstateSP == ATIVA_BRACO);
	PR = (CurrentstateSP == PECA_NO_T3);

	//Definir saidas nao boleanas

	//Conta_pecas
	if(Currentstate4 == CONTA_AZUL) 
		AZUIS = conta_a;
	if(Currentstate5 == CONTA_VERDE)
		VERDES = conta_b;

	

	//Debug
	#ifdef DEBUG
	printf("CurrentstateMF --> %d\n",CurrentstateMF);
	printf("CurrentstateI --> %d\n",CurrentstateI);
	printf("CurrentstateSP --> %d\n",CurrentstateSP);
	//printf("SV --> %d\n",SV);
	//printf("STR --> %d\n",STR);
	//printf("Timer15 : %ld\n",timer15.time);
	//printf("Currentstate4 --> %d\n",Currentstate4);
	//printf("Currentstate5 --> %d\n",Currentstate5);
	#endif

	//Escrever novos outputs  	
	write_outputs();

	//Aguarda pelo proximo ciclo 
	//sleep_abs(scan_time);
	
	}

}