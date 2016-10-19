/*******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : lDimensiones.h                        *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __LDIMENSIONES_H__
#define __LDIMENSIONES_H__


typedef struct nodoDimension {
       
       int dimension;						//Tamaño de la dimension
       struct nodoDimension *sgte;
       
}PtrlDimensiones, *lDimensiones;

void insertarDimension(lDimensiones *l, int dimension);
void eliminarListaDimension(lDimensiones *l);
int calcularDimensiones(lDimensiones l);
int contarRestoDimensiones(lDimensiones l, int dimFinal);

#endif
