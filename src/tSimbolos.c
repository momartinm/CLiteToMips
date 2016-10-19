/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : tSimbolos.c                           *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tSimbolos.h"
#include "transformador.h"

/*******************************************************************************
Esta funcin nos indica si una lista esta vacia.
*******************************************************************************/
int listaVacia(tSimbolos Ts)
{
	return Ts == NULL;
}


/*******************************************************************************
Esta funcin devuelve el ltimo elemento de una lista de variables.
*******************************************************************************/
tSimbolos ultimo(tSimbolos Ts)
{
	while (!listaVacia(Ts->sgte))
		Ts = Ts->sgte;
	return Ts;
}

static tSimbolos nuevoNodo(char* var, int tipo)
{
	tSimbolos aux;
	aux = (tSimbolos) malloc(sizeof(PtrtSimbolos));
	aux->nombre     = malloc(sizeof(char)*(strlen(var)+1));
	strcpy(aux->nombre,var);
	aux->tipo       = tipo;
	aux->tamano     = 0;
	aux->tamUltimo  = obtenerTamanoTipoDato(tipo);
	aux->variables  = NULL;
	aux->sgte       = NULL;
	
	//printf("TAMANO FUNCION %d\n", obtenerTamanoTipoDato(tipo));
	
	return aux;
}

/*******************************************************************************
Este metodo almacena una nueva variable indicando el tipo de variable y la linea
en la que ha sido definida esa variable.
*******************************************************************************/
void insertarSimbolo(tSimbolos *Ts, char* var, int tipo)
{
     tSimbolos nuevo = nuevoNodo(var,tipo);

     if (listaVacia(*Ts))
        *Ts = nuevo;
     else
         ultimo(*Ts)->sgte = nuevo;
}


/*******************************************************************************
Esta funcin devuelve el nombre de la variable almacenada en L.
*******************************************************************************/
char *obtenerNombre(tSimbolos Ts)
{
     return Ts->nombre;
}

/*******************************************************************************
Esta funcin devuelve el tipo de la variable los posibles valores que puede
devolver son:
	0:Globales
	1:INT
	2:BYTE
	3:CHAR
	4:LONG
	5:FLOAT
	6:VOID
*******************************************************************************/
int obtenerTipo(tSimbolos Ts)
{
    return Ts->tipo;
}

/*******************************************************************************
Esta funcin busca el elemento cuyo nombre se indica en la variable var, en caso
de que lo encuentre devuelve el puntero de la lista, en caso contrario devuelve
NULL.
*******************************************************************************/
tSimbolos buscarSimbolo(tSimbolos Ts, char* var)
{
      int comparacion;
	  
      while (!listaVacia(Ts))
      {
        comparacion = strcmp(obtenerNombre(Ts),var);
        if (comparacion != 0)
                Ts = Ts->sgte;
	else
		break;
      }
      return Ts;
}


int obtenerTamanoTipoDato(int tipo)
{
	int retorno;

	switch (tipo)
	{
		case -2: retorno = 0;
			break;
		case 0:  retorno = 4;
			break;
		case 1:  retorno = 4;
			break;
		case 2:  retorno = 4;
			break;
		case 3:	retorno = 4;
			break;
	}
	
	return retorno;
}

int obtenerOffset(int tipo, int tam, int ambito)
{
	int retorno;
	
	if (tam > 0)
	{
		switch (tipo)
		{
			case 1:  retorno = obtenerTamanoTipoDato(tipo) * tam;
						break; 
			default: retorno = 4;
		}
	}
	else
	{
		switch (tipo)
		{
			case 0:  retorno = 4;
				break;
			case 1:  retorno = 4;
				break;
			case 2:  retorno = 4;
				break;
			case 3:	retorno = 4;
				break;
			default: retorno = 0;
		}
	}
	
	return retorno;
}

