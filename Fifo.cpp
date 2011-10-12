#include "Fifo.h"

Fifo::Fifo(string nombre) {
	this->nombre = nombre;
	mknod(nombre.c_str(), S_IFIFO | 0666, 0); //crea el fifo
	fileDes = -1;
}

Fifo::~Fifo() {
}

int Fifo::escribir(const char* dato, int datoSize) {
	if (fileDes == -1)
		fileDes = open(nombre.c_str(), O_WRONLY); //abre el file descriptor para escritura

	int resultado = write(fileDes, (const void *) dato, datoSize);

	return resultado;
}

int Fifo::leer(char* buffer, int buffSize) {
	if (fileDes == -1)
		fileDes = open(nombre.c_str(), O_RDONLY); //abre el file descriptor para lectura

	int resultado = read(fileDes, (void *) buffer, buffSize);

	return resultado;
}

void Fifo::cerrar() {
	if (fileDes != -1)
		close(fileDes);
	fileDes = -1;
}

void Fifo::eliminar() {
	cerrar();
	unlink(nombre.c_str());
}

void Fifo::abrir() {
	if (fileDes == -1)
		mknod(nombre.c_str(), S_IFIFO | 0666, 0); //crea el fifo
}


