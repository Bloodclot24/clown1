#include "LockFile.h"

LockFile :: LockFile ( char* nombre ) {

	strcpy ( this->nombre,nombre );

	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_pid = getpid ();
	fd = open ( this->nombre,O_CREAT|O_RDWR,0777 );//( this->nombre,O_CREAT|O_WRONLY,0777 );
	lectura = 0;
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
	int resultado = read ( fd,(void*)buffer,buffsize );
	lectura += resultado;
	return resultado;
}

void LockFile :: cerrar () {
	close ( fd );
}

LockFile :: ~LockFile () {
	close ( fd );
}