int insertarNuevaVariable(tSimbolos Ts, int opcion, char* variable, int tipo, int ambito, int tam, char *dimensiones, char *valor)
{	tSimbolos simbolo;
	     
	if (opcion == 1)
		simbolo = ultimo(Ts);	else
		simbolo = Ts;

	if (simbolo != NULL)
	{
		simbolo->tamano += simbolo->tamUltimo;
		simbolo->tamUltimo = obtenerOffset(tipo,tam,ambito);  //Anadimos el tamano de la variable
		//printf("OFFSET VARIABLE %s: %d\n", variable, simbolo->tamano); 		
 		insertarVariable(&(simbolo->variables),variable,tipo,ambito,tam,dimensiones,valor,simbolo->tamano);
  		return 0;
	}  
   	else
	   return 1;
}

/*******************************************************************************
Este método elimina todos los nodos de una lista de eventos.
*******************************************************************************/
void eliminarLista(tSimbolos *Ts)
{
    tSimbolos aux;

	while (!listaVacia(*Ts))
	{
		aux = *Ts;
		(*Ts) = (*Ts)->sgte;
		//printf("FUNCION %s\n", aux->nombre);		
		eliminarListaVariables(&(aux->variables));
		free(aux->nombre);
		free(aux);
	}
}

int existeFuncion(tSimbolos Ts, char *nombreFuncion)
{
	if (buscarSimbolo(Ts,nombreFuncion) != NULL)
		return 1;
	
	return 0;
}

int existeVariableGlobal(tSimbolos Ts, char *nombreVariable)
{
	//Buscamos la variable en el nodo de globales
	
	if (buscarVariable(Ts->variables,nombreVariable) != NULL)
		return 1;
	
	return 0;
}

int existeVariable(tSimbolos Ts, char *nombreVariable)
{
	//Sino existe en el nodo de globales buscamos en el de la función
	if (buscarVariable(ultimo(Ts)->variables,nombreVariable) != NULL)
		return 1;
	
	return 0;
}

int tipoParametro(tSimbolos Ts, char *funcion, int parametro)
{
	tSimbolos aux;
	
	aux = buscarSimbolo(Ts,funcion);
	
	if (aux != NULL)
		return obtenerTipoParametro(aux->variables,parametro);
	else
		return -3;
}

int numeroParametros(tSimbolos Ts, char *funcion)
{
	tSimbolos aux;
	
	aux = buscarSimbolo(Ts,funcion);
	
	if (aux != NULL)
		return obtenerNumeroParametros(aux->variables);
	else
		return -1;
}

int obtenerOffsetFuncion(tSimbolos Ts, char *funcion)
{
	tSimbolos aux;
	int retorno;
	
	aux = buscarSimbolo(Ts,funcion);
	retorno = obtenerOffset(aux->tipo,0,0);
	
	//printf("TAMANO: %d\n", aux->tamano);
	
	if (aux != NULL)
		return aux->tamano + aux->tamUltimo + 4;
	else
		return -1;
}


int obtenerOffsetArgumento(tSimbolos Ts, char *nombrefuncion, int argumento)
{
	tSimbolos aux;
	lVariables variableAux;
	int resultado  = -1;
	int posicion   = 1;
	
	aux = buscarSimbolo(Ts,nombrefuncion);
	
	if (aux != NULL)
	{
		variableAux = aux->variables;
		
		while (posicion != argumento)
		{
			variableAux = variableAux->sgte;
			posicion++;
		}
		
		resultado = variableAux->offset;
		
	}
	
	return resultado;
}

int ObtenerOffSetVariable(tSimbolos Ts, char *nombrefuncion, char *argumento)
{

	tSimbolos aux;
	lVariables variableAux;
	int resultado  = -1;
	int posicion   = 1;

	aux = buscarSimbolo(Ts,nombrefuncion);
	
	if (aux != NULL)
	{
		variableAux = aux->variables;
		
		while (strcmp(obtenerNombreVariable(variableAux),argumento) != 0)
		{
			variableAux = variableAux->sgte;
		}
		
		resultado = obtenerOffsetVariable(variableAux);
	}

	return resultado;

}
	

