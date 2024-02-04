#include "IO.c"
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

//Instrucoes para debug
//#define DEBUG
#undef DEBUG_ESTADOS

// Strings para debug
char * stateNamesMF[] = {
	"PARADO",
	"PRESSIONADO_START",
	"OPERAR",
	"PRESSIONADO_STOP",
	"A_PARAR",
	"T1_T4_LIVRE"
};
char * stateNamesI[] = {
 "INICIO_ILUMINACAO",
	"LIGADA",
	"DESLIGADA"
}; 
char * stateNamesSPA[] = {
 	"INICIO_SEPARACAO_AZUIS",                
	"TAPETE_1_MOVE",
	"SV1_DETETA",
	"ESPERA_TROCA_T3_T2",
	"INICIO_TRANSF_AZUIS",
	"PECA_FORA_T1",
	"FIM_TRANSF_AZUIS",
	"ATIVA_BRACO1",
	"LEVAR_ATE_R1_A",
	"LEVAR_ATE_R2_A",                                              
	"PECA_NO_T3",
	"ESVAZIA_TAPETES_AZUIS",
	"WAIT"   
};
char * stateNamesSPV[] = {
	"INICIO_SEPARACAO_VERDES",                
	"TAPETE_4_MOVE",
	"SV2_DETETA",
	"ESPERA_TROCA_T2_T3",
	"INICIO_TRANSF_VERDES",
	"PECA_FORA_T4",
	"FIM_TRANSF_VERDES",
	"ATIVA_BRACO2",
	"LEVAR_ATE_R1_V",  
	"LEVAR_ATE_R2_V",
	"PECA_NO_T2",                                           
	"ESVAZIA_TAPETES_VERDES"
}; 
char * stateNamesCP[] = {
	"ESPERA_SENSORES",
	"PECAS_NOS_SENSORES",
	"PECAS_CORRETAS",
	"PECA_AZUL_TROCADA",
	"PECA_VERDE_TROCADA",
	"PECAS_TROCADAS"
}; 


//--------------Seccao do Tempo-------------------------------------//

//Definir um tipo para o temporizador
typedef struct{
	bool on;
	uint64_t time;	                   
} timerBlock;

// Tempo de ciclo em execução periodica
//uint64_t scan_time = 100; // 1 milissegundo  Apenas importante para a execução periódica

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

	//Atualiza os timers todos criados
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
	//st->time = 0;	

	// Comentado porque é importante guardar o timer15 quando este é parado para ativar outros estados

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
	T1_T4_LIVRE
} MODO_FUNCIONAMENTO;

//Estados da maquina de estados iluminacao
typedef enum{
	INICIO_ILUMINACAO,
	LIGADA,
	DESLIGADA
} ILUMINACAO;

//Estados da maquina de estados separacao de pecas azuis
typedef enum{
	INICIO_SEPARACAO_AZUIS,                
	TAPETE_1_MOVE,
	SV1_DETETA,
	ESPERA_TROCA_T3_T2,
	INICIO_TRANSF_AZUIS,
	PECA_FORA_T1,
	FIM_TRANSF_AZUIS,
	ATIVA_BRACO1,
	LEVAR_ATE_R1_A,
	LEVAR_ATE_R2_A,                                              // achamos que e preciso o r2
	PECA_NO_T3,
	ESVAZIA_TAPETES_AZUIS,
	WAIT      
} SEPARACAO_PECAS_AZUIS;


//Estados da maquina de estados separacao de pecas verdes
typedef enum{
	INICIO_SEPARACAO_VERDES,                
	TAPETE_4_MOVE,
	SV2_DETETA,
	ESPERA_TROCA_T2_T3,
	INICIO_TRANSF_VERDES,
	PECA_FORA_T4,
	FIM_TRANSF_VERDES,
	ATIVA_BRACO2,
	LEVAR_ATE_R1_V,  
	LEVAR_ATE_R2_V,
	PECA_NO_T2,                                           // achamos que e preciso o r1
	ESVAZIA_TAPETES_VERDES
} SEPARACAO_PECAS_VERDES;

