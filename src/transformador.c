/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : tranformador.c                        *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "transformador.h"


extern contenido eRegistros[NUMREGISTROS];

int definirTipoRegistro(int tipoDato)
{
	switch (tipoDato)
	{
		case 0:	return 1;
					break;
		case 1:  return 1;
					break;
		case 2:  return 1;
					break;
		case 3:  return 2;
					break;
		case 4:  return 4;
					break;
		case 5:  return 4;
					break;
	}
}


int obtenerLimite(int tipo, int limite)
{
	int resultado;

	switch (tipo)
	{
		case 1:	resultado = (limite == 0) ? INICIOENTEROS : FINENTEROS;
					break;
		case 2:	resultado = (limite == 0) ? INICIOFLOTANTES : FINFLOTANTES;
					break;
		case 3:	resultado = (limite == 0) ? INICIOTEMPORALES : FINTEMPORALES;
					break;
		case 4:  resultado = (limite == 0) ? INICIOARGUMENTOS : FINARGUMENTOS;
					break;
	}
	
	return resultado;
}

int obtenerTipoRegistro(int posicion)
{
	if (posicion <= FINENTEROS)
		return 1;
	else if (posicion <= FINFLOTANTES)
		return 2;
	else if (posicion <= FINTEMPORALES)
		return 3;
	else
		return 4;
}

//****************************************************************************//
//La función iniciar registro inicializa la estructura que almacena los arrays
//que representan los registros del compilarador MIPS.
//****************************************************************************//
void iniciarRegistros()
{
	int i;

	for (i=0; i<NUMREGISTROS; i++)
	{
		eRegistros[i].estado       = 0;
		eRegistros[i].tipo         = 0;
		eRegistros[i].referencia   = 0;
		eRegistros[i].profundidad  = 0;
		eRegistros[i].tipoRegistro = 0;
		bzero(eRegistros[i].id,256);
	}
	
}


void iniciarRegistro(int i)
{
	eRegistros[i].estado       = 0;
	eRegistros[i].tipo         = 0;
	eRegistros[i].referencia   = 0;
	eRegistros[i].profundidad  = 0;
	eRegistros[i].tipoRegistro = 0;
	bzero(eRegistros[i].id,256);
}


//****************************************************************************//
//Eete procedimiento realiza la inserción de la información de un registro
//tipo: Representa el tipo de registro al que se refiere, pudiendo introducir
//   1: Registros enteros de 32 bits que corresponde a los $s y $t
//   2: Registros del coprocesador flotante que se refieren a los $f
//   3: Registros enteros de 32 bits que se refieren a los registros $a
//   4: Registros enteros de 32 bits que se refieren a los registros $
//registro: Indica la posición del registro en el array de registros
//estado: Indica el estado del registro
//id: Indica el nombre de las variable, aunque existen representaciones especiales
// @: variables auxiliar seguidas de un entero que representa el resultado de una
//    operación.
// #:
// $:
//tipoElemento: Indica si el registro almacena una variable o una dir de memoria
//		su improtancia reside el uso de arrays.
//auxiliar: Indica si el registros es de tipo auxiliar o estable, es decir si
//	    almacena una variable o un resultado auxiliar que podrá ser eliminado.
//****************************************************************************//
void modificarRegistro(int tipo, int registro, char estado, char *id, char tipoElemento, char auxiliar)
{
	struct timeval tv;
	
	gettimeofday(&tv,NULL);
	
	//printf("Esto es una prueba %d con estado %d",registro, estado);
	
	eRegistros[registro].estado        = estado;
	eRegistros[registro].tipo          = tipoElemento;
	eRegistros[registro].tipoRegistro  = tipo;
	eRegistros[registro].referencia    = tv.tv_sec + tv.tv_usec;
	eRegistros[registro].auxiliar      = auxiliar;
	eRegistros[registro].profundidad   = valorFlag(11);
	bzero(eRegistros[registro].id,256);
	memcpy(eRegistros[registro].id,id,strlen(id));
}


//****************************************************************************//
//Esta función nos devuelve la posición en el array de registros de un determinado
//registro.
//registro: Es el registro del que queremos obtener la posición.
//****************************************************************************//
int obtenerPosicionRegistro(char *registro)
{
	char auxiliar[3];
	char tipo[2];

	int posicion;

	bzero(auxiliar,3);
	bzero(tipo,2);

	memcpy(tipo,(registro + 1),1);
	memcpy(auxiliar,(registro + 2), 2);

	posicion = atoi(auxiliar);
	
	if (strcmp(tipo,"t") == 0)
		return posicion;
	if (strcmp(tipo,"s") == 0)
		return posicion + 10;
	if (strcmp(tipo,"f") == 0)
		return posicion;
	if (strcmp(tipo,"a") == 0)
		return posicion;
		
	return -1;
}

//****************************************************************************//
//Este procedimiento nos devuelve el registros indicadole el tipo de registro
//y su posición en el array.
//tipo: Representa el tipo registro al que se refiere.
//posicion: Representa la posición en el correspondiente array.
//registro: Se utiliza para devolver el registro.
//****************************************************************************//
void obtenerRegistro(int tipo, int posicion, char *registro)
{
	char auxiliar[5];

	bzero(auxiliar,5);

	switch (tipo)
	{
		case 1:	if (posicion < 10)
			{
				sprintf(auxiliar,"$t%d", posicion);
				memcpy(registro,auxiliar,5);
			}
			else
			{
				sprintf(auxiliar,"$s%d", posicion - 10);
				memcpy(registro,auxiliar,5);
			}
			break;
		case 2: sprintf(auxiliar,"$f%d", posicion - 18);
			memcpy(registro,auxiliar,5);
			break;
		case 3:	sprintf(auxiliar,"$a%d", posicion - 50);
			memcpy(registro,auxiliar,5);
			break;
		case 4:	sprintf(auxiliar,"$v%d", posicion - 54);
			memcpy(registro,auxiliar,5);
			break;
	}
}

