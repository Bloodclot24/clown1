#ifndef DEBUG_H_
#define DEBUG_H_

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>

#include "LockFile.h"

using namespace std;

class Debug {
private:
	static Debug* instance;
	static LockFile lock;
	static bool modoDebug;
	Debug();

public:
	static Debug* getInstance();

	static void setModoDebug();

	static void destruir();

	static void escribir(string mensaje);

	static string intToString(int entero);
};

#endif /* DEBUG_H_ */
