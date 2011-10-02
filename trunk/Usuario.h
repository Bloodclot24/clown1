#ifndef USUARIO_H_
#define USUARIO_H_

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

class Usuario {
		
		friend ostream & operator<<(ostream &, const Usuario &);
	private:
		string nombre;
		int pid;
		vector<string> archivos; //deberia ser una lista!!

	public:
		Usuario (string nombre, int pid);
		Usuario (const Usuario& usuario);
		Usuario ();
		virtual ~Usuario();

		vector<string> getArchivos()
		{
			return archivos; //copia
		}

		int getPid()
		{
			return pid;
		}

		void agregarArchivo(string archivo)
		{
			archivos.insert(archivos.end(), archivo);
		}

		string getNombre()
		{
			return nombre;
		}

};

#endif /* USUARIO_H_ */
