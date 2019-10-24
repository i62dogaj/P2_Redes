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
  std::vector<int> contadorFichas;
  for (size_t z = 0; z < 7; z++) {
    contadorFichas.push_back(0);
  }

	//contadores
	int i,j;
	int recibidos;
  int nPartidas = 0;
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

										enviarMensaje(new_sd, "+Ok. Usuario conectado\n");

										for(j=0; j<(numClientes-1);j++){

											 bzero(buffer,sizeof(buffer));
											 sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
											 send(arrayClientes[j],buffer,strlen(buffer),0);
										}
								  }
								  else{
										enviarMensaje(new_sd, "Demasiados clientes conectados\n");
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
									 salirCliente(i,&readfds,&numClientes,arrayClientes,&usuario_correcto, &usuario_validado, &usuario_esperandoPartida, &usuario_jugando, usuarios);
                            //salirCliente(i,&readfds,&numClientes,arrayClientes);

									 //Solo si estaba jugando cambiamos lo siguiente
									 if (FD_ISSET(i, &usuario_jugando)){
										 int idPartida, socket1, socket2;
										 setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);

                               salirAmbos(partidas[idPartida], socket1, socket2, nPartidas);

										if(who == socket1){
											FD_SET(socket2, &usuario_validado);
											FD_CLR(socket2, &usuario_jugando);

											enviarMensaje(socket2,"+OK. Si quiere, puede iniciar otra partida.\n");
										}
										else{
											FD_SET(socket1, &usuario_validado);
											FD_CLR(socket1, &usuario_jugando);

											enviarMensaje(socket1,"+OK. Si quiere, puede iniciar otra partida.\n");
										}
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
											enviarMensaje(i,"-ERR. Usuario en estado correcto, no puede registrarse\n");
											break;
										}

										if (FD_ISSET(i, &usuario_validado)) {
											enviarMensaje(i,"-ERR. Usuario en estado validado, no puede registrarse\n");
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
											enviarMensaje(i,"-ERR. Sentencia de registro incorrecta\n");
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
												enviarMensaje(i,"-ERR. Usuario existente\n");
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
											enviarMensaje(i,"+Ok. Usuario Registrado\n");
										}
										file.close();

									}//CIERRE REGISTRO

									/*-----------------------------------------------------------------------
											USUARIO
									------------------------------------------------------------------------ */

									else if(strstr(buffer, "USUARIO")!=NULL){

                              /*if (FD_ISSET(i, &usuario_validado)) {
											enviarMensaje(i,"-ERR. Usuario en estado validado, no puede iniciar sesión\n");
											break;
                              }*/


										bool pedirContrasena=false; //variable para controlar si pedimos contraseña o no
										char usuario[20];
										int tamBuffer=strlen(buffer);
										bool var=false;

										std::string lineaFichero, busquedaUsuario;

										bzero(usuario, sizeof(usuario));
										strncpy(usuario, buffer+8, tamBuffer-9);//Metemos en usuario la cadena que va desde la posicion buffer+8
																														//hasta (tam_buffer-9) posiciones a la derecha

										//CONTROL MISMO USUARIO NO PUEDA ESTAR LOGUEADO EN 2 TERMINALES AL MISMO TIEMPO
										for (size_t z = 0; z < usuarios.size(); z++) {
											if (strcmp(usuarios[z].c_str(), usuario) == 0) {
												enviarMensaje(i,"-ERR. Este usuario ya se encuentra logueado en el sistema, no puede iniciar sesión\n");
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

										     enviarMensaje(i,"+OK. Usuario correcto\n");
										     file.close();
										     break;
										  }
										}

										if(!pedirContrasena){
										  enviarMensaje(i,"-ERR. Usuario incorrecto\n");
										  file.close();
										}
									}//CIERRE USUARIO

									/*-----------------------------------------------------------------------
									PASSWORD
									------------------------------------------------------------------------ */
									else if(strstr(buffer, "PASSWORD")!=NULL){

                              /*if (FD_ISSET(i, &usuario_validado)) {
											enviarMensaje(i,"-ERR. Usuario en estado validado, no puede iniciar sesión\n");
											break;
                              }*/

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

											         //a partir de aqui pasamos informacion al usuario
											         enviarMensaje(i,"+Ok. Usuario validado\n");

											         file.close();
											         break;
											      }
											      else{
											         enviarMensaje(i,"-ERR. Error en la validación\n");
											         file.close();
											         break;
											      }
											   }
											}
									  }

									  //En caso de introducir el password antes que el usuario
										else{
											enviarMensaje(i,"-ERR. Debe de introducir el usuario antes que la contraseña\n");
										}
									}//CIERRE PASSWORD


									/*-----------------------------------------------------------------------
									INICIAR PARTIDA
									------------------------------------------------------------------------ */
									else if(strcmp(buffer, "INICIAR-PARTIDA\n")==0){

										bool estado=false;

										//enviarMensaje(i,"Usuario validado ha entrado en una partida\n");


										/*if(FD_ISSET(i, &usuario_jugando)){
										enviarMensaje(i,"Usuario en estado jugando, por lo tanto no puede iniciar partida\n");
                              break;
                              }*/



										if(FD_ISSET(i, &usuario_validado)){//Comprobamos si el usuario esta validado para dejar entrar en una partida o no
										//if(true){
											if(nPartidas == 0){
                                    			FD_SET(i, &usuario_esperandoPartida);
												nuevaPartida(nPartidas, partidas, i);
											}

											else if(nPartidas > 0){
                                   				if (nPartidas <= MAX_PARTIDAS) {
                                       				for (size_t z = 0; z < partidas.size(); z++) {
                                          				if((partidas[z].getSocket1() == -1) && (estado == false)){

				                                            estado = true;
				                                            FD_SET(i, &usuario_esperandoPartida);
				                                            nuevaPartidaEnPosicion(nPartidas, partidas, i, z);

                                          				}

			                                          	else if((partidas[z].getSocket2() == -1) && (estado == false)){
					 											estado=true;
					                                        lanzarPartida(partidas[z], fichas[z], i);
					                                        FD_CLR(partidas[z].getSocket1(), &usuario_esperandoPartida);
					                                       	FD_SET(partidas[z].getSocket1(), &usuario_jugando);
					                                       	FD_SET(i, &usuario_jugando);
					                          				//partidas[z].limpiarMonton(contadorFichas); //Para probar final 2 (BORRAR)
			                                          	}
                                       }

                          // Si no ha encontrado ningún hueco en ninguna partida, crea una nueva.
                                   if((nPartidas != MAX_PARTIDAS) && (estado == false)){
                                     estado = true;
                                     FD_SET(i, &usuario_esperandoPartida);
                                     nuevaPartida(nPartidas, partidas, i);
    											}
  											}
                                 if((nPartidas == MAX_PARTIDAS) && (estado == false) && (!hayHueco(nPartidas, partidas))){
     											enviarMensaje(i,"Demasiadas partidas comenzadas.\n");
     										}
												//FUERA DEL FOR. aqui he recorrido el vector entero y no he encontrado ningun espacio libre en ninguna partida
										}


									 }
										 else{
											enviarMensaje(i,"-ERR. Debe introducir usuario y contraseña correctamente para poder jugar\n");
										 }
									  }//CIERRE INICIAR-PARTIDA


									/*-----------------------------------------------------------------------
									COLOCAR-FICHA
									------------------------------------------------------------------------ */
									else if(strstr(buffer, "COLOCAR-FICHA")!=NULL){

										//Tengo que saber en que partida esta el usuario que ha escrito colocar-ficha
										//Para poder cambiar el tablero que le corresponde y no otro

										if(FD_ISSET(i, &usuario_jugando)){

										//if(true){

											//Comprobacion extremo
											char *auxIzquierda, *auxDerecha;
											int izquierdo, derecho, valorExtremo;
											char izquierda[20], derecha[20];

											bzero(izquierda, sizeof(izquierda));
											bzero(derecha, sizeof(derecha));

											auxIzquierda = strstr(buffer, "izquierd"); //izquierda -> 1
											auxDerecha = strstr(buffer, "derech"); //derecha -> 2



											if((auxIzquierda == NULL) && ((auxDerecha == NULL))){
												enviarMensaje(i,"-ERR. No ha introducido el extremo\n");
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
                          //Aumentamos las veces que ha aparecido cada número
                          if(izquierdo != derecho){
                            contadorFichas[izquierdo]++;
                            contadorFichas[derecho]++;
                          }
                          else if(izquierdo == derecho){
                            contadorFichas[izquierdo]++;
                          }

													mostrarTableroAJugador(socket1, partidas[idPartida]);
													mostrarTableroAJugador(socket2, partidas[idPartida]);

													// ----------- FINAL POR QUEDARSE SIN FICHAS -----------
													if(partidas[idPartida].getJugador(i).nFichas() == 0){
                            saberGanadorNoFichas(i, socket1, socket2);
														salirAmbos(partidas[idPartida], socket1, socket2, nPartidas);
                            FD_SET(socket1, &usuario_validado);
                          	FD_SET(socket2, &usuario_validado);

                          	FD_CLR(socket1, &usuario_jugando);
                          	FD_CLR(socket2, &usuario_jugando);
													}
                          // ----------- FINAL POR PUNTOS -----------
                          else if((partidas[idPartida].montonVacio()) && (partidas[idPartida].getExtI() == partidas[idPartida].getExtD()) && (contadorFichas[partidas[idPartida].getExtI()] == 7)){
                            enviarMensaje(socket1, "\n+OK. La partida ya no puede avanzar.\n");
                            enviarMensaje(socket2, "\n+OK. La partida ya no puede avanzar.\n");
                            saberGanadorPuntos(partidas[idPartida], socket1, socket2);
                            salirAmbos(partidas[idPartida], socket1, socket2, nPartidas);
                            FD_SET(socket1, &usuario_validado);
                          	FD_SET(socket2, &usuario_validado);

                          	FD_CLR(socket1, &usuario_jugando);
                          	FD_CLR(socket2, &usuario_jugando);
                          }
                          //Ninguno puede poner ni robar --> Terminamos la partida
                          else if((partidas[idPartida].montonVacio()) && (partidas[idPartida].getJugador(socket1).tieneFichas()) && (partidas[idPartida].getJugador(socket2).tieneFichas()) && (!partidas[idPartida].getJugador(socket1).puedePoner(&partidas[idPartida])) && (!partidas[idPartida].getJugador(socket2).puedePoner(&partidas[idPartida]))){
                            enviarMensaje(socket1, "\n+OK. La partida ya no puede avanzar.\n");
                            enviarMensaje(socket2, "\n+OK. La partida ya no puede avanzar.\n");
                            saberGanadorPuntos(partidas[idPartida], socket1, socket2);
                            salirAmbos(partidas[idPartida], socket1, socket2, nPartidas);
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
													enviarMensaje(i, "-ERR. La ficha no puede ser colocada.\n");
												}
											}
											else{
												enviarMensaje(i, "-ERR No es tu turno.\n");
											}

					                   }
					                   else{
																enviarMensaje(i, "-ERR. No esta dentro de una partida por lo tanto no puede colocar ficha\n");
					                   }
					       			}//CIERRE COLOCAR-FICHA


									/*-----------------------------------------------------------------------
									ROBAR-FICHA
									------------------------------------------------------------------------ */
									else if(strcmp(buffer, "ROBAR-FICHA\n")==0){

										if(FD_ISSET(i, &usuario_jugando)){

										int idPartida, socket1, socket2;
									 	setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);

										//if(true){//Quitar esta linea
											if(i == partidas[idPartida].getTurno()){
												if(partidas[idPartida].getJugador(i).puedePoner(&partidas[idPartida])){
													enviarMensaje(i,"+Ok. No es necesario robar ficha\n");
												}
												else{
													if(!partidas[idPartida].montonVacio()){
														Ficha a;
														a = partidas[idPartida].robar();
														partidas[idPartida].getJugador(i).robarFicha(a);

														mostrarFichaRobada(a, i);

														mostrarTableroAJugador(i, partidas[idPartida]);

														mostrarManoJugador(partidas[idPartida].getJugador(i), i);

                            enviarMensaje(i, "\n+OK. Turno de partida.\n");
													}
													else{
														enviarMensaje(i,"+Ok. No quedan fichas en el montón\n");

                            // ----------- FINAL POR PUNTOS -----------
														//Si se atasca la partida porque los extremos son iguales y ya se han puesto todas las fichas de ese número
                            if((partidas[idPartida].getExtI() == partidas[idPartida].getExtD()) && (contadorFichas[partidas[idPartida].getExtI()] == 7)){
                              enviarMensaje(socket1, "\n+OK. La partida ya no puede avanzar.\n");
                              enviarMensaje(socket2, "\n+OK. La partida ya no puede avanzar.\n");
                              saberGanadorPuntos(partidas[idPartida], socket1, socket2);
                              salirAmbos(partidas[idPartida], socket1, socket2, nPartidas);
                              FD_SET(socket1, &usuario_validado);
                            	FD_SET(socket2, &usuario_validado);

                            	FD_CLR(socket1, &usuario_jugando);
                            	FD_CLR(socket2, &usuario_jugando);
                            }
														//Si no puede poner ni robar, pero el otro jugador sí puede poner --> Pasar turno
														else if((partidas[idPartida].getJugador(socket1).tieneFichas()) && (partidas[idPartida].getJugador(socket2).tieneFichas()) && (!partidas[idPartida].getJugador(i).puedePoner(&partidas[idPartida])) && (partidas[idPartida].getJugador(otroSocket(i,partidas[idPartida])).puedePoner(&partidas[idPartida]))){
															enviarMensaje(i,"+Ok. No puede poner fichas ni robar. Pase turno.\n");
														}
                            //Ninguno puede poner ni robar --> Terminamos la partida
                            else if((partidas[idPartida].getJugador(socket1).tieneFichas()) && (partidas[idPartida].getJugador(socket2).tieneFichas()) && (!partidas[idPartida].getJugador(socket1).puedePoner(&partidas[idPartida])) && (!partidas[idPartida].getJugador(socket2).puedePoner(&partidas[idPartida]))){
                              enviarMensaje(socket1, "\n+OK. La partida ya no puede avanzar.\n");
                              enviarMensaje(socket2, "\n+OK. La partida ya no puede avanzar.\n");
                              saberGanadorPuntos(partidas[idPartida], socket1, socket2);
                              salirAmbos(partidas[idPartida], socket1, socket2, nPartidas);
                              FD_SET(socket1, &usuario_validado);
                            	FD_SET(socket2, &usuario_validado);

                            	FD_CLR(socket1, &usuario_jugando);
                            	FD_CLR(socket2, &usuario_jugando);
                            }
                        	}
												}
											}
											else{
												enviarMensaje(i, "-ERR No es tu turno.\n");
											}
										}

										else{
					                      enviarMensaje(i,"-ERR. No esta dentro de una partida por lo tanto no puede colocar ficha\n");
					                   }

									}//CIERRE ROBAR-FICHA



									/*-----------------------------------------------------------------------
									PASO-TURNO
									------------------------------------------------------------------------ */
									else if(strcmp(buffer, "PASO-TURNO\n")==0){

										if(FD_ISSET(i, &usuario_jugando)){

										int idPartida, socket1, socket2;
									 	setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);


										//if(true){//Quitar esta linea
											if(i == partidas[idPartida].getTurno()){
												if((partidas[idPartida].getJugador(i).puedePoner(&partidas[idPartida])) or (!partidas[idPartida].montonVacio())){
													enviarMensaje(i,"+Ok. No es necesario pasar turno.\n");
												}
												else if((!partidas[idPartida].getJugador(i).puedePoner(&partidas[idPartida])) and (partidas[idPartida].montonVacio())){
													decidirTurno(i, socket1, socket2, partidas[idPartida]);
												}
											}
											else{
												enviarMensaje(i, "-ERR. No es tu turno.\n");
											}
										}

										else{
	                      enviarMensaje(i,"-ERR. No esta dentro de una partida por lo tanto no puede pasar turno\n");
	                   }

									}//CIERRE PASO-TURNO

									/*-----------------------------------------------------------------------
									IDPARTIDA
									------------------------------------------------------------------------ */
									else if(strcmp(buffer, "IDPARTIDA\n")==0){

										if((FD_ISSET(i, &usuario_jugando)) || (FD_ISSET(i, &usuario_esperandoPartida))){

										int idPartida, socket1, socket2;
									 	setIDPartidaySockets(i, partidas, idPartida, socket1, socket2);

										//if(true){//Quitar esta linea
											bzero(buffer,sizeof(buffer));
											sprintf(buffer, "PARTIDA %d\n", partidas[idPartida].getJugador(i).getIDPartida());
											send(i,buffer,strlen(buffer),0);
										}

									}//CIERRE IDPARTIDA


									else{
										enviarMensaje(i,"-Err. Comando no renococido\n");
									}
								}
							}
							 //Si el cliente introdujo ctrl+c
							 if(recibidos == 0)
							 {
								  printf("El socket %d, ha introducido ctrl+c\n", i);
								  //Eliminar ese socket
                          salirCliente(i,&readfds,&numClientes,arrayClientes,&usuario_correcto, &usuario_validado, &usuario_esperandoPartida, &usuario_jugando, usuarios);
                          //salirCliente(i,&readfds,&numClientes,arrayClientes);
							 }
						}
				  }
			 }
		}
	}

	close(sd);
	return 0;

}
