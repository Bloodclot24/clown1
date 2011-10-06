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
	//TODO pisar el archivo de usuarios
	lock.reset();
	vector<Usuario>::iterator it;
	for (it = usuarios.begin(); it != usuarios.end(); it++) {
		if ((*it).getPid() != usuario.getPid() || (*it).getNombre() != usuario.getNombre()) {//sobrecargar operador =
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

int GestorUsuarios :: agregarArchivo (string archivo, Usuario usuario)
{
	lock.tomarLock();
	int resultado = escribir(archivo, usuario.getPid(), usuario.getNombre());
	lock.liberarLock();
	return resultado;
}

int GestorUsuarios :: eliminarArchivo ( string archivo, Usuario usuario)
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
			if (!((*itArch).compare(archivo.c_str()) == 0 && (*it).getPid() == usuario.getPid() && (*it).getNombre() == usuario.getNombre())) {
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
			usuarios.insert(usuarios.end(), usuario); //verrrr por memoria TODO Constructor copia
		}
		Debug::getInstance()->escribir( "Actualizando usuario " + nombre + " pid " + Debug::intToString(pid) + " archivo " + archivo + "\n");
	}
}

int GestorUsuarios::escribir (string ruta,int pid, string nombre)
{
	string linea = nombre + "," + intToString(pid) + "," + ruta + "\n";
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
	string:: size_type coma2 = linea.find(",", 0);
	cadenaAux.assign(linea, 0, coma2);
	nombre = cadenaAux;
	string:: size_type coma3 = linea.find(",", coma2 + 1);
	cadenaAux.assign(linea, coma2 + 1, coma3 - coma2 -1);
	pid = atoi(cadenaAux.c_str());
	archivo.assign(linea,coma3 + 1,linea.length());
	return 0;
}

void GestorUsuarios::reset()
{
	lock.cerrar();
	lock.abrir();
}
