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
		inline Ficha(){};
		inline Ficha(int I, int D){
			setNI(I);
			setND(D);
		};

		//OBSERVADORES
		inline int getNI(){ return nI_;};
		inline int getND(){ return nD_;};
		inline void mostrarFicha(){
			cout << " |" << getNI() << "|" << getND() << "|" << endl;
		};
		inline bool esDoble(){
			if(getNI() == getND()){
				return true;
			}
			else{
				return false;
			}
		};

		//MODIFICADORES
		inline void setNI(int num){ nI_ = num; };
		inline void setND(int num){ nD_ = num; };

		//DESTRUCTOR
		~Ficha(){};

};


#endif
