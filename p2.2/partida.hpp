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
		vector <Ficha> fMayores_; //Para las fichas mayores al iniciar partida si no hubiera dobles
    Ficha masAlta_;

    int socket1_;
    int socket2_;
    int turno_;

  public:
    //Constructores
    Partida();

    //OBSERVADORES
    int getIDPartida();
	  int getIDJugador(string login);
    int getNJugadores();
    int getExtI();
    int getExtD();
    Ficha getMasAlta();
    Ficha iniciarPartida();
    bool tableroVacio();
    bool montonVacio();
    bool buscarFichaMonton(Ficha a);
    void mostrarMonton();
    void mostrarTablero();

    int getSocket1();
    int getSocket2();
    int getTurno();

    Jugador & getJugador(int socket);

		//MODIFICADORES
    void setIDPartida(int id);
    void setNJugadores(int n);
    void masNJugadores();
    void menosNJugadores();
    void nuevoJugador(Jugador *j);
		vector<Ficha> repartir();
		Ficha robar();
    void anadirFichaTablero(Ficha a, int lugar);

    void setSocket1(int socket);
    void setSocket2(int socket);
    void setTurno(int turno);

    //Cuando un jugador sale de la partida se devuelven las fichas que le quedaran
    //void anadirFichaMonton(Ficha a);


};


#endif
