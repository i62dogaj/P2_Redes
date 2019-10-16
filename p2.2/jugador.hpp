#ifndef JUGADOR_HPP
#define JUGADOR_HPP

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "ficha.hpp"
using namespace std;

class Partida;

class Jugador{

	private:
		int ID_;
		string login_;
		string password_;
		int conectado_;
		vector <Ficha> mano_;
		vector <Ficha> dobles_;
		int idPartida_;

	public:
		//Constructor
		Jugador(Partida *p);
		Jugador(int id, string login, string pass, Partida *p);

		//OBSERVADORES
		int getID();
		string getLogin();
		string getPass();
		int getConectado();
		int getIDPartida();
		vector<Ficha> getMano();
		int nDobles();
		void mostrarMano();
		bool existeFicha(Ficha a);
		int buscarFicha(Ficha a);
		bool tieneDobles();
	  Ficha dobleMasAlta();


		//MODIFICADORES
		void setID(int id);
		void setLogin(string login);
		void setPass(string pass);
		void setConectado(int flag);
		void setIDPartida(int id);
		void setManoInicial(vector <Ficha> vec);
		void robarFicha(Ficha a);
		void colocarFicha(Partida *p);
		void salirPartida(Partida *p);

};


#endif
