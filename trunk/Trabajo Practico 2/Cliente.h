#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <iostream>
#include <string.h>
#include "Persona.h"
#include "Mensaje.h"
#include "Cola.h"

using namespace std;

class Cliente {
private:
	Cola<mensaje> cola;

	mensaje enviarPeticion(mensaje peticion);

public:
	Cliente();
	virtual ~Cliente();

	string agregarPersona(string nombre, string direccion, string telefono);

	string modificarPersona(string nombre, string direccion, string telefono);

	string eliminarPersona(string nombre);

	Persona consultarPersona(string nombre);
};

#endif /* CLIENTE_H_ */