//****************************************************************************//
//Este procedimiento nos devuelvel el registro ocupado por una variable independientemente
//de su tipo.
//tipo: Indica el tipo de registro que se quiere buscar.
//variable: Indica el nombre de la variable.
//registro: Variable que se utiliza para devolver el registro ocupado.
//****************************************************************************//
void obtenerRegistroOcupado(int tipo, char *variable, char *registro)
{
	struct timeval tv;
	int encontrado = 0;
	int posicion;
	int tipoRegistro = definirTipoRegistro(tipo);
	
	//printf("BUSCO %s %d\n", variable, tipo);
	
	gettimeofday(&tv,NULL);
	
	posicion = obtenerLimite(tipoRegistro,0);
	
	while ((!encontrado) && (posicion <= obtenerLimite(tipoRegistro,1)))
	{
		//printf("Esto es lo que hay %s\n", eRegistros[posicion].id);
	
		if ((strcmp(variable,eRegistros[posicion].id) == 0) && (eRegistros[posicion].profundidad <= valorFlag(11)))
		{
			obtenerRegistro(tipoRegistro,posicion,registro);
			eRegistros[posicion].referencia = tv.tv_sec + tv.tv_usec;;
			encontrado = 1;
		}
		
		posicion++;
	}
}


int liberarRegistro(int tipo)
{
	long menor;
	int posicion     = 0;
	int encontrado   = 0;
	int tipoReg      = definirTipoRegistro(tipo);

	menor    = eRegistros[posicion].referencia;
	posicion = obtenerLimite(tipoReg,0);
	
	while (posicion < obtenerLimite(tipoReg,1))
	{
		if (eRegistros[posicion].referencia < menor)
		{
			menor      = eRegistros[posicion].referencia;
			encontrado = posicion;
		}
		
		posicion++;
	}
	
	return encontrado;
}

void liberarRegistrosAuxiliares()
{
	int i;
	
	//printf("COMIENZA LA LIBERACION\n");
	
	for (i=0; i < NUMREGISTROS; i++)
	{
		if (eRegistros[i].auxiliar == 1)
		{
			//printf("Es un registro auxiliar %s\n", eRegistros[i].id);
			iniciarRegistro(i);
		}
	}
}

//****************************************************************************//
//Este procedimiento nos ocupa un registro con una nueva variable
//de su tipo.
//tipo: Indica el tipo de registro que se quiere buscar.
//variable: Indica el nombre de la variable.
//registro: Variable que se utiliza para devolver el registro ocupado.
//auxiliar: Indica si el registro es para una variable o para un elemento auxiliar.
//tipoRegistro: Indica si el registro almacenara información o una dir de memoria.
//****************************************************************************//
int obtenerRegistroLibre(int tipo, char *variable, char *registro, char auxiliar, char tipoRegistro)
{
	int encontrado   = 0;
	int posicion     = 0;
	int tipoReg      = definirTipoRegistro(tipo);
	int liberado     = -1;
	
	posicion = obtenerLimite(tipoReg,0);
	
	//printf("TIPO %d POSICION %d\n", tipoReg, posicion);
	
	//printf("obtener registro %d %s empieza %d y termina %d\n", tipoReg, variable, posicion, obtenerLimite(tipoReg,1));

	while ((!encontrado) && (posicion <= obtenerLimite(tipoReg,1)))
	{
		//printf("EStado %d de %d\n", posicion, eRegistros[posicion].estado);
	
		if (eRegistros[posicion].estado == 0)
		{
			obtenerRegistro(tipoReg,posicion,registro);
			//printf("########################### AQUI LLEGO 1%s\n", registro);
			modificarRegistro(1,posicion,1,variable,tipoRegistro,auxiliar);
			encontrado = 1;
		}
		else
			posicion++;
		
	}
	
	if (!encontrado)
	{
		liberado =  liberarRegistro(tipo);
		if (liberado != -1)
		{
			obtenerRegistro(tipoReg,liberado,registro);		
			modificarRegistro(1,liberado,1,variable,tipoRegistro,auxiliar);
			return liberado;
		}
		else
			return -1;
	}
	else
		return posicion;
}


int enbargarRegistro(int tipo, char *variable, char *registro)
{
	int encontrado   = 0;
	int posicion     = 0;
	int tipoReg      = definirTipoRegistro(tipo);
	
	posicion = obtenerLimite(tipoReg,0);
	
	//printf("Posicion %d\n", posicion);
	
	modificarRegistro(1,posicion,1,variable,tipo,0);
	
	return posicion;
}

void eliminarRegistroProfundos(int profundidad)
{
	int i;
	
	for (i=0; i< NUMREGISTROS; i++)
	{
		if (eRegistros[i].profundidad == profundidad)
			modificarRegistro(obtenerTipoRegistro(i),i,0,"",0,0);	
	}
}

//****************************************************************************//
//Esta funcion nos indica si un registro es o no un registro utilizado
//para almacenar un auxiliar.
//registro: indica la posición del registro.
//****************************************************************************//
int esRegistroAuxiliar(int registro)
{
	return eRegistros[registro].auxiliar;
}

