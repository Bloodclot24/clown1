/*
 * ArchivoUsuarios.cpp
 *
 *  Created on: Oct 1, 2011
 *      Author: guido
 */

#include "ArchivoUsuarios.h"

ArchivoUsuarios::ArchivoUsuarios() : lock((char*)ARCHIVO_LOCK_USUARIOS)
{

	archivo.open(ARCHIVO_USUARIOS, std::fstream::in);
	if (!archivo.is_open()) {
		archivo.clear();
		archivo.open(ARCHIVO_USUARIOS, std::fstream::out);
	}
	archivo.close();
	archivo.open(ARCHIVO_USUARIOS, std::fstream::in | std::fstream::out);

}

ArchivoUsuarios::~ArchivoUsuarios()
{
}

int ArchivoUsuarios::escribir (string ruta,int pid, string nombre)
{
	string linea = nombre + "," + intToString(pid) + "," + ruta + "\n";
	cout << linea;
	lock.tomarLock ();
	lock.escribir((char*)linea.c_str(), linea.length());
//	archivo.seekp(0,ios::end);
//	archivo << linea;//falta busqueda y control de pid
	lock.liberarLock ();
	return 0;
}

int ArchivoUsuarios::leer (string& ruta,int& pid, string& nombre)
{
	char linea[512];
	lock.tomarLock ();
	int leidos = lock.leer(linea,512);
//	archivo.getline(linea,512);//falta busqueda y control de pid
	lock.liberarLock ();
	string linea1(linea);
	linea1.assign(linea1,0,leidos);
	parsearLinea(linea1,nombre,pid,ruta);
//	return (archivo.eof()?0:1);
	return leidos;

}

int ArchivoUsuarios::parsearLinea(string linea,string& nombre,int& pid,string& archivo)
{
	string cadenaAux;
	string:: size_type coma2 = linea.find(",", 0);
	cadenaAux.assign(linea, 0, coma2);
	nombre = cadenaAux;
	cout << "Nombre Parseado: " << nombre << endl; //para debug
	string:: size_type coma3 = linea.find(",", coma2 + 1);
	cadenaAux.assign(linea, coma2 + 1, coma3 - coma2 -1);
	pid = atoi(cadenaAux.c_str());
	cout << "PID Parseado: " << pid << endl; //para debug
	//archivo.assign(linea,coma3 + 1,linea.length());
	archivo.assign(linea,coma3 + 1,linea.length());
	cout << "Archivo Parseado: " << archivo << endl; //para debug
	return 0;
}
