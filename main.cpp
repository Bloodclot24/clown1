#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>

#include "GestorUsuarios.h"
#include "GestorDescargas.h"

#define BUFFSIZE		100
#define HIJO		0
#define PADRE		1
#define ERR0R		-1

using namespace std;

void mostrarMenu()
{
	cout<<"Bienvenido a ConcuShare!!!"<<endl;
	cout<<"Elija una opcion:"<<endl;
	cout<<"1. Compartir un archivo."<<endl;
	cout<<"2. Buscar un archivo."<<endl;
	cout<<"3. Salir"<<endl;
	cout<<"Opcion: ";
}

void mostrarMenuCompartir()
{
	cout<<"Elija una opcion:"<<endl;
	cout<<"1. Compartir un archivo."<<endl;
	cout<<"2. Dejar de compartir un archivo."<<endl;
	cout<<"3. Volver al menu anterior."<<endl;
	cout<<"Opcion: ";
}

int compartirArchivos(GestorUsuarios* gestorUsuarios, string nombre)
{
	int pidUsuario = getpid();
	string ruta;
	bool salir = false;

	while (!salir) {
		mostrarMenuCompartir();
		char opcion;
		cin >> opcion;

		switch (opcion)
		{
			case '1':
				cout << "Ingrese la ruta del archivo : " << endl;
				cin >> ruta;
				gestorUsuarios->agregarArchivo(ruta, pidUsuario, nombre);//ver si esta
				cout << "Archivo " << ruta << " compartido" << endl;
				break;

			case '2':
				cout << "Ingrese la ruta del archivo : " << endl;
				cin >> ruta;
				gestorUsuarios->eliminarArchivo(ruta, pidUsuario, nombre); //ver si esta
				cout << "Ha dejado de compartir el archivo " << ruta << endl;
				break;

			case '3':
				salir = true;
				break;

			default:
				cout << "No es una opcion valida, intente nuevamente." << endl;
				break;
		}
	}
	return PADRE;
}


int descargarArchivo(GestorDescargas* gestorDescargas, Usuario usuario) {

	cout << "Ingrese el numero de archivo que desea descargar" << endl;
	cout << usuario << endl;
	int numero;
	cin >> numero;
	int pidDescarga = fork(); //cuidado como sigue  el hijo!!!
	if(pidDescarga == HIJO) {
		string archivo = usuario.getArchivos()[numero];
		gestorDescargas->descargar(archivo, usuario.getPid());
		return HIJO;
	}
	return PADRE;
}

int buscarArchivos(GestorUsuarios* gestorUsuarios, GestorDescargas* gestorDescargas)
{
	cout << "Los archivos compartidos son: " << endl;

	vector<Usuario> usuarios = gestorUsuarios->buscarArchivos();
	vector<Usuario>::iterator it;
	int i = 0;
	for (it = usuarios.begin(); it != usuarios.end(); it++, i++) {
		cout << i << " - " << *it << endl;
	}
	cout << "Desea seleccinar un archivo para descargar? (s/n)" << endl;
	bool salir = false;
	while (!salir) {
		int numero;
		char opcion;
		cin >> opcion;

		switch (opcion)
		{
			case 's':
				cout << "Ingrese el numero del usuario al que pertenece el archivo : " << endl;
				cin >> numero; //control!!
				if (descargarArchivo(gestorDescargas, usuarios[numero]) == HIJO)
					return HIJO;
			case 'n':
				salir = true;
				break;

			default:
				cout << "No es una opcion valida, intente nuevamente." << endl;
				break;
		}
	}

	return PADRE;
}

int ejecutarMenu(GestorDescargas* gestorDescargas)
{
	GestorUsuarios gestorUsuarios;
	cout << "Ingrese su nombre de usuario" << endl;
	string nombre;
	cin >> nombre; //hace falta fin de linea?
	int pidUsuario = getpid();

	cout << "Usuario " << pidUsuario << ": escribo el dato [" << nombre << "] [" << pidUsuario << "] en el gestor"<< nombre.length() << endl;
//	gestorUsuarios.agregarUsuario(usuario, pidUsuario); //no habria q decir el tambanio del char?

	bool salir = false;
	while (!salir) {
		mostrarMenu();
		char opcion;
		cin >> opcion;

		switch (opcion)
		{
			case '1':
				compartirArchivos(&gestorUsuarios, nombre);
				break;

			case '2':
				if(buscarArchivos(&gestorUsuarios,gestorDescargas) == HIJO)
					return HIJO;
				break;

			case '3':
				cout << "Fin del Programa" << endl;//cerrar aca?
				salir = true;
				break;

			default:
				cout << "No es una opcion valida, intente nuevamente." << endl;
				break;
		}
	}
	return PADRE;

}

int main(int argc, char** argv)
{
	GestorDescargas gestorDescargas;
	int pid = fork ();
	if(pid == HIJO){
		int resultado = gestorDescargas.iniciarRecepcion();
		exit(resultado);
	}

	if(ejecutarMenu(&gestorDescargas) == HIJO)
		;	//cerrar aca?
	else
		cout << "Usuario " << getpid() << ": fin del proceso" << endl; //ESPERAR A LOS HIJOS
	exit ( 0 );

}
