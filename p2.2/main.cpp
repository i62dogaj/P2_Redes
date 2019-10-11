#include "ficha.hpp"
#include "jugador.hpp"
#include "partida.hpp"
#include <iostream>
using namespace std;

int main(){
	cout << endl;
	vector <Ficha> mano;
	Ficha a;
	Jugador j;
	Partida p;
	srand(time(NULL));

	//cout << " |"<< a.getNI() << "|" << a.getND() << "|" << endl;

	//j.mostrarMano();

	p.mostrarFichas();
	j.setManoInicial(p.repartir());
	cout << endl << endl;
	j.mostrarMano();
	cout << endl << endl;
	p.mostrarFichas();
	cout << endl << endl;
	j.robarFicha(p.robar());
	j.mostrarMano();
	cout << endl << endl;
	p.mostrarFichas();

	cout << endl;
}
