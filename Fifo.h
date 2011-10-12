#ifndef FIFO_H_
#define FIFO_H_

#include <string>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

class Fifo {

private:
	string nombre;
	int fileDes;

public:
	Fifo(string nombre);
	virtual ~Fifo();

	int escribir(const char* dato, int datoSize);
	int leer(char* buffer, int buffSize);
	void cerrar();
	void abrir();
	void eliminar();
};

#endif /* FIFO_H_ */
