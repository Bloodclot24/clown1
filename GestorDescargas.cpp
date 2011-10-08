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
			Vista::debug("recibi eof Espero peticion");
		} else if (sigint_handler.getGracefulQuit() == 0) { //por si recibe la senial mientras lee
			lockEscritura.liberarLock();
			buffer[bytesLeidos] = '\0'; //controlar esto
			string linea(buffer);
			int pidDestino = atoi(linea.substr(0, linea.find("|", 0)).c_str());
			string path = linea.substr(linea.find("|", 0) + 1, linea.length());

			Debug::getInstance()->escribir("Receptor " + Debug::intToString(getpid()) + " del proceso "	+
					Debug::intToString(pidOrigen) + ": lei el dato [" + linea + "] del fifo " + Debug::intToString(pidOrigen) + "\n");

			Vista::debug("CREO HIJO ");
			int pid = fork();
			if (pid == 0) {
				int resultado = enviar(pidOrigen, pidDestino, (char*) path.c_str());
				//canal.cerrar(); //TODO chequear que este no quede colgado
				lockEscritura.cerrar();
				Debug::destruir();
				SignalHandler :: destruir ();
				return resultado;
			}
		}
	}
	// se recibio la senial SIGINT, el proceso termina
	SignalHandler :: destruir ();
	Vista::debug("ATRAPO LA SENIALLLLLLLLLLL");

	lockEscritura.eliminar();
	canal.eliminar();

	Debug::getInstance()->escribir("Receptor " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(pidOrigen) + ": fin del proceso\n");
	Debug::destruir();
	return 0;
}


int GestorDescargas::enviar(int pidOrigen, int pidDestino, char* archivo) {

	string fifo = Debug::intToString(pidOrigen) + "_" + Debug::intToString(pidDestino);
	Fifo canal(fifo);
	LockFile lock(fifo + ".lockEscritura");
	int fd = open(archivo, O_RDONLY); //abre el file descriptor para lectura
	char buffer[BUFFSIZE];
	int resultado;
	while((resultado = read(fd, (void *) buffer, BUFFSIZE)) != 0) {
		lock.tomarLock();
		canal.escribir(buffer,resultado);
		string linea(buffer);
		Debug::getInstance()->escribir("Receptor " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(pidOrigen) + ": escribi linea [" + linea + "] en el fifo " + fifo + "\n");
	}
	close(fd);
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

	canal.eliminar();
	lockEscritura.cerrar();
}

int GestorDescargas::descargar(string path, Usuario usuarioOrigen, Usuario usuarioDestino)
{
	enviarRuta(usuarioOrigen.getPid(), getpid(), path);

	string pathFifoDescarga = Debug::intToString(usuarioOrigen.getPid()) + "_" + Debug::intToString(getpid());
	Fifo canalDescarga(pathFifoDescarga);
	LockFile lockDescargaEscritura(pathFifoDescarga + ".lockEscritura");

	string pathTotal = "descargas_" + usuarioDestino.getNombre() + "_" + Debug::intToString(usuarioDestino.getPid()) + "/" + path;
	int fd = open(pathTotal.c_str(), O_CREAT | O_WRONLY);
	char descarga[BUFFSIZE];
	int resultado;
	while((resultado = canalDescarga.leer(descarga,BUFFSIZE)) != 0) { // Estamos abusando de que cuando cierra el escritor lee eof.
		lockDescargaEscritura.liberarLock();
		write(fd, (const void *) descarga, resultado);
		string linea(descarga);
		Debug::getInstance()->escribir("Descarga " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(getppid()) + ": descargue el dato [" + linea + "] del fifo " + pathFifoDescarga + "\n");
	}
	close(fd);
	canalDescarga.eliminar();
	lockDescargaEscritura.eliminar();
	Debug::getInstance()->escribir("Descarga " + Debug::intToString(getpid()) + " del proceso " + Debug::intToString(getppid()) + ": finaliza descarga\n");
	return 0;
}
