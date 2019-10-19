#include "jugador.hpp"
#include "partida.hpp"
using namespace std;

Jugador::Jugador(Partida *p){
	setManoInicial(p->repartir());
	setConectado(1);
	//setIDPartida(p->getIDPartida());
	//setID(p->getNJugadores());
};

Jugador::Jugador(int id, Partida *p){
	//setLogin(login);
	//setPass(pass);
	setConectado(1);
	setManoInicial(p->repartir());
	setIDPartida(p->getIDPartida());
	setID(id);
	setPuntos(0);
};

int Jugador::getID(){
	return ID_;
};

string Jugador::getLogin(){
	return login_;
};

string Jugador::getPass(){
	return password_;
};

int Jugador::getConectado(){
	return conectado_;
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
  //mayor.setNI(dobles_[0].getNI());
  //mayor.setND(dobles_[0].getND());
	if(nDobles() > 1){
	  for(int i = 0; i < nDobles(); i++){
	    //Basta con comprobar uno de los valores
	    if(mayor.getNI() < dobles_[i].getNI()){
				mayor = dobles_[i];
	      //mayor.setNI(dobles_[i].getNI());
	      //mayor.setND(dobles_[i].getND());
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
	//mayor.setNI(mano_[0].getNI());
	//mayor.setND(mano_[0].getND());
	sumaMayor = mayor.getNI() + mayor.getND();
	for(int i = 0; i < mano_.size(); i++){
		aux = mano_[i].getNI() + mano_[i].getND();
		if(aux > sumaMayor){
			mayor = mano_[i];
			//mayor.setNI(mano_[i].getNI());
			//mayor.setND(mano_[i].getND());
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

void Jugador::setLogin(string login){
	login_ = login;
};

void Jugador::setPass(string pass){
	password_ = pass;
};

void Jugador::setConectado(int flag){
	conectado_ = flag;
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
	//bool colocada = false;
	Ficha a(NI,ND);
	/*do{ // ------ Escogemos la ficha ------
		cout << "NI: ";
		cin >> n;
		a.setNI(n);
		cout << "ND: ";
		cin >> n;
		a.setND(n);
		if(!existeFicha(a)) cout << "\nEsa ficha no existe en tu montón. \nIntroduce una que sí tengas.\n\n";
	}while(!existeFicha(a));*/
	if(!existeFicha(a)){
		return false;
	}
	
	pos = buscarFicha(a);
	// ------ Comprobamos que la primera que ponga, si sale él, sea la doble mayor -----
	if(p->tableroVacio()){
		if((mano_[pos].getNI() != p->getMasAlta().getNI()) && (mano_[pos].getND() != p->getMasAlta().getND())){
			//cout << "\nPara empezar la partida debe colocar su ficha doble más alta.\n";
			return false;
		}
		else{
			p->anadirFichaTablero(mano_[pos], 1); //No importa el lugar, se inserta sin más
			mano_.erase(mano_.begin()+pos);
			return true;
		}
	}
	else{ // ------ Escogemos el extremo y la colocamos ------
		//cout << "\n[1] Izquierda    [2] Derecha: ";
		//cin >> n;
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
		else return false;
	}
	// ------ Eliminamos la ficha de la mano ------
	/*if(colocada){
		mano_.erase(mano_.begin()+pos);
		p->mostrarTablero();
		return true;
	}
	else return false;*/
};


void Jugador::salirPartida(Partida *p){
	p->menosNJugadores();
	setIDPartida(-1);
	if(!mano_.empty()){
		/*for(int i = 0; i < mano_.size(); i++){
			p->anadirFichaMonton(mano_[i]);
		}*/
		mano_.clear();
	}
};
