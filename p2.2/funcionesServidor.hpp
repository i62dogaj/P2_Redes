#ifndef FUNCIONESSERVIDOR_HPP
#define FUNCIONESSERVIDOR_HPP

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#define MSG_SIZE 250
#define MAX_CLIENTS 50

struct user{
	string login;
	string password;
	//int conectado;
};


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);

    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}


//bool log_in(vector<struct user> &v){
void log_in(int new_sd){
	char buffer[MSG_SIZE];
	strcpy(buffer, "Introduzca login: \n");
	send(new_sd,buffer,strlen(buffer),0);
}

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


#endif
