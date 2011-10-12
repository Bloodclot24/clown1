#ifndef CONCUSHARE_H_
#define CONCUSHARE_H_

#include <getopt.h>

#include "GestorUsuarios.h"
#include "GestorDescargas.h"
#include "Debug.h"
#include "Vista.h"

#define HIJO		0
#define PADRE		1
#define SALIR		-1

using namespace std;

class ConcuShare {
private:
	GestorDescargas gestorDescargas;
	GestorUsuarios gestorUsuarios;
	Usuario usuario;
	list<int> hijos;

	int compartirArchivos();

	int descargarArchivo(Usuario usuarioOrigen);

	int buscarArchivos();

	int ejecutarMenu();

	void crearDirectorioDescargas(string nombre);

	int parsearLineaDeComandos(int argc, char** argv);

public:
	ConcuShare();
	virtual ~ConcuShare();

	int iniciar(int argc, char** argv);
};

#endif /* CONCUSHARE_H_ */
