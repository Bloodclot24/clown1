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
	Fifo canal(intToString(pidEnvia).c_str());
//while no haya senial se queda bloqueado?

	string mensaje = "Receptor " + Debug::getInstance()->intToString(getpid()) + " del proceso " + Debug::getInstance()->intToString(pidEnvia) + ": esperando para leer . . .\n";
	Debug::getInstance()->escribir(mensaje);

	string pathLockEscritura = intToString(pidEnvia) + ".lockEscritura";
	LockFile lockEscritura((char*) pathLockEscritura.c_str());
	while (true) {
		int bytesLeidos = canal.leer(buffer, BUFFSIZE);
		if(bytesLeidos == 0){ // leyo eof porque todos los escritores cerraron el canal y hay que abrir y cerrar para que se bloquee en la lectura.
			canal.cerrar();
			canal.abrir();
			bytesLeidos = canal.leer(buffer, BUFFSIZE);
		}
		lockEscritura.liberarLock();
		buffer[bytesLeidos] = '\0'; //controlar esto
		string linea(buffer);
		int pidDestino = atoi(linea.substr(0, linea.find("|", 0)).c_str());
		string path = linea.substr(linea.find("|", 0) + 1, linea.length());

//		mensaje = "Receptor " + Debug::getInstance()->intToString(getpid()) + " del proceso " + Debug::getInstance()->intToString(pidEnvia) + ": lei el dato [" + linea	+ "] del fifo " + Debug::getInstance()->intToString(pidEnvia) + "\n";
//		Debug::getInstance()->escribir(mensaje);

		int pid = fork();
		if (pid == 0) {
			int resultado = enviar(pidEnvia, pidDestino, (char*) path.c_str());
			canal.cerrar(); //TODO chequear que este no quede colgado
			lockEscritura.cerrar();
			exit(resultado);
		}
//}
	}
	lockEscritura.cerrar();
	canal.cerrar();

	mensaje = "Receptor " + Debug::getInstance()->intToString(getpid()) + " del proceso " + Debug::getInstance()->intToString(pidEnvia) + ": fin del proceso\n";
//	Debug::getInstance()->escribir(mensaje);

	return 0;
}


int GestorDescargas::enviar(int pidEnvia, int pidDestino, char* buffer) {

	string fifo = intToString(pidEnvia) + "_" + intToString(pidDestino);
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
		string mensaje = "Receptor " + Debug::getInstance()->intToString(getpid()) + " del proceso " + Debug::getInstance()->intToString(pidEnvia) + ": escribi linea [" + linea + "] en el fifo " + fifo + "\n";
		//Debug::getInstance()->escribir(mensaje);
	}
	//lock.tomarLock();
	//canal.escribir(intToString(EOF).c_str(), sizeof(EOF));
	archivo.close();
	lock.cerrar();
	canal.cerrar();
	return 0;
}

int GestorDescargas::descargar(string path,int pidEnvia, string nombre)
{
	int pid = getpid();
	string pathLockEscritura = intToString(pidEnvia) + ".lockEscritura";
	LockFile lockEscritura((char*)pathLockEscritura.c_str());
	Fifo canal((char*)intToString(pidEnvia).c_str());

	lockEscritura.tomarLock();
	string pidPath = intToString(pid) + "|" + path;
	canal.escribir(pidPath.c_str(), pidPath.length());

	canal.cerrar();//TODO lo descomente ahora
	lockEscritura.cerrar();

	string pathFifoDescarga = intToString(pidEnvia) + "_" + intToString(pid);
	string pathLockDescargaEscritura = pathFifoDescarga + ".lockEscritura";
	LockFile lockDescargaEscritura((char*)pathLockDescargaEscritura.c_str());
	Fifo canalDescarga((char*)pathFifoDescarga.c_str());

	ofstream archivo;
	string directorio = "descargas_" + nombre + "_" + intToString(getppid()) + "/";
	//TODO mover de lugar esto
	int estado = mkdir(directorio.c_str(), 0700);// TODO control de errores
	if(estado >= 0) {
		string mensaje = "Descarga " + Debug::getInstance()->intToString(getpid()) + " del proceso " + Debug::getInstance()->intToString(getppid()) + ": creo el directorio " + directorio + " para realizar la descarga\n";
		Debug::getInstance()->escribir(mensaje);
	}
	string pathTotal = directorio+path;
	archivo.open(pathTotal.c_str(),ofstream::out);//open del archivo path,  lee una linea en binario
	char descarga[BUFFSIZE];
	while(canalDescarga.leer(descarga,BUFFSIZE) != 0) { // Estamos abusando de que cuando cierra el escritor lee eof.
		lockDescargaEscritura.liberarLock();
		archivo << descarga << endl; //TODO endl esta mal parche!!!
		archivo.flush();
		string mensaje = "Descarga " + Debug::getInstance()->intToString(getpid()) + " del proceso " + Debug::getInstance()->intToString(getppid()) + ": descargue el dato [" + descarga + "] del fifo " + pathFifoDescarga + "\n";
		Debug::getInstance()->escribir(mensaje);
		cout << "Estoy descargando" << endl;
	}
	archivo.close();
	canalDescarga.cerrar();
	cout << "Me trabe en el cerrar del lock" << endl;
	lockDescargaEscritura.cerrar();
	string mensaje = "Descarga " + Debug::getInstance()->intToString(getpid()) + " del proceso " + Debug::getInstance()->intToString(getppid()) + ": finaliza descarga\n";
	Debug::getInstance()->escribir(mensaje);
	return 0;
}

string  GestorDescargas::intToString(int entero) {

	stringstream out;
	out<<entero;
	return out.str();
}
