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
	cout<<"=========================================="<<endl;
	cout<<"|               CONCUSHARE               |"<<endl;
	cout<<"=========================================="<<endl;
	cout<<"| Elija una opcion:                      |"<<endl;
	cout<<"| 1. Ingresar a mis archivos compartidos |"<<endl;
	cout<<"| 2. Buscar un archivo                   |"<<endl;
	cout<<"| 3. Salir                               |"<<endl;
	cout<<"=========================================="<<endl;
	cout<<" Opcion: ";
}

void mostrarMenuCompartir()
{
	cout<<"=========================================="<<endl;
	cout<<"| Elija una opcion:                      |"<<endl;
	cout<<"| 1. Compartir un archivo.               |"<<endl;
	cout<<"| 2. Dejar de compartir un archivo.      |"<<endl;
	cout<<"| 3. Volver al menu anterior.            |"<<endl;
	cout<<"=========================================="<<endl;
	cout<<"Opcion: ";
}

void mostrarArchivos(vector<Usuario>& usuarios, Usuario usuario)
{
	cout<<"------------------------------------------"<<endl;
	cout<<"Los archivos compartidos son:" << endl;

	vector<Usuario>::iterator it;
	int i = 0;
	for (it = usuarios.begin(); it != usuarios.end(); it++, i++) {
		if((*it).getNombre() != usuario.getNombre() || (*it).getPid() != usuario.getPid())
			cout << i << " - " << *it << endl;
	}
	cout<<"------------------------------------------"<<endl;
}

int compartirArchivos(GestorUsuarios* gestorUsuarios, Usuario usuario)
{
	string ruta;
	bool salir = false;

	while (!salir) {
		mostrarMenuCompartir();
		char opcion;
		cin >> opcion;

		switch (opcion)
		{
			case '1':
				cout<<"------------------------------------------"<<endl;
				cout << "Ingrese la ruta del archivo : " << endl;
				cin >> ruta;
				gestorUsuarios->agregarArchivo(ruta, usuario);//ver si esta
				cout << "Archivo " << ruta << " compartido" << endl;
				cout<<"------------------------------------------"<<endl;
				break;

			case '2':
				cout<<"------------------------------------------"<<endl;
				cout << "Ingrese la ruta del archivo : " << endl;
				cin >> ruta;
				gestorUsuarios->eliminarArchivo(ruta, usuario); //ver si esta
				cout << "Ha dejado de compartir el archivo " << ruta << endl;
				cout<<"------------------------------------------"<<endl;
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


int descargarArchivo(GestorDescargas* gestorDescargas, Usuario usuarioOrigen, Usuario usuarioDestino, list<int>& hijos)
{
	cout << "Ingrese el numero de archivo que desea descargar" << endl;
	cout << usuarioOrigen << endl;
	int numero;
	cin >> numero;
	int pidDescarga = fork(); //cuidado como sigue  el hijo!!!
	if(pidDescarga == HIJO) {
		string archivo = usuarioOrigen.getArchivos()[numero];
		Debug::getInstance()->escribir("Usuario " + usuarioDestino.getNombre() + " inicia descarga en proceso " + Debug::intToString(usuarioDestino.getPid()) + "\n");
		gestorDescargas->descargar(archivo, usuarioOrigen, usuarioDestino);
		cout << "Archivo " + archivo + " descargado" << endl;
		return HIJO;
	}
	hijos.insert(hijos.end(),pidDescarga);
	return PADRE;
}

int buscarArchivos(GestorUsuarios* gestorUsuarios, GestorDescargas* gestorDescargas, Usuario usuario, list<int>& hijos)
{
	vector<Usuario> usuarios = gestorUsuarios->buscarArchivos();
	mostrarArchivos(usuarios, usuario);

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
				if (descargarArchivo(gestorDescargas, usuarios[numero], usuario, hijos) == HIJO)
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
	}

	return PADRE;
}

int ejecutarMenu(GestorDescargas* gestorDescargas, list<int>& hijos, Usuario usuario)
{
	GestorUsuarios gestorUsuarios;
	Debug::getInstance()->escribir("Usuario " + usuario.getNombre() + " en proceso " + Debug::intToString(usuario.getPid()) + "\n"); //ver si esto va aca

	bool salir = false;
	while (!salir) {
		mostrarMenu();
		char opcion;
		cin >> opcion;

		switch (opcion)
		{
			case '1':
				compartirArchivos(&gestorUsuarios, usuario);
				break;

			case '2':
				if(buscarArchivos(&gestorUsuarios,gestorDescargas, usuario, hijos) == HIJO) {
					gestorUsuarios.cerrar();
					return HIJO;
				}
				break;

			case '3':
				cout << "Fin del Programa" << endl;//cerrar aca?
				gestorUsuarios.eliminarUsuario(usuario);
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
		Debug::getInstance()->escribir("Recepcion de usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
		exit(resultado);
	}

	cout<<"=========================================="<<endl;
	cout<<"=        Bienvenido a CONCUSHARE         ="<<endl;
	cout<<"=========================================="<<endl;
	cout << "Ingrese su nombre de usuario" << endl;
	string nombre;
	cin >> nombre;
	Usuario usuario(nombre,getpid());

	string directorio = "descargas_" + nombre + "_" + Debug::intToString(getpid()) + "/";
	int estado = mkdir(directorio.c_str(), 0700);// TODO control de errores
	if(estado >= 0)
		Debug::getInstance()->escribir("Proceso " + Debug::intToString(getpid()) + ": creo el directorio " + directorio + " para realizar la descarga\n");

	list<int> hijos;
	if(ejecutarMenu(&gestorDescargas, hijos, usuario) == HIJO) {
		Debug::getInstance()->escribir("Descarga de usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
		exit(0);
	} else {
		list<int>::iterator it;
		int estado, opciones;
		for( it = hijos.begin(); it != hijos.end(); it++)
			waitpid(*it,&estado,opciones);
		kill(pid,SIGINT);
		Debug::getInstance()->escribir("Usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
		exit(0);
	}

}
