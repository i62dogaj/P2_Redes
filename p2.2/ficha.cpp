#include "ficha.hpp"
using namespace std;

//Constructores
Ficha::Ficha(){};
Ficha::Ficha(int I, int D){
	setNI(I);
	setND(D);
};

//OBSERVADORES
int Ficha::getNI() const{
  return nI_;
};

int Ficha::getND() const{
  return nD_;
};

void Ficha::mostrarFicha(){
	cout << " |" << getNI() << "|" << getND() << "|" << endl;
};

bool Ficha::esDoble(){
	if(getNI() == getND()){
		return true;
	}
	else{
		return false;
	}
};

//MODIFICADORES
void Ficha::setNI(int num){
  nI_ = num;
};

void Ficha::setND(int num){
  nD_ = num;
};

//OPERADORES DE ASIGNACIÓN
Ficha & Ficha::operator=(Ficha const &a){
	setNI(a.getNI());
	setND(a.getND());

	// Se devuelve el objeto actual
	return *this;
}
