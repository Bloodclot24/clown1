#include "Vista.h"

Vista::Vista() {
}

Vista::~Vista() {
}

void Vista::mostrarBienvenida() {
	cout << "==========================================" << endl;
	cout << "=        Bienvenido a CONCUSHARE         =" << endl;
	cout << "==========================================" << endl;
	cout << "Ingrese su nombre de usuario" << endl;
}

void Vista::mostrarMenu() {
	cout << "==========================================" << endl;
	cout << "|               CONCUSHARE               |" << endl;
	cout << "==========================================" << endl;
	cout << "| Elija una opcion:                      |" << endl;
	cout << "| 1. Ingresar a mis archivos compartidos |" << endl;
	cout << "| 2. Buscar un archivo                   |" << endl;
	cout << "| 3. Salir                               |" << endl;
	cout << "==========================================" << endl;
	cout << "Opcion: ";
}

void Vista::mostrarMenuCompartir() {
	cout << "==========================================" << endl;
	cout << "| Elija una opcion:                      |" << endl;
	cout << "| 1. Compartir un archivo.               |" << endl;
	cout << "| 2. Dejar de compartir un archivo.      |" << endl;
	cout << "| 3. Volver al menu anterior.            |" << endl;
	cout << "==========================================" << endl;
	cout << "Opcion: ";
}

void Vista::mostrarArchivos(vector<Usuario>& usuarios, Usuario usuario) {
	cout << "------------------------------------------" << endl;
	cout << "Los archivos compartidos son:" << endl;

	vector<Usuario>::iterator it;
	int i = 0;
	for (it = usuarios.begin(); it != usuarios.end(); it++, i++) {
		if ((*it).getNombre() != usuario.getNombre()
				|| (*it).getPid() != usuario.getPid())
			cout << i << " - " << *it << endl;
	}
	cout << "------------------------------------------" << endl;
}

void Vista::mostrarUsuario(Usuario usuario) {
	cout << usuario << endl;
}

void Vista::mostrarUso(char* nombre) {
	cout << "Uso: " << endl;
	cout << "\t " << nombre << endl;
	cout << "\t " << nombre << " [opciones] " << endl;
	cout << "Opciones: " << endl;
	cout << "\t -d, --debug   Corre el programa en modo debug, generando la salida en un archivo de nombre debug." << endl;
}

void Vista::mostrarMensaje(string mensaje) {
	cout << mensaje << endl;
}

void Vista::mostrarMensajeInicial(string mensaje) {
	cout << "------------------------------------------" << endl;
	cout << mensaje << endl;
}

void Vista::mostrarMensajeFinal(string mensaje) {
	cout << mensaje << endl;
	cout << "------------------------------------------" << endl;
}

string Vista::pedirString() {
	string cadena;
	cin >> cadena;
	return cadena;
}

char Vista::pedirChar() {
	char caracter;
	cin >> caracter;
	return caracter;
}

int Vista::pedirInt(int inferior, int superior) {
	char numeroLeido = '0';
	int numero = 0;
	bool salir = false;
	while (!salir) {
		cin >> numeroLeido;
		numero = atoi(&numeroLeido);
		if (numero >= inferior && numero <= superior)
			salir = true;
		else
			cout << "No es una opcion valida, intente nuevamente." << endl;
	}
	return numero;
}

void Vista::debug(string mensaje, int entero) {
	//cout << mensaje << entero << endl;
}

void Vista::debug(string mensaje) {
	//cout << mensaje << endl;
}
