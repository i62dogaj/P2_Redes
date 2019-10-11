#ifndef FUNCIONESCLIENTE_HPP
#define FUNCIONESCLIENTE_HPP

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

struct user{
	string login;
	string password;
	//int conectado;
};

/* Esta función se encarga de leer el fichero usuarios.txt,
   que contiene los login y contraseñas de los usuarios ya
   registrados, e introducir la información en un vector de
   estructuras con los campos login y password. */
vector<struct user> cargarUsuarios(){

	ifstream file("usuarios.txt");
	int nUsers = 0;
	struct user usuario;
	vector<struct user> vectorU;
	string linea;
	char *token;
	char *cad;

	while(getline(file, linea)){
		cout << linea << endl;
		cad = new char [linea.length()+1];
		strcpy(cad, linea.c_str());
		if((token = strtok(cad, " ")) == NULL){
			perror("Error en strtok");
			exit(EXIT_FAILURE);
		}
		usuario.login = token;
		if((token = strtok(NULL, " ")) == NULL){
			perror("Error en strtok");
			exit(EXIT_FAILURE);
		}
		usuario.password = token;
		//usuario.conectado = 0;
		vectorU.push_back(usuario);

		//nUsers++;
	}
	//cout << "nUsers = " << nUsers << endl << endl << endl;
	/*cout << endl << endl;
	for(unsigned i = 0; i < vectorU.size(); i++){
		cout << vectorU[i].login << endl;
		cout << vectorU[i].password << endl << endl;
	}*/
	//cout << "nUsers = " << int(vectorU.size()) << endl << endl;

	return vectorU;

}


bool existe_user(string login, vector<struct user> &v){
	for(unsigned i = 0; i < v.size(); i++){
		if(login == v[i].login) return true;
	}
	return false;
}


bool check_password(string login, string password, vector<struct user> &v){
	for(unsigned i = 0; i < v.size(); i++){
		if(login == v[i].login){
			if(password == v[i].password){
				/*if(v[i].conectado == 1){
					cout << "\nEse usuario ya se ha logueado." << endl;
					return false;
				}
				else{
					v[i].conectado = 1;*/
					return true;
				//}
			}
			else{
				cout << "\nLa contraseña no es correcta." << endl;
				return false;
			}
		}
	}
}


/*bool check_conexion(struct user usuario){
	for(unsigned i = 0; i < v.size(); i++){
		if(login == v[i].login){
			if(password == v[i].password) return v[i].conectado;
		}
	}
}*/


bool log_in(vector<struct user> &v){
	string login, pass;
	cout << "\nIntroduzca usuario: ";
	cin >> login;
	if(!existe_user(login, v)){
		cout << "\nNo existe el usuario " << login << endl;
		return false;
	}
	else{
		cout << "Introduzca password: ";
		cin >> pass;
		if(check_password(login, pass, v)) return true;
		else return false;
	}
}

int menu(){
  int opcion = 0;
	while((opcion != 1) && (opcion != 2)){
	  cout << "[1] Iniciar sesión" << endl;
	  cout << "[2] Registrarse" << endl;
	  cout << "Escoge una opción: ";
		cin >> opcion;
	}
	return opcion;
}


#endif