//****************************************************************************//
//Esta funcion nos indica si el tipo de un registro es decir si almacena un valor
//o una dir de memoria
//tipo: Indica el tipo de registro que se quiere buscar.
//registro: Indica el nombre del registro.
//****************************************************************************//
int tipoRegistro(int tipo, char *registro)
{
	int resultado = -1;
	int posicion;
	
	posicion = obtenerPosicionRegistro(registro);

	if (posicion != -1)
		return eRegistros[posicion].tipo;
	
	return resultado;
}

//****************************************************************************//
//Este procedimiento genera la cabecera de la zona de definición de variables
//en ensamblador.
//tipo: Indica si se realiza la definición de las variables o de las constantes
//****************************************************************************//
void generarDefiniciones()
{
	char cadena[256];
	int  i;
	
	bzero(cadena,256);
	
	strcat(cadena,"\n.data	#Definición de elementos almacenados en la RAM");
	
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n",cadena);
}


void generarCabeceraPrograma()
{
	char cadena[256];
	int  i;
	
	bzero(cadena,256);
	
	strcat(cadena,".text");
	strcat(cadena,"\n");
	strcat(cadena,".globl main");	
	strcat(cadena,"\n");
	strcat(cadena,"jal main");
	
	activarFlag(5);
	
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n",cadena);
}

//****************************************************************************//
//Este procedimiento genera la estructura de una constante en ensamblador
//nombre: Indica el nombre de la constante.
//tipo  : Indica el tipo de la constante.
//valor : Indica el valor de la constante.
//****************************************************************************//
void generarConstante(char *nombre, int tipo, char *valor)
{
	char cadena[256];
	int  i;
	
	bzero(cadena,256);
	
	
	strcat(cadena,nombre);
	strcat(cadena,": ");
	
	switch (tipo)
	{
		case   0:strcat(cadena,".byte ");
					break;
		case   1:strcat(cadena,".half ");
					break;
		case   2:strcat(cadena,".word ");
					break;
		case   3:strcat(cadena,".float ");
					break;
		case   4:strcat(cadena,".double ");
					break;
		case -10:strcat(cadena,".asciiz ");
					break;
	}
	
	strcat(cadena,valor);
	
	escribirFichero(cadena,1,0);

	if (valorFlag(2) == 1)
		printf("%s\n",cadena);
}

//****************************************************************************//
//Este procedimiento genera la estructura de una variable en ensamblador
//nombre: Indica el nombre de la variable.
//tipo  : Indica el tipo de la variable.
//tamano: Indica el tamano de la variable.
//****************************************************************************//
void generarVariables(char *nombre, int tipo, int tamano)
{
	char cadena[256];
	char tamanoDatos[16];
	int  i;
	int  tamArray;
	
	bzero(cadena,256);
	bzero(tamanoDatos,16);
	
	strcat(cadena,nombre);
	strcat(cadena,": ");
	
	if (tamano == 0)
	{
	
		switch (tipo)
		{
			case   0:strcat(cadena,".byte 0");
						break;
			case   1:strcat(cadena,".half 0");
						break;
			case   2:strcat(cadena,".word 0");
						break;
			case   3:strcat(cadena,".float 0.0");
						break;
			case -10:strcat(cadena,".asciiz ");
						break;
		}
	}
	else
	{

		strcat(cadena,".space ");
	
		switch (tipo)
		{
			case   0:tamArray = tamano * 1;
						break;
			case   1:tamArray = tamano * 2;
						break;
			case   2:tamArray = tamano * 4;
						break;
			case   3:tamArray = tamano * 4;
						break;
			case   4:tamArray = tamano * 8;
						break;
		}
		
		sprintf(tamanoDatos,"%d",tamArray);
		strcat(cadena,tamanoDatos);
	}
	
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
}

//****************************************************************************//
//Este procedimiento genera la estructura de un proceso de carga en esamblador
//registro: Indica el registro sobre el que se quiere realizar la carga.
//registroAuxiliar: Es un registro especial para el caso en el que se realizan cargas
//		    de constantes de tipo enteras.
//carga: Indica el registro o la variable a cargar.
//tipoDato: Indica el tipo de dato que se va a cargar a modo de que se elija la 
//	    instrucción correspondiente.
//offset  : Indica el desplazamiento a utilizar, existe un caso en el que a pesar
//	    de que el desplazamiento sea cero se realizará, cuando cargamos una 
//	    dirección de memoria "ARRAYS"
//****************************************************************************//
void generarInstruccionCarga(char *registro, char *registroAuxiliar, char *carga, int tipoDato, int offset)
{
	char cadena[256];
	char auxiliar[256];
	
	int entero;
	float flotante;
	
	int  i;
	
	bzero(cadena,256);
	bzero(auxiliar,256);
	
	switch (tipoDato)
	{
		case 0:strcat(cadena,"lb ");		//cargar un char
					break;
		case 1:strcat(cadena,"lh ");		//cargar un short
					break;
		case 2:strcat(cadena,"lw ");		//cargar un int
					break;
		case 3:strcat(cadena,"l.s ");		//cargar un float
					break;
		case 4:strcat(cadena,"l.d ");		//cargar un double
					break;
		case 5:strcat(cadena,"li ");		//cargar un número literal
					break;
		case 6:strcat(cadena,"la ");		//Cargar dirección de memoria
					break;
		case 7:strcat(cadena,"li ");
				 	break;
		case -10:strcat(cadena, "la ");		//Cargar una string
					break;
	}
	
	strcat(cadena,registro);
	strcat(cadena,", ");
		
	if (tipoDato == 5)		//Proceso de carga de numero literal
		strcat(cadena,carga);
	else if (tipoDato == 7)  	//Proceso de carga de numero flotante literal
	{
		flotante = atof(carga);
		memcpy ((&entero), (&flotante), 4);
		sprintf(auxiliar,"%d",entero);
		strcat(cadena,auxiliar);
		strcat(cadena,"\n");
		sprintf(auxiliar,"mtc1 %s, %s", registro, registroAuxiliar);
		strcat(cadena,auxiliar);
	}
	else				//Proceso de carga de registros
	{
		if (offset == 0)
			strcat(cadena,carga);
		else if (offset == -1)
		{
			sprintf(auxiliar,"(%s)", carga);
			strcat(cadena,auxiliar);
		}	
		else
		{
			sprintf(auxiliar,"%d(%s)", offset, carga);
			strcat(cadena,auxiliar);
		}
	}
	
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
	
}


