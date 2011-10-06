#include "GestorDescargas.h"

GestorDescargas::GestorDescargas()
{
}

GestorDescargas::~GestorDescargas()
{
}

int GestorDescargas::iniciarRecepcion()
{
	// receptor de pedidos de archivos
	char buffer[BUFFSIZE];
	int pidOrigen = getppid();
	Fifo canal(Debug::intToString(pidOrigen));
	LockFile lockEscritura(Debug::intToString(pidOrigen) + ".lockEscritura");
	// event handler para la senial SIGINT (-2)
	SIGINT_Handler sigint_handler;
	SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );

	Debug::getInstance()->escribir("Receptor " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(pidOrigen) + ": esperando para leer . . .\n");

	// mientras no se reciba la senial SIGINT, el proceso realiza su trabajo
	while ( sigint_handler.getGracefulQuit() == 0 ) {
		int bytesLeidos = canal.leer(buffer, BUFFSIZE);
		if(bytesLeidos == 0){ // leyo eof porque todos los escritores cerraron el canal y hay que abrir y cerrar para que se bloquee en la lectura.
			canal.cerrar();
			canal.abrir();
			cout << "recibi eof Espero peticion" << endl;
			//bytesLeidos = canal.leer(buffer, BUFFSIZE);
		} else if (sigint_handler.getGracefulQuit() == 0) { //por si recibe la senial mientras lee
			lockEscritura.liberarLock();
			buffer[bytesLeidos] = '\0'; //controlar esto
			string linea(buffer);
			int pidDestino = atoi(linea.substr(0, linea.find("|", 0)).c_str());
			string path = linea.substr(linea.find("|", 0) + 1, linea.length());

			Debug::getInstance()->escribir("Receptor " + Debug::intToString(getpid()) + " del proceso "	+
					Debug::intToString(pidOrigen) + ": lei el dato [" + linea + "] del fifo " + Debug::intToString(pidOrigen) + "\n");

			//if (pidDestino != 0) { //por manejo incorrecto de fifo
			cout << "CREO HIJO " << endl;
			int pid = fork();
			if (pid == 0) {
				int resultado = enviar(pidOrigen, pidDestino,
						(char*) path.c_str());
				canal.eliminar(); //TODO chequear que este no quede colgado
				lockEscritura.cerrar();
				Debug::destruir();
				return resultado;
			}
		}
	}
	// se recibio la senial SIGINT, el proceso termina
	SignalHandler :: destruir ();
	cout << "ATRAPO LA SENIALLLLLLLLLLL" << endl;

	lockEscritura.eliminar();
	canal.eliminar();

	Debug::getInstance()->escribir("Receptor " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(pidOrigen) + ": fin del proceso\n");
	Debug::destruir();
	return 0;
}


int GestorDescargas::enviar(int pidOrigen, int pidDestino, char* buffer) {

	string fifo = Debug::intToString(pidOrigen) + "_" + Debug::intToString(pidDestino);
	Fifo canal(fifo);
	LockFile lock(fifo + ".lockEscritura");
	ifstream archivo;
	archivo.open(buffer,ifstream::in);// open del archivo path, lee una linea en binario
	char linea[BUFFSIZE*20];
	while(!archivo.eof()) {
		archivo.getline(linea,BUFFSIZE*20);
		lock.tomarLock();
		canal.escribir(linea,BUFFSIZE*20);
		Debug::getInstance()->escribir("Receptor " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(pidOrigen) + ": escribi linea [" + linea + "] en el fifo " + fifo + "\n");
	}
	archivo.close();
	lock.cerrar();
	canal.eliminar();
	return 0;
}

void GestorDescargas::enviarRuta(int pidOrigen, int pidDestino, string path)
{
	LockFile lockEscritura(Debug::intToString(pidOrigen) + ".lockEscritura");
	Fifo canal(Debug::intToString(pidOrigen));

	lockEscritura.tomarLock();
	string pidPath = Debug::intToString(pidDestino) + "|" + path;
	canal.escribir(pidPath.c_str(), pidPath.length());

	//canal.eliminar();
	lockEscritura.cerrar();
}

int GestorDescargas::descargar(string path, Usuario usuarioOrigen, Usuario usuarioDestino)
{
	enviarRuta(usuarioOrigen.getPid(), getpid(), path);

	string pathFifoDescarga = Debug::intToString(usuarioOrigen.getPid()) + "_" + Debug::intToString(getpid());
	Fifo canalDescarga(pathFifoDescarga);
	LockFile lockDescargaEscritura(pathFifoDescarga + ".lockEscritura");

	ofstream archivo;
	string pathTotal = "descargas_" + usuarioDestino.getNombre() + "_" + Debug::intToString(usuarioDestino.getPid()) + "/" + path;
	archivo.open(pathTotal.c_str(),ofstream::out);//open del archivo path,  lee una linea en binario
	char descarga[BUFFSIZE*20];
	while(canalDescarga.leer(descarga,BUFFSIZE*20) != 0) { // Estamos abusando de que cuando cierra el escritor lee eof.
		lockDescargaEscritura.liberarLock();
		archivo << descarga << endl; //TODO endl esta mal parche!!!
		archivo.flush();
		Debug::getInstance()->escribir("Descarga " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(getppid()) + ": descargue el dato [" + descarga + "] del fifo " + pathFifoDescarga + "\n");
	}
	archivo.close();
	canalDescarga.eliminar();
	lockDescargaEscritura.eliminar();
	Debug::getInstance()->escribir("Descarga " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(getppid()) + ": finaliza descarga\n");
	return 0;
}
