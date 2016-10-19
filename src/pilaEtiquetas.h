/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : pilaEtiquetas.h			      *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __PILAETIQUETAS_H__
#define __PILAETIQUETAS_H__

typedef struct nodoPilaE{
      
       char *cadena;
       struct nodoPilaE *sgte ;

} PtrPilaE, *PilaE;

int pilaEtiquetasVacia(PilaE P);
void apilarEtiqueta(PilaE *P, char *cadena);
char *desapilarEtiqueta(PilaE *P);
char *cabeceraPilaEtiquetas(PilaE *P);

#endif
