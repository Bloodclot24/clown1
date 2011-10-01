#include "GestorUsuarios.h"
#include <iostream>
#include <stdlib.h>

GestorUsuarios :: GestorUsuarios ()
{

}

GestorUsuarios :: ~GestorUsuarios () {
}
/*
int GestorUsuarios :: agregarUsuario (char* nombre, int pid) {

	lockFile.tomarLock ();
	int resultado = lockFile.escribir ((char*)nombre, strlen(nombre));
	resultado += lockFile.escribir ((char*)&pid, 2); //ojo con esto!!!!
	lockFile.liberarLock ();
	
	return resultado;
}
*/
int GestorUsuarios :: eliminarUsuario ( char* nombre,int pid ) {

	return 0;
}

int GestorUsuarios :: agregarArchivo (string archivo,int pid, string nombre)
{
	return archivoUsuarios.escribir(archivo,pid,nombre);
}

int GestorUsuarios :: eliminarArchivo ( string archivo,int pid, string nombre) {

	return 0;
}

vector<Usuario> GestorUsuarios::buscarArchivos() {
	actualizarUsuarios();
	return usuarios;
}

void GestorUsuarios :: cerrar () {
	archivoUsuarios.cerrar();
}

void GestorUsuarios :: actualizarUsuarios ()
{
	string nombre;
	int pid;
	string archivo;
	archivoUsuarios.reset();
	while ( archivoUsuarios.leer(archivo,pid,nombre) != 0) { //Si la lectura devuelve 0 bytes es porque estoy en eof
		//cout << "Voy a leer en actualizar usuarios." << endl; // para debug
		vector<Usuario>::iterator it;
		bool encontrado = false;
		for (it = usuarios.begin(); it != usuarios.end() && !encontrado; it++) {
			cout << "Pid del it: " << (*it).getPid() << " Pid: " << pid << endl;
			if ((*it).getPid() == pid) {
				cout << "Lo encontreeeeeeeeeeeeeeeee"<< endl;
				//TODO ver si es necesario ver si no estaba
				(*it).agregarArchivo(archivo);
				encontrado = true;
			}
		}
		if(!encontrado) {
			Usuario usuario(nombre, pid);
			usuario.agregarArchivo(archivo);
			usuarios.insert(usuarios.end(), usuario); //verrrr por memoria TODO Constructor copia
		}
	}


}
