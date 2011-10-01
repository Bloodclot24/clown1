/*
 * GestorDescargas.h
 *
 *  Created on: Sep 30, 2011
 *      Author: guido
 */

#ifndef GESTORDESCARGAS_H_
#define GESTORDESCARGAS_H_

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Fifo.h"
#include "Usuario.h"
#include "LockFile.h"

#define BUFFSIZE		100

using namespace std;

class GestorDescargas {
private:
	int enviar(int pidUsuario, int pidDestino, char* buffer);
	string intToString(int entero);
public:
	GestorDescargas();
	virtual ~GestorDescargas();

	int iniciarRecepcion();
	int descargar(string path,int pidEnvia);

};

#endif /* GESTORDESCARGAS_H_ */
