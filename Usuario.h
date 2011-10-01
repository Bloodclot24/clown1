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

};

#endif /* FIFO_H_ */
