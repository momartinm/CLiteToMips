/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : tSimbolos.h                           *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __TSIMBOLOS_H__
#define __TSIMBOLOS_H__

#include "lVariables.h"

typedef struct nodolista{
       
       char* nombre;					//Nombre de la funcion
       int tipo;						//Tipo de retorno de la funcion
       int tamano;					//Tamano de la funcion en pila
       int tamUltimo;				//Tamaño del último tipo de dato o retorno
       lVariables variables;		//Lista de variables asociadas a la funcion
       struct nodolista *sgte;
       
}PtrtSimbolos, *tSimbolos;


int listaVacia(tSimbolos Ts);
tSimbolos ultimo(tSimbolos Ts);
void insertarSimbolo(tSimbolos *Ts, char* var, int tipo);
char *obtenerNombre(tSimbolos Ts);
int obtenerTipo(tSimbolos Ts);
tSimbolos buscarSimbolo(tSimbolos Ts, char* var);
int insertarNuevaVariable(tSimbolos Ts, int opcion, char* variable, int tipo, int ambito, int tam, char *dimensiones, char *valor);
void eliminarLista(tSimbolos *Ts);
int obtenerTamanoTipoDato(int tipo);
int existeFuncion(tSimbolos Ts, char *nombreFuncion);
int existeVariableGlobal(tSimbolos Ts, char *nombreVariable);
int existeVariable(tSimbolos Ts, char *nombreVariable);
int tipoParametro(tSimbolos Ts, char *funcion, int parametro);
int numeroParametros(tSimbolos Ts, char *funcion);
int obtenerValoresVariable(tSimbolos Ts, char *nombreVariable, int parametro);
char *obtenerValorConstante(tSimbolos Ts, char *nombreVariable);
int obtenerOffsetFuncion(tSimbolos Ts, char *funcion);
int obtenerOffsetArgumento(tSimbolos Ts, char *nombrefuncion, int argumento);
int ObtenerOffSetVariable(tSimbolos Ts, char *nombrefuncion, char *argumento);
int obtenerOffsetVariableActual(tSimbolos Ts, char *nombreVariable);
void generarVariablesPrograma(tSimbolos Ts, int ambito);
void generarArraysPrograma(tSimbolos Ts);

#endif


