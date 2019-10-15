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
		vector <Ficha> dobles_;
		int idPartida_;

	public:
		//Constructor
		Jugador(Partida *p){
			setManoInicial(p->repartir());
			setConectado(1);
			setIDPartida(p->getIDPartida());
			setID(p->getNJugadores());
		};

		inline Jugador(int id, char login, char pass, Partida *p){
			setID(id);
			setLogin(login);
			setPass(pass);
			setConectado(1);
			setManoInicial(p->repartir());
			setIDPartida(p->getIDPartida());
			setID(p->getNJugadores());
		};




		//OBSERVADORES
		inline int getID(){ return ID_;};
		inline char getLogin(){ return login_;};
		inline char getPass(){ return password_;};
		inline int getConectado(){return conectado_;};
		inline int getIDPartida(){return idPartida_;};
		inline vector<Ficha> getMano(){ return mano_;};
		inline int nDobles(){ return dobles_.size();};

		inline void mostrarMano(){
			for(int i = 0; i < mano_.size(); i++){
				cout << " |" << mano_[i].getNI() << "|" << mano_[i].getND() << "|";
			}
			cout << endl;
		};

		inline bool existeFicha(Ficha a){
			for(int i = 0; i < mano_.size(); i++){
				if((a.getNI() == mano_[i].getNI()) && (a.getND() == mano_[i].getND())){
					return true;
				}
			}
			return false;
		};

		inline int buscarFicha(Ficha a){
			for(int i = 0; i < mano_.size(); i++){
				if((a.getNI() == mano_[i].getNI()) && (a.getND() == mano_[i].getND())){
					return i;
				}
			}
		};

		inline bool tieneDobles(){
			for(int i = 0; i < mano_.size(); i++){
				if(mano_[i].esDoble()){
					dobles_.push_back(mano_[i]);
					return true;
				}
			}
			return false;
		};

	/*	inline Ficha dobleMasAlta(){
			Ficha aux;
			if(nDobles() > 1){
				//En caso de que haya más de una doble, las ordenamos de menor a mayor
				for(int i = 0; i < dobles_.size(); i++){
					for(int j = 0; j < dobles_.size(); j++){
						if(dobles_[j] < dobles_[j+1]){ //CAMBIAERR
							//aux = dobles_[j]
							aux.setNI(dobles_[j].getNI());
							aux.setND(dobles_[j].getND());
							//dobles_[j] = dobles_[j+1]
							dobles_[j].setNI(dobles_[j+1].getNI());
							dobles_[j].setND(dobles_[j+1].getND());
							//dobles_[j+1] = aux
							dobles_[j+1].setNI(aux.getNI());
							dobles_[j+1].setND(aux.getND());
						}
					}
				}
			}
			//Devolvemos la mayor (la última si había más de una)
			return dobles_.back();
		};*/




		//MODIFICADORES
		inline void setID(int id){ ID_ = id; };
		inline void setLogin(char login){ login_ = login; };
		inline void setPass(char pass){ password_ = pass; }
		inline void setConectado(int flag){ conectado_ = flag;};
		inline void setIDPartida(int id){ idPartida_ = id;};
		inline void setManoInicial(vector <Ficha> vec){	mano_ = vec;};
		inline void robarFicha(Ficha a){ mano_.push_back(a); };
		

		inline void colocarFicha(Partida *p){
			int n;
			bool colocada = false;
			Ficha a;
			do{ // ------ Escogemos la ficha ------
				cout << "NI: ";
				cin >> n;
				a.setNI(n);
				cout << "ND: ";
				cin >> n;
				a.setND(n);
				if(!existeFicha(a)) cout << "\nEsa ficha no existe en tu montón. \nIntroduce una que sí tengas.\n\n";
			}while(!existeFicha(a));
			// ------ Escogemos el extremo y la colocamos ------
			if(p->tableroVacio()){
				p->anadirFichaTablero(a, 1); //No importa el lugar, se inserta sin más
				colocada = true;
			}
			else{
				cout << "\n[1] Izquierda    [2] Derecha: ";
				cin >> n;
				if(n == 1){
					if(p->getExtI() == a.getND()){
						p->anadirFichaTablero(a, n);
						colocada = true;
					}
					else cout << "\nEsta ficha no se puede colocar en este extremo. \nEscoge otra ficha, otro extremo o roba.\n\n";
				}
				else if(n == 2){
					if(p->getExtD() == a.getNI()){
						p->anadirFichaTablero(a, n);
						colocada = true;
					}
					else cout << "\nEsta ficha no se puede colocar en este extremo. \nEscoge otra ficha, otro extremo o roba.\n\n";
				}
			}
			// ------ Eliminamos la ficha de la mano ------
			if(colocada) mano_.erase(mano_.begin()+(buscarFicha(a)));
			p->mostrarTablero();
		};


		inline void salirPartida(Partida *p){
			p->menosNJugadores();
			setIDPartida(-1);
			if(!mano_.empty()){
				for(int i = 0; i < mano_.size(); i++){
					p->anadirFichaMonton(mano_[i]);
				}
				mano_.clear();
		 	}
		};

};


#endif
