/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : fichero.c                             *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fichero.h"

static FILE *salida;
static FILE *log;

char nombreFicheroSalida[255];

int abrirFichero(char *nombre, int fichero)
{

	if (fichero == 1)	//Fichero de salida
	{
		bzero(nombreFicheroSalida,255);
		strcpy(nombreFicheroSalida,nombre);

		salida = fopen( nombre ,"w");
		if(salida == NULL)
		{
			printf("ERROR DEL SISTEMA: El fichero %s no he podido ser creado\n", nombre);
			return -1;		}
	       	else
			return 0;
	}	
	else			//Fichero de Log
	{
		log = fopen( nombre ,"w");
		if(log == NULL)
		{
			printf("ERROR DEL SISTEMA: El fichero %s no he podido ser creado\n", nombre);
			return -1;		}
	       	else
			return 0;
	}
}


int escribirFichero(char *cadena, int fichero, int operacion)
{
	if (fichero == 1)	//Escribimos en el fichero de salida
		fprintf(salida,"%s\n",cadena);
	else if (fichero == 2)	//Escribimos en el fichero de log
	{
		if (operacion == 0)
			fprintf(log,"TRAZA SISTEMA: %s\n",cadena);
		else
			printf("TRAZA SISTEMA: %s\n",cadena);
	}
}

void cerrarFichero(int fichero)
{
	if (fichero == 1)
		fclose(salida);
	else
		fclose(log);
}

void cerrarFicheroIncompleto(int fichero)
{
	char cadena[255];

	if (fichero == 1)
	{
		bzero(cadena,255);
		sprintf(cadena,"rm %s",nombreFicheroSalida);
		system(cadena);
		fclose(salida);
	}
}
