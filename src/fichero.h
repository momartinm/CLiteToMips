/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : fichero.h                             *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __FICHERO_H__
#define __FICHERO_H__

int abrirFichero(char *nombre, int fichero);
int escribirFichero(char *cadena, int fichero, int operacion);
void cerrarFichero(int fichero);
void cerrarFicheroIncompleto(int fichero);

#endif
