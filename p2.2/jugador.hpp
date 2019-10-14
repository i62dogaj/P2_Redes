#ifndef JUGADOR_HPP
#define JUGADOR_HPP

#include <iostream>
#include <stdio.h>
#include <vector>
#include "ficha.hpp"
#include "partida.hpp"
using namespace std;

class Jugador{

	private:
		int ID_;
		char login_;
		char password_;
		int conectado_;
		vector <Ficha> mano_;
		int idPartida_;

	public:
		//Constructor
		Jugador(Partida *p){
			setManoInicial(p->repartir());
			setConectado(1);
		};

		inline Jugador(int id, char login, char pass, Partida *p){
			setID(id);
			setLogin(login);
			setPass(pass);
			setConectado(1);
			setManoInicial(p->repartir());
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
			cout << endl;
		};

		//MODIFICADORES
		inline void setID(int id){ ID_ = id; };
		inline void setLogin(char login){ login_ = login; };
		inline void setPass(char pass){ password_ = pass; }
		inline void setConectado(int flag){ conectado_ = flag;};
		inline void setManoInicial(vector <Ficha> vec){	mano_ = vec;};
		inline void robarFicha(Ficha a){ mano_.push_back(a); };

	/*	inline void colocarFicha(Partida p){
			int n;
			Ficha a;
			cout << "NI: ";
			cin >> n;
			a.setNI(n);
			cout << "ND: ";
			cin >> n;
			a.setND(n);
		};
*/
};


#endif
