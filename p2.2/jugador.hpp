#ifndef JUGADOR_HPP
#define JUGADOR_HPP

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include "ficha.hpp"
using namespace std;

class Partida;

class Jugador{

	private:
		int ID_;
		string login_;
		string password_;
		vector <Ficha> mano_;
		vector <Ficha> dobles_;
		int idPartida_;
		int puntos_;

	public:
		//Constructor
		Jugador(Partida *p);
		Jugador(int id, Partida *p);

		//OBSERVADORES
		int getID();
		string getLogin();
		string getPass();
		int getIDPartida();
		int getPuntos();
		vector<Ficha> getMano();
		int nDobles();
		int nFichas();
		string mostrarMano();
		bool existeFicha(Ficha a);
		int buscarFicha(Ficha a);
		bool tieneDobles();
	  Ficha dobleMasAlta();
		Ficha masAlta();
		bool puedePoner(Partida *p);
		bool tieneFichas();


		//MODIFICADORES
		void setID(int id);
		void setLogin(string login);
		void setPass(string pass);
		void setIDPartida(int id);
		void setPuntos(int puntos);
		void setManoInicial(vector <Ficha> vec);
		void robarFicha(Ficha a);
		bool colocarFicha(int NI, int ND, int extremo, Partida *p);
		void salirPartida(Partida *p);
		void girarFicha(Ficha *a);

};


#endif
