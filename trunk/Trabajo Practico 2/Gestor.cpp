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
			strcpy(respuesta.respuesta, "Ya existe en la base de datos la persona");
		else
			strcpy(respuesta.respuesta, "Se agrego a la base de datos la persona");
		break;
	case ELIMINAR:
		if (!baseDatos.eliminarPersona(peticion.registro))
			strcpy(respuesta.respuesta, "No existe en la base de datos la persona");
		else
			strcpy(respuesta.respuesta, "Se elimino de la base de datos la persona");
		break;
	case CONSULTAR:
		if (!baseDatos.consultarPersona(peticion.registro))
			strcpy(respuesta.registro.nombre, "");
		else
			respuesta.registro = peticion.registro;
		break;
	case MODIFICAR:
		if (!baseDatos.modificarPersona(peticion.registro))
			strcpy(respuesta.respuesta, "No existe en la base de datos la persona");
		else
			strcpy(respuesta.respuesta, "Se modifico en la base de datos la persona");
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
		cout << "Respuesta " << respuesta.respuesta << endl;
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
