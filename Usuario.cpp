#include "Usuario.h"

Usuario::Usuario() {
}

Usuario::~Usuario() {
}

Usuario::Usuario(string nombre, int pid) {

	this->nombre = nombre;
	this->pid = pid;

}

Usuario::Usuario(const Usuario& usuario) {
	nombre = usuario.nombre;
	pid = usuario.pid;
	archivos = usuario.archivos;
}

vector<string> Usuario::getArchivos() {
	return archivos; //copia
}

int Usuario::getPid() {
	return pid;
}

void Usuario::agregarArchivo(string archivo) {
	archivos.insert(archivos.end(), archivo);
}

void Usuario::eliminarArchivo(string archivo) {
	vector<string> archivosDeUsuario;
	vector<string>::iterator itArch;
	for (itArch = archivos.begin(); itArch != archivos.end(); itArch++) {
		if (*itArch != archivo)
			archivosDeUsuario.insert(archivosDeUsuario.end(), archivo);
	}
	archivos = archivosDeUsuario;
}

string Usuario::getNombre() {
	return nombre;
}

bool Usuario::operator==(const Usuario & otro) const {
	return (pid == otro.pid && nombre == otro.nombre);
}

bool Usuario::operator!=(const Usuario & otro) const {
	return !(pid == otro.pid && nombre == otro.nombre);
}

ostream & operator<<(ostream & salida, const Usuario & usuario) {
	salida << "Usuario: " << usuario.nombre << " pid: " << usuario.pid << endl;
	for (int i = 0; i < usuario.archivos.size(); i++)
		salida << "\t" << i << " - " << usuario.archivos[i] << endl;
	return salida;
}

