#include <stdio.h>
#include <stdlib.h>
#include "comun.h"
#include <unistd.h>
#include <signal.h>

int llega10=0; 	// Controla si llega la señal 10
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

	pid_grafico=fork();
	if(pid_grafico==0)
	{
		execl("servidor_ncurses","servidor_ncurses","5",NULL);
		//El servidor graf. necesita saber el n paradas (2-6)
		perror("Error en execl\n");
		exit(-1);
    	}
	// Si el servidor ncurses va mas rapido que este, envia la señal y termina
	// Entonces hacemos que si ya ha llegado la señal, se salte el pause
	
	if(!llega10) pause();	//Esperamos a recibir la señal si no se ha recibido

	clave = ftok("./fichcola.txt",18); 	// Creamos la clave
	id_cola=msgget(clave,0600|IPC_CREAT);	//Y la cola de mensajes

	struct tipo_elemento msg1;//={2,getpid(),1,IN,PINTAR,3};
	// tipo, pid_grafico, parada, inout, pintaborra, destino
	msg1.tipo = 2;
	msg1.pid = getpid();
	msg1.parada = 3;
	msg1.inout = IN;
	msg1.pintaborra = PINTAR;
	msg1.destino = 1;

	
	llega10=0;	
	msgsnd(id_cola,(struct msgbuf*)&msg1,sizeof(msg1)-sizeof(long),0);
	if(!llega10) pause();
	
	sleep(10);
	kill(pid_grafico,12); // Señal 12 para terminar el ncurses

	msgctl(id_cola,IPC_RMID,(struct msqid_ds *)NULL);
	return 0;
}
