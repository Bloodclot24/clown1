#ifndef LOCKFILE_H_
#define LOCKFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>

using namespace std;

class LockFile {

private:

	struct flock fl;
	int fd;
	char nombre [ 255 ];
	int lectura; // lo agregue yo

public:

	LockFile ( char* nombre );
	virtual ~LockFile();

	int tomarLock ();
	int liberarLock ();
	int escribir ( char* buffer,int buffsize );
	int leer (char* buffer,int buffsize );//lo agregue yo
	void cerrar(); //lo agregue yo
	void abrir() {
		fd = open ( this->nombre,O_CREAT|O_RDWR,0777 );
	}
};

#endif /* LOCKFILE_H_ */
