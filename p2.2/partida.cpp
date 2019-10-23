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

vector<Ficha> Partida::getFichasMayores(){
	return fMayores_;
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
  int sumaMayor, aux;//, k = 0;
	bool hayDobles = false;
  vector <Ficha> vec;
	/* Primero comprueba si los jugadores tienen fichas dobles */
  for(unsigned int i = 0; i < jugadores_.size(); i++){
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
    setMasAlta(fDobles_[0]);
		if(fDobles_.size() > 1){
		  for(unsigned int i = 0; i < fDobles_.size(); i++){
		    //Basta con comprobar uno de los valores
		    if(masAlta_.getNI() < fDobles_[i].getNI()){
          setMasAlta(fDobles_[i]);
		    }
		  }
		}
	}
	/* Si ningún jugador tiene fichas dobles, se pasaría a comprobar
	   la siguiente ficha más alta y comenzará quien la tenga.*/
	else if(!hayDobles){
    /*//Creamos un vector con 21 fichas (sin dobles) para comparar
  	for(int i = k; i < 7; i++){
  		for(int j = k; j < 7; j++){
  			Ficha a(i,j);
  			if(!a.esDoble()) vec.push_back(a);
  		}
  		k++;
  	}
    cout << "\n Vector sin dobles: \n\n ";
		for(int i = 0; i < vec.size(); i++){
			cout << "|" << vec[i].getNI() << "|" << vec[i].getND() << "|";
		}
    cout << endl;*/

		for(unsigned int i = 0; i < jugadores_.size(); i++){
				fMayores_.push_back(jugadores_[i].masAlta());
	  }
		/*cout << "\n\n Fichas mayores:\n";
		for(int i = 0; i < fMayores_.size(); i++){
			fflush(stdout);
			fMayores_[i].mostrarFicha();
		}*/
    setMasAlta(fMayores_[0]);
		sumaMayor = fMayores_[0].getNI() + fMayores_[0].getND();
	  for(unsigned int i = 0; i < fMayores_.size(); i++){
	    aux = fMayores_[i].getNI() + fMayores_[i].getND();
	    if(aux > sumaMayor){
        setMasAlta(fMayores_[i]);
				sumaMayor = aux;
	    }
	  }
	}
	/* Devolvemos la ficha mayor, doble o no, y los jugadores_
     comprobarán si la tienen, comenzando aquel que la tenga.*/
	return getMasAlta();
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

void Partida::setFichasMayores(vector <Ficha> vec){
	fMayores_ = vec;
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
  fMayores_.clear();
  setSocket1(p.getSocket1());
  setSocket2(p.getSocket2());

	// Se devuelve el objeto actual
	return *this;
}
