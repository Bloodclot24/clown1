#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

using namespace std;

class LockFile {

private:

	struct flock fl;
	int fd;
	string nombre;
	int lectura;

public:

	LockFile (string nombre );
	virtual ~LockFile();

	int tomarLock ();
	int liberarLock ();
	int escribir ( char* buffer,int buffsize );
	int leer (char* buffer,int buffsize );
	void cerrar();
	void abrir();
	void reset();
	void eliminar();
};

#endif /* LOCKFILE_H_ */
