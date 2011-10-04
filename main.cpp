#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <list>

#include "GestorUsuarios.h"
#include "GestorDescargas.h"
#include "Debug.h"

#define BUFFSIZE		100
#define HIJO		0
#define PADRE		1
#define ERR0R		-1

using namespace std;

void mostrarMenu()
{
	cout<<"===================================="<<endl;
	cout<<"=            CONCUSHARE            ="<<endl;
	cout<<"===================================="<<endl;
	cout<<"Elija una opcion:"<<endl;
	cout<<"1. Compartir un archivo."<<endl;
	cout<<"2. Buscar un archivo."<<endl;
	cout<<"3. Salir"<<endl;
	cout<<"===================================="<<endl;
	cout<<"Opcion: ";
}

void mostrarMenuCompartir()
{
	cout<<"===================================="<<endl;
	cout<<"Elija una opcion:"<<endl;
	cout<<"1. Compartir un archivo."<<endl;
	cout<<"2. Dejar de compartir un archivo."<<endl;
	cout<<"3. Volver al menu anterior."<<endl;
	cout<<"===================================="<<endl;
	cout<<"Opcion: ";
}

void mostrarArchivos(vector<Usuario>& usuarios)
{
	cout<<"------------------------------------"<<endl;
	cout << "Los archivos compartidos son: " << endl;

	vector<Usuario>::iterator it;
	int i = 0;
	for (it = usuarios.begin(); it != usuarios.end(); it++, i++) {
		cout << i << " - " << *it << endl;
	}
	cout<<"------------------------------------"<<endl;
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
				cout<<"------------------------------------"<<endl;
				cout << "Ingrese la ruta del archivo : " << endl;
				cin >> ruta;
				gestorUsuarios->agregarArchivo(ruta, pidUsuario, nombre);//ver si esta
				cout << "Archivo " << ruta << " compartido" << endl;
				cout<<"------------------------------------"<<endl;
				break;

			case '2':
				cout<<"------------------------------------"<<endl;
				cout << "Ingrese la ruta del archivo : " << endl;
				cin >> ruta;
				gestorUsuarios->eliminarArchivo(ruta, pidUsuario, nombre); //ver si esta
				cout << "Ha dejado de compartir el archivo " << ruta << endl;
				cout<<"------------------------------------"<<endl;
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


int descargarArchivo(GestorDescargas* gestorDescargas, Usuario usuario, string nombre, list<int>& hijos) {

	cout<<"------------------------------------"<<endl;
	cout << "Ingrese el numero de archivo que desea descargar" << endl;
	cout << usuario << endl;
	int numero;
	cin >> numero;
	cout<<"------------------------------------"<<endl;
	int pidDescarga = fork(); //cuidado como sigue  el hijo!!!
	if(pidDescarga == HIJO) {
		string archivo = usuario.getArchivos()[numero];
		string mensaje = "Usuario " + nombre + " inicia descarga en proceso " + Debug::getInstance()->intToString(getpid()) + "\n";
		Debug::getInstance()->escribir(mensaje);
		gestorDescargas->descargar(archivo, usuario.getPid(), nombre);
		return HIJO;
	}
	hijos.insert(hijos.end(),pidDescarga);
	return PADRE;
}

int buscarArchivos(GestorUsuarios* gestorUsuarios, GestorDescargas* gestorDescargas, string nombre, list<int>& hijos)
{
	vector<Usuario> usuarios = gestorUsuarios->buscarArchivos();
	mostrarArchivos(usuarios);

	cout << "Desea seleccionar un archivo para descargar? (s/n)" << endl;
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
				if (descargarArchivo(gestorDescargas, usuarios[numero], nombre, hijos) == HIJO)
					exit(HIJO);
				salir = true;
				break;
			case 'n':
				salir = true;
				break;

			default:
				cout << "No es una opcion valida, intente nuevamente." << endl;
				break;
		}
		cout<<"------------------------------------"<<endl;
	}

	return PADRE;
}

int ejecutarMenu(GestorDescargas* gestorDescargas, list<int>& hijos, string nombre)
{
	int pidUsuario = getpid();
	GestorUsuarios gestorUsuarios;
	string mensaje = "Usuario " + nombre + " en proceso " + Debug::getInstance()->intToString(pidUsuario) + "\n";
	Debug::getInstance()->escribir(mensaje); //ver si esto va aca

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
				if(buscarArchivos(&gestorUsuarios,gestorDescargas, nombre, hijos) == HIJO) {
					gestorUsuarios.cerrar();
					return HIJO;
				}
				break;

			case '3':
				cout << "Fin del Programa" << endl;//cerrar aca?
				gestorUsuarios.eliminarUsuario((char*)nombre.c_str(),pidUsuario);
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
		string mensaje = "Recepcion de usuario en proceso " + Debug::getInstance()->intToString(getpid()) + "finaliza el proceso\n";
		Debug::getInstance()->escribir(mensaje);
		exit(resultado);
	}

	cout<<"===================================="<<endl;
	cout<<"=     Bienvenido a CONCUSHARE      ="<<endl;
	cout<<"===================================="<<endl;
	cout << "Ingrese su nombre de usuario" << endl;
	string nombre;
	cin >> nombre; //hace falta fin de linea?

	list<int> hijos;
	if(ejecutarMenu(&gestorDescargas, hijos, nombre) == HIJO) {
		string mensaje = "Descarga de usuario en proceso " + Debug::getInstance()->intToString(getpid()) + "finaliza el proceso\n";
		Debug::getInstance()->escribir(mensaje);
		exit(0);
	} else {
		list<int>::iterator it;
		int estado, opciones;
		for( it = hijos.begin(); it != hijos.end(); it++)
			waitpid(*it,&estado,opciones);
		kill(pid,SIGINT);
		string mensaje = "Usuario en proceso " + Debug::getInstance()->intToString(getpid()) + "finaliza el proceso\n";
		Debug::getInstance()->escribir(mensaje);
		exit(0);
	}

}
