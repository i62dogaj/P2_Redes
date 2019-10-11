#ifndef JUGADOR_HPP
#define JUGADOR_HPP

#include <iostream>
#include <stdio.h>
#include <vector>
#include "ficha.hpp"
using namespace std;

class Jugador{

	private:
		int ID_;
		char login_;
		char password_;
		int conectado_;
		vector <Ficha> mano_;

	public:
		//Constructor
		Jugador(){};

		inline Jugador(int id, char login, char pass){
			setID(id);
			setLogin(login);
			setPass(pass);
			setConectado(1);
		};

		//OBSERVADORES
		inline int getID(){ return ID_;};
		inline char getLogin(){ return login_;};
		inline char getPass(){ return password_;};
		inline vector<Ficha> getMano(){ return mano_;};

		inline void mostrarMano(){
			for(int i = 0; i < mano_.size(); i++){
			cout << " |" << mano_[i].getNI() << "|" << mano_[i].getND() << "|";
			}
		};

		//MODIFICADORES
		inline void setID(int id){ ID_ = id; };
		inline void setLogin(char login){ login_ = login; };
		inline void setPass(char pass){ password_ = pass; }
		inline void setConectado(int flag){ conectado_ = flag;};
		inline void setManoInicial(vector <Ficha> vec){	mano_ = vec;};
		inline void robarFicha(Ficha a){ mano_.push_back(a); };

};


#endif