//****************************************************************************//
//Este procedimiento genera la estructura de un almacenamiento, es decir la
//actualización del valor de una constante en RAM.
//registro: Indica el registro sobre el que se quiere realizar la carga.
//registroAuxiliar: Es un registro especial para el caso en el que se realizan cargas
//		    de constantes de tipo enteras.
//carga: Indica el registro o la variable a cargar.
//tipoDato: Indica el tipo de dato que se va a cargar a modo de que se elija la 
//	    instrucción correspondiente.
//offset  : Indica el desplazamiento a utilizar, existe un caso en el que a pesar
//	    de que el desplazamiento sea cero se realizará, cuando cargamos una 
//	    dirección de memoria "ARRAYS"
//****************************************************************************//
void generarInstruccionAlmacenamiento(char *registro, char *registroAuxiliar, char *carga, int tipoDato, int offset)
{
	char cadena[256];
	char auxiliar[256];
	
	int  i;
	
	bzero(cadena,256);
	bzero(auxiliar,256);
	
	//printf("AQUI ENTRO YO\n");
	
	if (registroAuxiliar != NULL)  //Vamos a cargar una direccion
	{
		strcat(cadena,"la ");
		strcat(cadena,registroAuxiliar);
		strcat(cadena,", ");
		strcat(cadena,carga);
		strcat(cadena,"\n");
	}
	
	switch (tipoDato)
	{
		case 0:strcat(cadena,"sb ");	//vamos a realizar la carga en un char
			break;
		case 1:strcat(cadena,"sh ");	//vamos a realizar la carga en un short
			break;
		case 2:strcat(cadena,"sw ");	//vamos a realizar la carga en un int
			break;
		case 3:strcat(cadena,"s.s ");	//vamos a realizar la carga en un float
			break;
		case 4:strcat(cadena,"s.d ");	//vamos a realizar la carga en un doublr
			break;
	}
	
	//printf("AQUI LLEGO\n");
	
	strcat(cadena,registro);
	strcat(cadena,", ");
	
	if (registroAuxiliar != NULL)	//Si se ha pasado un registro de tipo auxiliar
	{
		if ((offset != 0) || (tipoRegistro(tipoDato,registroAuxiliar) == 0))
			sprintf(auxiliar,"%d(%s)", offset, registroAuxiliar);
		else
			sprintf(auxiliar,"%s", registroAuxiliar);
	}
	else	//Si no hay nada en registro auxiliar
	{
	
		if (offset != 0)	//Si el offset es distinto de cero
			sprintf(auxiliar,"%d(%s)", offset, carga);
		else if (tipoRegistro(tipoDato,carga) == 0)	//Se utiliza para la carga de la direcciones de memoria almacenadas en un registro
			sprintf(auxiliar,"(%s)", carga);
		else	
		{
			if (strcmp(carga,"$sp") == 0)
				sprintf(auxiliar,"(%s)", carga);
			else
				sprintf(auxiliar,"%s", carga);
		}
	}
		
	strcat(cadena,auxiliar);
	
	escribirFichero(cadena,1,0);

	if (valorFlag(2) == 1)
		printf("%s\n",cadena);

}

