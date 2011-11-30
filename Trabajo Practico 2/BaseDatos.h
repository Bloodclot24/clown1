#ifndef BASEDATOS_H_
#define BASEDATOS_H_

#include <vector>
#include <string.h>
#include "MemoriaCompartida.h"
#include "Semaforo.h"
#include "Registro.h"
#include "BloqueDeRegistros.h"


class BaseDatos {
private:
	Semaforo semaforo;
	BloqueDeRegistros registros;
	vector<MemoriaCompartida<BloqueDeRegistros> > memoria;

	void persistir();
	void recuperar();
	bool buscarPersona(Registro& persona);
	void agregarRegistro(Registro persona);
	void agregarMemoriaCompartida();

public:
	BaseDatos();
	virtual ~BaseDatos();

	bool consultarPersona(Registro& persona);
	bool agregarPersona(Registro persona);
	bool modificarPersona(Registro persona);
	bool eliminarPersona(Registro persona);

};

#endif /* BASEDATOS_H_ */
