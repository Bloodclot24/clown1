/*
 * GestorDescargas.cpp
 *
 *  Created on: Sep 30, 2011
 *      Author: guido
 */

#include "GestorDescargas.h"

GestorDescargas::GestorDescargas() {
	// TODO Auto-generated constructor stub

}

GestorDescargas::~GestorDescargas() {
	// TODO Auto-generated destructor stub
}

int GestorDescargas::iniciarRecepcion() {

	// receptor de pedidos de archivos
	char buffer[BUFFSIZE];
	int pidEnvia = getppid();
	Fifo canal(intToString(pidEnvia).c_str()); //TODO con pidUsuario
//while no haya senial se queda bloqueado?
	cout << "Receptor " << pidEnvia << ": esperando para leer . . ." << endl;
	string pathLockEscritura = intToString(pidEnvia) + ".lockEscritura";
	LockFile lockEscritura((char*)pathLockEscritura.c_str());

	int pidDestino;
	canal.leer(&pidDestino);
	int bytesLeidos = canal.leer(buffer,BUFFSIZE);
	buffer[bytesLeidos] = '\0';
	lockEscritura.liberarLock();

	cout << "Receptor " << pidEnvia << ": lei el dato [" << buffer << "] del pipe" << endl;

	int pid = fork ();
	if (pid == 0) {
		int resultado = enviar(pidEnvia,pidDestino, buffer);
		canal.cerrar();
		exit(resultado);
	}

//}
	cout << "Receptor " << pidEnvia << ": fin del proceso" << endl;
	canal.cerrar();
	return 0;
}


int GestorDescargas::enviar(int pidEnvia, int pidDestino, char* buffer) {

	string fifo = intToString(pidEnvia) + "_" + intToString(pidEnvia);
	Fifo canal(fifo.c_str());

	string pathLock = fifo + ".lockEscritura";
	LockFile lock((char*)pathLock.c_str());
	ifstream archivo;
	archivo.open(buffer,ifstream::in);//open del archivo path,  lee una linea en binario
	char linea[BUFFSIZE];
	while(!archivo.eof()) {
		archivo.getline(linea,BUFFSIZE);
		lock.tomarLock();
		canal.escribir(linea,BUFFSIZE);
	}
	canal.cerrar();
	return 0;
}

int GestorDescargas::descargar(string path,int pidEnvia)
{
	int pid = getpid();
	string pathLockEscritura = intToString(pidEnvia) + ".lockEscritura";
	LockFile lockEscritura((char*)pathLockEscritura.c_str());
	Fifo canal((char*)intToString(pidEnvia).c_str());

	lockEscritura.tomarLock();
	canal.escribir(&pid);
	canal.escribir(path.c_str(),path.length());

	string pathFifoDescarga = intToString(pidEnvia) + "_" + intToString(pid);
	string pathLockDescargaEscritura = pathFifoDescarga + ".lockEscritura";
	LockFile lockDescargaEscritura((char*)pathLockDescargaEscritura.c_str());
	Fifo canalDescarga((char*)pathFifoDescarga.c_str());

	canalDescarga.leer(&pid); //while!!!!
	lockDescargaEscritura.liberarLock();

	//cerrar!!!
	cout<<"descargandooooooooo" << pid <<endl;
	return 0;
}

string  GestorDescargas::intToString(int entero) {

	stringstream out;
	out<<entero;
	return out.str();
}
