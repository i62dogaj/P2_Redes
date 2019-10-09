#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#include "funcionesCliente.hpp"


int main ( )
{

	/*----------------------------------------------------
		Descriptor del socket y buffer de datos
	-----------------------------------------------------*/
	int s_cliente;
	struct sockaddr_in servidor;
	char buffer[250];
	socklen_t long_servidor;
	fd_set readfds, auxfds;
	int salida;
	int fin = 0;
	int conectado = 0;
  int opcion;


	/*---------------------------------------------------
		Se cargan los usuarios ya registrados
	----------------------------------------------------*/
	vector<struct user> vectorU = cargarUsuarios();

  opcion = menu();
  cout << opcion << endl;




	/* --------------------------------------------------
		Se abre el socket
	---------------------------------------------------*/
	s_cliente = socket (AF_INET, SOCK_STREAM, 0);
	if (s_cliente == -1)
	{
		perror("No se puede abrir el socket cliente\n");
		exit (1);
	}



	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(2000); // puerto
	servidor.sin_addr.s_addr =  inet_addr("127.0.0.1");


	/*---------------------------------------------
	    Antes de conectarse al servidor, se loguea
	----------------------------------------------*/
	while(conectado == 0){
		if(log_in(vectorU)){
			cout << "\nTe has logueado." << endl;
			conectado = 1;
		}
		else cout << "\nNo te has podido loguear." << endl;
	}


	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	long_servidor = sizeof(servidor);

	if (connect(s_cliente, (struct sockaddr *)&servidor, long_servidor) == -1)
	{
		perror ("Error de conexión");
		exit(1);
	}

	//Inicializamos las estructuras
	FD_ZERO(&auxfds);
	FD_ZERO(&readfds);

	FD_SET(0,&readfds);
	FD_SET(s_cliente,&readfds);


	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	do{
		auxfds = readfds;
		salida = select(s_cliente+1,&auxfds,NULL,NULL,NULL);

		//Tengo mensaje desde el servidor
		if(FD_ISSET(s_cliente, &auxfds)){

		    bzero(buffer,sizeof(buffer));
		    recv(s_cliente,buffer,sizeof(buffer),0);

		    cout << "\n" << buffer;

		    if(strcmp(buffer,"Demasiados clientes conectados\n") == 0)
			fin =1;

		    if(strcmp(buffer,"Desconexion servidor\n") == 0)
			fin =1;

		}
		else{

		    //He introducido información por teclado
		    if(FD_ISSET(0,&auxfds)){
			bzero(buffer,sizeof(buffer));

			fgets(buffer,sizeof(buffer),stdin);

			if(strcmp(buffer,"SALIR\n") == 0){
				fin = 1;

			}

			send(s_cliente,buffer,sizeof(buffer),0);

		    }


		}

	}while(fin == 0);

	close(s_cliente);

	return 0;

}