//Estados da maquina de estados separacao de pecas verdes
typedef enum{                
	ESPERA_SENSORES,
	PECAS_NOS_SENSORES,
	PECAS_CORRETAS,
	PECA_AZUL_TROCADA,
	PECA_VERDE_TROCADA,
	PECAS_TROCADAS
} CONTROLA_PECAS;

//Estados da maquina de estados Conta pecas
typedef enum{
	CONTA_AZUL,
	CONTA_VERDE
} CONTA_PECAS;

// Definição dos estados das variadas maquinas
MODO_FUNCIONAMENTO p_CurrentstatMF, CurrentstateMF, aux_CurrentstateMF;
ILUMINACAO  p_CurrentstateI, CurrentstateI, aux_CurrentstateI;
SEPARACAO_PECAS_AZUIS p_CurrentstateSPA, CurrentstateSPA, aux_CurrentstateSPA; 
SEPARACAO_PECAS_VERDES p_CurrentstateSPV, CurrentstateSPV, aux_CurrentstateSPV; 
CONTROLA_PECAS p_CurrentstateCP, CurrentstateCP, aux_CurrentstateCP;	
CONTA_PECAS Currentstate4, Currentstate5;

//Variaveis globais
int conta_a = 0, conta_b = 0, cor1 = 0, cor_aux1 = 0, cor2 = 0, cor_aux2 = 0;

//Guardar flancos anteriores
// p_name -> previus edge of name
bool p_STOP = false, p_START = false, p_STR1 = false , p_STR2 = false, p_SV1 = false, p_SV2 =false, p_ST3 = false, p_ST2 = false;

bool espera = false, stop = false;

//Variaveis auxiliar para guardar flancos
// re -> rising edge
// fe -> faling edge
bool re_STOP = false, re_START = false, re_STR1 = false, re_STR2= false, re_SV1 = false, re_SV2= false, fe_STOP = false, fe_START = false, fe_STR1 = false, fe_STR2, fe_SV1 = false, fe_SV2 = false;      
bool fe_ST3 = false, fe_ST2 = false, re_ST2 = false;

// Funcoes do tempo
void update_timers();
void start_timer( timerBlock *t);
void stop_timer(timerBlock *t);

//Funcoes para as transicoes das ME's
void ME_MODO_FUNCIONAMENTO();
void ME_ILUMINACAO();
void ME_SEPARACAO_PECAS_AZUIS();
void ME_SEPARACAO_PECAS_VERDES();
void ME_CONTROLA_PECAS();
void ME_CONTA_PECAS();
void init_ME();
void sync_ME();
void edge_detection();


void ME_MODO_FUNCIONAMENTO(){

	switch(CurrentstateMF){
		case PARADO:
			if(re_START == true)
			{	
				aux_CurrentstateMF = PRESSIONADO_START;
			}
			break;

		case PRESSIONADO_START:
			if(fe_START == true)
				aux_CurrentstateMF = OPERAR;
			break;

		case OPERAR:
			if(fe_STOP == true) //logica negada dai estarem ao contrario primeiro o falling edge e depois o rising edge
				aux_CurrentstateMF = PRESSIONADO_STOP;
			break;

		case PRESSIONADO_STOP:
			if(re_STOP == true)
				stop = true;

			if(stop == true && CurrentstateCP == PECAS_NOS_SENSORES)
			{
				aux_CurrentstateMF = A_PARAR;
				start_timer(&timer10);
				stop = false;
			}
			break;
			
		case A_PARAR:
			if((!SV1 && !SV2) && (timer10.time >= 1000)) //Alterado para 1 segundo . antes era 10 segundos de espera
			{	
				aux_CurrentstateMF = T1_T4_LIVRE;
				stop_timer(&timer10);
			}	
			else if( timer10.time >= 10000)          
			{
				start_timer(&timer10);
			}
			break;

		case T1_T4_LIVRE:
			if(CurrentstateSPA == INICIO_SEPARACAO_AZUIS || CurrentstateSPV == INICIO_SEPARACAO_VERDES) 
				aux_CurrentstateMF = PARADO;
			break;
		}

}

