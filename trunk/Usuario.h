#ifndef USUARIO_H_
#define USUARIO_H_

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

using namespace std;

class Usuario {

	friend ostream & operator<<(ostream &, const Usuario &);
private:
	string nombre;
	int pid;
	vector<string> archivos;

public:
	Usuario(string nombre, int pid);
	Usuario(const Usuario& usuario);
	Usuario();
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