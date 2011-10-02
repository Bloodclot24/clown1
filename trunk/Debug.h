#ifndef DEBUG_H_
#define DEBUG_H_

#include <signal.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "LockFile.h"

using namespace std;

class Debug
{
	private:
		static Debug* instance;
		LockFile lock;
		Debug();

	public:
		static Debug* getInstance ();
		static void destruir ();
		static void escribir ( string mensaje );
};

#endif /* DEBUG_H_ */
