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

	p.monstrarFichasMonton();
	cout << endl << endl;
	j1.mostrarMano();
	j2.mostrarMano();
	cout << endl << endl;
	p.monstrarFichasMonton();
	cout << endl << endl;
	j1.robarFicha(p.robar());
	j1.mostrarMano();
	cout << endl << endl;
	p.monstrarFichasMonton();

	cout << endl;
}
