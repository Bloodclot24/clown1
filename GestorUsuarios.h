#ifndef GESTOR_USUARIOS_H_
#define GESTOR_USUARIOS_H_

#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>

#include "Usuario.h"
#include "LockFile.h"

#define	ARCHIVO_LOCK_USUARIOS	"archivo_usuarios"


using namespace std;

class GestorUsuarios {

	private:
		vector<Usuario> usuarios; //esto es una lista, hace falta o leo todas las veces?
		//ArchivoUsuarios archivoUsuarios;
		
		LockFile lock;

		//Actualiza los datos de la lista de usuarios
		void actualizarUsuarios();
		void guardarUsuarios();
		int escribir (string ruta,int pid, string nombre);
		int leer (string& ruta,int& pid, string& nombre);
		int parsearLinea(string linea,string& nombre,int& pid,string& archivo);
		void cerrar();
		void reset();
		string intToString(int entero) { //TODO clase utilitaria
			stringstream out;
			out << entero;
			return out.str();
		}


	public:
		GestorUsuarios ();
		virtual ~GestorUsuarios();

//		int agregarUsuario ( char* nombre,int pid ); //string?
		int eliminarUsuario ( char* nombre,int pid );
		int agregarArchivo ( string archivo,int pid, string nombre); //string?
		int eliminarArchivo ( string archivo,int pid, string nombre);
		vector<Usuario> buscarArchivos(); //ojo copia



};

#endif /* GESTOR_USUARIOS_H_ */