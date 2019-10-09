#ifndef FICHA_HPP
#define FICHA_HPP

#include <iostream>
#include <stdio.h>
using namespace std;

class Ficha{

	private:
		int nI_; //Numero izquierdo
		int nD_; //Numero derecho		
	
	public:
		//OBSERVADORES
		inline int getNI(){ return nI_;};
		inline int getND(){ return nD_;};
		
		//MODIFICADORES
		inline void setNI(int num){ nI_ = num; };
		inline void setND(int num){ nD_ = num; };

};


#endif
