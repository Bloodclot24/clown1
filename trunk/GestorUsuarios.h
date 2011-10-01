#ifndef GESTOR_USUARIOS_H_
#define GESTOR_USUARIOS_H_

#include <unistd.h>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>

#include "Usuario.h"
#include "LockFile.h"

#define	ARCHIVO_USUARIOS	"/tmp/archivo_usuarios"

using namespace std;

class GestorUsuarios {

	private:
		vector<Usuario> usuarios; //esto es una lista, hace falta o leo todas las veces?
		LockFile lockFile;
		
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
