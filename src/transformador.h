/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : transformador.h                       *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#ifndef __TRANSFORMADOR_H__
#define __TRANSFORMADOR_H__

#define NUMREGISTROS     56
#define INICIOENTEROS     0
#define FINENTEROS       17
#define INICIOFLOTANTES  18
#define FINFLOTANTES	 49
#define INICIOTEMPORALES 50
#define FINTEMPORALES    53
#define INICIOARGUMENTOS 54
#define FINARGUMENTOS    55

#include "tSimbolos.h"
#include "control.h"
#include "fichero.h"

typedef struct
{
	char id[256];
	long referencia;	 //Marca de tiempo última utilización
	char estado;  		 //0 vacio 1 utilizado 2 descartado 3 reservado
	char tipo;    		 //0 direccion 1 valor
	char auxiliar;		 //0 variable 1 auxiliar
	int  profundidad;
	int  tipoRegistro; //Indica el tipo de registro 1: 2: 3: 4:
} contenido;

contenido eRegistros[NUMREGISTROS];

//Funciones generales de control de registros de ensamblador

void iniciarRegistros();
void modificarRegistro(int tipo, int registro, char estado, char *id, char tipoElemento, char auxiliar);
void obtenerRegistro(int tipo, int posicion, char *registro);
int obtenerRegistroLibre(int tipo, char *variable, char *registro, char auxiliar, char tipoRegistro);
void obtenerRegistroOcupado(int tipo, char *variable, char *registro);
int obtenerPosicionRegistro(char *registro);
int esRegistroAuxiliar(int registro);
int tipoRegistro(int tipo, char *registro);
void eliminarRegistroProfundos(int profundidad);
void liberarRegistrosAuxiliares();

//Funciones de generación de codigo ensamblador

void generarDefiniciones();
void generarCabeceraPrograma();
void generarConstante(char *nombre, int tipo, char *valor);
void generarVariables(char *nombre, int tipo, int tamano);
void generarInstruccionCarga(char *registro, char *registroAuxiliar, char *carga, int tipoDato, int offset);
void generarInstruccionAlmacenamiento(char *registro, char *registroAuxiliar, char *carga, int tipoDato, int offset);
void generarInstruccionOperacion(char *resultadoOp, char *operando1, char *operando2, int tipoDato, int tipoOperacion);
void generarTransformacion(int tipo, char *operando, int tipoDato, char *elemento);
//void generarIndiceArray(int tipoDato, int posicion, char *indice, char *elemento, char *registro, int operacion);
//void generarArray(tSimbolos TS, int tipoDato, char *elemento, int posicion, char *indice, char *registro, int operacion);
void generarArray(tSimbolos TS, int tipoDato, int tipoSalida, char *elemento, char *indice, char *registro, int operacion);
void generarSaltosEntero(int salto, int tipo, char *registro1, char *registro2, int *etiqueta, int *etiquetaNook, int tipoEtiqueta);
void generarMovimiento(int tipo, char *registroOrigen, char *registroDestino);
void solicitarFuncion(char *funcion);
void definirEntrada(int tipo);
void generarSalida(tSimbolos TS, char *variable, char *dimensiones);
void generarSalida(tSimbolos TS, char *variable, char *dimensiones);
void imprimirSaltoLinea();
void imprimirFinalPrograma();
void imprimirCadenasExpeciales();

#endif

