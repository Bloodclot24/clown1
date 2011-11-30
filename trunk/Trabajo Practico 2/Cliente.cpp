#include "Cliente.h"

Cliente::Cliente() : cola(ARCH_COLA, LETRA_COLA) {
}

Cliente::~Cliente() {
}

mensaje Cliente::enviarPeticion(mensaje peticion) {
	peticion.mtype = PETICION;
	peticion.id = getpid();
	cola.escribir(peticion);

	mensaje respuesta;
	cola.leer(getpid(), &respuesta);
	return respuesta;
}

string Cliente::agregarPersona(string nombre, string direccion, string telefono) {
	mensaje peticion;
	peticion.tipo = AGREGAR;
	strcpy(peticion.registro.nombre, nombre.c_str());
	strcpy(peticion.registro.direccion, direccion.c_str());
	strcpy(peticion.registro.telefono, telefono.c_str());
	return string(enviarPeticion(peticion).respuesta);
}

string Cliente::modificarPersona(string nombre, string direccion, string telefono) {
	mensaje peticion;
	peticion.tipo = MODIFICAR;
	strcpy(peticion.registro.nombre, nombre.c_str());
	strcpy(peticion.registro.direccion, direccion.c_str());
	strcpy(peticion.registro.telefono, telefono.c_str());
	return string(enviarPeticion(peticion).respuesta);
}

string Cliente::eliminarPersona(string nombre) {
	mensaje peticion;
	peticion.tipo = ELIMINAR;
	strcpy(peticion.registro.nombre, nombre.c_str());
	return string(enviarPeticion(peticion).respuesta);
}

Registro Cliente::consultarPersona(string nombre) {
	mensaje peticion;
	peticion.tipo = CONSULTAR;
	strcpy(peticion.registro.nombre, nombre.c_str());
	mensaje respuesta = enviarPeticion(peticion);
	return respuesta.registro;
}
