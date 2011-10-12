#ifndef USUARIO_H_
#define USUARIO_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Usuario {

	friend ostream & operator<<(ostream &, const Usuario &);
private:
	string nombre;
	int pid;
	vector<string> archivos;

public:
	Usuario();
	Usuario(string nombre, int pid);
	Usuario(const Usuario& usuario);
	virtual ~Usuario();

	vector<string> getArchivos();

	int getPid();

	void agregarArchivo(string archivo);

	void eliminarArchivo(string archivo);

	string getNombre();

	bool operator==(const Usuario & otro) const;

	bool operator!=(const Usuario & otro) const;

};

#endif /* USUARIO_H_ */
