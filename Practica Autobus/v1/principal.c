#include <stdio.h>
#include <stdlib.h>
#include "comun.h"
#include <unistd.h>
#include <signal.h>

#define MAX_CLIENTES 20
#define MAX_ESPERA 8
#define NUM_PARADAS 4

int llega10=0; 	// Controla si llega la señal 10
int creaproceso(const char * nombre); // Prototipo de la función
int creaservigraf(int ultimaparada);

void R10()
{
	llega10=1;
}

void R12()
{
	printf("Error: resolucion servidor grafico!\n");
	exit(-1);

}


int main()
{	
	// Declaración de variables	
	int pid_grafico, id_cola;
	key_t clave;

	signal(10,R10);	// Nos preparamos para recibir las señales
	signal(12,R12);

	int pid_svgraf = creaservigraf(NUM_PARADAS);
	
	if(!llega10) pause();	//Esperamos a recibir la señal si no se ha recibido
	llega10=0;	//Reiniciamos llega10

	srand(getpid()); //Generamos una semilla

	for(int i=1; i<=MAX_CLIENTES; i++)//Crea un cliente y espera un tiempo aleatorio
	{
		creaproceso("cliente");
		sleep(rand()%MAX_ESPERA+2); // Espera entre 2 y MAX_ESPERA
	}

	// Espera el fin de los clientes y avisa al serv grafico de fin
	for(int i=1; i<=MAX_CLIENTES; i++) wait(NULL); // espera que mueran todos los hijos	
	
	kill(pid_svgraf,12); // Señal 12 para terminar el ncurses

	msgctl(id_cola,IPC_RMID,(struct msqid_ds *)NULL);
	return 0;
}

int creaproceso(const char *nombre)// Devuelve el pid del proceso creado
{
	int pid=fork();
	if(pid==0)
	{
		execl(nombre,nombre,NULL);
		perror("Error en el execl eeeee\n");
		exit(-1);
	}
	else if(pid==-1)
	{
		perror("Error en el fork\n");
		exit(-1);
	}
	return pid;
}

int creaservigraf(int ultimaparada)
{
	int pid;
	char cadparada[10];
	
	sprintf(cadparada,"%d",ultimaparada); // Convierte entero a char

	pid=fork();
	if(pid==0)
	{
	execl("servidor_ncurses","servidor_ncurses",cadparada,NULL);
	perror("Error en execl aaaa\n");
	exit(-1);
	}
	else if(pid==-1)
	{
		perror("Error en el fork\n");
		exit(-1);
	}
	return pid;
	
}
