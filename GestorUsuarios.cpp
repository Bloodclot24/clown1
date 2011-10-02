#include "GestorUsuarios.h"
#include <iostream>
#include <stdlib.h>

GestorUsuarios :: GestorUsuarios (): lock((char*)ARCHIVO_LOCK_USUARIOS)
{

}

GestorUsuarios :: ~GestorUsuarios () {
}

int GestorUsuarios::eliminarUsuario(char* nombre, int pid)
{
	lock.tomarLock();
	actualizarUsuarios();
	//TODO pisar el archivo de usuarios
	lock.reset();
	vector<Usuario>::iterator it;
	for (it = usuarios.begin(); it != usuarios.end(); it++) {
		if ((*it).getPid() != pid || (*it).getNombre() != nombre) {//sobrecargar operador =
			vector<string> archivosDeUsuario = (*it).getArchivos();
			vector<string>::iterator itArch;
			for (itArch = archivosDeUsuario.begin(); itArch != archivosDeUsuario.end(); itArch++) {
					escribir(*itArch, (*it).getPid(), (*it).getNombre());
			}
		}
	}
	lock.liberarLock();
	return 0; // TODO ver return
}

int GestorUsuarios :: agregarArchivo (string archivo,int pid, string nombre)
{
	lock.tomarLock();
	int resultado = escribir(archivo,pid,nombre);
	lock.liberarLock();
	return resultado;
}

int GestorUsuarios :: eliminarArchivo ( string archivo,int pid, string nombre)
{
	lock.tomarLock();
	actualizarUsuarios();
	//TODO pisar el archivo de usuarios
	lock.reset();
	vector<Usuario>::iterator it;
	for (it = usuarios.begin(); it != usuarios.end(); it++) {
		vector<string> archivosDeUsuario = (*it).getArchivos();
		vector<string>::iterator itArch;
		cout << (*it).getPid() << (*it).getNombre() << endl;
		for (itArch = archivosDeUsuario.begin(); itArch != archivosDeUsuario.end(); itArch++) {
			if (!((*itArch).compare(archivo.c_str()) == 0 && (*it).getPid() == pid && (*it).getNombre() == nombre)) {
				escribir((*itArch), (*it).getPid(), (*it).getNombre());
			}
		}
	}
	lock.liberarLock();
	return 0; // TODO ver return
}

vector<Usuario> GestorUsuarios::buscarArchivos()
{
	lock.tomarLock();
	actualizarUsuarios();
	lock.liberarLock();
	return usuarios;
}

void GestorUsuarios :: cerrar ()
{
	lock.cerrar();
}

void GestorUsuarios :: actualizarUsuarios ()
{
	string nombre;
	int pid;
	string archivo;
	reset();
	usuarios.erase(usuarios.begin(), usuarios.end());
	while ( leer(archivo,pid,nombre) != 0) { //Si la lectura devuelve 0 bytes es porque estoy en eof
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

int GestorUsuarios::escribir (string ruta,int pid, string nombre)
{
	string linea = nombre + "," + intToString(pid) + "," + ruta + "\n";
//	lock.tomarLock ();
	lock.escribir((char*)linea.c_str(), linea.length());
//	lock.liberarLock ();
	return 0;
}

int GestorUsuarios::leer (string& ruta,int& pid, string& nombre)
{
	char linea[512];
	//lock.tomarLock ();
	int leidos = lock.leer(linea,512);//falta busqueda y control de pid
	//lock.liberarLock ();
	string linea1(linea);
	linea1.assign(linea1,0,leidos);
	parsearLinea(linea1,nombre,pid,ruta);
	return leidos;

}

int GestorUsuarios::parsearLinea(string linea,string& nombre,int& pid,string& archivo)
{
	string cadenaAux;
	string:: size_type coma2 = linea.find(",", 0);
	cadenaAux.assign(linea, 0, coma2);
	nombre = cadenaAux;
	cout << "Nombre Parseado: " << nombre << endl; //para debug
	string:: size_type coma3 = linea.find(",", coma2 + 1);
	cadenaAux.assign(linea, coma2 + 1, coma3 - coma2 -1);
	pid = atoi(cadenaAux.c_str());
	cout << "PID Parseado: " << pid << endl; //para debug
	//archivo.assign(linea,coma3 + 1,linea.length());
	archivo.assign(linea,coma3 + 1,linea.length());
	cout << "Archivo Parseado: " << archivo << endl; //para debug
	return 0;
}

void GestorUsuarios::reset()
{
	lock.cerrar();
	lock.abrir();
}
