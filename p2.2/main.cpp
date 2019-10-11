#include "ficha.hpp"
#include "jugador.hpp"
#include <iostream>
using namespace std;

int main(){
	cout << endl;
	vector <Ficha> mano;
	srand(time(NULL));
	Ficha a;
	cout << " |"<< a.getNI() << "|" << a.getND() << "|" << endl;

	Jugador j;
	j.mostrarMano();

	cout << endl;
}