int obtenerValoresVariable(tSimbolos Ts, char *nombreVariable, int parametro)
{
	lVariables variableAux;
	int resultado = -5;
	
	variableAux = buscarVariable(Ts->variables,nombreVariable);
	
	if (variableAux == NULL)
		variableAux = buscarVariable(ultimo(Ts)->variables,nombreVariable);
	
	switch (parametro)
	{
		case 1: resultado = obtenerTipoVariable(variableAux);
				break;
		case 2: resultado = obtenerAmbitoVariable(variableAux);
				break;
		case 3: resultado = obtenerTamanoVariable(variableAux);
				break;
//case 4: resultado = obtenerValorVariable(variableAux);
//				break;
	}
	
	return resultado;
}

char *obtenerValorConstante(tSimbolos Ts, char *nombreVariable)
{
	lVariables variableAux;
		
	variableAux = buscarVariable(Ts->variables,nombreVariable);
	
	if (variableAux == NULL)
	{
		variableAux = buscarVariable(ultimo(Ts)->variables,nombreVariable);
		
		if (variableAux == NULL)
			return NULL;
		else
			return obtenerValorVariable(variableAux);
	}
	else
		return obtenerValorVariable(variableAux);
}

int obtenerRestoVariable(tSimbolos Ts, char *nombreVariable, int dimension)
{
	lVariables variableAux;
	
	variableAux = buscarVariable(Ts->variables,nombreVariable);
	
	if (variableAux == NULL)
		variableAux = buscarVariable(ultimo(Ts)->variables,nombreVariable);
		
	return obtenerMulDimensiones(variableAux,dimension);
}

int obtenerOffsetVariableActual(tSimbolos Ts, char *nombreVariable)
{
	lVariables aux;

	aux = buscarVariable(Ts->variables,nombreVariable);

	if (aux != NULL)
	{
		return  obtenerOffsetVariable(aux);
	} 
	return 0;
}


/*******************************************************************************
Este procedimiento realiza generación de las variables en lenguaje ensamblador
Ts     : Representa la tabla de simbolos
ambito : Indica el ambito de las variables a generar, es decir deferencia entre
			variables globales, locales y constantes.
*******************************************************************************/
void generarVariablesPrograma(tSimbolos Ts, int ambito)
{
	lVariables aux;
	
	aux = Ts->variables;
	
	while (!listaVaciaVariable(aux))
	{
		if (obtenerAmbitoVariable(aux) == ambito)
		{
			if (ambito == 0)				//Comprobamos que es una constante
				generarConstante(obtenerNombreVariable(aux),obtenerTipoVariable(aux),obtenerValorVariable(aux));
			else if (aux->tamano == 0) //Comprobamos que es una variable y no es un array (Debido a que los arrays se definen al comienzo
				generarVariables(obtenerNombreVariable(aux),obtenerTipoVariable(aux),obtenerTamanoVariable(aux));
		}

		aux = aux->sgte;
	}
}

/*******************************************************************************
Este procedimiento realiza generación de las variables de tipo arrays del programa
en lenguaje ensamblador.
Ts     : Representa la tabla de simbolos
*******************************************************************************/
void generarArraysPrograma(tSimbolos Ts)
{

	tSimbolos tablaAux;
	lVariables auxiliar;

	tablaAux = Ts;

	while (!listaVacia(tablaAux))
	{
		auxiliar = tablaAux->variables;

		while (!listaVaciaVariable(auxiliar))
		{
			if (obtenerTamanoVariable(auxiliar) > 0)
			{
				generarVariables(obtenerNombreVariable(auxiliar),obtenerTipoVariable(auxiliar),obtenerTamanoArray(auxiliar));
			}

			auxiliar = auxiliar->sgte;
		}

		tablaAux = tablaAux->sgte;
	}

}
