#include "jugador.hpp"
#include "partida.hpp"
#include <iostream>
using namespace std;

/*int Menu(){
	int opt;
	cout << "\n\n	[0] SALIR\n";
	cout << "	[1] Iniciar Partida\n";
	cout << "	Opción: ";
	cin >> opt;
	cout << endl << endl;
	return opt;
}*/

int MenuPartida(int turno){
	int opt;
	cout << "\n\n	JUGADOR " << turno << "\n\n";
	cout << "	[0] SALIR\n";
	cout << "	[1] Ver Tablero\n";
	cout << "	[2] Ver Mano\n";
	cout << "	[3] Colocar ficha\n";
	cout << "	[4] Robar\n";
	cout << "	[5] Pasar turno\n\n";
	cout << "	Opción: ";
	cin >> opt;
	cout << endl << endl;
	return opt;
}

int main(){
	cout << endl;

	vector <Partida> partida;
	Partida p;
	partida.push_back(p);
	int id, turno;
	string buffer;
	//cout << "N Jugadores: " << p.getNJugadores() << endl;
	Ficha a;
	Jugador j0(0, &p);
	p.nuevoJugador(&j0);
	//j0.setID(id);
	cout << "Jugador 0:\n";
	buffer = j0.mostrarMano();
	cout << buffer << endl;
	Jugador j1(1, &p);
	p.nuevoJugador(&j1);
	//j1.setID(id);
	cout << "Jugador 1:\n";
	buffer = j1.mostrarMano();
	cout << buffer << endl;
/*	Jugador j2(2,"paco", "1234", &p);
	p.nuevoJugador(&j2);
	//j2.setID(id);
	cout << "Jugador 2:\n";
	j2.mostrarMano();
	cout << endl;*/
	int opt, NI, ND, extremo;



	/*do{
		opcion = Menu();
		switch(opcion){
			case 0:
				cout << "	Has salido del juego.\n";
				break;

			case 1:*/
				cout << "	Bienvenidos a la partida.\n";
				a = p.iniciarPartida();
				//a.setNI(p.iniciarPartida().getNI());
				//a.setND(p.iniciarPartida().getND());
				//cout << "Ficha más alta:\n";
				//a.mostrarFicha();
				//cout << "\n	PUNTOS JUGADOR 0: " << j0.getPuntos() << endl;
				//cout << "	PUNTOS JUGADOR 1: " << j1.getPuntos() << endl << endl;

				//cout << "JUGADOR: " << turno << endl;
				//fflush(stdout);
				if(j0.existeFicha(a)){
					turno = 0;
					cout << "	Comienza el jugador 0.\n\n";
				}
				else if(j1.existeFicha(a)){
					turno = 1;
					cout << "	Comienza el jugador 1.\n\n";
				}
			//	else if(turno == j2.getID()){ cout << "Comienza el jugador 2.\n\n";}
				//cout << "N Jugadores: " << p.getNJugadores() << endl;
				do{
					opt = MenuPartida(turno);
					switch(opt){
						case 0:
							if(turno == j0.getID()) j0.salirPartida(&p);
							else if(turno == j1.getID()) j1.salirPartida(&p);
						//	else if(turno == j2.getID()) j2.salirPartida(&p);
							cout << "	Has salido de la partida\n";
							break;

						case 1:
							buffer = p.mostrarTablero();
							cout << buffer << endl;
							break;

						case 2:
							if(turno == j0.getID()){
								buffer = j0.mostrarMano();
								cout << buffer << endl;
							}
							else if(turno == j1.getID()){
								buffer = j1.mostrarMano();
								cout << buffer << endl;
							}
						//	else if(turno == j2.getID()) j2.mostrarMano();
							//j0.mostrarMano();
							break;

						case 3:
						cout << "NI: ";
						cin >> NI;
						cout << "ND: ";
						cin >> ND;
						cout << "\n[1] Izquierda    [2] Derecha: ";
						cin >> extremo;
						if(turno == j0.getID()){
							if(j0.colocarFicha(NI, ND, extremo, &p)) turno = 1;
							else cout << "\nEsta ficha no se puede colocar\n";
						}
						else if(turno == j1.getID()){
							if(j1.colocarFicha(NI, ND, extremo, &p)) turno = 0;
							else cout << "\nEsta ficha no se puede colocar\n";
						}
						//	else if(turno == j2.getID()) j2.colocarFicha(&p);
							//j0.colocarFicha(&p);
							break;

						case 4:
								if(turno == j0.getID()){
									if(!j0.puedePoner(&p)){
										if(p.montonVacio()){
											cout << "	No quedan fichas ni puede poner. Pase turno.\n\n";
										}
										else{
											a = p.robar();
											j0.robarFicha(a);
											cout << "	Has robado la ficha |" <<	a.getNI() << "|" << a.getND() << "| del montón.\n\n";
											cout << "	Tu mano queda:\n\n";
											j0.mostrarMano();
										}
									}
									else{
										cout << "	No hace falta robar.\n\n";
									}
								}
								else if(turno == j1.getID()){
									if(!j1.puedePoner(&p)){
										if(p.montonVacio()){
											cout << "	No quedan fichas ni puede poner. Pase turno.\n\n";
										}
										else{
											a = p.robar();
											j1.robarFicha(a);
											cout << "	Has robado la ficha |" <<	a.getNI() << "|" << a.getND() << "| del montón.\n\n";
											cout << "	Tu mano queda:\n\n";
											j1.mostrarMano();
										}
									}
									else{
										cout << "	No hace falta robar.\n\n";
									}
								}

							break;

							case 5:
							if(turno == 0){
								if((j0.puedePoner(&p)) || (!p.montonVacio())){
									cout << "	No puede pasar turno mientras pueda poner fichas o robar.\n";
								}
								else if((!j0.puedePoner(&p)) && (p.montonVacio())){
									turno = 1;
									cout << "	Ha pasado turno.\n";
								}
							}
							else if(turno == 1){
								if((j1.puedePoner(&p)) || (!p.montonVacio())){
									cout << "	No puede pasar turno mientras pueda poner fichas o robar.\n";
								}
								else if((!j1.puedePoner(&p)) && (p.montonVacio())){
									turno = 0;
									cout << "	Ha pasado turno.\n";
								}
							}
								break;

						default:
							cout << "	Esa opción no existe.\n";
					}

					/* ------ FINAL 1 ------ */
					/* Si alguno ha colocado todas sus fichas, se termina la partida y gana. */
					if((j0.nFichas() == 0) && (opt != 0)){
						cout << "\n\n	SE ACABÓ LA PARTIDA.\n	EL JUGADOR 0 HA PUESTO TODAS SUS FICHAS Y HA GANADO.\n\n";// CON " << j1.getPuntos() << " PUNTOS.\n\n";
						opt = 0;
					}
					else if((j1.nFichas() == 0) && (opt != 0)){
						cout << "\n\n	SE ACABÓ LA PARTIDA.\n	EL JUGADOR 1 HA PUESTO TODAS SUS FICHAS Y HA GANADO.\n\n";// CON " << j0.getPuntos() << " PUNTOS.\n\n";
						opt = 0;
					}

					/* ------ FINAL 2 ------ */
					/* Si el montón está vacío y nadie puede poner ficha, se termina la Partida
					   y gana el que menos puntos tuviera. */
					if((j0.tieneFichas()) && (j1.tieneFichas()) && (!j0.puedePoner(&p)) && (!j1.puedePoner(&p)) && (p.montonVacio())){
						if(j0.getPuntos() < j1.getPuntos()){
							cout << "\n\n	SE ACABÓ LA PARTIDA.\n	HA GANADO EL JUGADOR 0.\n\n";// CON " << j0.getPuntos() << " PUNTOS.\n\n";
							opt = 0;
						}
						else if(j1.getPuntos() < j0.getPuntos()){
							cout << "\n\n	SE ACABÓ LA PARTIDA.\n	HA GANADO EL JUGADOR 1.\n\n";// CON " << j1.getPuntos() << " PUNTOS.\n\n";
							opt = 0;
						}
						else if(j0.getPuntos() == j1.getPuntos()){
							cout << "\n\n	SE ACABÓ LA PARTIDA.\n	EMPATE.\n\n";// ENTRE JUGADOR 0 (" << j0.getPuntos() << " puntos) y JUGADOR 1 (" << j1.getPuntos() << " puntos).\n\n";
							opt = 0;
						}
					}
				}while(opt != 0);
		/*		break;

			default:
				cout << "	Esa opción no existe.\n";
		}

	}while(opcion != 0);
*/


	cout << endl;
}
