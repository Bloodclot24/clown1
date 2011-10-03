#include "LockFile.h"

#include <iostream>
#include <stdio.h>

LockFile :: LockFile ( char* nombre ) {

	strcpy ( this->nombre,nombre );
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_pid = getpid ();
	abrir();//( this->nombre,O_CREAT|O_WRONLY,0777 );
	//TODO escribir en debug
}

int LockFile :: tomarLock () {
	fl.l_type = F_WRLCK;
	int resultado = fcntl ( fd,F_SETLKW,&fl );
	return resultado;
}

int LockFile :: liberarLock () {
	fl.l_type = F_UNLCK;
	int resultado = fcntl ( fd,F_SETLK,&fl );
	return resultado;
}

int LockFile :: escribir ( char* buffer,int buffsize ) {
	lseek ( fd,0,SEEK_END );
	int resultado = write ( fd,buffer,buffsize );
	return resultado;
}

int LockFile :: leer ( char* buffer,int buffsize ) {
	lseek ( fd,lectura,SEEK_SET );
	//lseek ( fd,0,SEEK_CUR );
	int resultado = read ( fd,(void*)buffer,buffsize );
	if(resultado == 0)
		return resultado;
	string linea(buffer);
	int pos = linea.find("\n", 0);
	lectura += pos + 1;
	return pos;
}

void LockFile :: cerrar () {
	close ( fd );
}

void LockFile :: reset() {
	//lseek ( fd,0,SEEK_SET);
	cerrar();
	unlink(nombre);
	abrir();
}

LockFile :: ~LockFile () {
	close ( fd );
}
