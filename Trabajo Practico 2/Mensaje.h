#ifndef MENSAJE_H_
#define MENSAJE_H_

#include "Registro.h"
#include <unistd.h>

#define	PETICION	1

typedef enum {
	AGREGAR, ELIMINAR, CONSULTAR, MODIFICAR
} T_PETICION;

typedef struct {
	long mtype;
	pid_t id;
	T_PETICION tipo;
	Registro registro;
	char respuesta[256];
} mensaje;

#endif /* MENSAJE_H_ */
