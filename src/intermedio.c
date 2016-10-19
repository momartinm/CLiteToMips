/******************************************************
* Programa    : MIPS R3000                            *
* Módulo      : intermedio.c                          *
* Autor       : Moises Martínez Muñoz                 *
* Autor       : Ignacio Álvarez Santiago              *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intermedio.h"

extern int etiqueta;
extern int identificador;
extern PilaE pEti;

lAuxiliar obtenerDatosFor(tSimbolos TS, PilaE *pFor)
{
	lAuxiliar aux;
	char cadena[255];
	char *auxiliar;

	bzero(cadena,255);

	strcat(cadena,desapilarEtiqueta(pFor));

	aux = NULL;
	auxiliar = strtok(cadena,",");

	while (auxiliar != NULL)
	{
		if (esOperador(auxiliar) == 0) //Es una variable o un número
		{
			if (posibleNumero(auxiliar) != 0) //Es un numero
				insertarAuxiliar(&aux,auxiliar,2,1,-1,NULL);
			else	// Es una variable
			{
				if (existeVariableGlobal(TS,auxiliar) == 1)
					insertarAuxiliar(&aux,auxiliar,obtenerValoresVariable(TS,auxiliar,1),2,-1,NULL);
				else
					insertarAuxiliar(&aux,auxiliar,obtenerValoresVariable(ultimo(TS),auxiliar,1),2,-1,NULL);
			}
		}
		else //Es un operador
			insertarAuxiliar(&aux,auxiliar,50,0,-1,NULL);
		
		auxiliar = strtok(NULL,",");
	}

	return aux;
}


void generarDatosFor(lAuxiliar *expresion, PilaE *pFor)
{
	lAuxiliar aux;
	char cadena[255];
	char auxiliar[255];

	bzero(cadena,255);
	bzero(auxiliar,255);

	while (!listaVaciaAuxiliar(*expresion))
	{
		//printf("%s",(*expresion)->elemento);	
		strcat(cadena,(*expresion)->elemento);
		strcat(cadena,",\0");

		aux = *expresion;
		(*expresion) = (*expresion)->sgte;
		free(aux->elemento);
		free(aux);
	}
	(*expresion) = NULL;
	apilarEtiqueta(pFor, cadena);
}

lAuxiliar generarPostFijo(lAuxiliar expresion)
{
	int operador;
	int tipo;
	int tipoDato;
	int posicion;
	
	char auxiliar[256];
	
	lAuxiliar L    = NULL;
	lAuxiliar nAux = NULL;
	Pila 	    P    = NULL;

	while (!listaVaciaAuxiliar(expresion))
	{	
		//printf("ELEMENTO: %s %s\n", expresion->elemento, expresion->indice);

		operador = esOperador(expresion->elemento);
		
		bzero(auxiliar,256);
		
		if (operador != 0)
		{
			if (operador == 8)		//Leido )
			{
				while (strcmp(cabeceraPila(&P),"(") != 0)
					insertarNodoAuxiliar(&L,desapilar(&P));

         	nAux = desapilar(&P);
         	free(nAux);
			}
			else if (operador == 9) //Leido (
			{	
				nAux = nuevoNodoAuxiliar("(", 50, 0, -1, "");
				apilar(&P,nAux);
			}
			else
			{
				while ((!pilaVacia (P)) && (strcmp(cabeceraPila(&P),"(") != 0) && (prioridad(cabeceraPila(&P)) >= prioridad(expresion->elemento)))
            				insertarNodoAuxiliar(&L,desapilar(&P));

            			nAux = nuevoNodoAuxiliar(expresion->elemento,expresion->tipoDato,expresion->tipo,expresion->posicion,expresion->indice);
         			apilar (&P,nAux);
			}
		}
		else
		{
			nAux = nuevoNodoAuxiliar(expresion->elemento,expresion->tipoDato,expresion->tipo,expresion->posicion,expresion->indice);
         		apilar (&P,nAux);
		}
		
		expresion = expresion->sgte;
	}
	
	while (!pilaVacia(P))
		insertarNodoAuxiliar(&L,desapilar(&P));
	
	return L;
}

arbol generarArbolExpresion(lAuxiliar *A)
{
	pilaArbol pAux;
	arbol     aAux;
	arbol	nodo1, nodo2, nodoAux;
	int operador;
	
	lAuxiliar  aux;

	
	while (!listaVaciaAuxiliar(*A))
	{
		aux = *A;
		
		//printf("ELMENTO: %s con Dimension %s %d\n", aux->elemento, aux->indice, aux->tipo);

		operador = esOperador(aux->elemento);
		//printf("INDICE: %s\n", aux->indice);
		nodoAux = nuevoNodoArbol(aux->elemento,aux->tipo,aux->tipoDato,aux->posicion,aux->indice);
		
		if (operador != 0)
		{
			nodo2 = desapilarArbol(&pAux);
			nodo1 = desapilarArbol(&pAux);
			
			insertarNodoArbol(&nodoAux,nodo2);
			insertarNodoArbol(&nodoAux,nodo1);

			apilarArbol(&pAux,nodoAux);
		}
		else
			apilarArbol(&pAux,nodoAux);
	
		(*A) = (*A)->sgte;
		free(aux->elemento);
		free(aux);
	}
	
	return nodoAux;
}

void buscarRegistro(arbol nodo, char *reg)
{
	char auxiliar[256];
	
	if (nodo->posicion != -1)
	{
		bzero(auxiliar,256);
		sprintf(auxiliar,"$%s%s",nodo->elemento,nodo->indice);
		//printf("DIMENSIONES %s\n", nodo->indice);		
		//printf("AUXILIAR %s %s\n", auxiliar, nodo->indice);
		obtenerRegistroOcupado(nodo->tipoDato,auxiliar,reg);
	}
	else
		obtenerRegistroOcupado(nodo->tipoDato,nodo->elemento,reg);
}

void generarRegistro(arbol nodo, tSimbolos TS, char *reg, int tipoFuncion, int auxiliares)
{
	char auxiliar[256];
	
	char rege[5];

	//printf("NODO: %s DIMENSIONES %s POSICION %d\n", nodo->elemento, nodo->indice, nodo->posicion);

	if (nodo->posicion != -1) //Si es un array
	{		
		bzero(auxiliar,256);
		sprintf(auxiliar,"$%s%s",nodo->elemento,nodo->indice);	
		obtenerRegistroLibre(nodo->tipoDato,auxiliar,reg,auxiliares,1);
		//printf("REGISTRO %s %s\n", auxiliar, reg);
		//generarArray(TS,nodo->tipoDato,nodo->elemento,nodo->posicion,nodo->indice,reg,1);	
		generarArray(TS,2,nodo->tipoDato,nodo->elemento,nodo->indice,reg,1);		
	}
	else
	{
		obtenerRegistroLibre(nodo->tipoDato,nodo->elemento,reg,auxiliares,1);
		
		//printf("Esto es el registro seleccionado %s y tipo %d\n", reg, nodo->tipoDato);

		if (nodo->tipo == 1) //Si se carga un literal entero
		{
			if (nodo->tipoDato < 3)	//Si es un entero se carga con 5
				generarInstruccionCarga(reg,NULL,nodo->elemento,5,0);
			else	//Si es flotante se cargan con 7
			{
				bzero(rege,5);
				obtenerRegistroLibre(2,nodo->elemento,rege,1,1);
				generarInstruccionCarga(rege,reg,nodo->elemento,7,0);
				modificarRegistro(1,obtenerPosicionRegistro(rege),0,"",0,0);
			}
		}
		else //Si se carga cualquieR cosa menos el literal
		{
			if ((tipoFuncion == 1) && (!existeVariableGlobal(TS, nodo->elemento)))
			{
				if (obtenerOffsetVariableActual(ultimo(TS),nodo->elemento) == 0)
					generarInstruccionCarga(reg,NULL,"$sp",nodo->tipoDato,-1);
				else
					generarInstruccionCarga(reg,NULL,"$sp",nodo->tipoDato,obtenerOffsetVariableActual(ultimo(TS),nodo->elemento));
			}
			else
				generarInstruccionCarga(reg,NULL,nodo->elemento,nodo->tipoDato,0);
		}
	}
}

void modificarTipoRegistro(arbol nodo1, arbol nodo2, char *reg1, char *reg2)
{
	int tipoOperacion;
	
	tipoOperacion = mayor(nodo1->tipoDato,nodo2->tipoDato);

	if ((nodo1->tipoDato > 2) && (nodo2->tipoDato < 3))
		generarTransformacion(tipoOperacion,reg2,nodo2->tipoDato,nodo2->elemento);
	else if ((nodo1->tipoDato < 3) && (nodo2->tipoDato > 2))
		generarTransformacion(tipoOperacion,reg1,nodo1->tipoDato,nodo1->elemento);
}

void eliminarRegistro(arbol nodo, char * reg)
{
	int posicion;

	//Se obtiene la posición del registro 1 de la operacion
	posicion = obtenerPosicionRegistro(reg);

	//Eliminación del nodo 1
	if ((nodo->tipoDato < 3) && (esRegistroAuxiliar(1,posicion))) //Se comprueba si es entero y se elimina
		modificarRegistro(1,posicion,0,"",0,0);
	else if ((nodo->tipoDato > 2) && (esRegistroAuxiliar(2,posicion)))	// Se comprueba si es real y se elimina
		modificarRegistro(2,posicion,0,"",0,0);
}

void generarInstruccionesExpresion(arbol a, tSimbolos TS, int tipoFuncion)
{
	arbol nodo1; 
	arbol nodo2;
	
	char reg1[5];
	char reg2[5];
	char regr[5];
	char rege[5];
	char regd[5];
	
	char idr[5];

	char auxiliar[256];
	
	int operador;
	int tipoOperacion;
	int pos1, pos2;
	int posicion;

	if (!arbolVacio(a))
	{
		nodo1 = a->izdo;
		nodo2 = a->dcho;
		
		generarInstruccionesExpresion(nodo1,TS,tipoFuncion);
		generarInstruccionesExpresion(nodo2,TS,tipoFuncion);
		
		operador = esOperador(a->elemento);
		
		//printf("LEO %s\n", a->elemento);
		
		if (operador != 0) //El nodo contiene un operador
		{
			//printf("ENTRO POR AQUI\n");
		
			//Inicializamos los registros auxiliares
			bzero(reg1,5);
			bzero(reg2,5);
			bzero(regr,5);
			bzero(regd,5);
			bzero(idr, 5);

			//Comprobamos el tipo de operacion
			tipoOperacion = mayor(nodo1->tipoDato,nodo2->tipoDato);
			
			//Generamos el identificador de el elemento auxiliar
			sprintf(idr,"@%d",identificador);
			identificador++;
			
			//Se recogen los registros de los elementos
			obtenerRegistroLibre(tipoOperacion,idr,regr,1,1);
			
			//printf("NODO1: %s %s\n", nodo1->elemento, nodo1->indice);
			//printf("NODO2: %s %s\n", nodo2->elemento, nodo2->indice);

			buscarRegistro(nodo1,reg1);
			buscarRegistro(nodo2,reg2);
			//printf("REGISTROS %s %s #################\n", reg1, reg2);
			modificarTipoRegistro(nodo1,nodo2,reg1,reg2);

			//printf("OPERADOR DE TIPO %d\n", operador);


			if (operador == 10) //Si el operador es una asignación se realiza una carga
			{
				//printf("ES UNA CARGA\n");

				obtenerRegistroOcupado(nodo1->tipoDato,nodo1->elemento,regd);
				
				if (strlen(regd) == 0)
					generarRegistro(nodo1,TS,regd,tipoFuncion,0);
			
				generarMovimiento(tipoOperacion,regd,reg2);
				generarInstruccionAlmacenamiento(reg2,NULL,nodo1->elemento,tipoOperacion,0);
			}
			else					  //Sino se realiza una operación
			{
				//printf("GENERAR OPERACION %s %s %s\n", regr, reg1, reg2);
				generarInstruccionOperacion(regr,reg1,reg2,tipoOperacion,esOperador(a->elemento));
			}
			//Se modifican los datos del nodo del arbol para adaptarlo a los nuevos datos generado por la operación

			memcpy(a->elemento,idr,strlen(idr));
			a->tipo     = nodo1->tipo;
			a->tipoDato = tipoOperacion;

			//Se obtiene la posición del registro 1 de la operacion
			//pos1 = obtenerPosicionRegistro(reg1);

			eliminarRegistro(nodo1,reg1);
			//Se elimina el nodo del arbol
			eliminarNodo(&(a->izdo));

			eliminarRegistro(nodo2,reg2);
			//Se elimina el nodo del arbol
			eliminarNodo(&(a->dcho));

		}
		else	//El nodo contiene un operando
		{
			//printf("aqui se utilizan los registro %s %d\n", a->elemento, a->posicion);
	
			//Se inicializan los elementos auxiliar
			bzero(regr,5);
			
			//Se comprueba si el registro ya existe
			obtenerRegistroOcupado(a->tipoDato,a->elemento,regr);

			
	
			if (strlen(regr) == 0)	//Si no existe	
				generarRegistro(a,TS,regr,tipoFuncion,0);
			else if (a->posicion > 0) //Es un Array
			{
				//printf("LLEGO AQUI\n");
				generarRegistro(a,TS,regr,tipoFuncion,0);
			}

			//printf("REGR: %s\n", regr);
		}
	}
}

void etiquetarArbol(arbol a, int tipo, arbol padre)
{
	arbol nodo1, nodo2;
	int operador;
	int operadorPadre;

	if (!arbolVacio(a))
	{
		nodo1 = a->dcho;
		nodo2 = a->izdo;

		operador = esOperador(a->elemento);
		
		if (padre != NULL)
		{
			operadorPadre = esOperador(padre->elemento);
			if (operadorPadre == 6)  //Si mi padre es un AND &
			{
				if (tipo == 1)	//Soy hijo derecho
				{
					modificarEtiquetaOk(a,obtenerEtiquetaOk(padre));
					modificarEtiquetaNook(a,obtenerEtiquetaNook(padre));
				}
				else		//Soy hijo izquierdo
				{
					modificarEtiquetaOk(a,obtenerEtiqueta(padre->dcho));
					modificarEtiquetaNook(a,obtenerEtiquetaNook(padre));
				}

				crearEtiqueta(a,etiqueta);
				etiqueta++;
			}
			else if (operadorPadre == 7)	//Si mi padre es un OR |
			{
				if (tipo == 1)	//Soy hijo derecho
				{
					modificarEtiquetaOk(a,obtenerEtiquetaOk(padre));
					modificarEtiquetaNook(a,obtenerEtiquetaNook(padre));	
				}
				else		//Soy hijo izquierdo
				{
					modificarEtiquetaOk(a,obtenerEtiquetaOk(padre));
					modificarEtiquetaNook(a,obtenerEtiqueta(padre->dcho));
				}

				crearEtiqueta(a,etiqueta);				
				etiqueta++;
			}
		}
		else	//Caso base nodo raiz
		{
			crearEtiqueta(a,etiqueta);
			crearEtiquetaOk(a,etiqueta);
			crearEtiquetaNook(a,etiqueta);
			
			etiqueta++;
		}
		
		etiquetarArbol(nodo1,1,a); //Nodo Derecho
		etiquetarArbol(nodo2,2,a); //Nodo Izquierdo

		if ((a->etiqueta != NULL) && padre != NULL && tipo != 1) 
			modificarEtiqueta(padre, a->etiqueta);
	}
}

void generarInstruccionesBucle(arbol a, tSimbolos TS, int tipo, arbol padre, arbol raiz, int tipoFuncion)
{
	arbol nodo1, nodo2;
	
	int i;

	char reg1[5];
	char reg2[5];
	char regr[5];
	char rege[5];
	char idr[5];
	char auxiliar[256];
	char cadena[256];
	
	int operador;
	int operadorPadre;
	int tipoOperacion;
	int pos1, pos2;
	int posicion;

	if (!arbolVacio(a))
	{
		nodo1 = a->dcho;
		nodo2 = a->izdo;
		
		generarInstruccionesBucle(nodo2,TS,2,a,raiz,tipoFuncion); //Nodo Izquierdo
		generarInstruccionesBucle(nodo1,TS,1,a,raiz,tipoFuncion); //Nodo Derecho

		bzero(reg1,5);
		bzero(reg2,5);
		bzero(regr,5);
		bzero(rege,5);

		operador = esOperador(a->elemento);				

		if (padre != NULL)
		{
			operadorPadre = esOperador(padre->elemento);

			bzero(auxiliar,256);
			bzero(cadena,256);
			
			if (operador == 0 && ((operadorPadre == 6) || (operadorPadre == 7))) 
			{
				//Se recogen los registros de los elementos
				obtenerRegistroLibre(a->tipoDato,a->elemento,reg1,1,0);

				sprintf(auxiliar,"__e%d: ",*a->etiqueta);
				strcat(cadena, auxiliar);				

				escribirFichero(cadena,1,0);
				if (valorFlag(2) == 1)
					printf("%s\n", cadena);
				
				
				if (tipoFuncion == 1)
				{
					if (existeVariableGlobal(TS,a->elemento))
						generarInstruccionCarga(reg1,NULL,a->elemento,a->tipoDato,0);
					else
						generarInstruccionCarga(reg1,NULL,"$sp",a->tipoDato,obtenerOffsetVariableActual(TS,a->elemento));
				}
				else
					generarInstruccionCarga(reg1,NULL,a->elemento,a->tipoDato,0);

				
				if (a->tipoDato > 2)
				{
					bzero(auxiliar,256);
					obtenerRegistroLibre(3,a->elemento,reg2,1,0);
					sprintf(auxiliar,"!%s",a->elemento);					
					obtenerRegistroLibre(2,auxiliar,regr,1,1);
					
					generarInstruccionCarga(regr,reg2,"0.0",7,0);
				
					if (raiz->etiquetaNook == a->etiquetaNook)
						generarSaltosFlotante(16,a->tipoDato,reg1,reg2,a->etiqueta,a->etiquetaNook,0);
					else
						generarSaltosFlotante(16,a->tipoDato,reg1,reg2,a->etiqueta,a->etiquetaNook,0);
				}
				else
				{
					if (raiz->etiquetaNook == a->etiquetaNook)
						generarSaltosEntero(17,a->tipoDato,reg1,NULL,a->etiqueta,a->etiquetaNook,1);
					else
						generarSaltosEntero(17,a->tipoDato,reg1,NULL,a->etiqueta,a->etiquetaNook,0);
				}
				
			} 
			else
			{ 
				if (operador == 0) 
				{
					//Se inicializan los elementos auxiliar
					bzero(regr,5);
					bzero(auxiliar,256);
					
					if (tipo == 2)
					{
						sprintf(auxiliar,"__e%d: ",*padre->etiqueta);
						strcat(cadena, auxiliar);				

						escribirFichero(cadena,1,0);
						if (valorFlag(2) == 1)
							printf("%s\n", cadena);
					}
			
					//Se comprueba si el registro ya existe
		      			obtenerRegistroOcupado(a->tipoDato,a->elemento,regr);
		         
					if (strlen(regr) == 0)	//Si no existe
						generarRegistro(a,TS,regr,tipoFuncion,1);		
				} 
				else 
				{
					if ((operador != 6) && (operador != 7))
					{
						//Comprobamos el tipo de operacion
						tipoOperacion = mayor(nodo1->tipoDato,nodo1->tipoDato);
				
						buscarRegistro(nodo2,reg1);
						buscarRegistro(nodo1,reg2);
						
						if (tipoOperacion > 2)
						{
							modificarTipoRegistro(nodo2,nodo1,reg1,reg2);
							if (raiz->etiquetaNook == a->etiquetaNook)
								generarSaltosFlotante(operadorInverso(a->elemento),tipoOperacion,reg1,reg2,a->etiqueta,a->etiquetaNook,1);
							else
								generarSaltosFlotante(operadorInverso(a->elemento),tipoOperacion,reg1,reg2,a->etiqueta,a->etiquetaNook,0);
						}
						else
						{
							if (raiz->etiquetaNook == a->etiquetaNook)
								generarSaltosEntero(operadorInverso(a->elemento),tipoOperacion,reg1,reg2,a->etiqueta,a->etiquetaNook,1);
							else
								generarSaltosEntero(operadorInverso(a->elemento),tipoOperacion,reg1,reg2,a->etiqueta,a->etiquetaNook,0);
						}
					}
					
				}		
			
				if (operadorPadre == 7 && operador != 7 && tipo != 1) //Si mi padre es un OR, yo no y venimos de una rama izquierda entonces Branch
				{
					if (raiz->etiquetaOk == padre->etiquetaOk)
						generarSaltosEntero(18,padre->tipoDato,NULL,NULL,padre->etiquetaOk,NULL,1);
					else
						generarSaltosEntero(18,padre->tipoDato,NULL,NULL,padre->etiquetaOk,NULL,0);
				}
			}
		}
		else
		{
			if (operador == 0) 
			{
				//Se inicializan los elementos auxiliar
				bzero(regr,5);
				bzero(auxiliar,256);
					
				if (tipo == 2)
				{
					sprintf(auxiliar,"e%d: ",*padre->etiqueta);
					strcat(cadena, auxiliar);				

					escribirFichero(cadena,1,0);
					printf("%s\n", cadena);
				}
			
				//Se comprueba si el registro ya existe
		      		obtenerRegistroOcupado(a->tipoDato,a->elemento,regr);
		         
				if (strlen(regr) == 0)	//Si no existe
					generarRegistro(a,TS,regr,tipoFuncion,1);		
			} 
			else 
			{
				if ((operador != 6) && (operador != 7))
				{
					//Comprobamos el tipo de operacion
					tipoOperacion = mayor(nodo1->tipoDato,nodo1->tipoDato);
				
					buscarRegistro(nodo2,reg1);
					buscarRegistro(nodo1,reg2);
						
					if (tipoOperacion > 2)
					{
						modificarTipoRegistro(nodo2,nodo1,reg1,reg2);
						if (raiz->etiquetaNook == a->etiquetaNook)
							generarSaltosFlotante(operadorInverso(a->elemento),tipoOperacion,reg1,reg2,a->etiqueta,a->etiquetaNook,1);
						else
							generarSaltosFlotante(operadorInverso(a->elemento),tipoOperacion,reg1,reg2,a->etiqueta,a->etiquetaNook,0);
					}
					else
					{
						if (raiz->etiquetaNook == a->etiquetaNook)
							generarSaltosEntero(operadorInverso(a->elemento),tipoOperacion,reg1,reg2,a->etiqueta,a->etiquetaNook,1);
						else
							generarSaltosEntero(operadorInverso(a->elemento),tipoOperacion,reg1,reg2,a->etiqueta,a->etiquetaNook,0);
					}
				}
			}
		}
	}
}

void tratarExpresion(arbol a, tSimbolos TS)
{
	arbol nodo1; 
	arbol nodo2;
	
	char regr[5];
	char reg1[5];
	char reg2[5];
	char regd[5];
	char rega[5];

	char auxiliar[256];
	char cadena[256];
	
	int operador;
	int ambito;
	int esfuncion;
	
	if (strcmp(obtenerNombre(ultimo(TS)),"main") == 0)
		esfuncion = 0;
	else
		esfuncion = 1;
		
	//printf("AQUI ENTRO AL TRATAR EXPRESION %s\n", a->elemento);

	operador = esOperador(a->elemento);

	if (operador == 10) //Asignaciones
	{
		nodo1 = a->izdo;
		nodo2 = a->dcho;
		
		ambito = obtenerValoresVariable(TS,nodo1->elemento,2);

		//printf("ENTRO AQUI AMBITO: %d\n", ambito);
		
		if (ambito != 0)
		{

			//printf("ENTRO AQUI 222\n");

			bzero(regr,5);
			bzero(reg1,5);
			bzero(reg2,5);
			bzero(regd,5);
			bzero(rega,5);
			bzero(auxiliar,255);

			generarInstruccionesExpresion(nodo2,TS,esfuncion);
			//printf("NODOOOOOOOO2 %s %s\n", nodo2->elemento, nodo2->indice);			
			sprintf(auxiliar,"%s%s",nodo2->elemento,nodo2->indice);
			obtenerRegistroOcupado(nodo2->tipoDato,nodo2->elemento,regr);
			
			//printf("Esto es una prueba %s, %d, %s\n", regr,nodo2->tipoDato,nodo2->elemento);

			//printf("HABER -->>>> %d\n", nodo1->posicion);

			if (nodo1->posicion != -1) //Es un array
			{
				//printf("ENTRO AQUI\n");
				//printf("ENTRO AQUI\n");
				//generarIndiceArray(nodo1->tipoDato,nodo1->posicion,nodo1->indice,nodo1->elemento,regr,2);
				//generarArray(TS,nodo1->tipoDato,nodo1->elemento,nodo1->posicion,nodo1->indice,regr,2

				generarArray(TS,2,nodo1->tipoDato,nodo1->elemento,nodo1->indice,regr,2);
			}
			else	//No es un array
			{
				//printf("AQUI COMIENZA EL RETORNO DE ALGO\n");
				//printf("ENTRO AQUI Y ES %s\n", nodo1->elemento);
				
				if (esfuncion == 1)
				{
					//printf("Esto es una prueba\n");
					if (existeVariableGlobal(TS,nodo1->elemento))
						generarInstruccionAlmacenamiento(regr,NULL,nodo1->elemento,nodo1->tipoDato,0);
					else
					{
						//printf("El nodo es %s y su offset es %d\n", nodo1->elemento, obtenerOffsetVariableActual(ultimo(TS),nodo1->elemento));
						
						//printf("EL OFFSET de %s es %d\n", nodo1->elemento, obtenerOffsetVariable(ultimo(TS),nodo1->elemento));

						//printf("ESTO ENTRO AQUI 123 %d %s\n", obtenerOffsetVariableActual(ultimo(TS),nodo1->elemento), nodo1->elemento);

						generarInstruccionAlmacenamiento(regr,NULL,"$sp",nodo1->tipoDato,obtenerOffsetVariableActual(ultimo(TS),nodo1->elemento));
						obtenerRegistroOcupado(nodo1->tipoDato,nodo1->elemento,rega);
						
						//printf("REGISTRO %s tipo %d\n",regr,esRegistroAuxiliar(obtenerPosicionRegistro(regr)));
						//printf("Registro aceptación %s\n", rega);
						
						//printf("ESTO ENTRO AQUI\n");
						
						if ((esRegistroAuxiliar(obtenerPosicionRegistro(regr)) == 1) && (strlen(rega) == 0) )
						{
							modificarRegistro(nodo1->tipoDato, obtenerPosicionRegistro(regr), 1, nodo1->elemento, 0, 0);
							//printf("AQUI DEBO ESCRIBIR\n");
						}
						else if (strlen(rega) > 0)
								generarMovimiento(nodo1->tipoDato, rega, regr);
						
					}
				}
				else
				{
					//Comprobamos si la variable está almacenada en un registro
					obtenerRegistroOcupado(nodo1->tipoDato,nodo1->elemento,regd);
					//Si existe actualizamos también ese registro debido a que aún no ha sido sustituido por el LRU.
					if (strlen(regd) != 0)
						generarMovimiento(nodo1->tipoDato, regd, regr);
						
					generarInstruccionAlmacenamiento(regr,NULL,nodo1->elemento,nodo1->tipoDato,0);
				}
			}
		}
		else
		{
			printf("ERROR SEMANTICO (Linea %d): No se puede modificar el valor de una constante\n", valorFlag(0));
			exit(0);
		}
	}
	else	//bucles if, while, for
	{
		etiquetarArbol(a,0,NULL);
		
		bzero(cadena,256);

		if (valorFlag(8) == 1) //IF 
		{
			desactivarFlag(8);
		
			bzero(auxiliar,256);
			sprintf(auxiliar,"__fin_if%d: ",*a->etiqueta);
			apilarEtiqueta(&pEti,auxiliar);	//Introducimos en la pila la etiqueta del fin if
			bzero(auxiliar,256);
			sprintf(auxiliar,"__error%d: ",*a->etiquetaNook);
			apilarEtiqueta(&pEti,auxiliar);	//Introducimos en la pila la etiqueta del else
			bzero(auxiliar,256);
			sprintf(auxiliar,"__fin_if%d ",*a->etiqueta);
			apilarEtiqueta(&pEti,auxiliar);	//Introducimos en la pila la etiqueta del fin if
		}
		else if (valorFlag(9) == 1) //WHILE
		{
		
			desactivarFlag(9);

			bzero(auxiliar,256);
			sprintf(auxiliar,"__while%d: ",*a->etiqueta);
			strcat(cadena,auxiliar);

			if (valorFlag(2) == 1)
				printf("%s\n", cadena);
			escribirFichero(cadena,1,0);

			bzero(auxiliar,256);
			sprintf(auxiliar,"__error%d: ",*a->etiquetaNook);
			apilarEtiqueta(&pEti,auxiliar);	//Introducimos en la pila la e
			
			bzero(auxiliar,256);
			sprintf(auxiliar,"j __while%d ",*a->etiqueta);
			apilarEtiqueta(&pEti,auxiliar);	//Introducimos en la pila la e
		}
		else if (valorFlag(10) == 1) //FOR
		{
			desactivarFlag(9);

			bzero(auxiliar,256);
			sprintf(auxiliar,"__for%d: ",*a->etiqueta);
			strcat(cadena,auxiliar);

			if (valorFlag(2) == 1)
				printf("%s\n", cadena);
			escribirFichero(cadena,1,0);

			bzero(auxiliar,256);
			sprintf(auxiliar,"__error%d: ",*a->etiquetaNook);
			apilarEtiqueta(&pEti,auxiliar);	//Introducimos en la pila la e
			
			bzero(auxiliar,256);
			sprintf(auxiliar,"j __for%d ",*a->etiqueta);
			apilarEtiqueta(&pEti,auxiliar);	//Introducimos en la pila la e
		}
		
		//activarFlag(5);
		generarInstruccionesBucle(a,TS,0,NULL,a,esfuncion);
		//desactivarFlag(5);

		bzero(auxiliar,256);
		bzero(cadena,256);
		
		sprintf(auxiliar,"__exito%d: ",*a->etiquetaNook);
		strcat(cadena,auxiliar);

		if (valorFlag(2) == 1)
			printf("%s\n", auxiliar);
		
		escribirFichero(auxiliar,1,0);
		
	}

	//printf("TERMINO\n");

	liberarRegistrosAuxiliares();	
	
}
