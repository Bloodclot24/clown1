/*
 * main.cpp
 *
 *  Created on: Nov 23, 2011
 *      Author: guido
 */
#include <iostream>
#include <stdio.h>
#include <cstring>
#include "BaseDatos.h"
#include "Registro.h"

int main(int argc, char**argv) {

	std::cout
			<< "+++++++++++++++Prueba Base De Datos Tp2 Tecnicas de Programacion Concurrente I+++++++++++++++"
			<< std::endl;
	std::cout << "Construyendo registro 1" << std::endl;
	Registro registro;
	std::string nombre("Guido Ygounet\0");
	std::string direccion("Mi casa 1234\0");
	std::string tel("1234567890\0");
	strcpy(registro.nombre, nombre.c_str());
	strcpy(registro.direccion, direccion.c_str());
	strcpy(registro.telefono, tel.c_str());
	printf("nombre:%s \n", registro.nombre);
	printf("dire:%s \n ", registro.direccion);
	printf("tel:%s\n ", registro.telefono);
	BaseDatos bd;
	std::cout << "Agregando registro en la Base de datos" << std::endl;
	bd.agregarPersona(registro);
	std::cout << "Registro 1 agregado" << std::endl;
	std::cout << "Consultando registro  de la Base de datos" << std::endl;
	strcpy(registro.nombre, nombre.c_str());
	//strcpy(registro.direccion, NULL);
	//strcpy(registro.telefono, NULL);
	bd.consultarPersona(registro);
	std::cout << "Registro obtenido de la base de datos " << std::endl;
	printf("nombre:%s \n", registro.nombre);
	printf("dire:%s \n ", registro.direccion);
	printf("tel:%s\n ", registro.telefono);
	std::cout << "Agregando nuevos registros a la bd" << std::endl;
	Registro registro1;
	std::string nombre1("Karen Roberts\0");
	std::string direccion1("Su casa 1234\0");
	std::string tel1("0987654321\0");
	strcpy(registro1.nombre, nombre1.c_str());
	strcpy(registro1.direccion, direccion1.c_str());
	strcpy(registro1.telefono, tel1.c_str());
	printf("nombre:%s \n", registro1.nombre);
	printf("dire:%s \n", registro1.direccion);
	printf("tel:%s \n", registro1.telefono);
	std::cout << "Agregando registro en la Base de datos" << std::endl;
	bd.agregarPersona(registro1);
	std::cout << "Registro 1 agregado" << std::endl;
	std::cout << "Consultando registro  de la Base de datos" << std::endl;
	strcpy(registro1.nombre, nombre1.c_str());
//	strcpy(registro1.direccion, NULL);
//	strcpy(registro1.telefono, NULL);
	bd.consultarPersona(registro1);
	std::cout << "Registro obtenido de la base de datos " << std::endl;
	printf("nombre:%s \n", registro1.nombre);
	printf("dire:%s \n", registro1.direccion);
	printf("tel:%s \n", registro1.telefono);

	Registro registro2;
	std::string nombre2("Gonzalo Ferrero\0");
	std::string direccion2("Casa 1234\0");
	std::string tel2("0123456789\0");
	strcpy(registro2.nombre, nombre2.c_str());
	strcpy(registro2.direccion, direccion2.c_str());
	strcpy(registro2.telefono, tel2.c_str());
	printf("nombre:%s \n", registro2.nombre);
	printf("dire:%s \n", registro2.direccion);
	printf("tel:%s \n", registro2.telefono);
	std::cout << "Agregando registro en la Base de datos" << std::endl;
	bd.agregarPersona(registro2);
	std::cout << "Registro 2 agregado" << std::endl;
	std::cout << "Consultando registro  de la Base de datos" << std::endl;
	strcpy(registro2.nombre, nombre2.c_str());
	//strcpy(registro2.direccion, NULL);
	//strcpy(registro2.telefono, NULL);
	bd.consultarPersona(registro2);
	std::cout << "Registro obtenido de la base de datos " << std::endl;
	printf("nombre:%s \n", registro2.nombre);
	printf("dire:%s \n", registro2.direccion);
	printf("tel:%s \n", registro2.telefono);
	std::string ndireccion1("1234\0");
	std::cout << "Modificando direccion registro Karen Roberts por 1234"
			<< std::endl;
	strcpy(registro1.nombre, nombre1.c_str());
	strcpy(registro1.direccion, ndireccion1.c_str());
	strcpy(registro1.telefono, tel1.c_str());
	bd.modificarPersona(registro1.nombre, registro1);
	std::cout << "Direccion registro Karen Roberts modificada" << std::endl;
	std::cout << "Consultando registro Karen" << std::endl;
	bd.consultarPersona(registro1);
	std::cout << "Registro obtenido de la base de datos " << std::endl;
	printf("nombre:%s \n", registro1.nombre);
	printf("dire:%s \n", registro1.direccion);
	printf("tel:%s \n", registro1.telefono);

	std::cout << "Eliminando registro Karen de la bd " << std::endl;
	bd.eliminarPersona(registro1.nombre);
	std::cout << "Registro Karen eliminado" << std::endl;
	std::cout << "Consultando registro Karen" << std::endl;
	bd.consultarPersona(registro1);
	std::cout << "Consultando registro Guido" << std::endl;
	bd.consultarPersona(registro);
	std::cout << "Registro obtenido de la base de datos " << std::endl;
	printf("nombre:%s \n", registro.nombre);
	printf("nombre:%s \n", registro.direccion);
	printf("tel:%s \n", registro.telefono);
	std::cout << "Consultando registro Gonzalo" << std::endl;
	bd.consultarPersona(registro2);
	std::cout << "Registro obtenido de la base de datos " << std::endl;
	printf("nombre:%s \n", registro2.nombre);
	printf("dire:%s \n", registro2.direccion);
	printf("tel:%s \n", registro2.telefono);

	return 0;

}

