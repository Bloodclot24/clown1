#include "LockFile.h"

LockFile::LockFile(string nombre) {

	this->nombre = nombre;
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_pid = getpid();
	abrir();
	//TODO escribir en debug
}

int LockFile::tomarLock() {
	fl.l_type = F_WRLCK;
	int resultado = fcntl(fd, F_SETLKW, &fl);
	return resultado;
}

int LockFile::liberarLock() {
	fl.l_type = F_UNLCK;
	int resultado = fcntl(fd, F_SETLK, &fl);
	return resultado;
}

int LockFile::escribir(char* buffer, int buffsize) {
	lseek(fd, 0, SEEK_END);
	int resultado = write(fd, buffer, buffsize);
	return resultado;
}

int LockFile::leer(char* buffer, int buffsize) {
	lseek(fd, lectura, SEEK_SET);
	//lseek ( fd,0,SEEK_CUR );
	int resultado = read(fd, (void*) buffer, buffsize);
	if (resultado == 0)
		return resultado;
	string linea(buffer);
	int pos = linea.find("\n", 0);
	lectura += pos + 1;
	return pos;
}

void LockFile::abrir() {
	fd = open(nombre.c_str(), O_CREAT | O_RDWR, 0777);
	lectura = 0;
}

void LockFile::cerrar() {
	close(fd); //ver si se elimina
}

void LockFile::eliminar() {
	close(fd); //ver si se elimina
	unlink(nombre.c_str());
}

void LockFile::reset() {
	//lseek ( fd,0,SEEK_SET);
	cerrar();
	unlink(nombre.c_str()); //ver esto!!!
	abrir();
}

LockFile::~LockFile() {
	close(fd);
}
