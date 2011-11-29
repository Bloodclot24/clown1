#ifndef GESTOR_H_
#define GESTOR_H_

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "Mensaje.h"
#include "Cola.h"
#include "SIGINT_Handler.h"
#include "SignalHandler.h"
#include "BaseDatos.h"

using namespace std;

class Gestor {
private:
	BaseDatos baseDatos;
	Cola<mensaje> cola;
	void ejecutarPeticion();
	void procesarPeticion();
	mensaje peticion;
	mensaje respuesta;

public:
	Gestor();
	virtual ~Gestor();
	void inciar();
};

#endif /* GESTOR_H_ */
