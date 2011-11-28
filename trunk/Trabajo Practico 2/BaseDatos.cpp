#include "BaseDatos.h"

BaseDatos::BaseDatos(): memoria(), semaforo("BaseDatos.cpp", 1), registros(){
	std::cout<< "Resultado de crear memoria "<< memoria.crear((char *) "BaseDatos.cpp",'g') << std::endl;
	//registros = NULL;
	recuperar();
	LockFile lock;
	lock.eliminar();
	//semaforo.p();
	//memoria.escribir(registros);
	//semaforo.v();
}


Registro BaseDatos::consultarPersona(Registro persona) {
	semaforo.p();
	registros = memoria.leer();
	semaforo.v();
	if(!registros.consultarRegistro(persona))
		std::cerr << "Error, la persona consultada no existe." << std::endl;
	return persona;
}


void BaseDatos::agregarPersona(Registro persona) {
	semaforo.p();
	registros = memoria.leer();
	if(!registros.agregarRegistro(persona))
		std::cerr << "Error, la persona que intenta agregar ya existe o no hay mas espacio disponible" << std::endl;
	else
		memoria.escribir(registros);
	semaforo.v();
}


void BaseDatos::modificarPersona(std::string nombre, Registro persona) {
	semaforo.p();
	registros = memoria.leer();
	if(!registros.modificarRegistro(persona))
		std::cerr << "Error, la persona que intenta modificar no existe." << std::endl;
	else
		memoria.escribir(registros);
	semaforo.v();
}


void BaseDatos::eliminarPersona(std::string nombre) {
	semaforo.p();
	registros = memoria.leer();
	Registro persona;
	strcpy(persona.nombre, nombre.c_str());
	if(!registros.eliminarRegistro(persona))
		std::cerr << "Error, la persona que intenta eliminar no existe." << std::endl;
	else
		memoria.escribir(registros);
	semaforo.v();
}


void BaseDatos::persistir() {
	semaforo.p();
	registros = memoria.leer();
	registros.persistir();
	semaforo.v();
}


void BaseDatos::recuperar() {
	registros = registros.recuperar();
	semaforo.p();
	memoria.escribir(registros);
	semaforo.v();

}


BaseDatos::~BaseDatos() {
	persistir();
	memoria.liberar();
	semaforo.eliminar();
}

