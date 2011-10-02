#include "Debug.h"

Debug* Debug :: instance = NULL;

Debug :: Debug () : lock((char*)"debug"){
}

Debug* Debug :: getInstance () {

	if ( instance == NULL )
		instance = new Debug ();

	return instance;
}

void Debug :: destruir () {
	if ( instance != NULL ) {
		delete ( instance );
		instance = NULL;
	}
	lock.cerrar(); //TODO eliminar!!
}

void Debug :: escribir ( string mensaje ) {
	lock.tomarLock();
	lock.escribir((char*)mensaje.c_str(), mensaje.length());
	lock.liberarLock();

}
