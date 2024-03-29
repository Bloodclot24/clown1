#ifndef GESTORDESCARGAS_H_
#define GESTORDESCARGAS_H_

#include <stdlib.h>
#include <list>
#include <sys/wait.h>

#include "Fifo.h"
#include "Usuario.h"
#include "Debug.h"
#include "SIGINT_Handler.h"
#include "SignalHandler.h"

#define BUFFSIZE		100

using namespace std;

class GestorDescargas {
private:
	int enviar(int pidUsuario, int pidDestino, char* archivo);

	void enviarRuta(int pidOrigen, int pidDestino, string path);

public:
	GestorDescargas();
	virtual ~GestorDescargas();

	int iniciarRecepcion();

	int descargar(string path, Usuario usuarioOrigen, Usuario usuarioDestino);

	void esperarFinalizacionDescargas(list<int> hijos);

};

#endif /* GESTORDESCARGAS_H_ */
