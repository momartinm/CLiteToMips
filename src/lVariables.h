/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : lVariables.h                          *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __LVARIABLES_H__
#define __LVARIABLES_H__

#include "lDimensiones.h"

typedef struct nodoVariable {
       
       char *nombre;						//Nombre de la variable
       int tipo;							//Tipo de variable
       int ambito;						//Ambito de la variable
	    int tamano;						//Tamano en el caso de que sea un array
	    lDimensiones dimensiones;		//Valor de la dimensiones
	    char *valor;						//valor de la variable
	    int offset;						//Valor del offset de la variable
       struct nodoVariable *sgte;
       
}PtrtVariables, *lVariables;

int listaVaciaVariable(lVariables lv);
lVariables ultimaVariable(lVariables lv);
void insertarVariable(lVariables *lv, char* var, int tipo, int ambito, int tam, char *dimensiones, char *valor, int offset);
char *obtenerNombreVariable(lVariables lv);
int obtenerTipoVariable(lVariables lv);
int obtenerAmbitoVariable(lVariables lv);
int obtenerTamanoVariable(lVariables lv);
int obtenerMulDimensiones(lVariables l, int dimFinal);
//int obtenerTamanosVariable(lVariables lv, int dimension);
char *obtenerValorVariable(lVariables lv);
int obtenerOffsetVariable(lVariables lv);
int obtenerTipoParametro(lVariables lv, int parametro);
int obtenerNumeroParametros(lVariables lv);
int obtenerTamanoArray(lVariables lv);
lVariables buscarVariable(lVariables lv, char* var);
void eliminarListaVariables(lVariables *lv);

#endif
