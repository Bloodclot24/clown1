#ifndef BLOQUEDEREGISTROS_H_
#define BLOQUEDEREGISTROS_H_

#include <iostream>
#include <string.h>
#include "Registro.h"
#include "LockFile.h"

#define MAX_REG_MEM 100

class BloqueDeRegistros {
private:

//	LockFile lock;
	Registro registros [MAX_REG_MEM];
	int cantidadDeRegistros;
	void acomodarBloque(int posicion);
	bool buscarRegistro(Registro registro);

public:
	BloqueDeRegistros();
	virtual ~BloqueDeRegistros();
	bool agregarRegistro(Registro registro);
	bool eliminarRegistro(Registro registro);
	bool modificarRegistro(Registro registro);
//	Registro consultarRegistro(Registro registro);
	bool consultarRegistro(Registro& registro);

	void persistir();
	BloqueDeRegistros recuperar();
};

#endif /* BLOQUEDEREGISTROS_H_ */
