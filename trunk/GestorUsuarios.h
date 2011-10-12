#ifndef GESTOR_USUARIOS_H_
#define GESTOR_USUARIOS_H_

#include <stdlib.h>

#include "Usuario.h"
#include "LockFile.h"
#include "Debug.h"

#define	ARCHIVO_LOCK_USUARIOS	"archivo_usuarios"

using namespace std;

class GestorUsuarios {

private:
	vector<Usuario> usuarios;
	LockFile lock;

	void actualizarUsuarios();

	int escribir(string ruta, int pid, string nombre);

	int leer(string& ruta, int& pid, string& nombre);

	int parsearLinea(string linea, string& nombre, int& pid, string& archivo);

public:
	GestorUsuarios();
	virtual ~GestorUsuarios();

	int eliminarUsuario(Usuario usuario);

	int agregarArchivo(string archivo, Usuario& usuario);

	int eliminarArchivo(string archivo, Usuario& usuario);

	vector<Usuario> buscarArchivos();

	void cerrar();
};

#endif /* GESTOR_USUARIOS_H_ */
