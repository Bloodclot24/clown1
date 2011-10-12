#include "ConcuShare.h"

ConcuShare::ConcuShare() {

}

ConcuShare::~ConcuShare() {

}

int ConcuShare::compartirArchivos()
{
	string ruta;
	int numero;
	bool salir = false;

	while (!salir) {
		Vista::mostrarMenuCompartir();
		bool valido = false;
		char opcion = Vista::pedirChar();

		switch (opcion)
		{
			case '1':
				while (!valido) {
					Vista::mostrarMensajeInicial("Ingrese la ruta del archivo : ");
					ruta = Vista::pedirString();
					if (open(ruta.c_str(), O_RDONLY) == -1) {
						Vista::mostrarMensaje("Archivo no valido, intente nuevamente.");
					} else {
						gestorUsuarios.agregarArchivo(ruta, usuario);
						Vista::mostrarMensajeFinal("Archivo " + ruta + " compartido");
						valido = true;
					}
				}
				break;

			case '2':
				Vista::mostrarUsuario(usuario);
				Vista::mostrarMensajeInicial("Ingrese el numero del archivo : ");
				numero = Vista::pedirInt(0, usuario.getArchivos().size() - 1);
				ruta = usuario.getArchivos()[numero];
				gestorUsuarios.eliminarArchivo(ruta, usuario);
				Vista::mostrarMensajeFinal("Ha dejado de compartir el archivo " + ruta);
				break;

			case '3':
				salir = true;
				break;

			default:
				Vista::mostrarMensaje("No es una opcion valida, intente nuevamente.");
				break;
		}
	}
	return PADRE;
}


int ConcuShare::descargarArchivo(Usuario usuarioOrigen)
{
	Vista::mostrarUsuario(usuarioOrigen);
	Vista::mostrarMensaje("Ingrese el numero de archivo que desea descargar");
	int numero = Vista::pedirInt(0, usuarioOrigen.getArchivos().size() - 1);
	Vista::mostrarMensajeFinal("");

	int pidDescarga = fork();
	if(pidDescarga == HIJO) {
		string archivo = usuarioOrigen.getArchivos()[numero];
		Debug::getInstance()->escribir("Usuario " + usuario.getNombre() + " inicia descarga en proceso " + Debug::intToString(usuario.getPid()) + "\n");
		gestorDescargas.descargar(archivo, usuarioOrigen, usuario);
		Vista::mostrarMensaje("Archivo " + archivo + " descargado");
		return HIJO;
	}
	hijos.insert(hijos.end(), pidDescarga);
	return PADRE;
}

int ConcuShare::buscarArchivos()
{
	vector<Usuario> usuarios = gestorUsuarios.buscarArchivos();

	if(usuarios.size() == 0 || (usuarios.size() == 1 && usuarios[0] == usuario))
		Vista::mostrarMensajeFinal("No hay archivos compartidos por otros usuarios.");
	else {

		Vista::mostrarArchivos(usuarios, usuario);
		Vista::mostrarMensaje("Desea seleccionar un archivo para descargar? (s/n)");
		int numero;
		bool salir = false;
		while (!salir) {
			char opcion = Vista::pedirChar();
			switch (opcion) {
			case 's':
				Vista::mostrarMensaje("Ingrese el numero del usuario al que pertenece el archivo : ");
				numero = Vista::pedirInt(0, usuarios.size() - 1);
				if (descargarArchivo(usuarios[numero]) == HIJO)
					return HIJO;
				salir = true;
				break;

			case 'n':
				salir = true;
				break;

			default:
				Vista::mostrarMensaje("No es una opcion valida, intente nuevamente.");
				break;
			}
		}
	}

	return PADRE;
}

int ConcuShare::ejecutarMenu()
{
	Debug::getInstance()->escribir("Usuario " + usuario.getNombre() + " en proceso " + Debug::intToString(usuario.getPid()) + "\n"); //ver si esto va aca

	bool salir = false;
	while (!salir) {
		Vista::mostrarMenu();
		char opcion = Vista::pedirChar();
		switch (opcion)
		{
			case '1':
				compartirArchivos();
				break;

			case '2':
				if(buscarArchivos() == HIJO) {
					gestorUsuarios.cerrar();
					return HIJO;
				}
				break;

			case '3':
				Vista::mostrarMensaje("Fin del Programa");
				gestorUsuarios.eliminarUsuario(usuario);
				salir = true;
				break;

			default:
				Vista::mostrarMensaje("No es una opcion valida, intente nuevamente.");
				break;
		}
	}
	return PADRE;

}

void ConcuShare::crearDirectorioDescargas(string nombre)
{
	string directorio = "descargas_" + nombre + "_" + Debug::intToString(getpid()) + "/";
	int estado = mkdir(directorio.c_str(), 0700);
	if(estado >= 0)
		Debug::getInstance()->escribir("Proceso " + Debug::intToString(getpid()) + ": creo el directorio " + directorio + " para realizar las descargas\n");
}

int ConcuShare::parsearLineaDeComandos(int argc, char** argv)
{
	int ch;
	Debug::getInstance();
	while ((ch = getopt(argc, argv, "hd")) != -1) {
		switch (ch) {
		case 'h':
			Vista::mostrarUso(argv[0]);
			return SALIR;
			break;
		case 'd':
			Debug::setModoDebug();
			break;
		default:
			Vista::mostrarUso(argv[0]);
			return SALIR;
			break;
		}
	}
	return 0;
}

int ConcuShare::iniciar(int argc, char** argv)
{
	if(parsearLineaDeComandos(argc, argv) == SALIR)
		return 0;

	int pid = fork ();
	if(pid == HIJO){
		int resultado = gestorDescargas.iniciarRecepcion();
		Debug::getInstance()->escribir("Recepcion de usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
		Debug::destruir();
		return resultado;
	}

	Vista::mostrarBienvenida();
	string nombre = Vista::pedirString();
	Usuario usuario(nombre,getpid());
	this->usuario = usuario;

	crearDirectorioDescargas(nombre);

	if(ejecutarMenu() == HIJO) {
		Debug::getInstance()->escribir("Descarga de usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
	} else {
		kill(pid,SIGINT);
		gestorDescargas.esperarFinalizacionDescargas(hijos);
		Debug::getInstance()->escribir("Usuario en proceso " + Debug::intToString(getpid()) + "finaliza el proceso\n");
	}
	Debug::destruir();
	return 0;
}
