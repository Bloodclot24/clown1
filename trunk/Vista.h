#ifndef VISTA_H_
#define VISTA_H_

#include <iostream>
#include <stdlib.h>

#include "Usuario.h"

using namespace std;

class Vista {
public:
	Vista();
	virtual ~Vista();

	static void mostrarBienvenida();

	static void mostrarMenu();

	static void mostrarMenuCompartir();

	static void mostrarArchivos(vector<Usuario>& usuarios, Usuario usuario);

	static void mostrarUsuario(Usuario usuario);

	static void mostrarUso(char* nombre);

	static string pedirString();

	static char pedirChar();

	static int pedirInt(int inferior, int superior);

	static void mostrarMensaje(string mensaje);

	static void mostrarMensajeInicial(string mensaje);

	static void mostrarMensajeFinal(string mensaje);

	static void debug(string mensaje, int entero);

	static void debug(string mensaje);

};

#endif /* VISTA_H_ */
