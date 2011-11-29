#include "Gestor.h"

Gestor::Gestor() : cola(ARCH_COLA, LETRA_COLA) {
}

Gestor::~Gestor() {
	cola.eliminar();
}

void Gestor::ejecutarPeticion() {

	switch (peticion.tipo) {
	case AGREGAR:
		if (!baseDatos.agregarPersona(peticion.registro))
			strcpy(respuesta.respuesta, "La persona ya existe");
		else
			strcpy(respuesta.respuesta, "Se agrego la persona a la base de datos");
		break;
	case ELIMINAR:
		if (!baseDatos.eliminarPersona(peticion.registro))
			strcpy(respuesta.respuesta, "La persona no existe");
		else
			strcpy(respuesta.respuesta, "Se elimino la persona en la base de datos");
		break;
	case CONSULTAR:
		if (!baseDatos.consultarPersona(peticion.registro))
			strcpy(respuesta.registro.nombre, "");
		else {
			strcpy(respuesta.registro.nombre, peticion.registro.nombre);//usar =
			strcpy(respuesta.registro.direccion, peticion.registro.direccion);
			strcpy(respuesta.registro.telefono, peticion.registro.telefono);
		}
		break;
	case MODIFICAR:
		if (!baseDatos.modificarPersona(peticion.registro))
			strcpy(respuesta.respuesta, "La persona no existe");
		else
			strcpy(respuesta.respuesta, "Se modifico la persona en la base de datos\0");
		break;
	default:
		;
	}
}

void Gestor::procesarPeticion() {
	if(cola.leer(PETICION, &peticion) > 0) {
		cout << "Peticion " << peticion.id << " mtype " << peticion.mtype << endl;
		cout << "Persona: " << peticion.registro.nombre << endl;
		cout << "Direccion: " << peticion.registro.direccion << endl;
		cout << "Telefono: " << peticion.registro.telefono << endl;
		ejecutarPeticion();
		respuesta.mtype = peticion.id;
		cout << "Respuesta " << respuesta.id << " mtype " << respuesta.mtype << endl;
		cout << "Persona: " << respuesta.registro.nombre << endl;
		cout << "Direccion: " << respuesta.registro.direccion << endl;
		cout << "Telefono: " << respuesta.registro.telefono << endl;
		cola.escribir(respuesta);
	}
}

void Gestor::inciar() {
	SIGINT_Handler sigint_handler;
	SignalHandler :: getInstance()->registrarHandler ( SIGTERM,&sigint_handler );
	while ( sigint_handler.getGracefulQuit() == 0 )
		procesarPeticion();
}
