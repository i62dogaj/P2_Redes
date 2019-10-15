#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include "ficha.hpp"
#include "jugador.hpp"
using namespace std;

class Partida{

  private:
    int idPartida_;
    int nJugadores_;
    vector <Ficha> monton_;
    vector <Ficha> tablero_;
    //vector <Jugador> jugadores_; //Para indicar el orden de juego
    vector <Ficha> fDobles_; //Para las fichas dobles al iniciar la partida

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


    /* En el vector fDobles_ se introducirá la mayor ficha doble
       que tenga cada jugador en el mismo índice que se encuentra en el
      vector jugadores_. Así, buscamos la mayor de todas y le decimos al
      jugador correspondiente que tiene la mayor ficha para empezar la
      partida. */
    /*inline int iniciarPartida(){
      Ficha mayor;
      int pos;
      for(int i = 0; i < jugadores_.size(); i++){
        if(jugadores_[i].tieneDobles()) fDobles_.push_back(jugadores_[i].dobleMasAlta());
      }
      //mayor = fDobles_[0]
      mayor.setNI(fDobles_[0].getNI());
      mayor.setND(fDobles_[0].getND());
      pos = 0;
      for(int i = 0; i < fDobles_.size(); i++){
        //Basta con comprobar uno de los valores
        if(mayor.getNI() < fDobles_[i].getNI()){
          //mayor = fDobles_[i]
          mayor.setNI(fDobles_[i].getNI());
          mayor.setND(fDobles_[i].getND());
          pos = i;
        }
      }*/
      /* Devolvemos la posición de la ficha doble mayor,
         que coincide con la posición del jugador en el
         vector jugadores_, así como con su ID. Así, cada
         jugador comprobará si le toca a él empezar a jugar.*/
      //return pos;
    //};

    inline bool tableroVacio(){
      if(tablero_.size() == 0) return true;
      else return false;
    };

    inline bool montonVacio(){
      if(monton_.size() == 0) return true;
      else return false;
    };

    inline bool buscarFicha(Ficha a){
			for(int i = 0; i < monton_.size(); i++){
				if((a.getNI() == monton_[i].getNI()) && (a.getND() == monton_[i].getND())){
					return true;
				}
			}
			return false;
		};

    inline void mostrarMonton(){
			for(int i = 0; i < monton_.size(); i++){
				cout << " |" << monton_[i].getNI() << "|" << monton_[i].getND() << "|" << endl;
			}
      cout << endl;
		};


    inline void mostrarTablero(){
      if(tableroVacio()) cout << "Aún no hay fichas en el tablero.\n";
      else{
        cout << " TABLERO: \n\n ";
  			for(int i = 0; i < tablero_.size(); i++){
  				cout << "|" << tablero_[i].getNI() << "|" << tablero_[i].getND() << "|";
  			}
        cout << endl;
      }
		};




		//MODIFICADORES
    inline void setIDPartida(int id){idPartida_ = id;};
    inline void setNJugadores(int n){nJugadores_ = n;};
    inline void masNJugadores(){setNJugadores(getNJugadores()+1);};
    inline void menosNJugadores(){setNJugadores(getNJugadores()-1);};

    /*inline void nuevoJugador(Jugador *j){
      jugadores_.push_back(*j);
      masNJugadores();
    };*/

		inline vector<Ficha> repartir(){
      srand(time(NULL));
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
      srand(time(NULL));
			int pos = rand()%(monton_.size());
			Ficha a(monton_[pos].getNI(), monton_[pos].getND());
			monton_.erase(monton_.begin()+pos);
			return a;
		};

    inline void anadirFichaTablero(Ficha a, int lugar){
      if(tableroVacio()) tablero_.push_back(a);
      else{
        if(lugar == 1) tablero_.insert(tablero_.begin(), a);
        else if(lugar == 2) tablero_.push_back(a);
      }
    };

    //Cuando un jugador sale de la partida se devuelven las fichas que le quedaran
    inline void anadirFichaMonton(Ficha a){ monton_.push_back(a); };


};


#endif
