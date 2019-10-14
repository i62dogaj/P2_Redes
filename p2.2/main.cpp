#include "ficha.hpp"
#include "jugador.hpp"
#include "partida.hpp"
#include <iostream>
using namespace std;

int main(){
	cout << endl;
	vector <Partida> partida;
	Partida p(partida.size());
	partida.push_back(p);
	Jugador j1(&p);
	Jugador j2(&p);
	srand(time(NULL));

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
	p.mostrarTablero();

	cout << endl;
}
