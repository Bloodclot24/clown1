#ifndef GESTOR_USUARIOS_H_
#define GESTOR_USUARIOS_H_

#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>

#include "Usuario.h"
#include "LockFile.h"
#include "Debug.h"

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
	int escribir(string ruta, int pid, string nombre);
	int leer(string& ruta, int& pid, string& nombre);
	int parsearLinea(string linea, string& nombre, int& pid, string& archivo);
	void reset();

public:
	GestorUsuarios();
	virtual ~GestorUsuarios();

	int eliminarUsuario(Usuario usuario);
	int agregarArchivo(string archivo, Usuario& usuario);
	int eliminarArchivo(string archivo, Usuario& usuario);
	vector<Usuario> buscarArchivos(); //ojo copia
	void cerrar();
};

#endif /* GESTOR_USUARIOS_H_ */
