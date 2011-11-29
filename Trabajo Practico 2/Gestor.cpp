#include "Gestor.h"

Gestor::Gestor(const string & archivo) : cola(ARCH_COLA, LETRA_COLA) {
}

Gestor::~Gestor() {
}

void Gestor::ejecutarPeticion() {
	cout << "ejecutaar gestor" << endl;
	switch (peticion.tipo) {
	case AGREGAR:
		if (!baseDatos.agregarPersona(peticion.registro))
			strcpy(respuesta.respuesta, "La persona ya existe");
		else
			strcpy(respuesta.respuesta, "Se agrego la persona a la base de datos");
		break;
	case ELIMINAR:
		if (!baseDatos.eliminarPersona(string(peticion.registro.nombre)))
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
		if (!baseDatos.modificarPersona(string(peticion.registro.nombre), peticion.registro))
			strcpy(respuesta.respuesta, "La persona no existe");
		else
			strcpy(respuesta.respuesta, "Se modifico la persona en la base de datos\0");
		break;
	default:
		;
	}
}

void Gestor::procesarPeticion() {
	cola.leer(PETICION, &peticion);
	ejecutarPeticion();
	respuesta.mtype = peticion.id;
	cola.escribir(respuesta);
}

void Gestor::inciar() {
	SIGINT_Handler sigint_handler;
	SignalHandler :: getInstance()->registrarHandler ( SIGTERM,&sigint_handler );
	while ( sigint_handler.getGracefulQuit() == 0 ) {
		cout << "while" << endl;
		procesarPeticion();
	}

}
