#include "jugador.hpp"
#include "partida.hpp"
#include <iostream>
using namespace std;

int Menu(){
	int opt;
	cout << "\n\n	[0] SALIR\n";
	cout << "	[1] Iniciar Partida\n";
	cout << "	Opción: ";
	cin >> opt;
	cout << endl << endl;
	return opt;
}

int MenuPartida(){
	int opt;
	cout << "\n\n	[0] SALIR\n";
	cout << "	[1] Ver Tablero\n";
	cout << "	[2] Ver Mano\n";
	cout << "	[3] Colocar ficha\n";
	cout << "	[4] Robar\n\n";
	cout << "	Opción: ";
	cin >> opt;
	cout << endl << endl;
	return opt;
}

int main(){
	cout << endl;

	vector <Partida> partida;
	Partida p(1); //ID a 1 en prueba
	partida.push_back(p);
	int id, pos;
	//cout << "N Jugadores: " << p.getNJugadores() << endl;
	Ficha a;
	Jugador j0(0,"irene", "1234", &p);
	id = p.nuevoJugador(&j0);
	j0.setID(id);
	cout << "Jugador 0:\n";
	j0.mostrarMano();
	cout << endl;
	Jugador j1(1,"pepe", "1234", &p);
	id = p.nuevoJugador(&j1);
	j1.setID(id);
	cout << "Jugador 1:\n";
	j1.mostrarMano();
	cout << endl;
	Jugador j2(2,"paco", "1234", &p);
	id = p.nuevoJugador(&j2);
	j2.setID(id);
	cout << "Jugador 2:\n";
	j2.mostrarMano();
	cout << endl;
	int opt;

	do{
		opt = Menu();
		switch(opt){
			case 0:
				cout << "	Has salido del juego.\n";
				break;

			case 1:
				cout << "Bienvenido a la partida.\n";
				/*pos = p.iniciarPartida();
				cout << "JUGADOR: " << pos << endl;
				if(pos == j0.getID()) cout << "Comienza el jugador 0.\n\n";
				else if(pos == j1.getID()) cout << "Comienza el jugador 1.\n\n";
				else if(pos == j2.getID()) cout << "Comienza el jugador 2.\n\n";*/
				//cout << "N Jugadores: " << p.getNJugadores() << endl;
				do{
					opt = MenuPartida();
					switch(opt){
						case 0:
							/*if(pos == j0.getID()) j0.salirPartida(&p);
							else if(pos == j1.getID()) j1.salirPartida(&p);
							else if(pos == j2.getID()) j2.salirPartida(&p);*/
							cout << "	Has salido de la partida\n";
							break;

						case 1:
							p.mostrarTablero();
							break;

						case 2:
							/*if(pos == j0.getID()) j0.mostrarMano();
							else if(pos == j1.getID()) j1.mostrarMano();
							else if(pos == j2.getID()) j2.mostrarMano();*/
							j0.mostrarMano();
							break;

						case 3:
							/*if(pos == j0.getID()) j0.colocarFicha(&p);
							else if(pos == j1.getID()) j1.colocarFicha(&p);
							else if(pos == j2.getID()) j2.colocarFicha(&p);*/
							j0.colocarFicha(&p);
							break;

						case 4:
							if(p.montonVacio()) cout << "No quedan fichas en el montón para robar.\n\n";
							else{
								if(!j0.puedePoner(&p)){
									a = p.robar();
									/*if(pos == j0.getID()) j0.robarFicha(a);
									else if(pos == j1.getID()) j1.robarFicha(a);
									else if(pos == j2.getID()) j2.robarFicha(a);*/
									j0.robarFicha(a);
									cout << "Has robado la ficha |" <<	a.getNI() << "|" << a.getND() << "| del montón.\n\n";
									cout << "Tu mano queda:\n\n";
									/*if(pos == j0.getID()) j0.mostrarMano();
									else if(pos == j1.getID()) j1.mostrarMano();
									else if(pos == j2.getID()) j2.mostrarMano();*/
									j0.mostrarMano();
								}
								else{
									cout << "No hace falta robar.\n\n";
								}
							}
							break;

						default:
							cout << "	Esa opción no existe.\n";
					}
				}while(opt != 0);
				break;

			default:
				cout << "	Esa opción no existe.\n";
		}

	}while(opt != 0);



	cout << endl;
}
