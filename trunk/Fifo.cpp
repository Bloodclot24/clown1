#include "Fifo.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


Fifo :: Fifo ( const char* nombre ) {

	strcpy ( this->nombre,nombre );
	// se crea el fifo
	mknod ( nombre,S_IFIFO|0666,0 );
	// se inicializa el descriptor en -1
	this->fileDes = -1;
}

Fifo :: ~Fifo () {
}

int Fifo :: escribir (const char* dato,int datoSize ) {

	// se abre el file descriptor para escritura
	if ( this->fileDes == -1)
		this->fileDes = open ( this->nombre,O_WRONLY );

	// se escriben los datos en el fifo
	int resultado = write ( this->fileDes,(const void *)dato,datoSize );

	return resultado;
}

int Fifo :: leer ( char* buffer,int buffSize ) {

	// se abre el file descriptor para lectura
	if ( this->fileDes == -1 )
		this->fileDes = open ( this->nombre,O_RDONLY );//O_RDWR
	// se leen los datos del fifo
	int resultado = read ( this->fileDes,(void *)buffer,buffSize );
	//if (resultado == 0) //si leo 0 bytes estoy en eof
	//	cerrar();
	//std::string eofRecibido((char*)buffer);
	//if(atoi(eofRecibido.c_str()) == EOF)
		//resultado = 0;
	std::cout << "Resultado al leer del fifo de nombre " << nombre << ":" << resultado << std::endl;
	return resultado;
}

int Fifo :: escribir ( int* dato ) {

	// se abre el file descriptor para escritura
	if ( this->fileDes == -1)
		this->fileDes = open ( this->nombre,O_WRONLY );

	// se escriben los datos en el fifo
	int resultado = write ( this->fileDes,(const void *)dato,4 );

	return resultado;
}

int Fifo :: leer ( int* dato ) {

	// se abre el file descriptor para lectura
	if ( this->fileDes == -1 )
		this->fileDes = open ( this->nombre,O_RDONLY );

	// se leen los datos del fifo
	int resultado = read ( this->fileDes,(void *)dato,4 );

	return resultado;
}

void Fifo :: cerrar () {
	close ( this->fileDes );
	fileDes = -1;//lo agregue
	unlink ( this->nombre );
}
/*LUCAS
void Fifo :: cerrar () {
if(fileDes!=-1)
close ( this->fileDes );
fileDes=-1;
}

void Fifo::borrar(){
cerrar();
unlink ( this->nombre );
}
*/

void Fifo :: abrir (){
	//if (fileDes == -1) {
		// se crea el fifo
		mknod(nombre, S_IFIFO | 0666, 0);
	//}
}
