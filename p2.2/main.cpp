#include "funciones.hpp"
int main(){
	cout << endl;
	vector<struct user> vectorU = cargarUsuarios();

	if(log_in(vectorU)){
		cout << "\nTe has logueado." << endl;
	}
	/*else{
		cout << "La contraseña no es correcta." << endl;
	}*/
	cout << endl;
}
