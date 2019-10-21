#include "jugador.hpp"
#include "partida.hpp"
using namespace std;

Jugador::Jugador(int id, Partida *p){
	setManoInicial(p->repartir());
	setIDPartida(p->getIDPartida());
	setID(id);
	setPuntos(0);
};

int Jugador::getID(){
	return ID_;
};

int Jugador::getIDPartida(){
	return idPartida_;
};

int Jugador::getPuntos(){
	int aux;
	for(int i = 0; i < mano_.size(); i++){
		aux = mano_[i].getNI() + mano_[i].getND();
		puntos_ += aux;
	}
	return puntos_;
};

vector<Ficha> Jugador::getMano(){
	return mano_;
};

int Jugador::nDobles(){
	return dobles_.size();
};

int Jugador::nFichas(){
	return mano_.size();
};

string Jugador::mostrarMano(){
	char cad[250], I[10], D[10];

	strcpy (cad,"\nFICHAS |");
	for(int i = 0; i < mano_.size(); i++){
		sprintf(I, "%d", mano_[i].getNI());
		sprintf(D, "%d", mano_[i].getND());
		strcat (cad,I);
		strcat (cad,"|");
		strcat (cad,D);
		if(i == mano_.size()-1) strcat (cad,"|");
		else strcat (cad,"||");
		//cout << " |" << mano_[i].getNI() << "|" << mano_[i].getND() << "|";
	}
	string cadena(cad);
	return cadena;
};

bool Jugador::existeFicha(Ficha a){
	for(int i = 0; i < mano_.size(); i++){
		if((a.getNI() == mano_[i].getNI()) && (a.getND() == mano_[i].getND())){
			return true;
		}
	}
	return false;
};

int Jugador::buscarFicha(Ficha a){
	for(int i = 0; i < mano_.size(); i++){
		if((a.getNI() == mano_[i].getNI()) && (a.getND() == mano_[i].getND())){
			return i;
		}
	}
	return 100000;
};

bool Jugador::tieneDobles(){
	for(int i = 0; i < mano_.size(); i++){
		if(mano_[i].esDoble()){
			dobles_.push_back(mano_[i]);
			//fflush(stdout);
			//mano_[i].mostrarFicha();
		}
	}
	if(nDobles() > 0){
		return true;
	}
	else{
		return false;
	}
};

Ficha Jugador::dobleMasAlta(){
	Ficha mayor;
	mayor = dobles_[0];
	if(nDobles() > 1){
	  for(int i = 0; i < nDobles(); i++){
	    //Basta con comprobar uno de los valores
	    if(mayor.getNI() < dobles_[i].getNI()){
				mayor = dobles_[i];
	    }
	  }
	}
	//cout << "Jugador " << getID() << "-> Ficha doble mayor: " << endl;
	//fflush(stdout);
	//mayor.mostrarFicha();
	return mayor;
};

Ficha Jugador::masAlta(){
	Ficha mayor;
	int sumaMayor, aux;
	mayor = mano_[0];
	sumaMayor = mayor.getNI() + mayor.getND();
	for(int i = 0; i < mano_.size(); i++){
		aux = mano_[i].getNI() + mano_[i].getND();
		if(aux > sumaMayor){
			mayor = mano_[i];
			sumaMayor = aux;
		}
	}
	return mayor;
};

bool Jugador::puedePoner(Partida *p){
	if(p->tableroVacio()) return true;
	for(int i = 0; i < mano_.size(); i++){
		if((p->getExtI() == mano_[i].getNI()) || (p->getExtI() == mano_[i].getND()) || (p->getExtD() == mano_[i].getNI()) || (p->getExtD() == mano_[i].getND())) return true;
	}
	return false;
};

bool Jugador::tieneFichas(){
	if(nFichas() > 0){
		return true;
	}
	else{
		return false;
	}
};


void Jugador::setID(int id){
	ID_ = id;
};

void Jugador::setIDPartida(int id){
	idPartida_ = id;
};

void Jugador::setPuntos(int puntos){
	puntos_ = puntos;
};

void Jugador::setManoInicial(vector <Ficha> vec){
	mano_ = vec;
};

void Jugador::robarFicha(Ficha a){
	mano_.push_back(a);
};

void Jugador::girarFicha(Ficha *a){
	int aux;
	aux = a->getNI();
	a->setNI(a->getND());
	a->setND(aux);
};

bool Jugador::colocarFicha(int NI, int ND, int extremo, Partida *p){
	int pos;
	Ficha a(NI,ND);
	if(!existeFicha(a)){
		//cout << "\nNo existe la ficha.\n";
		return false;
	}

	pos = buscarFicha(a);
	// ------ Comprobamos que la primera que ponga, si sale él, sea la doble mayor -----
	if(p->tableroVacio()){
		if((mano_[pos].getNI() != p->getMasAlta().getNI()) or (mano_[pos].getND() != p->getMasAlta().getND())){
			//cout << "\nPara empezar la partida debe colocar su ficha doble más alta.\n";
			return false;
		}
		else if((mano_[pos].getNI() == p->getMasAlta().getNI()) && (mano_[pos].getND() == p->getMasAlta().getND())){
			p->anadirFichaTablero(mano_[pos], 1); //No importa el lugar, se inserta sin más
			mano_.erase(mano_.begin()+pos);
			return true;
		}
	}
	else{ // ------ Comprobamos los extremos y la colocamos ------
		if(extremo == 1){ //Comprobamos los números de los extremos y giramos si hace falta
			if(p->getExtI() == mano_[pos].getNI()){
				girarFicha(&mano_[pos]);
				p->anadirFichaTablero(mano_[pos], extremo);
				mano_.erase(mano_.begin()+pos);
				return true;
			}
			else if(p->getExtI() == mano_[pos].getND()){
				p->anadirFichaTablero(mano_[pos], extremo);
				mano_.erase(mano_.begin()+pos);
				return true;
			}
			else {
				//cout << "\nEsta ficha no se puede colocar en este extremo. \nEscoge otra ficha, otro extremo o roba.\n\n";
				return false;
			}
		}
		else if(extremo == 2){
			if(p->getExtD() == mano_[pos].getNI()){
				p->anadirFichaTablero(mano_[pos], extremo);
				mano_.erase(mano_.begin()+pos);
				return true;
			}
			else if(p->getExtD() == mano_[pos].getND()){
				girarFicha(&mano_[pos]);
				p->anadirFichaTablero(mano_[pos], extremo);
				mano_.erase(mano_.begin()+pos);
				return true;
			}
			else {
				//cout << "\nEsta ficha no se puede colocar en este extremo. \nEscoge otra ficha, otro extremo o roba.\n\n";
				return false;
			}
		}
	}
};


void Jugador::salirPartida(Partida *p){
	if(!mano_.empty()){
		mano_.clear();
	}
	 if(p->getNJugadores() != 0) p->vaciarJugadores();
	 p->menosNJugadores();
};
