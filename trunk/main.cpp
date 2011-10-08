#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <list>
#include <getopt.h>
//#include <unistd.h>

#include "GestorUsuarios.h"
#include "GestorDescargas.h"
#include "Debug.h"
#include "Vista.h"

#define BUFFSIZE		100
#define HIJO		0
#define PADRE		1
#define ERR0R		-1

using namespace std;

int compartirArchivos(GestorUsuarios* gestorUsuarios, Usuario& usuario)
{
	string ruta;
	int numero;
	bool salir = false;
	while (!salir) {
		Vista::mostrarMenuCompartir();
		char opcion = Vista::pedirChar();

		switch (opcion)
		{
			case '1':
				Vista::mostrarMensajeInicial("Ingrese la ruta del archivo : ");
				ruta = Vista::pedirString();
				gestorUsuarios->agregarArchivo(ruta, usuario);//ver si esta
				Vista::mostrarMensajeFinal("Archivo " + ruta + " compartido");
				break;

			case '2':
				Vista::mostrarUsuario(usuario);
				Vista::mostrarMensajeInicial("Ingrese el numero del archivo : ");
				numero = Vista::pedirInt(0, usuario.getArchivos().size() - 1);
				ruta = usuario.getArchivos()[numero];
				gestorUsuarios->eliminarArchivo(ruta, usuario);
				Vista::mostrarMensajeFinal("Ha dejado de compartir el archivo " + ruta);
				break;

			case '3':
				salir = true;
				break;

			default:
				Vista::mostrarMensaje("No es una opcion valida, intente nuevamente.");
				break;
		}
	}
	return PADRE;
}


int descargarArchivo(GestorDescargas* gestorDescargas, Usuario usuarioOrigen, Usuario usuarioDestino, list<int>& hijos)
{
	Vista::mostrarUsuario(usuarioOrigen);
	Vista::mostrarMensaje("Ingrese el numero de archivo que desea descargar");
	int numero = Vista::pedirInt(0, usuarioOrigen.getArchivos().size() - 1);
	Vista::mostrarMensajeFinal("");

	int pidDescarga = fork(); //cuidado como sigue  el hijo!!!
	if(pidDescarga == HIJO) {
		string archivo = usuarioOrigen.getArchivos()[numero];
		Debug::getInstance()->escribir("Usuario " + usuarioDestino.getNombre() + " inicia descarga en proceso " + Debug::intToString(usuarioDestino.getPid()) + "\n");
		gestorDescargas->descargar(archivo, usuarioOrigen, usuarioDestino);
		Vista::mostrarMensaje("Archivo " + archivo + " descargado");
		Debug::destruir();
		Vista::debug("PID2 descarga hijo ", getpid());
		return HIJO;
	} //else { verrr
		hijos.insert(hijos.end(), pidDescarga);
		Vista::debug("PID2 descarga padre ", getpid());
		return PADRE;
	//}
}

int buscarArchivos(GestorUsuarios* gestorUsuarios, GestorDescargas* gestorDescargas, Usuario& usuario, list<int>& hijos)
{
	vector<Usuario> usuarios = gestorUsuarios->buscarArchivos();
	Vista::mostrarArchivos(usuarios, usuario);

	Vista::mostrarMensaje("Desea seleccionar un archivo para descargar? (s/n)");
	int numero;
	bool salir = false;
	while (!salir) {
		char opcion = Vista::pedirChar();
		switch (opcion)
		{
			case 's':
				Vista::mostrarMensaje("Ingrese el numero del usuario al que pertenece el archivo : ");
				numero = Vista::pedirInt(0, usuarios.size() - 1);
				if (descargarArchivo(gestorDescargas, usuarios[numero], usuario, hijos) == HIJO)
					return HIJO;
				salir = true;
				break;

			case 'n':
				salir = true;
				break;

			default:
				Vista::mostrarMensaje("No es una opcion valida, intente nuevamente.");
				break;
		}
	}

	return PADRE;
}

int ejecutarMenu(GestorDescargas* gestorDescargas, list<int>& hijos, Usuario& usuario)
{
	GestorUsuarios gestorUsuarios;
	Debug::getInstance()->escribir("Usuario " + usuario.getNombre() + " en proceso " + Debug::intToString(usuario.getPid()) + "\n"); //ver si esto va aca

	bool salir = false;
	while (!salir) {
		Vista::mostrarMenu();
		char opcion = Vista::pedirChar();
		switch (opcion)
		{
			case '1':
				compartirArchivos(&gestorUsuarios, usuario);
				break;

			case '2':
				if(buscarArchivos(&gestorUsuarios,gestorDescargas, usuario, hijos) == HIJO) {
					gestorUsuarios.cerrar();
					Vista::debug("PID2 menu ", getpid());
					return HIJO;
				}
				break;

			case '3':
				Vista::mostrarMensaje("Fin del Programa");
				gestorUsuarios.eliminarUsuario(usuario);
				salir = true;
				break;

			default:
				Vista::mostrarMensaje("No es una opcion valida, intente nuevamente.");
				break;
		}
	}
	return PADRE;

}

int parsearLineaDeComandos(int argc, char** argv) {
	int ch;
	int index = 0;
	struct option options[] = { { "help", 0, NULL, 'h' },
								{ "debug", 0, NULL,	'd' }, };
	//while ((ch = getopt_long(argc, argv, "hd", options, &index)) != -1) { //para opciones largas

	while ((ch = getopt(argc, argv, "hd")) != -1) {
		switch (ch) {
		case 'h':
			Vista::mostrarUso(argv[0]);
			return 1;
			break;
		case 'd':

			break;
		default:
			Vista::mostrarUso(argv[0]);
			return 1;
			break;
		}
	}
	return 0;
}

int main(int argc, char** argv)
{
	if(parsearLineaDeComandos(argc, argv) != 0)
		return 0;

	GestorDescargas gestorDescargas;
	int pid = fork ();
	if(pid == HIJO){
		int resultado = gestorDescargas.iniciarRecepcion();
		Debug::getInstance()->escribir("Recepcion de usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
		Debug::destruir();
		Vista::debug("PID0 ", getpid());
		return resultado;
	}
	Vista::debug("PID1 ", getpid());

	Vista::mostrarBienvenida();

	string nombre = Vista::pedirString();
	Usuario usuario(nombre,getpid());

	string directorio = "descargas_" + nombre + "_" + Debug::intToString(getpid()) + "/";
	int estado = mkdir(directorio.c_str(), 0700);// TODO control de errores
	if(estado >= 0)
		Debug::getInstance()->escribir("Proceso " + Debug::intToString(getpid()) + ": creo el directorio " + directorio + " para realizar la descarga\n");

	list<int> hijos;
	if(ejecutarMenu(&gestorDescargas, hijos, usuario) == HIJO) {
		Debug::getInstance()->escribir("Descarga de usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
		Debug::destruir();
		Vista::debug("PID2 ", getpid());
	} else {
		list<int>::iterator it;
		int estado, opciones;
		for( it = hijos.begin(); it != hijos.end(); it++)
			waitpid(*it,&estado,opciones);
		kill(pid,SIGINT);
		Debug::getInstance()->escribir("Usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
		Debug::destruir();
		Vista::debug("PID3 ", getpid());
	}
	return 0;
}