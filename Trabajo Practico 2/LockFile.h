#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

#define ARCH_LOCK "basedatos.bd"

using namespace std;

class LockFile {
private:
	struct flock fl;
	int fd;
	string nombre;
	int posicion;

public:
	LockFile();
	virtual ~LockFile();

	int tomarLock ();

	int liberarLock ();

	int escribir ( char* buffer,int buffsize );

	int leer (char* buffer,int buffsize );

	void cerrar();

	void eliminar();

	void setPosicion(int nuevaPosicion);
};

#endif /* LOCKFILE_H_ */
