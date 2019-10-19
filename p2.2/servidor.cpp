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


#define MSG_SIZE 250
#define MAX_CLIENTS 50


/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);



int main ( )
{

	/*----------------------------------------------------
		Descriptor del socket y buffer de datos
	-----------------------------------------------------
	std::fstream file;
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
  fd_set readfds, auxfds, ask_password, auth, waiting_for_player, playing;
  int salida;
  int arrayClientes[MAX_CLIENTS];
  std::map<int, std::string> usuarios;
  int numClientes = 0;
  //contadores
  int i,j,k;
  int recibidos;
  char identificador[MSG_SIZE];

  int on, ret;
*/

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
    //signal(SIGINT,manejador);

	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
	while(1){

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

											strcpy(buffer, "+Ok. Usuario conectado\n");

											send(new_sd,buffer,strlen(buffer),0);

											for(j=0; j<(numClientes-1);j++){

												 bzero(buffer,sizeof(buffer));
												 sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
												 send(arrayClientes[j],buffer,strlen(buffer),0);
											}
									  }
									  else
									  {
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"Demasiados clientes conectados\n");
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

									  for (j = 0; j < numClientes; j++){
											send(arrayClientes[j], "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
											close(arrayClientes[j]);
											FD_CLR(arrayClientes[j],&readfds);
									  }
											close(sd);
											exit(-1);


								 }
								 //Mensajes que se quieran mandar a los clientes (implementar)

							}
							//INFORMACION RECIBIDA DEL CLIENTE
							else{
								 bzero(buffer,sizeof(buffer));

								 recibidos = recv(i,buffer,sizeof(buffer),0);

								 if(recibidos > 0){

									 if(strcmp(buffer,"SALIR\n") == 0){
										 salirCliente(i,&readfds,&numClientes,arrayClientes);
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
												strcpy(buffer,"-ERR. Usuario en estado correcto, no puede registrarse\0");
												send(i,buffer,strlen(buffer),0);
												break;
											}

											if (FD_ISSET(i, &usuario_validado)) {
												bzero(buffer,sizeof(buffer));
												strcpy(buffer,"-ERR. Usuario en estado validado, no puede registrarse\0");
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
												strcpy(buffer,"-ERR. Sentencia de registro incorrecta\0");
												send(i,buffer,strlen(buffer),0);
												break;
											}

											//Con estos -1 quitamos los '\0' || lenght_aux_user2 -> Tamaño desde " -p" hasta el final del buffer
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
													strcpy(buffer,"-ERR. Usuario existente\0");
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
												strcpy(buffer,"+Ok. Usuario Registrado\0");
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
												strcpy(buffer,"-ERR. Usuario en estado validado, no puede inicar sesión\0");
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
													strcpy(buffer,"-ERR. Este usuario ya se encuentra logueado en el sistema, no puede inicar sesión\0");
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
											     strcpy(buffer,"+OK. Usuario correcto\0");
											     send(i,buffer,strlen(buffer),0);
											     file.close();
											     break;
											  }
											}

											if(!pedirContrasena){
											  bzero(buffer,sizeof(buffer));
											  strcpy(buffer,"-ERR. Usuario incorrecto\0");
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
												         strcpy(buffer,"+Ok. Usuario validado\0");
												         send(i,buffer,strlen(buffer),0);

												         file.close();
												         break;
												      }
												      else{
												         bzero(buffer,sizeof(buffer));
												         strcpy(buffer,"-ERR. Error en la validación\0");
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
												strcpy(buffer,"-ERR. Debe de introducir el usuario antes que la contraseña\0");
												send(i,buffer,strlen(buffer),0);
											}
										}//CIERRE PASSWORD


										/*-----------------------------------------------------------------------
										INICIAR PARTIDA
										------------------------------------------------------------------------ */
										else if(strcmp(buffer, "INICIAR-PARTIDA\n")==0){

											bool estado=false;

		                           if(FD_ISSET(i, &usuario_validado)){//Comprobamos si el usuario esta validado para dejar entrar en una partida o no
											//bzero(buffer,sizeof(buffer));
	                              //strcpy(buffer,"Usuario validado ha entrado en una partida\0");
	                              //send(i,buffer,strlen(buffer),0);

												if(partidas.size()>0){
													for (size_t z = 0; z < partidas.size(); z++) {
														if((partidas[z].getSocket2() == -1) && (estado == false)){

															estado=true;

															Ficha a;

															int socket1=partidas[z].getSocket1();

															partidas[z].setSocket2(i);
															int socket2=partidas[z].getSocket2();


															FD_CLR(socket1, &usuario_esperandoPartida);
															FD_SET(socket1, &usuario_jugando);
															FD_SET(i, &usuario_jugando);

															bzero(buffer,sizeof(buffer));
															strcpy(buffer,"+OK. Empienza la partida.\0");
															send(i, buffer, strlen(buffer), 0);
															send(socket1 ,buffer,strlen(buffer),0);

															Jugador j1(socket1, &partidas[z]);
															partidas[z].nuevoJugador(&j1);


															Jugador j2(socket2, &partidas[z]);
															partidas[z].nuevoJugador(&j2);


															bzero(buffer,sizeof(buffer));
															strcpy(buffer, j1.mostrarMano().c_str());
															send(socket1,buffer,strlen(buffer),0);

															bzero(buffer,sizeof(buffer));
															strcpy(buffer, j2.mostrarMano().c_str());
															send(socket2,buffer,strlen(buffer),0);




															a = partidas[z].iniciarPartida();

															if(j1.existeFicha(a)){
																bzero(buffer,sizeof(buffer));
																strcpy(buffer,"\0+OK. Turno de partida\0");
																send(socket1, buffer, strlen(buffer),0);

																bzero(buffer,sizeof(buffer));
																strcpy(buffer,"\0+OK. Turno del otro jugador\0");
																send(socket2, buffer, strlen(buffer),0);


															}
															else if(j2.existeFicha(a)){
																bzero(buffer,sizeof(buffer));
																strcpy(buffer,"\0+OK. Turno de partida\0");
																send(socket2 ,buffer,strlen(buffer),0);

																bzero(buffer,sizeof(buffer));
																strcpy(buffer,"\0+OK. Turno del otro jugador\0");
																send(socket1 ,buffer,strlen(buffer),0);
															}

															//Comprobar quien tiene el doble más alto o la ficha mas mas alta
															//Y decirle a este que es su turno y al otro que espere

														}

													}
													//FUERA DEL FOR. aqui he recorrido el vector entero

													if ( (estado == false) && (partidas.size() < 10) ) {
														Partida nuevo;
														nuevo.setSocket1(i);
														FD_SET(i, &usuario_esperandoPartida);
														partidas.push_back(nuevo);

														bzero(buffer,sizeof(buffer));
														strcpy(buffer,"+Ok. Petición Recibida. Quedamos a la espera de más jugadores\0");
														send(i,buffer,strlen(buffer),0);
													}


												}
												else{
													//inicializar el vector
													Partida nuevo;
													nuevo.setSocket1(i);
													FD_SET(i, &usuario_esperandoPartida);
													partidas.push_back(nuevo);

													bzero(buffer,sizeof(buffer));
													strcpy(buffer,"+Ok. Petición Recibida. Quedamos a la espera de más jugadores\0");
													send(i,buffer,strlen(buffer),0);
												}
		                           }
		                           else{
		                              bzero(buffer,sizeof(buffer));
		                              strcpy(buffer,"-ERR. Debe introducir usuario y contraseña correctamente para poder jugar\0");
		                              send(i,buffer,strlen(buffer),0);
		                           }
		                        }//CIERRE INICIAR-PARTIDA

										/*-----------------------------------------------------------------------
										COLOCAR-FICHA
										------------------------------------------------------------------------ */
										else if(strstr(buffer, "COLOCAR-FICHA")!=NULL){

											//Tengo que saber en que partida esta el usuario que ha escrito colocar-ficha
											//Para poder cambiar el tablero que le corresponde y no otro

		                           //if(FD_ISSET(i, &usuario_correcto)){

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
													strcpy(buffer,"-ERR. No ha introducido el extremo\0");
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

												/*
												int idPartida;
												for(int z = 0; z < partidas.size(); z++){
													if((partidas[z].getSocket1() == i) || (partidas[z].getSocket2() == i)){
														idPartida = z;
													}
												}
												partidas[idPartida].getJugador(i).colocarFicha(&partidas[idPartida]);
												*/

		                           }
		                           else{
		                              bzero(buffer,sizeof(buffer));
		                              strcpy(buffer,"-ERR. No esta dentro de una partida por lo tanto no puede introducir ficha\0");
		                              send(i,buffer,strlen(buffer),0);
		                           }
		               }



										else{
											bzero(buffer,sizeof(buffer));
											strcpy(buffer,"-ERR. Comando no renococido\0");
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
