#ifndef FUNCIONES_HPP
#define FUNCIONES_HPP

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <signal.h>

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


void setIDPartidaySockets(int &i, std::vector<Partida> &partidas, int &idPartida, int &socket1, int &socket2){
	for(int z = 0; z < partidas.size(); z++){
		if((partidas[z].getSocket1() == i) || (partidas[z].getSocket2() == i)){
			idPartida = z;
		}
	}

 socket1 = partidas[idPartida].getSocket1();
 socket2 = partidas[idPartida].getSocket2();
}


void decidirTurnoInicial(Jugador &j1, Jugador &j2, Ficha &a, int socket1, int socket2, Partida &p){
	char buffer[250];
	if(j1.existeFicha(a)){
		p.setTurno(socket1);
		bzero(buffer,sizeof(buffer));
		sprintf(buffer, "\n+OK. Turno de partida\n");
		send(socket1, buffer, strlen(buffer),0);

		bzero(buffer,sizeof(buffer));
		sprintf(buffer, "\n+OK. Turno del otro jugador\n");
		send(socket2, buffer, strlen(buffer),0);
	}
	else if(j2.existeFicha(a)){
		p.setTurno(socket2);
		bzero(buffer,sizeof(buffer));
		sprintf(buffer, "\n+OK. Turno de partida\n");
		send(socket2 ,buffer,strlen(buffer),0);

		bzero(buffer,sizeof(buffer));
		sprintf(buffer, "\n+OK. Turno del otro jugador\n");
		send(socket1 ,buffer,strlen(buffer),0);
	}
}


void decidirTurno(int i, int socket1, int socket2, Partida &p){
	char buffer[250];
	if(i == socket1){
		p.setTurno(socket2);

		bzero(buffer, sizeof(buffer));
		sprintf(buffer, "\n+OK. Turno del otro jugador.\n");
		send(socket1, buffer, strlen(buffer), 0);

		bzero(buffer, sizeof(buffer));
		sprintf(buffer, "\n+OK. Turno de partida.\n");
		send(socket2, buffer, strlen(buffer), 0);
	}
	else{
		p.setTurno(socket1);

		bzero(buffer, sizeof(buffer));
		sprintf(buffer, "\n+OK. Turno del otro jugador.\n");
		send(socket2, buffer, strlen(buffer), 0);

		bzero(buffer, sizeof(buffer));
		sprintf(buffer, "\n+OK. Turno de partida.\n");
		send(socket1, buffer, strlen(buffer), 0);
	}
}


void nuevaPartida(vector<Partida> &partidas, int i, fd_set usuario_esperandoPartida){
		Partida nuevo;
		char buffer[250];
		nuevo.setSocket1(i);
		FD_SET(i, &usuario_esperandoPartida);
		nuevo.setIDPartida(partidas.size());
		partidas.push_back(nuevo);

		bzero(buffer,sizeof(buffer));
		strcpy(buffer,"+Ok. Petición Recibida. Quedamos a la espera de más jugadores\0");
		send(i,buffer,strlen(buffer),0);
}

void nuevaPartidaEnPosicion(vector<Partida> &partidas, int i, fd_set usuario_esperandoPartida, int pos){
		Partida nuevo;
		char buffer[250];
		nuevo.setSocket1(i);
		FD_SET(i, &usuario_esperandoPartida);
		partidas[pos] = nuevo;
		partidas[pos].setIDPartida(pos);

		bzero(buffer,sizeof(buffer));
		strcpy(buffer,"+Ok. Petición Recibida. Quedamos a la espera de más jugadores\0");
		send(i,buffer,strlen(buffer),0);
}



void mostrarFichaRobada(Ficha &a, int socket){
	char buffer[250];
	bzero(buffer,sizeof(buffer));
	sprintf(buffer, "%s\n", a.mostrarFicha().c_str());
	send(socket,buffer,strlen(buffer),0);
}

void mostrarManoJugador(Jugador &j, int socket){
	char buffer[250];
	bzero(buffer,sizeof(buffer));
	sprintf(buffer, "%s\n", j.mostrarMano().c_str());
	send(socket,buffer,strlen(buffer),0);
}


void mostrarTableroAJugador(int socket, Partida &p){
	char buffer[250];
	bzero(buffer,sizeof(buffer));
	sprintf(buffer, "%s\n", p.mostrarTablero().c_str());
	send(socket,buffer,strlen(buffer),0);
}


#endif
