#include <stdio.h>
#include "comun.h"

#define NUM_PARADAS 4
#define T_BUS 5

void visualiza(int id_cola, int parada, int inout, int pintaborra, int destino);
int llega10=0;

void R10()
{
    llega10=1;
}

void R12()
{


}

void main()
{
    int id_cola, parada, destino;
    signal(10,R10);
    signal(12,R12);

    
    id_cola = crea_cola(ftok("./fichcola.txt",18)); // Crea la cola de mensajes


    srand(getpid()); //Iniciamos la semilla aleatoria
    //Genera parada de llegada y bajada aleatorias
    parada = rand()%NUM_PARADAS+1; 
    destino = rand()%NUM_PARADAS+1; 
    
    visualiza(id_cola,parada,IN,PINTAR,destino); //Se visualiza en la parada de llegada

    sleep(T_BUS); // SE espera el tiempo del autobus en moverse

    visualiza(id_cola,parada,IN,BORRAR,destino);
    visualiza(id_cola,parada,OUT,PINTAR,destino);


}

void visualiza(int id_cola, int parada, int inout, int pintaborra, int destino)
{//Id cola mensajes, parada a subir, col entrada o salida, pintar o borrar y parada de destino
    struct tipo_elemento peticion;
    peticion.tipo=2; //Persona
    peticion.pid=getpid();
    peticion.parada=parada;
    peticion.inout=inout;
    peticion.destino=destino;
    peticion.pintaborra=pintaborra;

    msgsnd(id_cola, (struct msgbuf *) &peticion,sizeof(peticion)-sizeof(long),0);

    // Si el proceso se va a borrar, no recibe señal, solo si se pinta
    if (pintaborra==PINTAR && !llega10)
    {
        pause();
        llega10=0;
    }

}