#include "Persona.h"

Persona::Persona(char* nomb, char* dir, char* tel) : nombre(nomb), direccion(dir), telefono(tel) {
}

Persona::~Persona() {
}

ostream & operator<<(ostream & salida, const Persona & persona) {
	salida << "Persona: " << persona.nombre << endl;
	salida << "Direccion: " << persona.direccion << endl;
	salida << "Telefono: " << persona.telefono << endl;
	return salida;
}
