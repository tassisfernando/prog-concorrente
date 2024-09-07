/******************************************************************************
*   arquivo...: jantar_dos_filosofos.c
*   descri??o.: Um cl�ssico da programa��o paralela, Dijkstra (1965)
*
*
*   autor.....: Uilian Ries      <uilianries@gmail.com>
*   data......: 27/11/2012
*
********************************************************************************/
//-- INCLUDE --------------------------------------------------------------------
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//-- CONSTANTES -----------------------------------------------------------------
#define QUANT		(5)									//Quantidade de fil�sofos
#define ESQUERDA	(id_filosofo + QUANT - 1) % QUANT	//Id do fil�sofo a esquerda do id
#define DIREITA		(id_filosofo + 1) % QUANT			//Id do fil�sofo a direita do id
#define PENSANDO	(0)									//Id para estado pensado
#define FAMINTO		(1)									//Id para estado de fome
#define COMENDO		(2)									//Id para estado comendo
//-- GLOBAL ---------------------------------------------------------------------
int estado [QUANT];								//Estado dos fil�sofos
pthread_mutex_t mutex;							//Regi�o cr�tica
pthread_mutex_t mux_filo [QUANT];				//Mutex por fil�sofo
pthread_t jantar[QUANT];						//Todos os fil�sofos
//-- PROTOTIPA��O ---------------------------------------------------------------
void * filosofo ( void * param );
void pegar_hashi ( int id_filosofo );
void devolver_hashi ( int id_filosofo );
void intencao ( int id_filosofo );
void comer ( int id_filosofo );
void pensar ( int id_filosofo );
//-------------------------------------------------------------------------------
/*!
* @fn void * filosofo ( void * param )
* @brief Fun��o que simula um fil�sofo
* @param vparam id do fil�sofo
*/
void * filosofo ( void * vparam )
{
	int * id = (int *) (vparam);	//Repassa o id do fil�sofo

	printf("Filosofo %d foi criado com sucesso\n", *(id) );

	while ( 1 )
	{
		pensar( *(id) );			//Aguarda o fil�sofo pensar
		pegar_hashi( *(id) );		//Fil�sofo pega os hashis
		comer( *(id) );				//Aguarda comer
		devolver_hashi( *(id) );	//Devolver os hashis pra mesa
	}

	pthread_exit( (void*) 0 );		//Legado do retorno
}
//-------------------------------------------------------------------------------
/*!
* @fn void pegar_hashi ( int id_filosofo )
* @brief Fun��o que fil�sofo pega os hashis da mesa
* @param id_filosofo id do fil�sofo
*/
void pegar_hashi ( int id_filosofo )
{
	pthread_mutex_lock( &(mutex) );					//Entra na regi?o cr�tica
	printf("Filosofo %d esta faminto\n", id_filosofo);
	estado[ id_filosofo ] = FAMINTO;				//Altera o estado do fil�sofo
	intencao( id_filosofo );						//Inten��o de pegar os hashis
	pthread_mutex_unlock( &(mutex) );				//Sai na regi�o cr�tica
	pthread_mutex_lock( &(mux_filo[id_filosofo]) );	//Bloqueia os hashis
}
//-------------------------------------------------------------------------------
/*!
* @fn void devolver_hashi ( int id_filosofo )
* @brief Fun��o que fil�sofo devolve os hashis para a mesa
* @param id_filosofo id do fil�sofo
*/
void devolver_hashi ( int id_filosofo )
{
	pthread_mutex_lock ( &(mutex) );	//Entra na regi?o cr�tica
	printf("Filosofo %d esta pensando\n", id_filosofo);
	estado[ id_filosofo ] = PENSANDO;	//Terminou de comer
	intencao( ESQUERDA );				//V� se o vizinho da esquerda pode comer agora
	intencao( DIREITA );				//V� se o vizinho da direita pode comer agora
	pthread_mutex_unlock ( &(mutex) );	//Sai da regi?o cr�tica
}
//-------------------------------------------------------------------------------
/*!
* @fn void intencao ( int id_filosofo )
* @brief Fun��o que fil�sofo tem inten��o de pegar os hashis para comer
* @param id_filosofo id do fil�sofo
*/
void intencao ( int id_filosofo )
{
	printf("Filosofo %d esta com intencao de comer\n", id_filosofo);
	if( (estado[id_filosofo] == FAMINTO) &&	//Se o fil�sofo est� come fome
		(estado[ESQUERDA] != COMENDO) &&	//Se o vizinho da esquerda n�o est� comendo
		(estado[DIREITA] != COMENDO ) )		//Se o vizinho da direita n?o est� comendo
	{
		printf("Filosofo %d ganhou a vez de comer\n", id_filosofo);
		estado[ id_filosofo ] = COMENDO;	//Fil�sofo ganho a vez de comer
		pthread_mutex_unlock( &(mux_filo[id_filosofo]) );	//Libera os hashis
	}
}
//-------------------------------------------------------------------------------
/*!
* @fn void pensar ( int id_filosofo )
* @brief Fun��o que fil�sofo fica pensando
*/
void pensar ( int id_filosofo )
{
	int r = (rand() % 10 + 1);
	
	printf("Filosofo %d pensa por %d segundos\n", id_filosofo, r );
	sleep( r );	//Gasta um tempo em segundos
}
//-------------------------------------------------------------------------------
/*!
* @fn void comer ( int id_filosofo )
* @brief Fun��o que fil�sofo fica comendo
*/
void comer ( int id_filosofo )
{
	int r = (rand() % 10 + 1);
	
	printf("Filosofo %d come por %d segundos\n", id_filosofo, r);
	sleep( r );	//Gasta um tempo em segundos

}
//-------------------------------------------------------------------------------
int main ( void )
{
	int cont;	//Contador auxiliar
	int status;	//Cria��o da thread

	//Inicia os mutexes
	pthread_mutex_init( &(mutex), NULL);
	for( cont = 0 ; cont < QUANT ; cont++ )
	{
		pthread_mutex_init( &(mux_filo[cont]), NULL );
	}

	//Inicia threads (fil�sofos)
	for( cont = 0 ; cont < QUANT ; cont++ )
	{
		//verifica se ocorreu erro
		status = pthread_create( &(jantar[cont]), NULL, filosofo, (void *) &(cont) );
		if ( status )
		{
			printf("Erro criando thread %d, retornou codigo %d\n", cont, status );
			return EXIT_FAILURE;
		}
	}

	//Destroi antes de sair
	pthread_mutex_destroy( &(mutex) );
	for( cont = 0 ; cont < QUANT ; cont++ )
	{
		pthread_mutex_destroy( &(mux_filo[cont]) );
	}
	pthread_exit( NULL );

	return EXIT_SUCCESS;
}

