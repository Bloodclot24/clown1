#include "BaseDatos.h"

BaseDatos::BaseDatos(): semaforo("BaseDatos.cpp", 1) {
	recuperar();
	LockFile lock;
	lock.eliminar();
}

BaseDatos::~BaseDatos() {
	persistir();
	semaforo.eliminar();
}

bool BaseDatos::consultarPersona(Registro& persona) {
	semaforo.p();
	bool encontrado = buscarPersona(persona);
	semaforo.v();
	return encontrado;
}

bool BaseDatos::buscarPersona(Registro& persona) {
	bool encontrado = false;
	//recorre todos los bloques de las memorias compartidas
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	for(it = memoria.begin(); it != memoria.end() && !encontrado; it++) {
		registros = (*it).leer();
		if(registros.consultarRegistro(persona)) //carga los datos de la persona si la encuentra
			encontrado = true;
	}
	return encontrado;
}

bool BaseDatos::agregarPersona(Registro persona) {
	semaforo.p();
	bool resultado;
	if(buscarPersona(persona))
		resultado = false;
	else { //la persona no esta en la base de datos
		agregarRegistro(persona); //agrega al final el registro
		resultado = true;
	}
	semaforo.v();
	return resultado;
}

void BaseDatos::agregarRegistro(Registro persona) {
	registros = memoria[memoria.size() - 1].leer();
	if(!registros.agregarRegistro(persona)) { //si no hay lugar en el ultimo bloque, agrega otro
		agregarMemoriaCompartida();
		registros.agregarRegistro(persona); //agrega el registro en el nuevo bloque
	}
	memoria[memoria.size() - 1].escribir(registros);
}

bool BaseDatos::modificarPersona(Registro persona) {
	semaforo.p();
	bool modificado = false;
	//recorre todos los bloques de las memorias compartidas
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	for(it = memoria.begin(); it != memoria.end() && !modificado; it++) {
		registros = (*it).leer();
		if(registros.modificarRegistro(persona)) {//retorna false si no encuentra el registro
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
	//recorre todos los bloques de las memorias compartidas
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	for(it = memoria.begin(); it != memoria.end() && !eliminado; it++) {
		registros = (*it).leer();
		if(registros.eliminarRegistro(persona)) {//retorna false si no encuentra el registro
			(*it).escribir(registros);
			eliminado = true;
		}
	}
	semaforo.v();
	return eliminado;
}

void BaseDatos::persistir() {
	semaforo.p();
	//recorre todos los bloques de las memorias compartidas
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	for(it = memoria.begin(); it != memoria.end(); it++) {
		registros = (*it).leer();
		registros.persistir();
		(*it).liberar();
	}
	semaforo.v();
}

void BaseDatos::recuperar() {
	//carga en las memorias compartidas los registros del archivo de base de datos
	int cantidadRegistrosCargados = MAX_REG_MEM;
	while(cantidadRegistrosCargados == MAX_REG_MEM) { //si el bloque anterior esta completo, sigue cargando mas bloques
		agregarMemoriaCompartida();
		registros.recuperar();
		semaforo.p();
		memoria[memoria.size() - 1].escribir(registros);
		semaforo.v();
		cantidadRegistrosCargados = registros.getCantidadDeRegistros();
	}
}

void BaseDatos::agregarMemoriaCompartida() {
	MemoriaCompartida<BloqueDeRegistros> memoriaCompartida;
	memoriaCompartida.crear((char *) "BaseDatos.cpp", memoria.size());
	registros = memoriaCompartida.leer();
	registros.setNumeroBloque(memoria.size());
	memoria.insert(memoria.end(), memoriaCompartida);
}
