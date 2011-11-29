#include <iostream>
#include <unistd.h>
#include <string>
#include "Gestor.h"

using namespace std;

int main(int argc, char * argv[]) {

	int pid = fork();

	if (pid != 0) {
		cout << "construc" << endl;
		Gestor gestor("archivoBaseDatos");
		cout << "iniciar" << endl;
		gestor.inciar();
		cout << "Fin del proceso gesstor" << endl;
	} else {
		char salir = '0';
		cout << "Para salir presione 's'" << endl;
		do {
			cin >> salir;
		} while (salir != 's');
		kill(pid, SIGTERM);
	}

	return 0;
}
