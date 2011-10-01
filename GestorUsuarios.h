#ifndef GESTOR_USUARIOS_H_
#define GESTOR_USUARIOS_H_

#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>

#include "Usuario.h"
#include "ArchivoUsuarios.h"

using namespace std;

class GestorUsuarios {

	private:
		vector<Usuario> usuarios; //esto es una lista, hace falta o leo todas las veces?
		ArchivoUsuarios archivoUsuarios;
		
		//Actualiza los datos de la lista de usuarios
		void actualizarUsuarios();
		void guardarUsuarios();

	public:
		GestorUsuarios ();
		virtual ~GestorUsuarios();

//		int agregarUsuario ( char* nombre,int pid ); //string?
		int eliminarUsuario ( char* nombre,int pid );
		int agregarArchivo ( string archivo,int pid, string nombre); //string?
		int eliminarArchivo ( string archivo,int pid, string nombre);
		vector<Usuario> buscarArchivos(); //ojo copia
		void cerrar ();

};

#endif /* GESTOR_USUARIOS_H_ */
