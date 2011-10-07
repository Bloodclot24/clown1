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
#include "Debug.h"
#include "SIGINT_Handler.h"
#include "SignalHandler.h"

#define BUFFSIZE		100

using namespace std;

class GestorDescargas {
private:
	int enviar(int pidUsuario, int pidDestino, char* buffer);
	void enviarRuta(int pidOrigen, int pidDestino, string path);
public:
	GestorDescargas();
	virtual ~GestorDescargas();

	int iniciarRecepcion();
	int descargar(string path, Usuario usuarioOrigen, Usuario usuarioDestino);

};

#endif /* GESTORDESCARGAS_H_ */