//****************************************************************************//
//Este procedimiento genera la estructura de un operación
//resultadoOp: Esta variable lamcena el registro donde se almacenará el resultado
//operando1: Esta variable almacena el primer operador de la operacion
//operando2: Esta variable almacena el segundo operador de la operacion
//tipoDato: Esta avriable indica el tipo de la operacion
//tipoOperacion: Esta avriabel indica el tipo de operacion siendo
//	      1: SUMA
//	      2: RESTA
//	      3: MULTIPLICACION
//	      4: DIVISION
//	      5: MODULO DIVISION ENTERA
//****************************************************************************//
void generarInstruccionOperacion(char *resultadoOp, char *operando1, char *operando2, int tipoDato, int tipoOperacion)
{
	char cadena[256];
	char auxiliar[256];
	
	int  i;
	int  opEspecial = 0;
	
	bzero(cadena,256);
	bzero(auxiliar,256);


	if (tipoDato < 3)
	{
		switch (tipoOperacion)
		{
			case 1:strcat(cadena,"add ");
						break;
			case 2:strcat(cadena,"sub ");
						break;
			case 3:strcat(cadena,"mult ");
					 opEspecial = 1;
						break;
			case 4:strcat(cadena,"div ");
					 opEspecial = 2;
						break;
			case 5:strcat(cadena,"div ");
					 opEspecial = 3;
						break;
		}
	}
	else if (tipoDato == 3)
	{
		switch (tipoOperacion)
		{
			case 1:strcat(cadena,"add.s ");
						break;
			case 2:strcat(cadena,"sub.s ");
						break;
			case 3:strcat(cadena,"mul.s ");
						break;
			case 4:strcat(cadena,"div.s ");
						break;
		}
	}
	else if (tipoDato == 4)
	{
		switch (tipoOperacion)
		{
			case 1:strcat(cadena,"add.d ");
						break;
			case 2:strcat(cadena,"sub.d ");
						break;
			case 3:strcat(cadena,"mul.d ");
						break;
			case 4:strcat(cadena,"div.d ");
						break;
		}
	}
	else
	{
		strcat(cadena,"addi ");
	}
	
	if ((tipoDato < 3) && (tipoOperacion > 2))
	{
		strcat(cadena,operando1);
		strcat(cadena,", ");
		strcat(cadena,operando2);
	}
	else
	{
		strcat(cadena,resultadoOp);
		strcat(cadena,", ");
		strcat(cadena,operando1);
		strcat(cadena,", ");
		strcat(cadena,operando2);
	}
	
	switch (opEspecial)	//Generación de resultados para multiplicacion, division y modulo entero
	{
			case 1:sprintf(auxiliar,"\nmflo %s", resultadoOp);
						break;
			case 2:sprintf(auxiliar,"\nmflo %s", resultadoOp);
						break;
			case 3:sprintf(auxiliar,"\nmfhi %s", resultadoOp);
						break;
	}

	strcat(cadena,auxiliar);
	
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
}


//****************************************************************************//
//Este procedimiento genera la estructura de una transformacion de entero a real.
//tipo: Almacena el tipo de real al que se quiere transformar
//operando: Almacena el registro entero donde esta almacenado el valor
//tipoDato
//elemento: 
//****************************************************************************//
void generarTransformacion(int tipo, char *operando, int tipoDato, char *elemento)
{
	int i;

	char cadena[256];
	char opAuxiliar1[5];
	char opAuxiliar2[5];
	int  auxiliar;
	
	bzero(cadena,256);

	auxiliar = esRegistroAuxiliar(obtenerPosicionRegistro(operando));
	obtenerRegistroLibre(tipo,elemento,opAuxiliar1,auxiliar,1);
	
	strcat(cadena,"mtc1 ");
	strcat(cadena,operando);
	strcat(cadena,", ");
	strcat(cadena,opAuxiliar1);
	strcat(cadena,"\n");

	for (i = 0; i <=  valorFlag(5); i++)
		strcat(cadena,"\t");

	switch (tipo)
	{
		case 3:	strcat(cadena,"cvt.s.w ");
					break;
		case 4:	strcat(cadena,"cvt.d.s ");
					break;	
	}

	obtenerRegistroLibre(tipo,elemento,opAuxiliar2,auxiliar,1);
	
	strcat(cadena,opAuxiliar2);
	strcat(cadena,", ");
	strcat(cadena,opAuxiliar1);
	
	memcpy(operando,opAuxiliar2,strlen(opAuxiliar2));

	//Falta eliminar operando y opAuxiliar1
	
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
	
}

