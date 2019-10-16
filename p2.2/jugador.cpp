#include "jugador.hpp"
#include "partida.hpp"
using namespace std;

Jugador::Jugador(Partida *p){
	setManoInicial(p->repartir());
	setConectado(1);
	setIDPartida(p->getIDPartida());
	//setID(p->getNJugadores());
};

Jugador::Jugador(int id, string login, string pass, Partida *p){
	setLogin(login);
	setPass(pass);
	setConectado(1);
	setManoInicial(p->repartir());
	setIDPartida(p->getIDPartida());
	setID(id);
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

vector<Ficha> Jugador::getMano(){
	return mano_;
};

int Jugador::nDobles(){
	return dobles_.size();
};

void Jugador::mostrarMano(){
	for(int i = 0; i < mano_.size(); i++){
		cout << " |" << mano_[i].getNI() << "|" << mano_[i].getND() << "|";
	}
	cout << endl;
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
};

bool Jugador::tieneDobles(){
	cout << "Jugador " << getID() << endl;
	for(int i = 0; i < mano_.size(); i++){
		if(mano_[i].esDoble()){
			dobles_.push_back(mano_[i]);
			mano_[i].mostrarFicha();
		}
	}
	if(nDobles() > 0) return true;
	else return false;
};

Ficha Jugador::dobleMasAlta(){
	Ficha aux;
	if(nDobles() > 1){
		//En caso de que haya más de una doble, las ordenamos de menor a mayor
		for(int i = 0; i < dobles_.size(); i++){
			for(int j = 0; j < dobles_.size(); j++){
				//Basta con comprobar uno de los valores
				if(j != dobles_.size()){
					if(dobles_[j].getNI() > dobles_[j+1].getNI()){
						//aux = dobles_[j]
						aux.setNI(dobles_[j].getNI());
						aux.setND(dobles_[j].getND());
						//dobles_[j] = dobles_[j+1]
						dobles_[j].setNI(dobles_[j+1].getNI());
						dobles_[j].setND(dobles_[j+1].getND());
						//dobles_[j+1] = aux
						dobles_[j+1].setNI(aux.getNI());
						dobles_[j+1].setND(aux.getND());
					}
				}
			}
		}
	}
	aux.setNI((dobles_.back()).getNI());
	aux.setND((dobles_.back()).getND());
	cout << "Jugador " << getID() << endl;
	aux.mostrarFicha();
	//Devolvemos la mayor (la última si había más de una)
	return aux;
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

void Jugador::setManoInicial(vector <Ficha> vec){
	mano_ = vec;
};

void Jugador::robarFicha(Ficha a){
	mano_.push_back(a);
};

void Jugador::colocarFicha(Partida *p){
	int n;
	bool colocada = false;
	Ficha a;
	do{ // ------ Escogemos la ficha ------
		cout << "NI: ";
		cin >> n;
		a.setNI(n);
		cout << "ND: ";
		cin >> n;
		a.setND(n);
		if(!existeFicha(a)) cout << "\nEsa ficha no existe en tu montón. \nIntroduce una que sí tengas.\n\n";
	}while(!existeFicha(a));
	// ------ Escogemos el extremo y la colocamos ------
	if(p->tableroVacio()){
		p->anadirFichaTablero(a, 1); //No importa el lugar, se inserta sin más
		colocada = true;
	}
	else{
		cout << "\n[1] Izquierda    [2] Derecha: ";
		cin >> n;
		if(n == 1){
			if(p->getExtI() == a.getND()){
				p->anadirFichaTablero(a, n);
				colocada = true;
			}
			else cout << "\nEsta ficha no se puede colocar en este extremo. \nEscoge otra ficha, otro extremo o roba.\n\n";
		}
		else if(n == 2){
			if(p->getExtD() == a.getNI()){
				p->anadirFichaTablero(a, n);
				colocada = true;
			}
			else cout << "\nEsta ficha no se puede colocar en este extremo. \nEscoge otra ficha, otro extremo o roba.\n\n";
		}
	}
	// ------ Eliminamos la ficha de la mano ------
	if(colocada) mano_.erase(mano_.begin()+(buscarFicha(a)));
	p->mostrarTablero();
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
