#include "Usuario.h"
#include <iostream>
#include <stdlib.h>

Usuario :: Usuario (string nombre, int pid) {

	this->nombre = nombre;//string
	
}

Usuario :: Usuario () {
}

Usuario :: ~Usuario () {
}


ostream & operator<<(ostream & salida, const Usuario & usuario){

	salida << "Archivos compartidos por: " << usuario.nombre << " pid: " << usuario.pid << endl;
	for(int i = 0; i < usuario.archivos.size(); i++)
		salida << i << " - " << usuario.archivos[i] << endl;
	return salida;
}

