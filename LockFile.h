#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

class LockFile {

private:

	struct flock fl;
	int fd;
	string nombre;
	int lectura; // lo agregue yo

public:

	LockFile (string nombre );
	virtual ~LockFile();

	int tomarLock ();
	int liberarLock ();
	int escribir ( char* buffer,int buffsize );
	int leer (char* buffer,int buffsize );//lo agregue yo
	void cerrar(); //lo agregue yo
	void abrir();
	void reset();
	void eliminar();
};

#endif /* LOCKFILE_H_ */
