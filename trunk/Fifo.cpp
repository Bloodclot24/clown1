#include "Fifo.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


Fifo :: Fifo ( const char* nombre )
{
	strcpy ( this->nombre,nombre );
	mknod ( nombre,S_IFIFO|0666,0 ); //crea el fifo
	this->fileDes = -1;
}

Fifo :: ~Fifo ()
{
}

int Fifo :: escribir (const char* dato,int datoSize )
{
	if ( this->fileDes == -1)
		this->fileDes = open ( this->nombre,O_WRONLY ); //abre el file descriptor para escritura

	int resultado = write ( this->fileDes,(const void *)dato,datoSize );

	return resultado;
}

int Fifo :: leer ( char* buffer,int buffSize )
{
	if ( this->fileDes == -1 )
		this->fileDes = open ( this->nombre,O_RDONLY ); //abre el file descriptor para lectura

	int resultado = read ( this->fileDes,(void *)buffer,buffSize );

	return resultado;
}

void Fifo :: cerrar ()
{
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
