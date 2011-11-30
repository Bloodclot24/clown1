#!/bin/sh

g++ VistaGestor.cpp BaseDatos.cpp SignalHandler.cpp Semaforo.cpp BloqueDeRegistros.cpp LockFile.cpp Gestor.cpp -o gestor 

g++  VistaCliente.cpp Cliente.cpp Persona.cpp -o cliente



