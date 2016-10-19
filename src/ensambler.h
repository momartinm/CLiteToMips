/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : emsambler.h                           *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __ENSAMBLER_H__
#define __ENSAMBLER_H__

typedef struct nodoEnsambler 
{
       
		 char *variable;
		 char *funcion;
       int  direccion;
	    char tamano; 		//1: 32 bits | 2: 64 bits
	    char tipo;   		//0 direccion 1 valor
	    
       struct nodoEnsambler *sgte;
       
}PtrtEnsambler, *pEnsambler;


int ensamblerVacio(pEnsambler E);
pEnsambler ultimoEnsambler(pEnsambler E);
void insertarEnsambler(pEnsambler *E, char *variable, char *funcion, int direccion, char tamano, char tipo);
void eliminarEnsambler(pEnsambler *E);

#endif
