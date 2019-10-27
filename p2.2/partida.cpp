#include "partida.hpp"
#include "jugador.hpp"
using namespace std;

Partida::Partida(){ //Generamos todas las fichas sin repetirlas
  setNJugadores(0);
  socket1_ = -1;
  socket2_ = -1;
	int k = 0;
	for(int i = k; i < 7; i++){
		for(int j = k; j < 7; j++){
			Ficha a(i,j);
			/* Si no queremos que haya dobles para probar la segunda
			   forma de empezar, añadir (!a.esDoble()). Normalmente
				 siempre salen dobles. */
			if(!buscarFichaMonton(a)) monton_.push_back(a);
		}
		k++;
	}
};

int Partida::getIDPartida(){
	return idPartida_;
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

//MANIPULACION SERVIDOR (INICIAR PARTIDA)
int Partida::getSocket1(){
   return socket1_;
};

int Partida::getSocket2(){
   return socket2_;
};

int Partida::getTurno(){
   return turno_;
};

vector<Ficha> Partida::getMonton(){
	return monton_;
};

vector<Ficha> Partida::getTablero(){
	return tablero_;
};

vector<Ficha> Partida::getFichasDobles(){
	return fDobles_;
};

Jugador & Partida::getJugador(int socket){
  if(socket == getSocket1()){
    return jugadores_[0];
  }
  else if(socket == getSocket2()){
    return jugadores_[1];
  }

};


Ficha Partida::getMasAlta(){
  return masAlta_;
};


/* En el vector fDobles_ se introducirá la mayor ficha doble
   que tenga cada jugador. Así, buscamos la mayor de todas,
   los jugadores comprobarán si la tienen y comenzará el que
   la tenga. */
Ficha Partida::iniciarPartida(){
  int k = 6;
	bool hayDobles = false;
  std::vector <Ficha> vec;
  Ficha a;
	/* Primero comprueba si los jugadores tienen fichas dobles_
    y las mete en el vector fDobles_. */
  for(unsigned int i = 0; i < jugadores_.size(); i++){
    if(jugadores_[i].tieneDobles()){
			hayDobles = true;
			fDobles_.push_back(jugadores_[i].dobleMasAlta());
		}
  }
	if(hayDobles){
    setMasAlta(fDobles_[0]);
		if(fDobles_.size() > 1){
		  for(unsigned int i = 0; i < fDobles_.size(); i++){
		    //Basta con comprobar uno de los valores
		    if(getMasAlta().getNI() < fDobles_[i].getNI()){
          setMasAlta(fDobles_[i]);
		    }
		  }
		}
    return getMasAlta();
	}
	/* Si ningún jugador tiene fichas dobles, se pasaría a comprobar
	   la siguiente ficha más alta y comenzará quien la tenga.*/
	else if(!hayDobles){
    /* Creamos un vector auxiliar con todas las fichas excepto las dobles.
      Se crean de mayor a menor (|6|5|, |6|4|, ...). */
    for(int i = k; i > -1; i--){
  		for(int j = k; j > -1; j--){
  			Ficha a(i,j);
  			if(!a.esDoble()) vec.push_back(a);
  		}
  		k--;
  	}
    /* Comprobamos una a una si algún jugador tiene la ficha, y si es así,
        deberá salir con esa. */
    for(int i = 0; i < vec.size(); i++){
      if((jugadores_[0].existeFicha(vec[i]) == true) || (jugadores_[1].existeFicha(vec[i]) == true)){
        if(vec[i].getNI() > vec[i].getND()){
          a.setNI(vec[i].getND());
          a.setND(vec[i].getNI());
          vec[i] = a;
        }
        setMasAlta(vec[i]);
        return getMasAlta();
      }
    }

	}

};

bool Partida::tableroVacio(){
  if(tablero_.size() == 0) return true;
  else return false;
};

bool Partida::montonVacio(){
  if(monton_.size() == 0) return true;
  else return false;
};

bool Partida::buscarFichaMonton(Ficha a){
	for(unsigned int i = 0; i < monton_.size(); i++){
		if((a.getNI() == monton_[i].getNI()) && (a.getND() == monton_[i].getND())){
			return true;
		}
	}
	return false;
};

/*string Partida::mostrarMonton(){
  char cad[250], I[10], D[10];
  if(montonVacio()){
    //cout << "Aún no hay fichas en el tablero.\n";
    strcpy (cad,"\nMONTÓN ");
  }
  else{
    strcpy (cad,"\nMONTÓN |");
    for(int i = 0; i < monton_.size(); i++){
  		sprintf(I, "%d", monton_[i].getNI());
  		sprintf(D, "%d", monton_[i].getND());
  		strcat (cad,I);
  		strcat (cad,"|");
  		strcat (cad,D);
  		if(i == monton_.size()-1) strcat (cad,"|");
  		else strcat (cad,"||");
  		//cout << " |" << monton_[i].getNI() << "|" << monton_[i].getND() << "|";
  	}
  }
  string cadena(cad);
  return cadena;
};*/

string Partida::mostrarTablero(){
  char cad[250], I[10], D[10];
  if(tableroVacio()){
    //cout << "Aún no hay fichas en el tablero.\n";
    strcpy (cad,"\nTABLERO ");
  }
  else{
    strcpy (cad,"\nTABLERO |");
    for(unsigned int i = 0; i < tablero_.size(); i++){
  		sprintf(I, "%d", tablero_[i].getNI());
  		sprintf(D, "%d", tablero_[i].getND());
  		strcat (cad,I);
  		strcat (cad,"|");
  		strcat (cad,D);
  		if(i == tablero_.size()-1) strcat (cad,"|");
  		else strcat (cad,"||");
  		//cout << " |" << tablero_[i].getNI() << "|" << tablero_[i].getND() << "|";
  	}
  }
  string cadena(cad);
  return cadena;
};

int Partida::compararFichas(Ficha f1, Ficha f2){
  int mayor1, mayor2;
  if(f1.getNI() < f1.getND()){
    mayor1 = f1.getND();
  }
  else{
    mayor1 = f1.getNI();
  }

  if(f2.getNI() < f2.getND()){
    mayor2 = f2.getND();
  }
  else{
    mayor2 = f2.getNI();
  }

  if(mayor1 < mayor2){
    return 1;
  }
  else{
    return 0;
  }
};

void Partida::setIDPartida(int id){
	idPartida_ = id;
};

void Partida::setNJugadores(int n){
	nJugadores_ = n;
};

void Partida::setMonton(vector <Ficha> vec){
	monton_ = vec;
};

void Partida::setTablero(vector <Ficha> vec){
	tablero_ = vec;
};

void Partida::setFichasDobles(vector <Ficha> vec){
	fDobles_ = vec;
};

void Partida::setMasAlta(Ficha a){
  masAlta_ = a;
};

void Partida::masNJugadores(){
	setNJugadores(getNJugadores()+1);
};

void Partida::menosNJugadores(){
	setNJugadores(getNJugadores()-1);
};

void Partida::nuevoJugador(Jugador *j){
	bool introducido = false;
  jugadores_.push_back(*j);
  masNJugadores();
  if(getSocket1() == -1){
    setSocket1(j->getID());
		introducido = true;
  }
  else if((introducido == false) && (getSocket2() == -1)){
    setSocket2(j->getID());
  }
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

void Partida::setSocket1(int socket){
   socket1_ = socket;
};

void Partida::setSocket2(int socket){
   socket2_ = socket;
};

void Partida::setTurno(int turno){
   turno_ = turno;
};

void Partida::vaciarJugadores(){
	jugadores_.clear();
};


//OPERADORES DE ASIGNACIÓN
Partida & Partida::operator=(Partida &p){
  /* Copiamos todo excepto el ID, porque lo queremos mantener,
    la ficha más alta, porque se modificará al iniciar la
    partida, y el turno. */
  setNJugadores(p.getNJugadores());
  setMonton(p.getMonton());
	tablero_.clear();
  fDobles_.clear();
  setSocket1(p.getSocket1());
  setSocket2(p.getSocket2());

	// Se devuelve el objeto actual
	return *this;
}