//****************************************************************************//
//Este procedimiento genera la estructura para realizar cargas y almacenamientos
//Sobre arrays que se encuentran en memoria RAM.
//tipoDato:Variable que indica el tipo de dato del array.
//posicion:Variable que indica la posición del array
//indice: Variable que lamacena el nombre de la avriable que actua como indice
//	  en el caso que exista.
//elemento: Variable que almacena el nombre de la variable de tipo array
//operacion: Varibale que indica si se realiza una carga un almacenamiento
//****************************************************************************//
//void generarArray(tSimbolos TS, int tipoDato, char *elemento, int posIndice, char *indice, char *registro, int operacion)
void generarArray(tSimbolos TS, int tipoDato, int tipoSalida, char *elemento, char *indice, char *registro, int operacion)
{
	char auxiliar[256];
	char auxiliarIndice[256];
	char *cadena; 
	int contador;
	int global;
	int offset;
	
	char rega[5];		//Registro de almancenamiento de la dirección Base del Array.
	char regp[5];		//Registro de almacenamiento del valor del indice.
	char regm[5];		//Registro de almacenamiento 
	char rega1[4];		//Registro de almacenamiento de $a1
	char rega2[4];		//Registro de almacenamiento de $a2
	char rega3[4];		//Registro de almacenamiento de $a3
	char rzero[6];		//Registro de almacenamiento de $zero

	bzero(rega,5);			
	bzero(regp,5);			
	bzero(regm,5);			
	bzero(auxiliar,256);
	bzero(auxiliarIndice,256);

	//Cargamos las registro temporal que queremos utilizar.
	strcpy(rega1,"$a1\0");
	strcpy(rega2,"$a2\0");
	strcpy(rega3,"$a3\0");
	strcpy(rzero,"$zero\0");
	
	//Obtención de la dirección Base del array.
	//Se realiza la carga en un registro de tipo entero (T,S).
	
	//sprintf(auxiliar,"%s",elemento);
	
	obtenerRegistroOcupado(tipoDato,elemento,rega);
	if (strlen(rega) == 0)
		obtenerRegistroLibre(1,elemento,rega,0,0);
	generarInstruccionCarga(rega,NULL,elemento,6,0);
	
	//Leemos las distintas dimensiones

	strcpy(auxiliarIndice,indice);

	//printf("ESTO ES UNA PRUEBA %d %s\n", obtenerValoresVariable(TS, elemento, 3), auxiliarIndice);

	contador = obtenerValoresVariable(TS, elemento, 3) - 1;
	cadena   = strtok(auxiliarIndice,",");

	if (obtenerValoresVariable(TS, elemento, 3) == 1)
	{
		global = existeVariableGlobal(TS,cadena);
		offset = obtenerOffsetVariableActual(ultimo(TS),cadena);

		if (offset == 0)
			offset = -1;

		if (posibleNumero(cadena) == 0)
		{
			if ((valorFlag(1) == 1) && (!global))
			{
				generarInstruccionCarga(rega3,NULL,"$sp",2,offset);
			}
			else
				generarInstruccionCarga(rega3,NULL,cadena,2,0);
		}
		else
		{
			sprintf(auxiliar,"%s",cadena);
			generarInstruccionCarga(rega3,NULL,auxiliar,5,0);
		}
	}
	else
	{
		generarMovimiento(1, rega3, rzero);

		while (cadena != NULL)
		{
			//printf("PASO %d\n", contador);
			global = existeVariableGlobal(TS,cadena);
			offset = obtenerOffsetVariableActual(ultimo(TS),cadena);	

			if (offset == 0)
				offset = -1;		
	
			if (contador > 0)
			{
				if (posibleNumero(cadena) == 0)
				{
					if ((valorFlag(1) == 1) && (!global))
						generarInstruccionCarga(rega1,NULL,"$sp",2,offset);
					else
						generarInstruccionCarga(rega1,NULL,cadena,2,0);
				}
				else
				{
					sprintf(auxiliar,"%s",cadena);
					generarInstruccionCarga(rega1,NULL,auxiliar,5,0);
				}
			
				bzero(auxiliar,256);
				sprintf(auxiliar,"%d",obtenerRestoVariable(TS, elemento, contador));
				generarInstruccionCarga(rega2,NULL,auxiliar,5,0);

				generarInstruccionOperacion(rega2,rega2,rega1,2,3);
			}
			else
			{
				if (posibleNumero(cadena) == 0)
				{
					if ((valorFlag(1) == 1) && (!global))
						generarInstruccionCarga(rega2,NULL,"$sp",2,offset);
					else
						generarInstruccionCarga(rega2,NULL,cadena,2,0);
				}
				else
				{
					sprintf(auxiliar,"%s",cadena);
					generarInstruccionCarga(rega2,NULL,auxiliar,5,0);
				}	
			}

			
			generarInstruccionOperacion(rega3,rega3,rega2,2,1);	
		
			cadena = strtok(NULL,",");
			contador--;
		}
	}

	
	//Cargamos en un registro el tipo de dato
	
	sprintf(auxiliar, "%d",desplazamiento(tipoDato));
	obtenerRegistroLibre(2,auxiliar,regm,0,1);
	generarInstruccionCarga(regm,NULL,auxiliar,5,0);
	
	//Operación de multiplicación por el tipo de dato
	generarInstruccionOperacion(rega2,regm,rega3,2,3); //Donde poner rega2 habia un regm	
	
	//Realizariamos la suma
	generarInstruccionOperacion(rega,rega,rega2,2,1);

	
	//printf("AQUI VA EL TIPO DEL registro %d %s %s\n", tipoDato, registro, rega);
	
	//Realizamos la carga en el registro final
	if (operacion == 1)
		generarInstruccionCarga(registro,NULL,rega,tipoSalida,-1);
	else
		generarInstruccionAlmacenamiento(registro,NULL,rega,tipoSalida,0);
}



//****************************************************************************//
//Este procedimiento genera la estructura de un salto condicional a una etiqueta
//salto: Indica el tipo de salto que queremos realizar.
//tipo: Tipo del dato de la operacion.
//registro1: Indica el primer registro de la comparacion.
//registro2: Indica el segundo registro de la comparacion, en caso de que solo se
//				 utilice un parametro este será null.
//etiqueta: Indica la etiqueta sobre la que se saltará.
//****************************************************************************//
void generarSaltosEntero(int salto, int tipo, char *registro1, char *registro2, int *etiqueta, int *etiquetaNook, int tipoEtiqueta)
{
	int i;

	char cadena[256];
	char auxiliar[256];
	
	bzero(cadena,256);
	bzero(auxiliar,256);
	
	if (salto == 18)
	{
		if (tipoEtiqueta == 1)
			sprintf(auxiliar,"b __exito%d",*etiqueta);
		else
			sprintf(auxiliar,"b __e%d",*etiqueta);

		strcat(cadena,auxiliar);
	}
	else
	{

		switch (salto)
		{
			case 11: strcat(cadena,"beq ");
						break;
			case 12: strcat(cadena,"ble ");
						break;
			case 13: strcat(cadena,"bge ");
						break;
			case 14: strcat(cadena,"blt ");
						break;
			case 15: strcat(cadena,"bgt ");
						break;
			case 16: strcat(cadena,"bne ");
						break;
			case 17: strcat(cadena,"beq ");
						break;
		}

		strcat(cadena,registro1);
		strcat(cadena,", ");
	
		if (salto < 17)
			strcat(cadena,registro2);
		else
			strcat(cadena,"$zero");
		
		if (tipoEtiqueta == 1)
			sprintf(auxiliar,", __error%d",*etiquetaNook);
		else
			sprintf(auxiliar,", __e%d",*etiquetaNook);
	
		strcat(cadena,auxiliar); 
		
	}
	
	escribirFichero(cadena,1,0);

	//printf("IMPRIMO UNA ETIQUETA 1 %s\n", cadena);
	
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
	
}


