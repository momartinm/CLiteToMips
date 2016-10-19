/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : lVariables.c                          *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lVariables.h"


/*******************************************************************************
Esta funcion nos indica si una lista esta vacia.
*******************************************************************************/
int listaVaciaVariable(lVariables lv)
{
	return lv == NULL;
}

/*******************************************************************************
Esta funcion devuelve el ltimo elemento de una lista de variables.
*******************************************************************************/
lVariables ultimaVariable(lVariables lv)
{
	while (!listaVacia(lv->sgte))
		lv = lv->sgte;
	return lv;
}

lDimensiones generarListaDimensiones(char *dimensiones, int *numDimensiones)
{
	int contador;
	char *auxiliar;
	lDimensiones listaDim;

	contador = 0;
	listaDim = NULL;
	auxiliar = strtok(dimensiones,",");
	
	while (auxiliar != NULL)
	{
		contador++;
		insertarDimension(&listaDim,atoi(auxiliar));
		auxiliar = strtok(NULL,",");
	}
	
	*numDimensiones = contador;
	
	return listaDim;
}

static lVariables nuevoNodoVariable(char* var, int tipo, int ambito, int tam, char *dimensiones, char *valor, int offset)
{

	int i;
	int numDimensiones;
	lVariables aux;

	aux = (lVariables) malloc(sizeof(PtrtVariables));
	aux->nombre      = malloc(sizeof(char)*(strlen(var)+1));
	strcpy(aux->nombre,var);
	aux->tipo        = tipo;
	aux->ambito      = ambito;
	aux->dimensiones = NULL;
	aux->tamano      = 0;
	aux->valor       = NULL;
	aux->offset      = offset;
	aux->sgte        = NULL;
	
	if ((ambito != 0) && (dimensiones != NULL))
	{
		if (strlen(dimensiones) > 0)
		{
			aux->dimensiones = generarListaDimensiones(dimensiones,&numDimensiones);
			aux->tamano      = numDimensiones;
		}
	}

	if (valor != NULL)
	{
		aux->valor  = malloc( sizeof(char) * (strlen(valor)+1) );
		strcpy(aux->valor,valor);
	}
	
	return aux;
}

int obtenerMulDimensiones(lVariables l, int dimFinal)
{
	return contarRestoDimensiones(l->dimensiones,dimFinal);
}

/*******************************************************************************
Este metodo almacena una nueva variable indicando el tipo de variable y la linea
en la que sha sido definida esa variable.
*******************************************************************************/
void insertarVariable(lVariables *lv, char* var, int tipo, int ambito, int tam, char *dimensiones, char *valor, int offset)
{
     lVariables nuevo = nuevoNodoVariable(var,tipo,ambito,tam,dimensiones,valor,offset);

     if (listaVaciaVariable(*lv))
        *lv = nuevo;
     else
         ultimaVariable(*lv)->sgte = nuevo;
}

/*******************************************************************************
Esta funcin devuelve el nombre de la variable almacenada en L.
*******************************************************************************/
char *obtenerNombreVariable(lVariables lv)
{
     return lv->nombre;
}

/*******************************************************************************
Esta funcion devuelve el tipo de la variable los posibles valores que puede
devolver son:
	-2:VOID
	-1:CADENA
	0:CHAR
	1:SHORT
	2:INT
	3:FLOAT
*******************************************************************************/
int obtenerTipoVariable(lVariables lv)
{
    return lv->tipo;
}

/*******************************************************************************
Esta funcion devuelve el ambito de la variable los posibles valores que puede
devolver son:
	0:CONSTANTE 
	1:LOCAL
	2:GLOBAL
	3:PARAMETROS
*******************************************************************************/
int obtenerAmbitoVariable(lVariables lv)
{
    return lv->ambito;
}

int obtenerTamanoVariable(lVariables lv)
{
    return lv->tamano;
}

char *obtenerValorVariable(lVariables lv)
{
    return lv->valor;
}

int obtenerOffsetVariable(lVariables lv)
{
    return lv->offset;
}


/*******************************************************************************
Esta funcion busca el elemento cuyo nombre se indica en la variable var, en caso
de que lo encuentre devuelve el puntero de la lista, en caso contrario devuelve
NULL.
*******************************************************************************/
lVariables buscarVariable(lVariables lv, char* var)
{
	  int comparacion;
	  
      while (!listaVacia(lv))
      {
            comparacion = strcmp(obtenerNombreVariable(lv),var);
			if (comparacion != 0)
                lv = lv->sgte;
			else
				break;
      }
      return lv;
}


int obtenerTipoParametro(lVariables lv, int parametro)
{
	int posicion = 1;
	int salida;
	
	while (!listaVacia(lv))
	{
		if (posicion == parametro)
		{
			salida = obtenerTipoVariable(lv);
			break;
		}
		posicion++;
		lv = lv->sgte;
	}
	
	return salida;
}

int obtenerNumeroParametros(lVariables lv)
{
	int numero = 0;
	
	while (!listaVacia(lv))
	{
		if (obtenerAmbitoVariable(lv) == 3)
			numero++;
		else
			break;
		lv = lv->sgte;
	}
	
	return numero;
}

int obtenerTamanoArray(lVariables lv)
{
	return calcularDimensiones(lv->dimensiones);
}


/*******************************************************************************
Este método elimina todos los nodos de una lista de variables.
*******************************************************************************/
void eliminarListaVariables(lVariables *lv)
{
    lVariables aux;

	while (!listaVaciaVariable(*lv))
	{
		aux = *lv;
		(*lv) = (*lv)->sgte;
		free(aux->nombre);
		free(aux);
	}
}
