#ifndef FICHA_HPP
#define FICHA_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string.h>
using namespace std;

class Ficha{

	private:
		int nI_; //Numero izquierdo
		int nD_; //Numero derecho

	public:
		//Constructores
		Ficha();
		Ficha(int I, int D);

		//OBSERVADORES
		int getNI() const;
		int getND() const;
		string mostrarFicha();
		bool esDoble();

		//MODIFICADORES
		void setNI(int num);
		void setND(int num);

		//OPERADORES DE ASIGNACIÓN
		Ficha & operator=(Ficha const &a);

		//DESTRUCTOR
		~Ficha(){};

};


#endif