void generarSaltosFlotante(int salto, int tipo, char *registro1, char *registro2, int *etiqueta, int *etiquetaNook, int tipoEtiqueta)
{
	int i;

	char cadena[256];
	char auxiliar[256];
	
	bzero(cadena,256);
	bzero(auxiliar,256);

	if (salto == 18)
	{
		if (tipoEtiqueta == 1)
			sprintf(auxiliar,"b __exito%d",*etiqueta);
		else
			sprintf(auxiliar,"b __e%d",*etiqueta);
		strcat(cadena,auxiliar);
	}
	else
	{

		switch (salto)
		{
			case 11: strcat(cadena,"c.eq.s ");
						break;
			case 12: strcat(cadena,"c.le.s ");
						break;
			case 13: strcat(cadena,"c.lt.s ");
						break;
			case 14: strcat(cadena,"c.lt.s ");
						break;
			case 15: strcat(cadena,"c.le.s ");
						break;
			case 16: strcat(cadena,"c.eq.s ");
						break;
		}

		strcat(cadena,registro1);
		strcat(cadena,", ");
		strcat(cadena,registro2);			
		strcat(cadena,"\n");

		switch (salto)
		{
			case 11: strcat(cadena,"bc1t ");
						break;
			case 12: strcat(cadena,"bc1t");
						break;
			case 13: strcat(cadena,"bc1f ");
						break;
			case 14: strcat(cadena,"bc1t ");
						break;
			case 15: strcat(cadena,"bc1f ");
						break;
			case 16: strcat(cadena,"bc1f ");
						break;
		}
		
		if (tipoEtiqueta == 1)
			sprintf(auxiliar,"__error%d",*etiquetaNook);
		else
			sprintf(auxiliar,"__e%d",*etiquetaNook);
			
		strcat(cadena,auxiliar); 
	}
	
	escribirFichero(cadena,1,0);

	//printf("IMPRIMO UNA ETIQUETA 2 %s\n", cadena);
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);

}

void generarMovimiento(int tipo, char *registroOrigen, char *registroDestino)
{
	int i;

	char cadena[256];
	char auxiliar[256];
	
	bzero(cadena,256);
	bzero(auxiliar,256);
	
	if (tipo < 3)
		strcat(cadena,"move ");
	else
		strcat(cadena,"mov.s ");
	
	strcat(cadena,registroOrigen);
	strcat(cadena,", ");
	strcat(cadena,registroDestino);
	
	escribirFichero(cadena,1,0);

	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
}


void definirSalidas(int tipo)
{
	char cadena[256];
	
	bzero(cadena,256);
	
	switch (tipo)
	{
		case 0:   strcat(cadena,"li $v0, 11");
					 break;
		case 1:   strcat(cadena,"li $v0, 1");
					 break;
		case 2:   strcat(cadena,"li $v0, 1");
				    break;
		case 3:   strcat(cadena,"li $v0, 2");
					 break;
		case -10: strcat(cadena,"li $v0, 4");
					 break;
	}	
	
	escribirFichero(cadena,1,0);
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
}

void definirEntradas(int tipo)
{
	char cadena[256];
	
	bzero(cadena,256);
	
	switch (tipo)
	{
		case 0:   strcat(cadena,"li $v0, 12");
					 break;
		case 1:   strcat(cadena,"li $v0, 5");
					 break;
		case 2:   strcat(cadena,"li $v0, 5");
				    break;
		case 3:   strcat(cadena,"li $v0, 6");
					 break;
		case -10: strcat(cadena,"li $v0, 8");
					 break;
	}	
	
	escribirFichero(cadena,1,0);
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
}

void solicitarFuncion(char *funcion)
{
	char cadena[256];
	
	bzero(cadena,256);

	strcat(cadena,"jal ");
	strcat(cadena,funcion);
	
	escribirFichero(cadena,1,0);

	if (valorFlag(2) == 1)
		printf("%s\n", cadena);

	iniciarRegistros();
}


