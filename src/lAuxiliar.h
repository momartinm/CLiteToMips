/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : lAuxiliar.h                           *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __LAUXILIAR_H__
#define __LAUXILIAR_H__

typedef struct nodoAuxiliar {
       
       char * elemento;
       int tipo;
       int tipoDato;
       int posicion;
       char *indice;
       struct nodoAuxiliar *sgte;
       
}PtrlAuxiliar, *lAuxiliar;


int listaVaciaAuxiliar(lAuxiliar l);
lAuxiliar ultimoAuxiliar(lAuxiliar l);
char *obtenerAuxiliar(lAuxiliar l);
int obtenerTipoAuxiliar(lAuxiliar l);
int obtenerDatoAuxiliar(lAuxiliar l);
int obtenerPosicionAuxiliar(lAuxiliar l);
char *obtenerIndiceAuxiliar(lAuxiliar l);
lAuxiliar nuevoNodoAuxiliar(char *elemento, int tipoDato, int tipo, int posicion, char *indice);
void insertarAuxiliar(lAuxiliar *l, char *elemento, int tipoDato, int tipo, int posicion, char *indice);
void insertarNodoAuxiliar(lAuxiliar *l, lAuxiliar nodo);
void eliminarListaAuxiliar(lAuxiliar *l);

#endif
