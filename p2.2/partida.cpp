#include "partida.hpp"
#include "jugador.hpp"
using namespace std;

Partida::Partida(int id){ //Asignamos un ID y generamos todas las fichas sin repetirlas
  setIDPartida(id);
  setNJugadores(0);
	int k = 0;
	for(int i = k; i < 7; i++){
		for(int j = k; j < 7; j++){
			Ficha a(i,j);
			/* Si no queremos que haya dobles para probar la segunda
			   forma de empezar añadir (!a.esDoble()). Normalmente
				 siempre salen dobles. */
			if(!buscarFicha(a)) monton_.push_back(a);
		}
		k++;
	}
};

int Partida::getIDPartida(){
	return idPartida_;
};

int Partida::getIDJugador(string login){
	for(int i = 0; i < jugadores_.size(); i++){
		if(jugadores_[i].getLogin() == login) return i;
	}
	//return 1000;
};

int Partida::getNJugadores(){
	return jugadores_.size();
};

int Partida::getExtI(){
	return (tablero_.front()).getNI();
};

int Partida::getExtD(){
	return (tablero_.back()).getND();
};

Ficha Partida::getMasAlta(){
  return masAlta_;
};


/* En el vector fDobles_ se introducirá la mayor ficha doble
   que tenga cada jugador. Así, buscamos la mayor de todas,
   los jugadores comprobarán si la tienen y comenzará el que
   la tenga. */
Ficha Partida::iniciarPartida(){
  Ficha fichaAux;
  int pos, sumaMayor, aux;
	bool hayDobles = false;
	/* Primero comprueba si los jugadores tienen fichas dobles */
  for(int i = 0; i < jugadores_.size(); i++){
    if(jugadores_[i].tieneDobles()){
			hayDobles = true;
			fDobles_.push_back(jugadores_[i].dobleMasAlta());
		}
  }
	if(hayDobles){
		/*cout << "\n\n Fichas dobles:\n";
		for(int i = 0; i < fDobles_.size(); i++){
			fflush(stdout);
			fDobles_[i].mostrarFicha();
		}*/
	  //mayor = fDobles_[0]
	  masAlta_.setNI(fDobles_[0].getNI());
	  masAlta_.setND(fDobles_[0].getND());
	  pos = 0;
		if(fDobles_.size() > 1){
		  for(int i = 0; i < fDobles_.size(); i++){
		    //Basta con comprobar uno de los valores
		    if(masAlta_.getNI() < fDobles_[i].getNI()){
		      //masAlta_ = fDobles_[i]
		      masAlta_.setNI(fDobles_[i].getNI());
		      masAlta_.setND(fDobles_[i].getND());
		      pos = i;
		    }
		  }
		}
	}
	/* Si ningún jugador tiene fichas dobles, se pasaría a comprobar
	   la siguiente ficha más alta y comenzará quien la tenga.*/
	else if(!hayDobles){
		for(int i = 0; i < jugadores_.size(); i++){
				fMayores_.push_back(jugadores_[i].masAlta());
	  }
		/*cout << "\n\n Fichas mayores:\n";
		for(int i = 0; i < fMayores_.size(); i++){
			fflush(stdout);
			fMayores_[i].mostrarFicha();
		}*/
	  //masAlta_ = fMayores_[0]
	  masAlta_.setNI(fMayores_[0].getNI());
	  masAlta_.setND(fMayores_[0].getND());
		sumaMayor = fMayores_[0].getNI() + fMayores_[0].getND();
	  pos = 0;
	  for(int i = 0; i < fMayores_.size(); i++){
	    aux = fMayores_[i].getNI() + fMayores_[i].getND();
	    if(aux > sumaMayor){
	      //masAlta_ = fMayores_[i]
	      masAlta_.setNI(fMayores_[i].getNI());
	      masAlta_.setND(fMayores_[i].getND());
				sumaMayor = aux;
	      pos = i;
	    }
	  }
	}
	/* Devolvemos la ficha mayor, doble o no, y los jugadores_
     comprobarán si la tienen, comenzando aquel que la tenga.*/
	return masAlta_;
};

bool Partida::tableroVacio(){
  if(tablero_.size() == 0) return true;
  else return false;
};

bool Partida::montonVacio(){
  if(monton_.size() == 0) return true;
  else return false;
};

bool Partida::buscarFicha(Ficha a){
	for(int i = 0; i < monton_.size(); i++){
		if((a.getNI() == monton_[i].getNI()) && (a.getND() == monton_[i].getND())){
			return true;
		}
	}
	return false;
};

void Partida::mostrarMonton(){
	for(int i = 0; i < monton_.size(); i++){
		cout << " |" << monton_[i].getNI() << "|" << monton_[i].getND() << "|" << endl;
	}
  cout << endl;
};

void Partida::mostrarTablero(){
  if(tableroVacio()) cout << "Aún no hay fichas en el tablero.\n";
  else{
    cout << "\n TABLERO: \n\n ";
		for(int i = 0; i < tablero_.size(); i++){
			cout << "|" << tablero_[i].getNI() << "|" << tablero_[i].getND() << "|";
		}
    cout << endl;
  }
};

void Partida::setIDPartida(int id){
	idPartida_ = id;
};

void Partida::setNJugadores(int n){
	nJugadores_ = n;
};

void Partida::masNJugadores(){
	setNJugadores(getNJugadores()+1);
};

void Partida::menosNJugadores(){
	setNJugadores(getNJugadores()-1);
};

int Partida::nuevoJugador(Jugador *j){
  jugadores_.push_back(*j);
  masNJugadores();
	return jugadores_.size();
};

vector<Ficha> Partida::repartir(){
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

Ficha Partida::robar(){
  srand(time(NULL));
	int pos = rand()%(monton_.size());
	Ficha a(monton_[pos].getNI(), monton_[pos].getND());
	monton_.erase(monton_.begin()+pos);
	return a;
};

void Partida::anadirFichaTablero(Ficha a, int lugar){
  if(tableroVacio()) tablero_.push_back(a);
  else{
    if(lugar == 1) tablero_.insert(tablero_.begin(), a);
    else if(lugar == 2) tablero_.push_back(a);
  }
};

//Cuando un jugador sale de la partida se devuelven las fichas que le quedaran
//void Partida::anadirFichaMonton(Ficha a){
//	monton_.push_back(a);
//};
