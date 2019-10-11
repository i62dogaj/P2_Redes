#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <vector>
using namespace std;

class Partida{

  private:
    vector <Ficha> fichas_;

  public:
    //Constructores
    inline Partida(){ //Generamos todas las fichas sin repetirlas
			int k = 0;
			for(int i = k; i < 7; i++){
				for(int j = k; j < 7; j++){
					Ficha a(i,j);
					if(!buscarFicha(a)) fichas_.push_back(a);
				}
				k++;
			}

    };


    //OBSERVADORES
    inline bool buscarFicha(Ficha a){
			for(int i = 0; i < fichas_.size(); i++){
				if((a.getNI() == fichas_[i].getNI()) && (a.getND() == fichas_[i].getND())){
					return true;
				}
			}
			return false;
		};
    inline void mostrarFichas(){
			for(int i = 0; i < fichas_.size(); i++){
				cout << " |" << fichas_[i].getNI() << "|" << fichas_[i].getND() << "|" << endl;
			}
		};



		//MODIFICADORES
		inline vector<Ficha> repartir(){
			vector <Ficha> vec;
			for(int i = 0; i < 7; i++){
				int pos = rand()%(fichas_.size());
				vec.push_back(fichas_[pos]);
				fichas_.erase(fichas_.begin()+pos);
			}
			return vec;
		};

		inline Ficha robar(){
			int pos = rand()%(fichas_.size());
			Ficha a(fichas_[pos].getNI(), fichas_[pos].getND());
			fichas_.erase(fichas_.begin()+pos);
			return a;
		};


};


#endif