void generarSalida(tSimbolos TS, char *variable, char *dimensiones)
{
	int tipo;
	int tamano;
	int global;
	int desplazamiento = 0;
	char reg[5];
	char rega[4];
	char regf[5];
	char regv[4];
	char cadena[256];
	
	bzero(cadena,256);
	bzero(rega,4);
	bzero(regf,5);
	bzero(reg,5);
	bzero(regv,4);
	
	strcat(cadena,"syscall");
	strcat(rega,"$a0");
	strcat(regf,"$f12");
	strcat(regv,"$v0");

	global = existeVariableGlobal(TS,variable);
	tipo   = obtenerValoresVariable(TS,variable,1);
	tamano = obtenerValoresVariable(TS,variable,3);


	if ((valorFlag(1) == 1) && (!global))
		desplazamiento = obtenerOffsetVariableActual(ultimo(TS),variable);
	
	definirSalidas(tipo);
	
	obtenerRegistroOcupado(tipo,variable,reg);

	//printf("AQUI ESTOY %s\n", reg);
	
	if (tipo == 3)
	{
		if (tamano > 0)
				generarArray(TS,2,tipo,variable,dimensiones,regf,1);
		else
		{
			if (strlen(reg) == 0)
			{
				enbargarRegistro(tipo, variable, regf);
				if ((valorFlag(1) == 1) && (!global))//Recojo de la pila
				{
					if (desplazamiento == 0)
						generarInstruccionCarga(regf,NULL,"$sp",tipo,-1);
					else
						generarInstruccionCarga(regf,NULL,"$sp",tipo,desplazamiento);
				}
				else
					generarInstruccionCarga(regf,NULL,variable,tipo,0);
			}
			else if (strcmp(reg,"$f12") != 0)
				generarMovimiento(tipo, regf, reg);
		}	
	}
	else if (tipo == 0)
	{
		if (tamano > 0)
			generarArray(TS,2,tipo,variable,dimensiones,rega,2);
		else
		{
			if (strlen(reg) != 0)
				generarMovimiento(tipo, rega, reg);
			else
			{
				if ((valorFlag(1) == 1) && (!global)) //Recojo de la pila
				{
					if (desplazamiento == 0)
						generarInstruccionCarga(rega,NULL,"$sp",tipo,-1);
					else	
						generarInstruccionCarga(rega,NULL,"$sp",tipo,desplazamiento);
				}
				else
					generarInstruccionCarga(rega,NULL,variable,tipo,0);
			}
		}
	}
	else
	{
			if (tamano > 0)
				generarArray(TS,2,tipo,variable,dimensiones,rega,1);
			else
			{
				if (strlen(reg) == 0)
				{
					if ((valorFlag(1) == 1) && (!global))//Recojo de la pila
					{
						if (desplazamiento == 0)
							generarInstruccionCarga(rega,NULL,"$sp",tipo,-1);
						else
							generarInstruccionCarga(rega,NULL,"$sp",tipo,desplazamiento);
					}
					else
						generarInstruccionCarga(rega,NULL,variable,tipo,0);
				}
				else
					generarMovimiento(tipo, rega, reg);
			}
	}
	
	escribirFichero(cadena,1,0);
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
}

void generarEntrada(tSimbolos TS, char *variable, char *dimensiones)
{
	int tipo;
	int tamano;
	int global;
	int desplazamiento = 0;
	char reg[5];
	char rega0[4];
	char rega1[4];
	char regv[4];
	char regf[4];
	char cadena[256];
	
	bzero(cadena,256);
	
	bzero(reg,5);
	bzero(rega0,4);
	bzero(rega1,4);
	bzero(regv,4);
	bzero(regf,4);
	
	strcat(regv, "$v0");
	strcat(rega0,"$a0");
	strcat(rega1,"$a1");
	strcat(regf, "$f0");
	strcat(cadena,"syscall");
	
	global = existeVariableGlobal(TS,variable);
	tipo   = obtenerValoresVariable(TS,variable,1);
	tamano = obtenerValoresVariable(TS,variable,3);

	if (valorFlag(1) == 1)
			desplazamiento = obtenerOffsetVariableActual(ultimo(TS),variable);		
	
	definirEntradas(tipo);
	
	escribirFichero(cadena,1,0);
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
	
	obtenerRegistroOcupado(tipo,variable,reg);
		
	if (tipo == 3)
	{
		if (tamano > 0)
		{
			generarArray(TS,2,tipo,variable,dimensiones,regf,2);
		}
		else
		{
			if (strlen(reg) != 0)
				generarMovimiento(tipo, regv, reg);		
		
			if ((valorFlag(1) == 1) && (!global)) //Recojo de la pila
				generarInstruccionAlmacenamiento(regf,NULL,"$sp",tipo,desplazamiento);
			else
				generarInstruccionAlmacenamiento(regf, NULL, variable, tipo, 0);
		}
	}
	else if (tipo == 0)
	{
		if (tamano > 0)
			generarArray(TS,2,tipo,variable,dimensiones,rega0,2);
		else
		{
			if (strlen(reg) != 0)
				generarMovimiento(tipo, regv, reg);
			else
			{
				if ((valorFlag(1) == 1) && (!global)) //Recojo de la pila
					generarInstruccionAlmacenamiento(regv,NULL,"$sp",tipo,desplazamiento);
				else
					generarInstruccionAlmacenamiento(regv,NULL,variable,tipo,0);
			}
		}
	}
	else
	{
		if (tamano > 0)
			generarArray(TS,2,tipo,variable,dimensiones,regv,2);
		else
		{
			if (strlen(reg) != 0)
				generarMovimiento(tipo, regv, reg);
			else
			{
				if ((valorFlag(1) == 1) && (!global)) //Recojo de la pila
					generarInstruccionAlmacenamiento(regv,NULL,"$sp",tipo,desplazamiento);
				else
					generarInstruccionAlmacenamiento(regv,NULL,variable,tipo,0);
			}
		}
	}
}

void imprimirSaltoLinea()
{
	char cadena[256];
	
	definirSalidas(-10);
	
	bzero(cadena,256);
	strcat(cadena,"la $a0 exp_salto");
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
	
	bzero(cadena,256);
	strcat(cadena,"syscall");
	escribirFichero(cadena,1,0);

	if (valorFlag(2) == 1)	
		printf("%s\n", cadena);
}

void imprimirFinalPrograma()
{
	char cadena[256];
	
	bzero(cadena,256);
	strcat(cadena,"li $v0, 10");
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
	
	bzero(cadena,256);
	strcat(cadena,"syscall");
	escribirFichero(cadena,1,0);
	
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
	
}

void imprimirCadenasExpeciales()
{
	char cadena[256];
	
	bzero(cadena,256);
	strcat(cadena,"exp_salto: .asciiz \"\\n\"");
	
	escribirFichero(cadena,1,0);
	if (valorFlag(2) == 1)
		printf("%s\n", cadena);
	
	
}
