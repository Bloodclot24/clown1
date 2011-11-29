#ifndef PERSONA_H_
#define PERSONA_H_

#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

class Persona {
//TODO ver si es necesaria esta clase
	friend ostream & operator<<(ostream &, const Persona &);
private:
	string nombre;
	string direccion;
	string telefono;
public:
	Persona(char* nombre, char* direccion, char* telefono);
	virtual ~Persona();

	string getNombre() {
		return nombre;
	}
};

#endif /* PERSONA_H_ */
