#include "Debug.h"

Debug* Debug::instance = NULL;
LockFile Debug::lock("debug");

Debug::Debug() {
}

Debug* Debug::getInstance() {

	if (instance == NULL)
		instance = new Debug();

	return instance;
}

void Debug::destruir() {
	if (instance != NULL) {
		delete (instance);
		instance = NULL;
	}
	lock.cerrar(); //TODO eliminar!!
}

void Debug::escribir(string mensaje) {
	const struct tm* tm_ptr;
	time_t hora;
	string msj;

	hora = time(0);
	tm_ptr = localtime(&hora);

	msj = "<Dia: " + intToString(1 + tm_ptr->tm_mon) + "/" + intToString(tm_ptr->tm_mday) + "/" + intToString(1900 + tm_ptr->tm_year) + "; Hora: " + intToString(tm_ptr->tm_hour) +":" + intToString(tm_ptr->tm_min)  +":" + intToString(tm_ptr->tm_sec)  + "> ";
	msj = msj + mensaje;
	lock.tomarLock();
	lock.escribir((char*) msj.c_str(), msj.length());
	lock.liberarLock();
}

string Debug::intToString(int entero) {
	stringstream out;
	out << entero;
	return out.str();
}
