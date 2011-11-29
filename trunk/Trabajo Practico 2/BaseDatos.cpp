#include "BaseDatos.h"

BaseDatos::BaseDatos(): semaforo("BaseDatos.cpp", 1) {
	//registros = NULL;
	recuperar();
	LockFile lock;
	lock.eliminar();
	//semaforo.p();
	//memoria.escribir(registros);
	//semaforo.v();
	cout << "Fin del constructor " << endl;
}


bool BaseDatos::consultarPersona(Registro& persona) {
	semaforo.p();
	bool encontrado = buscarPersona(persona);
	semaforo.v();
	if(!encontrado)
		std::cerr << "Error, la persona consultada no existe." << std::endl;
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
	if(buscarPersona(persona)) {
		std::cerr << "Error, la persona que intenta agregar ya existe" << std::endl;
		return false;
	}
	registros = memoria[memoria.size() - 1].leer();
	if(!registros.agregarRegistro(persona)) {
		std::cerr << "No hay mas espacio en este bloque" << std::endl;
		MemoriaCompartida<BloqueDeRegistros> memoriaCompartida;
		std::cout<< "Resultado de crear memoria "<< memoriaCompartida.crear((char *) "BaseDatos.cpp", memoria.size()) << std::endl;
		registros = memoriaCompartida.leer();
		registros.setNumeroBloque(memoria.size());
		registros.agregarRegistro(persona);
		memoria.insert(memoria.end(), memoriaCompartida); //ver q pasa con las referencias!
	}
	memoria[memoria.size() - 1].escribir(registros);
	semaforo.v();
	return true;
}


bool BaseDatos::modificarPersona(Registro persona) {
	semaforo.p();
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	bool modificado = false;
	for(it = memoria.begin(); it != memoria.end() && !modificado; it++) {
		registros = (*it).leer();
		if(registros.modificarRegistro(persona)) {
			(*it).escribir(registros);
			modificado = true;
		} else
			std::cerr << "la persona no esta en este bloque" << std::endl;
	}
	semaforo.v();
	return modificado;
}


bool BaseDatos::eliminarPersona(Registro persona) {
	semaforo.p();
	vector<MemoriaCompartida<BloqueDeRegistros> >::iterator it;
	bool eliminado = false;
	for(it = memoria.begin(); it != memoria.end() && !eliminado; it++) {
		registros = (*it).leer();
		if(registros.eliminarRegistro(persona)) {
			(*it).escribir(registros);
			eliminado = true;
		} else
			std::cerr << "la persona no esta en este bloque" << std::endl;
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
	}
	semaforo.v();
}


void BaseDatos::recuperar() {
	int cantidadRegistrosCargados = MAX_REG_MEM;
	for(int i = 0; cantidadRegistrosCargados == MAX_REG_MEM; i++) {//while?
		MemoriaCompartida<BloqueDeRegistros> memoriaCompartida;
		std::cout<< "Resultado de crear memoria "<< memoriaCompartida.crear((char *) "BaseDatos.cpp", i) << std::endl;
		registros.setNumeroBloque(i);
		registros = registros.recuperar();
		semaforo.p();
		memoriaCompartida.escribir(registros);
		semaforo.v();
		memoria.insert(memoria.end(), memoriaCompartida); //ver q pasa con las referencias!
		cantidadRegistrosCargados = registros.getCantidadDeRegistros();
		cout << cantidadRegistrosCargados << endl;
	}
}


BaseDatos::~BaseDatos() {
	persistir();
	memoria[0].liberar();
	semaforo.eliminar();
}

