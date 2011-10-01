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
	int pidUsuario = getppid();
	Fifo canal(intToString(pidUsuario).c_str()); //TODO con pidUsuario
//while no haya senial se queda bloqueado?
	cout << "Receptor " << pidUsuario << ": esperando para leer . . ." << endl;

	int bytesLeidos = canal.leer(buffer, BUFFSIZE);
	buffer[bytesLeidos] = '\0';
	cout << "Receptor " << pidUsuario << ": lei el dato [" << buffer
			<< "] del pipe" << endl;

	int pid = fork ();
	if (pid == 0) {
		int resultado = enviar(pidUsuario,buffer);
		canal.cerrar();
		exit(resultado);
	}

//}
	cout << "Receptor " << pidUsuario << ": fin del proceso" << endl;
	canal.cerrar();
	return 0;
}


int GestorDescargas::enviar(int pidUsuario, char* buffer) {

	Usuario();//a partir del buffer
	string fifo = intToString(pidUsuario) + "_" ; //pidUsuario_pidDestino_path
	Fifo canal(fifo.data());

	ifstream archivo;
	archivo.open(buffer,ifstream::in);//open del archivo path,  lee una linea en binario
	char linea[BUFFSIZE];
	while(!archivo.eof()) {
		archivo.getline(linea,BUFFSIZE);
		//toma lock escritura
		canal.escribir(linea,BUFFSIZE);
		//libera el de lectura
	}
	canal.cerrar();
	return 0;
}

int GestorDescargas::descargar(string path,int pidEnvia) {
	return 0;
}

string  GestorDescargas::intToString(int entero) {

	stringstream out;
	out<<entero;
	return out.str();
}
