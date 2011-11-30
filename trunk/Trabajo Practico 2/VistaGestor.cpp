#include <iostream>
#include <unistd.h>
#include <string>
#include "Gestor.h"

using namespace std;

int main(int argc, char * argv[]) {

	int pid = fork();

	if (pid != 0) { //padre
		char salir = '0';
		cout << "Para salir presione 's'" << endl;
		do {
			cin >> salir;
		} while (salir != 's' && salir != 'S');
		kill(pid, SIGINT);
	} else { //hijo
		Gestor gestor;
		gestor.inciar();
		cout << "Fin del proceso gestor" << endl;
	}

	return 0;
}
