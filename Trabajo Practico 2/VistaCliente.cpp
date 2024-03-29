#include <iostream>
#include <unistd.h>
#include <string>
#include "Cliente.h"

using namespace std;

void mostrarBienvenida() {
	cout << "==========================================" << endl;
	cout << "=               Bienvenido               =" << endl;
	cout << "==========================================" << endl;
}

string pedirString() {
	char cadena[120];
	cin.getline(cadena,120);
	return string(cadena);
}

string ejecutarMenu() {
	cout << "==========================================" << endl;
	cout << "|             Base de Datos              |" << endl;
	cout << "==========================================" << endl;
	cout << "| Elija una opcion:                      |" << endl;
	cout << "| 1. Buscar una persona                  |" << endl;
	cout << "| 2. Agregar una persona                 |" << endl;
	cout << "| 3. Modificar una persona               |" << endl;
	cout << "| 4. Eliminar una persona                |" << endl;
	cout << "| 5. Salir                               |" << endl;
	cout << "==========================================" << endl;
	cout << "Opcion: ";
	return pedirString();
}

bool respondeSi() {
	string opcion;
	do {
		opcion = pedirString();
	} while (opcion.c_str()[0] != 's'&& opcion.c_str()[0] != 'n');
	return opcion.c_str()[0] == 's';
}

void pedirDatosPersona(string &nombre, string &direccion, string &telefono){
	cout << "------------------------------------------" << endl;
	cout << "Ingrese el nombre de la persona" << endl;
	nombre = pedirString();
	cout << "Ingrese la direccion de la persona" << endl;
	direccion = pedirString();
	cout << "Ingrese el telefono de la persona" << endl;
	telefono = pedirString();
}

void agregarPersona(Cliente &cliente) {
	string nombre, direccion, telefono;
	pedirDatosPersona(nombre, direccion, telefono);
	cout << "Desea agregar la persona " << nombre << " a la base de datos (s/n)?" << endl;
	if(respondeSi())
		cout << cliente.agregarPersona(nombre, direccion, telefono) << " " << nombre << endl;
	cout << "------------------------------------------" << endl;
}

void modificarPersona(Cliente &cliente) {
	string nombre, direccion, telefono;
	pedirDatosPersona(nombre, direccion, telefono);
	cout << "Desea modificar la persona " << nombre << " en la base de datos (s/n)?" << endl;
	if(respondeSi())
		cout << cliente.modificarPersona(nombre, direccion, telefono) << " " << nombre << endl;
	cout << "------------------------------------------" << endl;
}

void eliminarPersona(Cliente &cliente) {
	cout << "------------------------------------------" << endl;
	cout << "Ingrese el nombre de la persona a eliminar" << endl;
	string nombre = pedirString();
	cout << "Desea eliminar la persona " << nombre << " de la base de datos (s/n)?" << endl;
	if(respondeSi())
		cout << cliente.eliminarPersona(nombre) << " " << nombre << endl;
	cout << "------------------------------------------" << endl;
}

void consultarPersona(Cliente &cliente) {
	cout << "------------------------------------------" << endl;
	cout << "Ingrese el nombre de la persona buscada" << endl;
	string nombre = pedirString();
	Registro persona = cliente.consultarPersona(nombre);
	if(strcmp(persona.nombre,"") != 0) {
		cout << "Nombre: " << persona.nombre << endl;
		cout << "Direccion: " << persona.direccion << endl;
		cout << "Telefono: " << persona.telefono << endl;
	} else
		cout << "No se encontro la persona " << nombre << "  en la base de datos" << endl;
}

int main(int argc, char * argv[]) {
	Cliente cliente;
	mostrarBienvenida();

	bool salir = false;
	while (!salir) {
		string opcion = ejecutarMenu();
		switch (opcion.c_str()[0])
		{
			case '1':
				consultarPersona(cliente);
				break;
			case '2':
				agregarPersona(cliente);
				break;
			case '3':
				modificarPersona(cliente);
				break;
			case '4':
				eliminarPersona(cliente);
				break;
			case '5':
				salir = true;
				break;
			default:
				cout << "No es una opcion valida, intente nuevamente." << endl;
				break;
		}
	}
	cout << "Fin del proceso cliente" << endl;
	return 0;
}
