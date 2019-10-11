#ifndef FICHA_HPP
#define FICHA_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
using namespace std;

class Ficha{

	private:
		int nI_; //Numero izquierdo
		int nD_; //Numero derecho

	public:
		//Constructores
		inline Ficha(){
			int nI = rand()%6+1;
			int nD = rand()%6+1;
			setNI(nI);
			setND(nD);
		};

		//OBSERVADORES
		inline int getNI(){ return nI_;};
		inline int getND(){ return nD_;};

		//MODIFICADORES
		inline void setNI(int num){ nI_ = num; };
		inline void setND(int num){ nD_ = num; };

		//DESTRUCTOR
		~Ficha(){};

};


#endif
