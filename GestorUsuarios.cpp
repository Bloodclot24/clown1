#include "GestorUsuarios.h"

GestorUsuarios :: GestorUsuarios (): lock(ARCHIVO_LOCK_USUARIOS)
{
}

GestorUsuarios :: ~GestorUsuarios ()
{
}

int GestorUsuarios::eliminarUsuario(Usuario usuario)
{
	lock.tomarLock();
	actualizarUsuarios();
	lock.eliminar();
	lock.abrir();
	vector<Usuario>::iterator it;
	for (it = usuarios.begin(); it != usuarios.end(); it++) {
		if ((*it) != usuario) {
			vector<string> archivosDeUsuario = (*it).getArchivos();
			vector<string>::iterator itArch;
			for (itArch = archivosDeUsuario.begin(); itArch != archivosDeUsuario.end(); itArch++)
					escribir(*itArch, (*it).getPid(), (*it).getNombre());
		}
	}
	lock.liberarLock();
	return 0; // TODO ver return
}

int GestorUsuarios :: agregarArchivo (string archivo, Usuario& usuario)
{
	lock.tomarLock();
	int resultado = escribir(archivo, usuario.getPid(), usuario.getNombre());
	lock.liberarLock();
	usuario.agregarArchivo(archivo);
	return resultado;
}

int GestorUsuarios :: eliminarArchivo ( string archivo, Usuario& usuario)
{
	lock.tomarLock();
	actualizarUsuarios();
	lock.eliminar();
	lock.abrir();
	vector<Usuario>::iterator it;
	for (it = usuarios.begin(); it != usuarios.end(); it++) {
		vector<string> archivosDeUsuario = (*it).getArchivos();
		vector<string>::iterator itArch;
		for (itArch = archivosDeUsuario.begin(); itArch != archivosDeUsuario.end(); itArch++) {
			if (!(*itArch == archivo && *it == usuario))
				escribir(*itArch, (*it).getPid(), (*it).getNombre());
		}
	}
	lock.liberarLock();
	usuario.eliminarArchivo(archivo);
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
	lock.reset();
	usuarios.erase(usuarios.begin(), usuarios.end());
	while ( leer(archivo,pid,nombre) != 0) { //Si la lectura devuelve 0 bytes corresponde al eof
		vector<Usuario>::iterator it;
		bool encontrado = false;
		for (it = usuarios.begin(); it != usuarios.end() && !encontrado; it++) {
			if ((*it).getPid() == pid) {
				(*it).agregarArchivo(archivo);
				encontrado = true;
			}
		}
		if(!encontrado) {
			Usuario usuario(nombre, pid);
			usuario.agregarArchivo(archivo);
			usuarios.insert(usuarios.end(), usuario);
		}
		Debug::getInstance()->escribir( "Actualizando usuario " + nombre + " pid " + Debug::intToString(pid) + " archivo " + archivo + "\n");
	}
}

int GestorUsuarios::escribir (string ruta,int pid, string nombre)
{
	string linea = nombre + "," + Debug::intToString(pid) + "," + ruta + "\n";
	lock.escribir((char*)linea.c_str(), linea.length());
	return 0;
}

int GestorUsuarios::leer (string& ruta,int& pid, string& nombre)
{
	char linea[512];
	int leidos = lock.leer(linea,512);//falta busqueda y control de pid
	string linea1(linea);
	linea1.assign(linea1,0,leidos);
	parsearLinea(linea1,nombre,pid,ruta);
	return leidos;

}

int GestorUsuarios::parsearLinea(string linea,string& nombre,int& pid,string& archivo)
{
	string cadenaAux;
	string:: size_type posComa1 = linea.find(",", 0);
	cadenaAux.assign(linea, 0, posComa1);
	nombre = cadenaAux;
	string:: size_type posComa2 = linea.find(",", posComa1 + 1);
	cadenaAux.assign(linea, posComa1 + 1, posComa2 - posComa1 -1);
	pid = atoi(cadenaAux.c_str());
	archivo.assign(linea,posComa2 + 1,linea.length());
	return 0;
}
