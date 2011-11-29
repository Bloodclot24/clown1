#include "BloqueDeRegistros.h"

BloqueDeRegistros::BloqueDeRegistros() /*: lock("baseDatos")*/ {
	this->cantidadDeRegistros = 0;
}


bool BloqueDeRegistros::agregarRegistro(Registro registro) {
	bool agregado = false;
	bool existe = buscarRegistro(registro);
	if (!existe && cantidadDeRegistros < MAX_REG_MEM) {
		registros[cantidadDeRegistros] = registro;
		cantidadDeRegistros++;
		agregado = true;
	}
	return agregado;
}


bool BloqueDeRegistros::eliminarRegistro(Registro registro) {
	bool eliminado = false;
	for(int i = 0; i < cantidadDeRegistros && !eliminado; i++){
		if(strcmp(registros[i].nombre,registro.nombre) == 0){
			acomodarBloque(i);
			eliminado = true;
			cantidadDeRegistros--;
		}
	}
	return eliminado;
}


bool BloqueDeRegistros::modificarRegistro(Registro registro) {
	bool modificado = false;
	for(int i = 0; i < cantidadDeRegistros && !modificado; i++){
		if (strcmp(registros[i].nombre, registro.nombre) == 0) {
			strcpy(registros[i].direccion, registro.direccion);
			strcpy(registros[i].telefono, registro.telefono);
			modificado = true;
		}
	}
	return modificado;
}


bool BloqueDeRegistros::consultarRegistro(Registro& registro) {
	bool encontrado = false;
	for (int i = 0; i < cantidadDeRegistros && !encontrado; i++) {
		if (strcmp(registros[i].nombre, registro.nombre) == 0) {
			strcpy(registro.direccion, registros[i].direccion);
			strcpy(registro.telefono, registros[i].telefono);
			encontrado = true;
		}
	}
	return encontrado;
}


void BloqueDeRegistros::acomodarBloque(int posicion) {
	for(int i = posicion; i < cantidadDeRegistros; i++)
		registros[i] = registros[i+1];
}


bool BloqueDeRegistros::buscarRegistro(Registro registro) {
	bool encontrado = false;
	for (int i = 0; i < cantidadDeRegistros && !encontrado; i++) {
		if (strcmp(registros[i].nombre, registro.nombre) == 0)
			encontrado = true;
	}
	return encontrado;
}


void BloqueDeRegistros::persistir() {
	LockFile lock;
	lock.tomarLock();
	for(int i = 0; i < cantidadDeRegistros; i++){
		lock.escribir(registros[i].nombre, 61 * sizeof(char));
		lock.escribir(registros[i].direccion, 120 * sizeof(char));
		lock.escribir(registros[i].telefono, 13 * sizeof(char));
	}
	lock.liberarLock();
}


BloqueDeRegistros BloqueDeRegistros::recuperar() {
	LockFile lock;
	lock.tomarLock();
	int i = 0;
	while(lock.leer(registros[i].nombre, 61 * sizeof(char)) != 0 && i < MAX_REG_MEM) {
		lock.leer( registros[i].direccion, 120 * sizeof(char) );
		lock.leer(registros[i].telefono, 13 * sizeof(char));
		i++;
		cantidadDeRegistros++;
	}
	lock.liberarLock();
	return (*this);
}


BloqueDeRegistros::~BloqueDeRegistros() {
	LockFile lock;
	lock.cerrar();
//	lock.eliminar();
}

