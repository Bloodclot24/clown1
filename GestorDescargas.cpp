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
	LockFile lockEscritura((char*) pathLockEscritura.c_str());
	while (true) {
		cout<< "Estoy antes del leer para recibir pedidos" <<endl;
		int bytesLeidos = canal.leer(buffer, BUFFSIZE);
		cout<< "despues de lees en canal para recibir pedidos de envios" <<endl;
		lockEscritura.liberarLock();
		buffer[bytesLeidos] = '\0'; //controlar esto
		string linea(buffer);
		int pidDestino = atoi(linea.substr(0, linea.find("|", 0)).c_str());
		string path = linea.substr(linea.find("|", 0) + 1, linea.length());

		cout << "Receptor " << pidEnvia << ": lei el dato [" << buffer
				<< "] del pipe" << pidDestino << endl;

		int pid = fork();
		if (pid == 0) {
			int resultado = enviar(pidEnvia, pidDestino, (char*) path.c_str());
	//		canal.cerrar(); //TODO chequear que este no quede colgado
			lockEscritura.cerrar();
			exit(resultado);
		}
//}
	}
	cout << "Receptor " << pidEnvia << ": fin del proceso" << endl;
	lockEscritura.cerrar();
	canal.cerrar();
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
		cout << "ya escribi " << linea << endl;
	}
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

	//canal.cerrar();
	lockEscritura.cerrar();

	string pathFifoDescarga = intToString(pidEnvia) + "_" + intToString(pid);
	string pathLockDescargaEscritura = pathFifoDescarga + ".lockEscritura";
	LockFile lockDescargaEscritura((char*)pathLockDescargaEscritura.c_str());
	Fifo canalDescarga((char*)pathFifoDescarga.c_str());

	ofstream archivo;
	string directorio = "descargas_" + nombre + "_" + intToString(getppid()) + "/";
	//TODO mover de lugar esto
	int estado = mkdir(directorio.c_str(), 0700);// TODO control de errores
	cout << "Directorio [ " << directorio <<" ] creado para realizar la descarga." << endl; //debug
	string pathTotal = directorio+path;
	archivo.open(pathTotal.c_str(),ofstream::out);//open del archivo path,  lee una linea en binario
	char descarga[BUFFSIZE];
	while(canalDescarga.leer(descarga,BUFFSIZE) != 0) {
		lockDescargaEscritura.liberarLock();
		cout <<"descargandooooooooo" << descarga << endl;
		archivo << descarga << endl; //TODO endl esta mal parche!!!
		archivo.flush();
	}
	lockDescargaEscritura.liberarLock(); //ver si esta bien
	archivo.close();
	canalDescarga.cerrar();
	lockDescargaEscritura.cerrar();
	cout<<"DESCARGADO" <<endl;
	return 0;
}

string  GestorDescargas::intToString(int entero) {

	stringstream out;
	out<<entero;
	return out.str();
}
