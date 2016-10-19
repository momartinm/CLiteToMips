/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : arbol.h                               *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/


#ifndef __ARBOL_H__
#define __ARBOL_H__

typedef struct nodoArbol {
       
       char *elemento;
       int tipo;
       int tipoDato;
       int posicion;
       char *indice;
       int *etiquetaOk;
       int *etiquetaNook;
       int *etiqueta;
       struct nodoArbol *dcho;
       struct nodoArbol *izdo;
       
}PtrArbol, *arbol;


int arbolVacio(arbol a);
arbol nuevoNodoArbol(char *elemento, int tipo, int tipoDato, int posicion, char* indice);
void insertarArbolinsertarArbol(arbol *a, char *elemento, int tipo, int tipoDato, int posicion, char* indice);
void insertarNodoArbol(arbol *a, arbol nodo);
int *obtenerEtiquetaOk(arbol a);
int *obtenerEtiquetaNook(arbol a);
int *obtenerEtiqueta(arbol a);
void crearEtiqueta(arbol a, int valor);
void crearEtiquetaOk(arbol a, int valor);
void crearEtiquetaNook(arbol a, int valor);
void modificarEtiquetaOk(arbol a, int *valor);
void modificarEtiquetaNook(arbol a, int *valor);
void eliminarNodo(arbol *a);

#endif