void ME_ILUMINACAO(){

	switch (CurrentstateI){
		case INICIO_ILUMINACAO:
			if(stop == true)
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
			
void ME_SEPARACAO_PECAS_AZUIS(){
	switch (CurrentstateSPA)
	{
	case INICIO_SEPARACAO_AZUIS:
		if(CurrentstateMF == OPERAR|| CurrentstateMF == A_PARAR)
			aux_CurrentstateSPA = TAPETE_1_MOVE;
		break;
		
	case TAPETE_1_MOVE:
		if(re_SV1 == true)                                         
			aux_CurrentstateSPA = SV1_DETETA;

		if(CurrentstateMF == T1_T4_LIVRE)
		{
			aux_CurrentstateSPA = ESVAZIA_TAPETES_AZUIS;
			start_timer(&timer15);
		}
		break;

	case SV1_DETETA:
        if((CurrentstateCP == PECAS_CORRETAS) || (CurrentstateCP == PECA_AZUL_TROCADA) || (CurrentstateCP == PECAS_TROCADAS))
			aux_CurrentstateSPA = INICIO_TRANSF_AZUIS;

		if(CurrentstateCP == PECA_VERDE_TROCADA)
			aux_CurrentstateSPA = ESPERA_TROCA_T3_T2;
		break;

	case ESPERA_TROCA_T3_T2: 
		if(fe_ST2 == true)
			aux_CurrentstateSPA = INICIO_TRANSF_AZUIS;
		break;

	case INICIO_TRANSF_AZUIS:
		if(fe_SV1 == true)
			aux_CurrentstateSPA = PECA_FORA_T1;
     	break;

	case PECA_FORA_T1:
		if(fe_STR1 == true)
			aux_CurrentstateSPA = FIM_TRANSF_AZUIS;	
		break;

	case FIM_TRANSF_AZUIS:
		if(CurrentstateCP == PECAS_CORRETAS || CurrentstateCP == PECA_VERDE_TROCADA)
			aux_CurrentstateSPA = LEVAR_ATE_R1_A;

		if((CurrentstateCP == PECA_AZUL_TROCADA) || (CurrentstateCP == PECAS_TROCADAS)) 
			aux_CurrentstateSPA = ATIVA_BRACO1;
		break;
		
	case LEVAR_ATE_R1_A:
			if (fe_ST2 == true)
				aux_CurrentstateSPA = TAPETE_1_MOVE;
		break;
		
	case ATIVA_BRACO1:
		if(SPE1 == true)
			aux_CurrentstateSPA = PECA_NO_T3;
		break;
	
	case PECA_NO_T3:
		if(SPR1 == true)
			aux_CurrentstateSPA = LEVAR_ATE_R2_A;
		break;

	case LEVAR_ATE_R2_A:
		if((fe_ST3 == true) && (espera == false))
			aux_CurrentstateSPA = TAPETE_1_MOVE;

		if((fe_ST3 == true) && (espera == true))
			aux_CurrentstateSPA = WAIT;
		break;
		
	case WAIT:
		if(CurrentstateCP == ESPERA_SENSORES)
		{
			aux_CurrentstateSPA = TAPETE_1_MOVE;
		}
		break;

	case ESVAZIA_TAPETES_AZUIS:
		if(timer15.time >= 15000)
		{
			aux_CurrentstateSPA = INICIO_SEPARACAO_AZUIS;
			stop_timer(&timer15);
		}
		break;
	}
}	

void ME_SEPARACAO_PECAS_VERDES(){

	switch (CurrentstateSPV)
	{
	case INICIO_SEPARACAO_VERDES:
		if(CurrentstateMF == OPERAR || CurrentstateMF == A_PARAR)
			aux_CurrentstateSPV = TAPETE_4_MOVE;	
		break;
		
	case TAPETE_4_MOVE:
		if(re_SV2 == true)                                         	
			aux_CurrentstateSPV = SV2_DETETA;

		if (CurrentstateMF == T1_T4_LIVRE)
			aux_CurrentstateSPV = ESVAZIA_TAPETES_VERDES;
		break;

	case SV2_DETETA:
        if((CurrentstateCP == PECAS_CORRETAS) || (CurrentstateCP == PECA_VERDE_TROCADA))
			aux_CurrentstateSPV = INICIO_TRANSF_VERDES;

		if((CurrentstateCP == PECA_AZUL_TROCADA) || (CurrentstateCP == PECAS_TROCADAS))
			aux_CurrentstateSPV = ESPERA_TROCA_T2_T3;
		break;

	case ESPERA_TROCA_T2_T3: 
		if(fe_ST3 == true)
			aux_CurrentstateSPV = INICIO_TRANSF_VERDES;
		break;

	case INICIO_TRANSF_VERDES:
		if(fe_SV2 == true)
			aux_CurrentstateSPV = PECA_FORA_T4;
    	break;

	case PECA_FORA_T4:
		if(fe_STR2 == true)
			aux_CurrentstateSPV = FIM_TRANSF_VERDES;			
		break;

	case FIM_TRANSF_VERDES:
		if(CurrentstateCP == PECAS_CORRETAS || CurrentstateCP == PECA_AZUL_TROCADA)
			aux_CurrentstateSPV = LEVAR_ATE_R2_V;

		if((CurrentstateCP == PECA_VERDE_TROCADA) || (CurrentstateCP == PECAS_TROCADAS)) 
			aux_CurrentstateSPV = ATIVA_BRACO2;
		break;
		
	case LEVAR_ATE_R2_V:
		if(fe_ST3 == true)
			aux_CurrentstateSPV = TAPETE_4_MOVE;
		break;
		
	case ATIVA_BRACO2:
		if(SPE2 == true)
			aux_CurrentstateSPV = PECA_NO_T2;			
		break;

	case PECA_NO_T2:
		if(SPR2 == true)
			aux_CurrentstateSPV = LEVAR_ATE_R1_V;
		break; 

	case LEVAR_ATE_R1_V:
		if(fe_ST2 == true)
			aux_CurrentstateSPV = TAPETE_4_MOVE;
		break;

	case ESVAZIA_TAPETES_VERDES:
		if( timer15.time >= 15000 && (CurrentstateSPA == ESVAZIA_TAPETES_AZUIS))
		{	
			aux_CurrentstateSPV = INICIO_SEPARACAO_VERDES;
		}
			
		break;
	}
}	

void ME_CONTROLA_PECAS(){
	switch (CurrentstateCP)
	{
	case ESPERA_SENSORES:
		if(SV1 != false && SV2 != false){
			cor_aux1 = SV1;
			cor_aux2  = SV2;
			aux_CurrentstateCP = PECAS_NOS_SENSORES;
		}
	break;
	
	case PECAS_NOS_SENSORES:
		if(cor_aux1 == 1 && cor_aux2 == 4)
		{
			aux_CurrentstateCP = PECAS_CORRETAS;
			espera = false;
		}
			
		if(cor_aux1 == 4 && cor_aux2 == 4)
		{
			aux_CurrentstateCP = PECA_AZUL_TROCADA;
			espera = false;
		}
			
		if(cor_aux1 == 1 && cor_aux2 == 1)
		{
			aux_CurrentstateCP = PECA_VERDE_TROCADA;
			espera = false;
		}
		
		if(cor_aux1 == 4 && cor_aux2 == 1)
		{
			aux_CurrentstateCP = PECAS_TROCADAS;
			espera = true;
		}
		else
		{
			espera = false;
		}
	break;

	case PECAS_CORRETAS:
		if(CurrentstateSPA == LEVAR_ATE_R1_A && CurrentstateSPV == LEVAR_ATE_R2_V)
			aux_CurrentstateCP = ESPERA_SENSORES;
	break;
	
	case PECA_AZUL_TROCADA:
		if(CurrentstateSPV == LEVAR_ATE_R2_V)
			aux_CurrentstateCP = ESPERA_SENSORES;
	break;

	case PECA_VERDE_TROCADA:
		if(CurrentstateSPA == LEVAR_ATE_R1_A)
			aux_CurrentstateCP = ESPERA_SENSORES;
	break;

	case PECAS_TROCADAS:
		if(CurrentstateSPV == LEVAR_ATE_R1_V)
			aux_CurrentstateCP = ESPERA_SENSORES;
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
	

	//SEPARACAO_PECAS_AZUIS	
	p_CurrentstateSPA = CurrentstateSPA;
	CurrentstateSPA = aux_CurrentstateSPA;
	
	//SEPARACAO_PECAS_VERDES
	p_CurrentstateSPV = CurrentstateSPV;
	CurrentstateSPV = aux_CurrentstateSPV;

	//CONTROLA_PECAS
	p_CurrentstateCP = CurrentstateCP;
	CurrentstateCP = aux_CurrentstateCP;

}

void edge_detection(){	

	//-----------------MODO_FUNCIONAMENTO-----------------------------------//

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

	//---------------------SEPARACAO_PECAS_AZUIS_VERDES---------------------------//

	//Flancos Ascendentes SV1
	if( p_SV1 == false && SV1 != false) //true == 0 e as vezes SV = 4
		re_SV1 = true;
	else
		re_SV1 = false;

	//Flancos descendentes SV1
	if( p_SV1 != false && SV1 == false)
		fe_SV1 = true;
	else
		fe_SV1 = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_SV1 = SV1;
	
	
	//Flancos Ascendentes SV2
	if( p_SV2 == false && SV2 != false) //true == 1 e as vezes SV = 4
		re_SV2 = true;
	else
		re_SV2 = false;

	//Flancos descendentes SV2
	if( p_SV2 != false && SV2 == false)
		fe_SV2 = true;
	else
		fe_SV2 = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_SV2 = SV2;


	//Flancos Ascendentes STR1
	if( p_STR1 == false && STR1 == true)
		re_STR1 = true;
	else
		re_STR1 = false;

	//Flancos descendentes STR
	if( p_STR1 == true && STR1 == false)
		fe_STR1 = true;
	else
		fe_STR1 = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_STR1 = STR1;

	
	//Flancos Ascendentes STR2
	if( p_STR2 == false && STR2 == true)
		re_STR2 = true;
	else
		re_STR2 = false;

	//Flancos descendentes STR
	if( p_STR2 == true && STR2 == false)
		fe_STR2 = true;
	else
		fe_STR2 = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_STR2 = STR2;


	//Flancos descendentes ST3
	if( p_ST3 == true && ST3 == false)
		fe_ST3 = true;
	else
		fe_ST3 = false;

	//No proximo ciclo, o valor anterior passa a ser  o atual
	p_ST3 = ST3;


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
	CurrentstateSPA = INICIO_SEPARACAO_AZUIS;	
	CurrentstateSPV = INICIO_SEPARACAO_VERDES;	
	CurrentstateCP = ESPERA_SENSORES;						
	Currentstate4 = CONTA_AZUL;
	Currentstate5 = CONTA_VERDE;

}

//-----------------------Main----------------------------------//
int main(){

	//Autoria do trabalho
	printf("\nTrabalho realizado por:\n");
	printf("up202004966 ---> Bruno Costa\n");
	printf("up202007913 ---> Gabriel Rocha\n");

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
	ME_SEPARACAO_PECAS_AZUIS();
	ME_SEPARACAO_PECAS_VERDES();
	ME_CONTROLA_PECAS();
	ME_CONTA_PECAS();

	//Atualizar estados apenas no fim do ciclo
	sync_ME();
	

	//Definir as saidas boleanas

	//MODO FUNCIONAMENTO
	LSTOP = (CurrentstateMF == PARADO ) || (CurrentstateMF == PRESSIONADO_START );
	LSTART = (CurrentstateMF == OPERAR);
	E1 = (CurrentstateMF == OPERAR ) || (CurrentstateMF == PRESSIONADO_STOP );
	E2 = (CurrentstateMF == OPERAR ) || (CurrentstateMF == PRESSIONADO_STOP );


	//ILUMINACAO
	LWAIT = (CurrentstateI == LIGADA);

	//SEPARACAO DE PECAS AZUIS

	T1A = ((CurrentstateSPA == TAPETE_1_MOVE) || (CurrentstateSPA == INICIO_TRANSF_AZUIS));
	T2A = (CurrentstateSPA == INICIO_TRANSF_AZUIS) || (CurrentstateSPA == LEVAR_ATE_R1_A) || (CurrentstateSPA == PECA_FORA_T1) || //presente nas duas maquinas SPA e SPV, mas sem sobreposição
	(CurrentstateSPA == ESVAZIA_TAPETES_AZUIS) || (CurrentstateSPV == LEVAR_ATE_R1_V) || (CurrentstateSPV == ESVAZIA_TAPETES_VERDES); 
	T3A = (CurrentstateSPA == LEVAR_ATE_R2_A) || (CurrentstateSPA == ESVAZIA_TAPETES_AZUIS) || (CurrentstateSPV == INICIO_TRANSF_VERDES) || //presente nas duas maquinas SPA e SPV, mas sem sobreposição
	(CurrentstateSPV == LEVAR_ATE_R2_V) || (CurrentstateSPV == PECA_FORA_T4) || (CurrentstateSPV == ESVAZIA_TAPETES_VERDES);
	PE1 = (CurrentstateSPA == ATIVA_BRACO1);
	PR1 = (CurrentstateSPA == PECA_NO_T3);
	

	//SEPARACAO DE PECAS VERDES
	T4A = (CurrentstateSPV == TAPETE_4_MOVE) || (CurrentstateSPV == INICIO_TRANSF_VERDES) ;
	PE2 = (CurrentstateSPV == ATIVA_BRACO2);
	PR2 = (CurrentstateSPV == PECA_NO_T2);

	//CONTROLA PECAS 
	/*Maquina sem saidas ativadas*/

	//Definir saidas nao boleanas

	//Conta_pecas
	if(Currentstate4 == CONTA_AZUL) 
		AZUIS = conta_a;
	if(Currentstate5 == CONTA_VERDE)
		VERDES = conta_b;

	//Debug
	#ifdef DEBUG
	//printf("SV1 --> %d\n",SV1);
	//printf("SV2 --> %d\n",SV2);
	//printf("T2A --> %d\n",T2A);
	//printf("T3A --> %d\n",T3A);
	//printf("STR1 --> %d\n",STR1);
	//printf("SPE2 --> %d\n",SPE2);
	//printf("T2A --> %d\n",T2A);
	//printf("Timer15 : %ld\n",timer15.time);
	printf("CurrentstateMF --> %s\n " , stateNamesMF[CurrentstateMF]);
	//printf("CurrentstateI --> %s\n " , stateNamesI[CurrentstateI]);
	printf("CurrentstateSPA --> %s\n " , stateNamesSPA[CurrentstateSPA]);
	printf("CurrentstateSPV --> %s\n " , stateNamesSPV[CurrentstateSPV]);
	printf("CurrentstateCP --> %s\n\n" , stateNamesCP[CurrentstateCP]);
	//printf("STOP --> %d\n",stop);
	#endif
	
	//Escrever novos outputs  	
	write_outputs();

	//Aguarda pelo proximo ciclo apenas na execução periodica
	//sleep_abs(scan_time);
	
	}
		
}