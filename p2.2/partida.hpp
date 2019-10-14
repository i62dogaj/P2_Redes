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
    int idPartida_;
    int nJugadores_;
    vector <Ficha> monton_;
    vector <Ficha> tablero_;

  public:
    //Constructores
    inline Partida(int id){ //Asignamos un ID y generamos todas las fichas sin repetirlas
      setIDPartida(id);
      setNJugadores(0);
			int k = 0;
			for(int i = k; i < 7; i++){
				for(int j = k; j < 7; j++){
					Ficha a(i,j);
					if(!buscarFicha(a)) monton_.push_back(a);
				}
				k++;
			}

    };


    //OBSERVADORES
    inline int getIDPartida(){return idPartida_;};
    inline int getNJugadores(){return nJugadores_;};

    inline bool buscarFicha(Ficha a){
			for(int i = 0; i < monton_.size(); i++){
				if((a.getNI() == monton_[i].getNI()) && (a.getND() == monton_[i].getND())){
					return true;
				}
			}
			return false;
		};
    inline void monstrarFichasMonton(){
			for(int i = 0; i < monton_.size(); i++){
				cout << " |" << monton_[i].getNI() << "|" << monton_[i].getND() << "|" << endl;
			}
		};



		//MODIFICADORES
    inline void setIDPartida(int id){idPartida_ = id;};
    inline void setNJugadores(int n){nJugadores_ = n;};
    inline void masNJugadores(){setNJugadores(getNJugadores()+1);};
    inline void menosNJugadores(){setNJugadores(getNJugadores()-1);};

		inline vector<Ficha> repartir(){
      masNJugadores();
			vector <Ficha> vec;
			for(int i = 0; i < 7; i++){
				int pos = rand()%(monton_.size());
				vec.push_back(monton_[pos]);
        monton_.erase(monton_.begin()+pos);
			}
			return vec;
		};

		inline Ficha robar(){
			int pos = rand()%(monton_.size());
			Ficha a(monton_[pos].getNI(), monton_[pos].getND());
			monton_.erase(monton_.begin()+pos);
			return a;
		};

  /*  inline void anadirFichaTablero(int lugar){

    };
*/

};


#endif
