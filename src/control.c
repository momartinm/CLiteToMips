/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : control.c                             *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "control.h"

struct control eControl;

/*******************************************************************************
Esta método nos permite inicializar todos los valore de control que deben ser 
inicializados cuando se ejecuta un nuevo subprograma.
*******************************************************************************/
void iniciarControl()
{
     eControl.linea         = 1;
     eControl.funcionActiva = 0;
     eControl.log           = 0;
     eControl.logfichero    = 0;
     eControl.expresion     = 0;
     eControl.tabuladores   = 0;
     eControl.variablesEns  = 0;
     eControl.constantesEns = 0;
     eControl.leidoIf 	    = 0;
     eControl.leidoWhile    = 0;
     eControl.leidoFor      = 0; 
     eControl.profundidad   = 1;
     eControl.retorno       = 0;
}

/*******************************************************************************
Esta método nos permite activar cualquiera de los flag de la variable eControl.
*******************************************************************************/
void activarFlag(int flag)
{
     switch(flag)
     {			case 1: eControl.funcionActiva = 1;
				break;
        		case 2: eControl.log = 1;
				break;
       			case 3: eControl.logfichero = 1;
				break;
			case 4: eControl.expresion = 1;
			   	break;
			case 5: eControl.tabuladores++;
            			break;
			case 6: eControl.variablesEns = 1;
				break;
			case 7: eControl.constantesEns = 1;
				break;
			case 8: eControl.leidoIf = 1;
				break;
			case 9: eControl.leidoWhile = 1;
				break;
			case 10: eControl.leidoFor = 1;
				 break;
			case 11: eControl.profundidad++;
				 break;
			case 12: eControl.retorno = 1;
				 break; 				
     }
}



/*******************************************************************************
Esta método nos permite activar cualquiera de los flag de la variable eControl.
*******************************************************************************/
void desactivarFlag(int flag)
{
     switch(flag)
     {
			case 1: eControl.funcionActiva = 0;
				break;
			case 2: eControl.log = 0;
             			break;
      			case 3: eControl.logfichero = 0;
      	         		break;
			case 4: eControl.expresion = 0;
                 		break;
         		case 5: eControl.tabuladores--;
             			break;
			case 6: eControl.variablesEns = 0;
				break;
			case 7: eControl.constantesEns = 0;
				break;
			case 8: eControl.leidoIf = 0;
				break;
			case 9: eControl.leidoWhile = 0;
				break;
			case 10: eControl.leidoFor = 0;
				 break; 
			case 11: eControl.profundidad--;
				 break; 
			case 12: eControl.retorno = 0;
				 break; 
     }
}


/*******************************************************************************
Esta funcón nos devuelve el valor de cualquiera de los flag de la variable
eControl.
*******************************************************************************/
int valorFlag(int flag)
{
      switch(flag)
      {
			case 0:return eControl.linea;
				    break;
			case 1:return eControl.funcionActiva;
				    break;
			case 2:return eControl.log;					 break;			case 3:return eControl.logfichero;					 break;
			case 4:return eControl.expresion;
					 break;
			case 5:return eControl.tabuladores;
					 break;
			case 6:return eControl.variablesEns;
					 break;
			case 7:return eControl.constantesEns;
					 break;
			case 8:return eControl.leidoIf;
					 break;
			case 9:return eControl.leidoWhile;
					 break;
			case 10:return eControl.leidoFor;
					 break;
			case 11:return eControl.profundidad;
					 break;
			case 12:return eControl.retorno;
					break;
      }
}

/*******************************************************************************
Este método sirve para asignar la posición de la nueva rutina.
*******************************************************************************/
void incrementarLinea()
{
     eControl.linea++;
}

//Funciones auxiliar para la gestión del análisis semántico

int mayor(int tipo1, int tipo2)
{
	if (tipo1 > tipo2)
		return tipo1;
	else
		return tipo2;
}


int operadorInverso(char *caracter)
{
	if (strcmp(caracter,"==") == 0)
		return 16;
	if (strcmp(caracter,"<=") == 0)
		return 15;
	if (strcmp(caracter,">=") == 0)
		return 14;
	if (strcmp(caracter,"<") == 0)
		return 13;
	if (strcmp(caracter,">") == 0)
		return 12;
	if (strcmp(caracter,"!=") == 0)
		return 11;

	return 0;
}

int esOperador(char *caracter)
{
	
	if (strcmp(caracter,"+") == 0)
		return 1;
	if (strcmp(caracter,"-") == 0)
		return 2;
	if (strcmp(caracter,"*") == 0)
		return 3;
	if (strcmp(caracter,"/") == 0)
		return 4;
	if (strcmp(caracter,"%") == 0)
		return 5;
	if (strcmp(caracter,"&") == 0)
		return 6;
	if (strcmp(caracter,"|") == 0)
		return 7;
	if (strcmp(caracter,")") == 0)
		return 8;
	if (strcmp(caracter,"(") == 0)
		return 9;
	if (strcmp(caracter,"=") == 0)
		return 10;
	if (strcmp(caracter,"==") == 0)
		return 11;
	if (strcmp(caracter,"<=") == 0)
		return 12;
	if (strcmp(caracter,">=") == 0)
		return 13;
	if (strcmp(caracter,"<") == 0)
		return 14;
	if (strcmp(caracter,">") == 0)
		return 15;
	if (strcmp(caracter,"!=") == 0)
		return 16;
	return 0;
}

void iniciarCadena(char cadena[], int tam)
{
	int i;
	
	for (i=0; i<tam; i++)
		cadena[i] = 0;
}

int prioridad (char *cad) 
{
	if (strcmp (cad, (char *) "=") == 0)
		return 0;
	if (strcmp (cad, (char *) "&") == 0)
		return 0;
	if (strcmp (cad, (char *) "|") == 0)
		return 0;
	if (strcmp (cad, (char *) "==") == 0)
		return 1;
	if (strcmp (cad, (char *) "<=") == 0)
		return 1;
	if (strcmp (cad, (char *) ">=") == 0)
		return 1;
	if (strcmp (cad, (char *) "<") == 0)
		return 1;
	if (strcmp (cad, (char *) ">") == 0)
		return 1;
	if (strcmp (cad, (char *) "!=") == 0)
		return 1;
	if (strcmp (cad, (char *) "+") == 0)
		return 1;
	if (strcmp (cad, (char *) "-") == 0)
		return 1;
	if (strcmp (cad, (char *) "*") == 0)
		return 2;
	if (strcmp (cad, (char *) "/") == 0)
		return 2;
	if (strcmp (cad, (char *) "%") == 0)
		return 2;
}

int desplazamiento(int tipo)
{
	if (tipo == 0)
		return 1;
	if (tipo == 1)
		return 2;
	if (tipo == 2)
		return 4;
	if (tipo == 3)
		return 4;
	if (tipo == 4)
		return 8;	
}

int posibleNumero(char *cadena)
{
	char caracter = cadena[0];
	
	if ((caracter > 47) && (caracter < 58))
		return 1;
	else
		return 0;	
}
