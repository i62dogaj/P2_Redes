#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <ctime>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <fstream>
#include "partida.hpp"
#include "jugador.hpp"
#include "funciones.hpp"

bool salir = false;

void manejador(int sigsum){
    printf("\nSe ha capturado la señal %d \n", sigsum);
    salir = true;
    signal(SIGINT, manejador);
}


#define MSG_SIZE 250
#define MAX_CLIENTS 30
#define MAX_PARTIDAS 10


/*
 * El servidor ofrece el servicio de un chat
 */


int main ( )
{

	/*----------------------------------------------------
		Descriptor del socket y buffer de datos
	-----------------------------------------------------*/

	std::fstream file;
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;

	fd_set readfds, auxfds, usuario_correcto, usuario_validado, usuario_jugando, usuario_esperandoPartida;

	int salida;
	int arrayClientes[MAX_CLIENTS];
	int numClientes=0;
	std::map<int, std::string> usuarios; //vector de usuarios donde asocia cada nombre de usuario a su socket
	std::vector<Partida> partidas;

	std::map<int, Ficha> fichas;

	//contadores
	int i,j,k;
	int recibidos;
	char identificador[MSG_SIZE];

	int on, ret;







	/* --------------------------------------------------
		Se abre el socket
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);
	}

	// Activaremos una propiedad del socket que permitir· que otros
	// sockets puedan reutilizar cualquier puerto al que nos enlacemos.
	// Esto permitir· en protocolos como el TCP, poder ejecutar un
	// mismo programa varias veces seguidas y enlazarlo siempre al
	// mismo puerto. De lo contrario habrÌa que esperar a que el puerto
	// quedase disponible (TIME_WAIT en el caso de TCP)
	on=1;
	ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));



	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}


   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el
		tamaño de su estructura, el resto de información (familia, puerto,
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
	from_len = sizeof (from);


	if(listen(sd,1) == -1){
		perror("Error en la operación de listen");
		exit(1);
	}

	//Inicializar los conjuntos fd_set
	FD_ZERO(&readfds);
	FD_ZERO(&auxfds);
	FD_SET(sd,&readfds);
	FD_SET(0,&readfds);


	//Capturamos la señal SIGINT (Ctrl+c)
	signal(SIGINT,manejador);

	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
	while(1){
		if(salir){
			for (j = 0; j < numClientes; j++){
				send(arrayClientes[j], "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
				close(arrayClientes[j]);
				FD_CLR(arrayClientes[j],&readfds);
			}
				close(sd);
				exit(-1);
		}
		//Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)

		auxfds = readfds;

		salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);

		if(salida > 0){


			 for(i=0; i<FD_SETSIZE; i++){

				  //Buscamos el socket por el que se ha establecido la comunicación
				  if(FD_ISSET(i, &auxfds)) {

						if( i == sd){

							 if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
								  perror("Error aceptando peticiones");
							 }
							 else
							 {
								  if(numClientes < MAX_CLIENTS){
										arrayClientes[numClientes] = new_sd;
										numClientes++;
										FD_SET(new_sd,&readfds);

										sprintf(buffer, "+Ok. Usuario conectado\n");

										send(new_sd,buffer,strlen(buffer),0);

										for(j=0; j<(numClientes-1);j++){

											 bzero(buffer,sizeof(buffer));
											 sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
											 send(arrayClientes[j],buffer,strlen(buffer),0);
										}
								  }
								  else{
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"Demasiados clientes conectados\n");
										send(new_sd,buffer,strlen(buffer),0);
										close(new_sd);
								  }

							 }


						}
						else if (i == 0){
							 //Se ha introducido información de teclado DESDE EL SERVIDOR
							 bzero(buffer, sizeof(buffer));
							 fgets(buffer, sizeof(buffer),stdin);

							 //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
							 if(strcmp(buffer,"SALIR\n") == 0){
								 salir = true;
							 }
							 //Mensajes que se quieran mandar a los clientes (implementar)

						}
						//INFORMACION RECIBIDA DEL CLIENTE
						else{
							 bzero(buffer,sizeof(buffer));

							 recibidos = recv(i,buffer,sizeof(buffer),0);

							 if(recibidos > 0){

								 if(strcmp(buffer,"SALIR\n") == 0){
									 int who = i;
									 salirCliente(i,&readfds,&numClientes,arrayClientes);

									 int idPartida, socket1, socket2;
									 setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);

									 partidas[idPartida].getJugador(socket1).salirPartida(&partidas[idPartida]);
									 partidas[idPartida].getJugador(socket2).salirPartida(&partidas[idPartida]);
									 partidas[idPartida].setSocket1(-1);
									 partidas[idPartida].setSocket2(-1);

									if(who == socket1){
										FD_SET(socket2, &usuario_validado);
										FD_CLR(socket2, &usuario_jugando);

										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"+OK. Si quiere, puede iniciar otra partida\n");
										send(socket2,buffer,strlen(buffer),0);
									}
									else{
										FD_SET(socket1, &usuario_validado);
										FD_CLR(socket1, &usuario_jugando);

										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"+OK. Si quiere, puede iniciar otra partida\n");
										send(socket1,buffer,strlen(buffer),0);
									}

								 }

								 else
								 {
									 /*-----------------------------------------------------------------------
									 REGISTRO
									 ------------------------------------------------------------------------ */
									if(strstr(buffer, "REGISTRO")!=NULL)
									{
										//PONER USUARIO A ESTADO VALIDADO CUANDO SE REGISTRE CORRECTAMENTE

										if (FD_ISSET(i, &usuario_correcto)) {
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-ERR. Usuario en estado correcto, no puede registrarse\n");
											send(i,buffer,strlen(buffer),0);
											break;
										}

										if (FD_ISSET(i, &usuario_validado)) {
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-ERR. Usuario en estado validado, no puede registrarse\n");
											send(i,buffer,strlen(buffer),0);
											break;
										}

										bool anadir=true;
										char *auxUser1, *auxUser2, *auxPasswd1;
										char usuario[20], contrasena[20];

										std::string lineaFichero;
										std::string busquedaUsuario;

										bzero(usuario, sizeof(usuario));
										bzero(contrasena, sizeof(contrasena));


										//Buscamos la subcadena "-u" en el buffer, si la encuentra mete la cadena que va desde "-u" hasta final de buffer
										//Si no la encuentra entonces mete NULL
										auxUser1=strstr(buffer, "-u");//+3 -> comienzo usuario
										auxUser2=strstr(buffer, " -p");//final usuario

										auxPasswd1=strstr(buffer, "-p");//+3 comienzo password

										/*-----------------------------------------------------------------------
										CONSIDERACIONES
									 	CONTROLAR QUE LA CONTRASEÑA NO SE PUEDA ESCRIBIR ANTES QUE EL USUARIO
										CONTROLAR QUE ESTANDO UN USUARIO CONECTADO NADIE MAS PUEDA ACCEDER CON ESA CUENTA
										------------------------------------------------------------------------ */

										//Si no se encuentra ocurrencia con -u ó -p en el mensaje del cliente
										if((auxUser1 == NULL) or ((auxPasswd1 == NULL))){
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-ERR. Sentencia de registro incorrecta\n");
											send(i,buffer,strlen(buffer),0);
											break;
										}

										//Con estos -1 quitamos los '\n' || lenght_aux_user2 -> Tamaño desde " -p" hasta el final del buffer
										int tamBuffer=strlen(buffer)-1, tamAuxUser2=strlen(auxUser2)-1, tamAuxPasswd1=strlen(auxPasswd1)-1;

										strncpy(usuario, auxUser1+3, tamBuffer-tamAuxUser2-12);//metemos en user, desde esa posicion, un numero x de bytes
										strncpy(contrasena, auxPasswd1+3, tamAuxPasswd1-3);


										file.open("usuarios.txt", std::fstream::in);

										//Controlamos si el usuario a registrar se encuentra ya en el archivo
										while(std::getline(file,lineaFichero))//metemos en linea_fichero usuario y contraseña (linea completa del fichero)
										{
											busquedaUsuario=lineaFichero.substr(0, strlen(usuario)); //metemos en busqueda_usuario solo el usuario
											if(strcmp(busquedaUsuario.c_str(), usuario)==0)
											{
												anadir=false;
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"-ERR. Usuario existente\n");
												send(i,buffer,strlen(buffer),0);
												break;
											}
										}

										file.close();
										file.open("usuarios.txt", std::fstream::app);

										//Si anadir sigue siendo true (usuario no encontrado en el archivo) entonces lo introducimos
										//Si añadir false, quiere decir que el usuario ya se encuentra en el archivo
										//Por lo tanto no lo metemos
										if(anadir)
										{
											file << usuario << " " << contrasena << "\n";
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"+Ok. Usuario Registrado\n");
											send(i,buffer,strlen(buffer),0);
										}
										file.close();

									}//CIERRE REGISTRO

									/*-----------------------------------------------------------------------
											USUARIO
									------------------------------------------------------------------------ */

									else if(strstr(buffer, "USUARIO")!=NULL){

										if (FD_ISSET(i, &usuario_validado)) {
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-ERR. Usuario en estado validado, no puede iniciar sesión\n");
											send(i,buffer,strlen(buffer),0);
											break;
										}


										bool pedirContrasena=false; //variable para controlar si pedimos contraseña o no
										char usuario[20];
										int tamBuffer=strlen(buffer);
										bool var=false;

										std::string lineaFichero, busquedaUsuario;

										bzero(usuario, sizeof(usuario));
										strncpy(usuario, buffer+8, tamBuffer-9);//Metemos en usuario la cadena que va desde la posicion buffer+8
																														//hasta (tam_buffer-9) posiciones a la derecha

										//CONTROL MISMO USUARIO NO PUEDA LOGUEARSE 2 VECES
										for (size_t z = 0; z < usuarios.size(); z++) {
											if (strcmp(usuarios[z].c_str(), usuario) == 0) {
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"-ERR. Este usuario ya se encuentra logueado en el sistema, no puede iniciar sesión\n");
												send(i,buffer,strlen(buffer),0);
												var=true;
											}
										}

										if(var == true){
											break;
										}

										file.open("usuarios.txt", std::fstream::in);

										//Buscamos el usuario introducido por cliente en la base de datos
										//En caso de encontrarlo ponemos variable de estado a true para proseguir con la contraseña
										while(std::getline(file,lineaFichero)){

											//auxUser1 = strstr(lineaFichero.c_str(), " ");

										  busquedaUsuario=lineaFichero.substr(0, (strlen(strstr(lineaFichero.c_str(), " ")) - 1));

										  //std::cout << "Impresion valor busquedaUsuario: " << busquedaUsuario << '\n';

										  if(strcmp(busquedaUsuario.c_str(), usuario)==0){ //vamos comparando cada usuario del archivo con el introducido
										     pedirContrasena=true;
										     usuarios[i]=usuario; //mete el usuario en el vector, en la posicion: socket de ese usuario

										     FD_SET(i, &usuario_correcto);//añadimos el socket "i" (socket el usuario que estamos tratando ahora mismo)
											   									//al conjunto de sockets usuarios_correctos

										     bzero(buffer,sizeof(buffer));
										     sprintf(buffer,"+OK. Usuario correcto\n");
										     send(i,buffer,strlen(buffer),0);
										     file.close();
										     break;
										  }
										}

										if(!pedirContrasena){
										  bzero(buffer,sizeof(buffer));
										  sprintf(buffer,"-ERR. Usuario incorrecto\n");
										  send(i,buffer,strlen(buffer),0);
										  file.close();
										}
									}//CIERRE USUARIO

									/*-----------------------------------------------------------------------
									PASSWORD
									------------------------------------------------------------------------ */
									else if(strstr(buffer, "PASSWORD")!=NULL){

										if(FD_ISSET(i, &usuario_correcto)){//comprobamos que el usuario con este socket
																					  //se encuentre en usuarios correctos (usuario bien introducido)
																				  	  //para dejarle introducir la contraseña
											char contrasena[20];
											bzero(contrasena,sizeof(contrasena));

											std::string lineaFichero, busquedaUsuario, busquedaPass;

											strncpy(contrasena, buffer+9, strlen(buffer)-10);

											file.open("usuarios.txt", std::fstream::in);

											while(std::getline(file,lineaFichero)){
											   busquedaUsuario=lineaFichero.substr(0, strlen(usuarios[i].c_str())); //usuarios[i].c_str -> cadena con el usuario del socket i
											   if(strcmp(busquedaUsuario.c_str(), usuarios[i].c_str())==0){
											      busquedaPass=lineaFichero.substr(strlen(usuarios[i].c_str())+1, strlen(lineaFichero.c_str())-strlen(usuarios[i].c_str())-1);
											      if(strcmp(busquedaPass.c_str(), contrasena)==0){
											         FD_SET(i, &usuario_validado); //añadimos el socket i al conjunto de usuarios_validados
																								//posteriormente comprobaremos en el conjunto usuarios_validados
																								//para dejar al usuario iniciar partida o no


											         FD_CLR(i, &usuario_correcto); //borramos el socket i del conjunto de usuarios correctos
																								//porque ahora pertenece al conjunto de usuarios validados

											         bzero(buffer,sizeof(buffer)); //a partir de aqui pasamos informacion al usuario
											         sprintf(buffer,"+Ok. Usuario validado\n");
											         send(i,buffer,strlen(buffer),0);

											         file.close();
											         break;
											      }
											      else{
											         bzero(buffer,sizeof(buffer));
											         sprintf(buffer,"-ERR. Error en la validación\n");
											         send(i,buffer,strlen(buffer),0);
											         file.close();
											         break;
											      }
											   }
											}
									  }

									  //En caso de introducir el password antes que el usuario
										else{
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-ERR. Debe de introducir el usuario antes que la contraseña\n");
											send(i,buffer,strlen(buffer),0);
										}
									}//CIERRE PASSWORD


									/*-----------------------------------------------------------------------
									INICIAR PARTIDA
									------------------------------------------------------------------------ */
									else if(strcmp(buffer, "INICIAR-PARTIDA\n")==0){

										bool estado=false;


										//bzero(buffer,sizeof(buffer));
										//sprintf(buffer,"Usuario validado ha entrado en una partida\n");
										//send(i,buffer,strlen(buffer),0);

										/*
										if(FD_ISSET(i, &usuario_jugando)){
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"Usuario en estado jugando, por lo tanto no puede iniciar partida\n");
										send(i,buffer,strlen(buffer),0);
										break;
										}

										*/

										//if(FD_ISSET(i, &usuario_validado)){//Comprobamos si el usuario esta validado para dejar entrar en una partida o no
										if(true){
											if(partidas.size() == 0){
												nuevaPartida(partidas, i, usuario_esperandoPartida);
											}
											else if(partidas.size() > 0){
												for (size_t z = 0; z < partidas.size(); z++) {
													if((partidas[z].getSocket1() == -1) && (estado == false)){
														estado = true;
														nuevaPartidaEnPosicion(partidas, i, usuario_esperandoPartida, z);
													}
													else if((partidas[z].getSocket2() == -1) && (estado == false)){

														estado=true;

														int socket1=partidas[z].getSocket1();

														partidas[z].setSocket2(i);
														int socket2=partidas[z].getSocket2();


														FD_CLR(socket1, &usuario_esperandoPartida);
														FD_SET(socket1, &usuario_jugando);
														FD_SET(i, &usuario_jugando);

														bzero(buffer,sizeof(buffer));
														sprintf(buffer,"+OK. Empieza la partida.\n");
														send(i, buffer, strlen(buffer), 0);
														send(socket1 ,buffer,strlen(buffer),0);

														Jugador j1(socket1, &partidas[z]);
														partidas[z].nuevoJugador(&j1);

														Jugador j2(socket2, &partidas[z]);
														partidas[z].nuevoJugador(&j2);

														mostrarManoJugador(j1, socket1);
														mostrarManoJugador(j2, socket2);


														fichas[z] = partidas[z].iniciarPartida();
														partidas[z].setMasAlta(fichas[z]);

													/*	bzero(buffer, sizeof(buffer));
														sprintf(buffer, "Partida %d:", partidas[z].getIDPartida());
														cout << buffer << endl;

														bzero(buffer, sizeof(buffer));
														sprintf(buffer, "%s\n", fichas[z].mostrarFicha().c_str());
														cout << buffer << endl << endl;*/

														//Comprobar quien tiene el doble más alto o la ficha mas mas alta
														//Y decirle a este que es su turno y al otro que espere
														decidirTurnoInicial(j1, j2, fichas[z], socket1, socket2, partidas[z]);

													}

												}
												//FUERA DEL FOR. aqui he recorrido el vector entero y no he encontrado ningun espacio libre en ninguna partida

												if ( (estado == false) && (partidas.size() < MAX_PARTIDAS) ) {
													nuevaPartida(partidas, i, usuario_esperandoPartida);
												}



											}
											else if((partidas.size() == 10) && (partidas.back().getSocket2() != -1)){
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"Demasiadas partidas comenzadas.\n");
												send(i,buffer,strlen(buffer),0);
											}
									 }
										 else{
											bzero(buffer,sizeof(buffer));
											sprintf(buffer,"-ERR. Debe introducir usuario y contraseña correctamente para poder jugar\n");
											send(i,buffer,strlen(buffer),0);
										 }
									  }//CIERRE INICIAR-PARTIDA


									/*-----------------------------------------------------------------------
									COLOCAR-FICHA
									------------------------------------------------------------------------ */
									else if(strstr(buffer, "COLOCAR-FICHA")!=NULL){

										//Tengo que saber en que partida esta el usuario que ha escrito colocar-ficha
										//Para poder cambiar el tablero que le corresponde y no otro

										//if(FD_ISSET(i, &usuario_jugando)){

										if(true){

											//Comprobacion extremo
											char *auxIzquierda, *auxDerecha;
											int izquierdo, derecho, valorExtremo;
											char izquierda[20], derecha[20];

											bzero(izquierda, sizeof(izquierda));
											bzero(derecha, sizeof(derecha));

											auxIzquierda = strstr(buffer, "izquierd"); //izquierda -> 1
											auxDerecha = strstr(buffer, "derech"); //derecha -> 2



											if((auxIzquierda == NULL) && ((auxDerecha == NULL))){
												bzero(buffer,sizeof(buffer));
												sprintf(buffer,"-ERR. No ha introducido el extremo\n");
												send(i,buffer,strlen(buffer),0);
												break;
											}

											if(auxIzquierda != NULL){
												valorExtremo=1;
											}
											else{
												valorExtremo=2;
											}

											strncpy(izquierda, buffer+15, 1);
											strncpy(derecha, buffer+17, 1);

											izquierdo = atoi(izquierda);
											derecho = atoi(derecha);

											//El ahorcamiento por aqui y así queda todo bien ordenadito


											int idPartida, socket1, socket2;
 										 	setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);

											if(i == partidas[idPartida].getTurno()){
												if(partidas[idPartida].getJugador(i).colocarFicha(izquierdo, derecho, valorExtremo, &partidas[idPartida]) == true){

													mostrarTableroAJugador(socket1, partidas[idPartida]);
													mostrarTableroAJugador(socket2, partidas[idPartida]);

													if(partidas[idPartida].getJugador(i).nFichas() == 0){
														bzero(buffer,sizeof(buffer));
														sprintf(buffer, "\n+OK. Has ganado la partida.\n");
														send(i,buffer,strlen(buffer),0);
														if(i == socket1){
															bzero(buffer,sizeof(buffer));
															sprintf(buffer, "\n+OK. Has perdido la partida.\n");
															send(socket2,buffer,strlen(buffer),0);
														}
														else{
															bzero(buffer,sizeof(buffer));
															sprintf(buffer, "\n+OK. Has perdido la partida.\n");
															send(socket1,buffer,strlen(buffer),0);
														}

														partidas[idPartida].getJugador(socket1).salirPartida(&partidas[idPartida]);
			 										 	partidas[idPartida].getJugador(socket2).salirPartida(&partidas[idPartida]);
														partidas[idPartida].setSocket1(-1);
														partidas[idPartida].setSocket2(-1);

														FD_SET(socket1, &usuario_validado);
														FD_SET(socket2, &usuario_validado);

														FD_CLR(socket1, &usuario_jugando);
														FD_CLR(socket2, &usuario_jugando);

													}
													else{

														mostrarManoJugador(partidas[idPartida].getJugador(socket1), socket1);
														mostrarManoJugador(partidas[idPartida].getJugador(socket2), socket2);

														decidirTurno(i, socket1, socket2, partidas[idPartida]);

													}
												}
												else{
													bzero(buffer, sizeof(buffer));
													sprintf(buffer, "-ERR. La ficha no puede ser colocada.\n");
													send(i, buffer, strlen(buffer), 0);
												}
											}
											else{
												bzero(buffer, sizeof(buffer));
												sprintf(buffer, "-ERR No es tu turno.\n");
												send(i, buffer, strlen(buffer), 0);
											}

					                   }
					                   else{
					                      bzero(buffer,sizeof(buffer));
																sprintf(buffer, "-ERR. No esta dentro de una partida por lo tanto no puede colocar ficha\n");
					                      send(i,buffer,strlen(buffer),0);
					                   }
					       			}//CIERRE COLOCAR-FICHA


									/*-----------------------------------------------------------------------
									ROBAR-FICHA
									------------------------------------------------------------------------ */
									else if(strcmp(buffer, "ROBAR-FICHA\n")==0){

										//if(FD_ISSET(i, &usuario_jugando)){

										int idPartida, socket1, socket2;
									 	setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);

										if(true){//Quitar esta linea
											if(i == partidas[idPartida].getTurno()){
												if(partidas[idPartida].getJugador(i).puedePoner(&partidas[idPartida])){
													bzero(buffer,sizeof(buffer));
													sprintf(buffer,"+Ok. No es necesario robar ficha\n");
													send(i,buffer,strlen(buffer),0);
												}
												else{
													if(!partidas[idPartida].montonVacio()){
														Ficha a;
														a = partidas[idPartida].robar();
														partidas[idPartida].getJugador(i).robarFicha(a);

														mostrarFichaRobada(a, i);

														mostrarTableroAJugador(i, partidas[idPartida]);

														mostrarManoJugador(partidas[idPartida].getJugador(i), i);
													}
													else{
														bzero(buffer,sizeof(buffer));
														sprintf(buffer,"+Ok. No quedan fichas en el montón\n");
														send(i,buffer,strlen(buffer),0);

														decidirTurno(i, socket1, socket2, partidas[idPartida]);

													}
												}
											}
											else{
												bzero(buffer, sizeof(buffer));
												sprintf(buffer, "-ERR No es tu turno.\n");
												send(i, buffer, strlen(buffer), 0);
											}
										}

										else{
					                      bzero(buffer,sizeof(buffer));
					                      sprintf(buffer,"-ERR. No esta dentro de una partida por lo tanto no puede colocar ficha\n");
					                      send(i,buffer,strlen(buffer),0);
					                   }

									}//CIERRE ROBAR-FICHA



									/*-----------------------------------------------------------------------
									PASO-TURNO
									------------------------------------------------------------------------ */
									else if(strcmp(buffer, "PASO-TURNO\n")==0){

										//if(FD_ISSET(i, &usuario_jugando)){

										int idPartida, socket1, socket2;
									 	setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);


										if(true){//Quitar esta linea
											if(i == partidas[idPartida].getTurno()){
												if((partidas[idPartida].getJugador(i).puedePoner(&partidas[idPartida])) or (!partidas[idPartida].montonVacio())){
													bzero(buffer,sizeof(buffer));
													sprintf(buffer,"+Ok. No es necesario pasar turno.\n");
													send(i,buffer,strlen(buffer),0);
												}
												else if((!partidas[idPartida].getJugador(i).puedePoner(&partidas[idPartida])) and (partidas[idPartida].montonVacio())){
													decidirTurno(i, socket1, socket2, partidas[idPartida]);
												}
											}
											else{
												bzero(buffer, sizeof(buffer));
												sprintf(buffer, "-ERR. No es tu turno.\n");
												send(i, buffer, strlen(buffer), 0);
											}
										}

										else{
					                      bzero(buffer,sizeof(buffer));
					                      sprintf(buffer,"-ERR. No esta dentro de una partida por lo tanto no puede pasar turno\n");
					                      send(i,buffer,strlen(buffer),0);
					                   }

									}//CIERRE PASO-TURNO

									/*-----------------------------------------------------------------------
									IDPARTIDA
									------------------------------------------------------------------------ */
									else if(strcmp(buffer, "IDPARTIDA\n")==0){

										//if(FD_ISSET(i, &usuario_jugando)){

										int idPartida, socket1, socket2;
									 	setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);


										if(true){//Quitar esta linea
											bzero(buffer, sizeof(buffer));
											sprintf(buffer, "PARTIDA %d\n", partidas[idPartida].getJugador(i).getIDPartida());
											send(i,buffer,strlen(buffer),0);
										}

									}//CIERRE IDPARTIDA


									else{
										bzero(buffer,sizeof(buffer));
										sprintf(buffer,"-Err. Comando no renococido\n");
										send(i,buffer,strlen(buffer),0);
									}
								}
							}
							 //Si el cliente introdujo ctrl+c
							 if(recibidos == 0)
							 {
								  printf("El socket %d, ha introducido ctrl+c\n", i);
								  //Eliminar ese socket
								  salirCliente(i,&readfds,&numClientes,arrayClientes);
							 }
						}
				  }
			 }
		}
	}

	close(sd);
	return 0;

}
