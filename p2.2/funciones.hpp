#ifndef FUNCIONES_HPP
#define FUNCIONES_HPP

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <iostream>

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){

    char buffer[250];
    int j;

    close(socket);
    FD_CLR(socket,readfds);

    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);

    (*numClientes)--;

    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);

    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,strlen(buffer),0);


}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    //signal(SIGINT,manejador);

    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}

void setIDPartidaySockets(int &i, std::vector<Partida> &partidas, int &idPartida, int &socket1, int &socket2){
	for(int z = 0; z < partidas.size(); z++){
		if((partidas[z].getSocket1() == i) || (partidas[z].getSocket2() == i)){
			idPartida = z;
		}
	}

 socket1 = partidas[idPartida].getSocket1();
 socket2 = partidas[idPartida].getSocket2();
}


#endif
