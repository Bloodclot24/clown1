#include "GestorUsuarios.h"
#include <iostream>
#include <stdlib.h>

GestorUsuarios :: GestorUsuarios () : lockFile(ARCHIVO_USUARIOS) {

}

GestorUsuarios :: ~GestorUsuarios () {
}

int GestorUsuarios :: agregarUsuario (char* nombre, int pid) {

	lockFile.tomarLock ();
	int resultado = lockFile.escribir ((char*)nombre, strlen(nombre));
	resultado += lockFile.escribir ((char*)&pid, 2); //ojo con esto!!!!
	lockFile.liberarLock ();
	
	return resultado;
}

int GestorUsuarios :: eliminarUsuario ( char* nombre,int pid ) {

	return 0;
}

int GestorUsuarios :: agregarArchivo (string archivo,int pid ) {

	lockFile.tomarLock ();
	int resultado = lockFile.escribir ((char*)archivo.c_str(), archivo.length());//falta busqueda y control de pid
	lockFile.liberarLock ();
	
	return resultado;
}

int GestorUsuarios :: eliminarArchivo ( string archivo,int pid ) {

	return 0;
}

vector<Usuario> GestorUsuarios::buscarArchivos() {
	actualizarUsuarios();
	return usuarios;
}

void GestorUsuarios :: cerrar () {
	lockFile.cerrar();
}

void GestorUsuarios :: actualizarUsuarios () {
}
