#include <vector>
#include <string.h>
#include "MemoriaCompartida.h"
#include "Semaforo.h"
#include "Registro.h"
#include "BloqueDeRegistros.h"



#ifndef BASEDATOS_H_
#define BASEDATOS_H_


class BaseDatos {
private:
	Semaforo semaforo;
	BloqueDeRegistros registros;
	MemoriaCompartida<BloqueDeRegistros> memoria;


	void persistir();
	void recuperar();

public:
	BaseDatos();
	virtual ~BaseDatos();

	Registro consultarPersona(Registro persona);
	void agregarPersona(Registro persona);
	void modificarPersona(std::string nombre, Registro persona);
	void eliminarPersona(std::string nombre);

};

#endif /* BASEDATOS_H_ */
