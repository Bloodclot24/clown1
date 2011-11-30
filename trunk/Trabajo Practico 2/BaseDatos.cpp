#include "BaseDatos.h"

BaseDatos::BaseDatos(): semaforo("BaseDatos.cpp", 1) {

	//lock.tomarLock();
	recuperar();
	LockFile lock;
	//lock.liberarLock();
	lock.eliminar();
	cout << "Fin constructor" << endl;
}

BaseDatos::~BaseDatos() {
	//LockFile lock;
	//lock.tomarLock();
	persistir();
	//lock.liberarLock();
	semaforo.eliminar();
}

bool BaseDatos::consultarPersona(Registro& persona) {
	semaforo.p();
	bool encontrado = buscarPersona(persona);
	semaforo.v();
	return encontrado;
}

bool BaseDatos::buscarPersona(Registro& persona) {
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	bool encontrado = false;
	for(it = memoria.begin(); it != memoria.end() && !encontrado; it++) {
		registros = (*it).leer();
		if(registros.consultarRegistro(persona))
			encontrado = true;
	}
	return encontrado;
}

bool BaseDatos::agregarPersona(Registro persona) {
	semaforo.p();
	bool resultado;
	if(buscarPersona(persona))
		resultado = false;
	else {
		agregarRegistro(persona);
		resultado = true;
	}
	semaforo.v();
	return resultado;
}

void BaseDatos::agregarRegistro(Registro persona) {
	registros = memoria[memoria.size() - 1].leer();
	if(!registros.agregarRegistro(persona)) {
		agregarMemoriaCompartida();
		registros.agregarRegistro(persona);
	}
	memoria[memoria.size() - 1].escribir(registros);
}

bool BaseDatos::modificarPersona(Registro persona) {
	semaforo.p();
	bool modificado = false;
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	for(it = memoria.begin(); it != memoria.end() && !modificado; it++) {
		registros = (*it).leer();
		if(registros.modificarRegistro(persona)) {
			(*it).escribir(registros);
			modificado = true;
		}
	}
	semaforo.v();
	return modificado;
}

bool BaseDatos::eliminarPersona(Registro persona) {
	semaforo.p();
	bool eliminado = false;
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	for(it = memoria.begin(); it != memoria.end() && !eliminado; it++) {
		registros = (*it).leer();
		if(registros.eliminarRegistro(persona)) {
			(*it).escribir(registros);
			eliminado = true;
		}
	}
	semaforo.v();
	return eliminado;
}

void BaseDatos::persistir() {
	semaforo.p();
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	for(it = memoria.begin(); it != memoria.end(); it++) {
		registros = (*it).leer();
		registros.persistir();
		(*it).liberar();
	}
	semaforo.v();
}

void BaseDatos::recuperar() {
	int cantidadRegistrosCargados = MAX_REG_MEM;
	while(cantidadRegistrosCargados == MAX_REG_MEM) {
		agregarMemoriaCompartida();
		registros.recuperar();
		semaforo.p();
		memoria[memoria.size() - 1].escribir(registros);
		semaforo.v();
		cantidadRegistrosCargados = registros.getCantidadDeRegistros();
		cout << cantidadRegistrosCargados << endl;
	}
}

void BaseDatos::agregarMemoriaCompartida() {
	MemoriaCompartida<BloqueDeRegistros> memoriaCompartida;
	cout << "Resultado de mem: " << memoriaCompartida.crear((char *) "BaseDatos.cpp", memoria.size()) << endl;
	registros = memoriaCompartida.leer();
	registros.setNumeroBloque(memoria.size());
	memoria.insert(memoria.end(), memoriaCompartida);
}
