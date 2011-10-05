#include "Usuario.h"

Usuario :: Usuario ()
{
}

Usuario :: ~Usuario ()
{
}

Usuario :: Usuario (string nombre, int pid) {

	this->nombre = nombre;
	this->pid = pid;
	
}

Usuario ::Usuario (const Usuario& usuario)
{
	nombre = usuario.nombre;
	pid = usuario.pid;
	archivos = usuario.archivos;
}

ostream & operator<<(ostream & salida, const Usuario & usuario)
{
	salida << "Usuario: " << usuario.nombre << " pid: " << usuario.pid << endl;
	for(int i = 0; i < usuario.archivos.size(); i++)
		salida << "\t" << i << " - " << usuario.archivos[i] << endl;
	return salida;
}
/*
bool OrdenDeAnulacion::operator==(const Orden & otra) const
{
	return false;
}
*/
