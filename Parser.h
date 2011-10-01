#ifndef PARSER_H_
#define PARSER_H_
#include <iostream>

class Parser
{
	private:
		
	public:
		Parser();
		~Parser();
		
		/*Metodo encargado de leer una linea del archivo.
		 * Recibe el archivo abierto para lectura y devuelve en cadena
		 * la linea leida
		 * Autor:Guido Nahuel Ygounet*/
		bool LeerLinea(std::string &cadena,std::fstream &archivoEntrada);
		
		/*Metodo encargado de parsear una linea de vertices, recibe la linea
		 * y devuelve al tipo y el id del vertice en cuestion
		 * Autor:Guido Nahuel Ygounet*/
		void ParsearVertices(std::string linea,char &tipo,int &identificacion);
		
		/*Metodo encargado de parsear una linea de aristas, recibe la linea
		 * y devuelve el id de los vertices que conecta
		 * Autor:Guido Nahuel Ygounet*/
		void ParsearAristas(std::string linea,int &dato,int &identificacion);
		
		/*Metodo encargado de parsear una linea de mensaje, recibe la linea
		 * y devuelve el id del servidor de origen y destino del mensaje
		 * junto con el mensaje leido 
		 * Autor:Guido Nahuel Ygounet*/
		void ParsearMensaje(std::string linea,int& servidorOrigen,int& servidorDestino,std::string& mensaje);
		
};
#endif /*PARSER_H_*/
