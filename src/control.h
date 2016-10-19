/******************************************************
* Programa    : MIPS R3000                            *
* M�dulo      : control.h                             *
* Autor       : Moises Mart�nez Mu�oz                 *
* Autor       : Ignacio �lvarez Santiago              *
******************************************************/

#ifndef __CONTROL_H__
#define __CONTROL_H__


struct control
{
 	int linea;	
   	int funcionActiva;
	int log; 
   	int logfichero;	int expresion;
	int tabuladores;
	int constantesEns;
	int variablesEns;
	int leidoIf;
	int leidoWhile;
	int leidoFor;
	int profundidad;
	int retorno;
};

extern struct control eControl;


void iniciarControl();
void activarFlag(int flag);
void desactivarFlag(int flag);
int valorFlag(int flag);
void incrementarLinea();
int mayor(int tipo1, int tipo2);
int operadorInverso(char *caracter);
int esOperador(char *caracter);
void iniciarCadena(char cadena[], int tam);
int prioridad (char *cad);
int desplazamiento(int tipo);
int posibleNumero(char *cadena);

#endif
