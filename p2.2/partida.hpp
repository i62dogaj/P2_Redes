#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include "ficha.hpp"
using namespace std;

class Jugador;

class Partida{

  private:
    int idPartida_;
    int nJugadores_;
    vector <Ficha> monton_;
    vector <Ficha> tablero_;
    vector <Jugador> jugadores_; //Para indicar el orden de juego
    vector <Ficha> fDobles_; //Para las fichas dobles al iniciar la partida
    Ficha masAlta_;

    int socket1_;
    int socket2_;
    int turno_;

  public:
    //Constructores
    Partida();

    //OBSERVADORES
    int getIDPartida();
    int getNJugadores();
    int getExtI();
    int getExtD();
    vector<Ficha> getMonton();
    vector<Ficha> getTablero();
    vector<Ficha> getFichasDobles();
    Ficha getMasAlta();
    Ficha iniciarPartida();
    bool tableroVacio();
    bool montonVacio();
    bool buscarFichaMonton(Ficha a);
    //string mostrarMonton();
    string mostrarTablero();

    int getSocket1();
    int getSocket2();
    int getTurno();
    Jugador &getJugador(int socket);

    int compararFichas(Ficha f1, Ficha f2);


		//MODIFICADORES
    void setIDPartida(int id);
    void setNJugadores(int n);
    void setMonton(vector <Ficha> vec);
    void setTablero(vector <Ficha> vec);
    void setFichasDobles(vector <Ficha> vec);
    void setMasAlta(Ficha a);
    void masNJugadores();
    void menosNJugadores();
    void nuevoJugador(Jugador *j);
		vector<Ficha> repartir();
		Ficha robar();
    void anadirFichaTablero(Ficha a, int lugar);

    void setSocket1(int socket);
    void setSocket2(int socket);
    void setTurno(int turno);

		void vaciarJugadores();
  /*  void limpiarMonton(vector<int> &contadorFichas){
      for (size_t i = 0; i < monton_.size(); i++) {
        if(monton_[i].getNI() == monton_[i].getND()){
          contadorFichas[monton_[i].getNI()]++;
        }
        else{
          contadorFichas[monton_[i].getNI()]++;
          contadorFichas[monton_[i].getND()]++;
        }
      }
      monton_.clear();
    };*/


    //OPERADORES DE ASIGNACIÓN
		Partida & operator=(Partida &p);


};


#endif
