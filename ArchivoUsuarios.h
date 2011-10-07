#ifndef ARCHIVOUSUARIOS_H_
#define ARCHIVOUSUARIOS_H_

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "LockFile.h"

#define ARCHIVO_USUARIOS	"/tmp/archivo_usuarios"
#define	ARCHIVO_LOCK_USUARIOS	"archivo_usuarios"

using namespace std;

class ArchivoUsuarios {
private:
	LockFile lock;
	fstream archivo;

	int parsearLinea(string linea, string& nombre, int& pid, string& archivo);

public:
	ArchivoUsuarios();
	virtual ~ArchivoUsuarios();

	int escribir(string ruta, int pid, string nombre);
	int leer(string& ruta, int& pid, string& nombre);

	string intToString(int entero) { //TODO clase utilitaria

		stringstream out;
		out << entero;
		return out.str();
	}
	void cerrar() {
		archivo.close();
		lock.cerrar();
	}

	void reset() {
		archivo.seekg(0, ios::beg);
		lock.cerrar();
		lock.abrir();
	}
};

#endif /* ARCHIVOUSUARIOS_H_ */
