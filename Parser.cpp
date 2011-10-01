#include "Parser.h"
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

Parser::Parser(){

}

Parser::~Parser(){
	
}

bool Parser::LeerLinea(string &cadena,fstream &archivoEntrada)
{
		int valor=600;
		char linea[600];
		//Leo una linea, tomo un margen de caracteres para evitar errores
		//dado que el mensaje no contiene mas de 512 caracteres.
		archivoEntrada.getline((char*)&linea,valor);
		cadena=linea;
		return true;
}

void Parser::ParsearAristas(string linea,int &dato,int &identificacion)
{
	std::string cadenaAux;  
	std::string:: size_type coma2 = linea.find(",", 2); 
	cadenaAux.assign(linea, 2, coma2 -2);
	dato= atoi(cadenaAux.c_str()); 
	std::string:: size_type coma3 = linea.find(",", coma2 +1);
	cadenaAux.assign(linea, coma2 +1, coma3- coma2 -1);
	identificacion = atoi(cadenaAux.c_str());
	
}

void Parser::ParsearVertices(string linea,char &dato,int &identificacion)
{
	std::string cadenaAux;  
	std::string:: size_type coma2 = linea.find(",", 2); 
	cadenaAux.assign(linea, 2, coma2 -2);
	dato = linea[coma2-1];
	std::string:: size_type coma3 = linea.find(",", coma2 +1);
	cadenaAux.assign(linea, coma2 +1, coma3 - coma2 -1);
	identificacion = atoi(cadenaAux.c_str());

}


void Parser::ParsearMensaje(string linea,int& servidorOrigen,int& servidorDestino,string& mensaje)
{
	std::string cadenaAux;  
	std::string:: size_type coma2 = linea.find(",", 2); 
	cadenaAux.assign(linea, 2, coma2 -2);
	servidorOrigen= atoi(cadenaAux.c_str());
	std::string:: size_type coma3 = linea.find(",", coma2 + 1);
	cadenaAux.assign(linea, coma2 + 1, coma3 - coma2 -1);
	servidorDestino = atoi(cadenaAux.c_str());
	coma3 ++;
	mensaje.assign(linea,coma3,linea.length());
}
	
