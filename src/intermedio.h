/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : intermedio.h            	            *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __INTERMEDIO_H__
#define __INTERMEDIO_H__

#include "tSimbolos.h"
#include "arbol.h"
#include "pila.h"
#include "pilaArbol.h"
#include "pilaEtiquetas.h"
#include "lAuxiliar.h"
#include "control.h"


//Cabeceras de las funciones

lAuxiliar generarPostFijo(lAuxiliar expresion);
arbol generarArbolExpresion(lAuxiliar *A);
void buscarRegistro(arbol nodo, char *reg);
void generarRegistro(arbol nodo, tSimbolos TS, char *reg, int tipoFuncion, int auxiliares);
void eliminarRegistro(arbol nodo, char * reg);
void generarInstruccionesExpresion(arbol a, tSimbolos TS, int tipoFuncion);
void etiquetarArbol(arbol a, int tipo, arbol padre);
void generarInstruccionesBucle(arbol a, tSimbolos TS, int tipo, arbol padre, arbol raiz, int tipoFuncion);
void tratarExpresion(arbol a, tSimbolos TS);
void generarDatosFor(lAuxiliar *expresion, PilaE *pFor);
lAuxiliar obtenerDatosFor(tSimbolos TS, PilaE *pFor);

#endif
