#include "ficha.hpp"
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
	Partida p(partida.size()); //ID a 1 en prueba
	partida.push_back(p);
	Jugador j(&p);
	//p.nuevoJugador(&p);
	Ficha a;
	int opt;

	do{
		opt = Menu();
		switch(opt){
			case 0:
				cout << "	Has salido del juego.\n";
				break;

			case 1:
				cout << "Bienvenido a la partida.\n";
				do{
					opt = MenuPartida();
					switch(opt){
						case 0:
							cout << "	Has salido de la partida\n";
							break;

						case 1:
							p.mostrarTablero();
							break;

						case 2:
							j.mostrarMano();
							break;

						case 3:
							j.colocarFicha(&p);
							break;

						case 4:
							if(p.montonVacio()) cout << "No quedan fichas en el montón para robar.\n\n";
							else{
								a = p.robar();
								j.robarFicha(a);
								cout << "Has robado la ficha |" <<	a.getNI() << "|" << a.getND() << "| del montón.\n\n";
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

/*
	//p.mostrarMonton();
	p.mostrarTablero();
	cout << endl << endl;
	cout << "Jugador 1:\n";
	j1.mostrarMano();
	cout << "\nJugador 2: \n";
	j2.mostrarMano();
	cout << endl << endl;
	//p.mostrarMonton();
	cout << endl << endl << "Jugador 1:\n";
	j1.colocarFicha(&p);
	cout << "\nJugador 1:\n";
	j1.mostrarMano();
	j2.robarFicha(p.robar());
	cout << "\nJugador 2:\n";
	j2.mostrarMano();
	cout << endl << endl;
	cout << "MONTÓN:\n";
	p.mostrarMonton();
	cout << "\nTABLERO:\n";
	p.mostrarTablero();

	cout << endl << endl << "Jugador 2:\n";
	j2.mostrarMano();
	cout << "\nJugador 2:\n";
	j2.colocarFicha(&p);
	cout << "\nJugador 2:\n";
	j2.mostrarMano();
	cout << "\nTABLERO:\n";
	p.mostrarTablero();*/

	cout << endl;
}
