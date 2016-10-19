%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "control.h"
#include "tSimbolos.h"
#include "arbol.h"//#include "pila.h"
#include "pilaArbol.h"
#include "pilaEtiquetas.h"
#include "lAuxiliar.h"
#include "transformador.h"
#include "intermedio.h"
#include "fichero.h"


#define YYDEBUG 1
#define BUFEMS  4096
#define DIMENSIONES 4

tSimbolos  TS   = NULL;		//Variable de almancenamiento de la tabla de simbolos.
lAuxiliar  lAux = NULL;
lAuxiliar  lEx  = NULL;
PilaE pEti	= NULL;
PilaE pFor	= NULL;
arbol aAux	= NULL;
Pila pEx	= NULL;

int retornoFuncion 	= 0;
int numArgumento 		= 1;
int numeroArgumentos;			//Variable auxiliar
int numParametros;			//Variable auxiliar
int auxiliarArray;			//Variable auxiliar de almacenamiento de valor de dimensiones.
int auxiliarTipo;			//Variable auxiliar sde almacenamiento de los tipos de funciones y variables, reutilización de valores.
int auxiliarOffSet;

char buffer[256];			//Utilización para el almacenamiento de nombres o generación de cadenas con sprintf.
char funcionActual[256];		//Variable auxiliar para la comprobación de la función que se trata actualmente.
char funcionLlamada[256];
char bufferCorto[32];			//Utilización para el almacenamiento de nombres cortos o generción de cadenas con sprintf cortas.
char registro[5];			//Utilización para el almancenamiento de los nombre de los registros a utilizar.

int etiqueta = 0;
int identificador = 0;

%}

%union 
{
	char cadena [4096];
	char cadenaCorta [32];
	int entero;
	float real;
	
	struct {
		int entero;
		float real;
		char nombre [4096];
		int tipo; //-1 Texto(STRING) -2 void 0:char 1:entero corto 2:entero 3:float
		char dimensiones[32];
	} general;
	
	struct {
		int entero;
		float real;
		int tipo; //1:entero 3:float
	} numeros;
	
	int dimArrays[4];
}

%start programa

%token <cadena> TEXTO
%token <cadena> CARACTER
%token <general> ID
%token <entero> ENTERO
%token <real> REAL
%token <entero> INT
%token <entero> CHAR
%token <entero> FLOAT
%token <entero> LONG
%token <entero> SHORT
%token <entero> VOID
%token IF
%token ELSE
%token FOR
%token WHILE
%token MAIN
%token RETURN
%token DEFINE
%token ESCRIBIR
%token ESCRIBIRLN
%token LEER
%token MAS
%token MASUNARIO
%token MENOS
%token MENOSUNARIO
%token MULTIPLICACION
%token DIVISION
%token MODULO
%token MASIGUAL
%token MENOSIGUAL
%token MULIGUAL
%token DIVIGUAL
%token COMPARACION
%token MEI
%token MAI
%token DISTINTO
%token MENOR
%token MAYOR
%token ASIGNACION
%token AND
%token PUNTOYCOMA
%token OR
%token COMA
%token PARENTESISIZ
%token PARENTESISDE
%token CORCHETEIZ
%token CORCHETEDE
%token DOSPUNTOS
%token INICIO
%token FIN
%token NOT
%token COMILLAS
%token COMILLASIMPLE

%type <cadenaCorta>  dimension
%type <entero>  tipo_dato%type <entero>  elemento_base
%type <entero>  base_expresion_logica 
%type <entero>  elemento_expresion_matematica
%type <entero>  elemento_expresion_logica
%type <entero>  expresion_matematica
%type <entero>	 expresion_logica
%type <entero>  sentencia_especial
%type <entero>  sentencia_llamada
%type <entero>  elemento_argumento
%type <general> identificador_base
%type <general> valor_dimension
%type <general> dimension_operacion
%type <general> elemento_base_parametro
%type <general> elemento_constante
%type <general> identificador_operacion
%type <numeros> numero
%type <general> nombre_funcion
%type <general> elemento_variable


%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

programa
	: constantes elementos programa_principal
	;
	
/****************************************************
  	ZONA DE DECLARACION DE CONSTANTES
****************************************************/

constantes
	: constante constantes
	| /*LAMBDA*/
	;

constante 
    : DEFINE ID elemento_constante
		{
			if (!existeVariableGlobal(TS,$2.nombre))
			{
				insertarNuevaVariable(TS,0,$2.nombre,$3.tipo,0,0,NULL,$3.nombre); 			
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): La constante %s ya ha sido definida\n", valorFlag(0), ($2.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-3);
			}
		}
    ;

elemento_constante
    : numero
		{
			bzero(bufferCorto,32);

			if ($1.tipo < 3)
				sprintf(bufferCorto,"%d",$1.entero);
			else
				sprintf(bufferCorto,"%f",$1.real);
			strcpy($$.nombre,bufferCorto);
			$$.tipo   = $1.tipo;
		}
    | TEXTO
		{
			strcpy($$.nombre,$1);
			$$.tipo = -10;
		}
    ;
	
/****************************************************
  	INICIO ZONA DE DECLARACION DE FUNCIONES 
			Y VARIABLES GLOBALES
****************************************************/

elementos 
	: elementos elemento_vf 
	| /*LAMBDA*/
	;

elemento_vf
	: tipo_dato identificador_base estructura_variables PUNTOYCOMA
		{
			if (!existeVariableGlobal(TS,$2.nombre)) 
			{
				insertarNuevaVariable(TS,0,$2.nombre,$1,2,$2.entero,$2.dimensiones,0);
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable global %s ya definida\n", valorFlag(0),($2.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-4);
			}
		}
	| cabecera_funcion estructura_funcion 
		{

			desactivarFlag(1); 
			bzero(buffer,256);
			strcat(buffer,"#FIN FUNCION\n");
			escribirFichero(buffer,1,0);
			if (valorFlag(2) == 1)
				printf("%s\n",buffer);

			if (valorFlag(12) == 1)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Falta el retorno de la funcion %s\n", valorFlag(0), (funcionActual + 3));
				cerrarFicheroIncompleto(1);
				exit(-5);
			}

			bzero(funcionActual,256);
			iniciarRegistros();
		}
	;
	
estructura_variables
	: estructura_variables COMA identificador_base 
		{
			if (!existeVariableGlobal(TS,$3.nombre)) 
			{
				insertarNuevaVariable(TS,0,$3.nombre,$<entero>-1,2,$3.entero,$3.dimensiones,0);
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable global %s ya definida\n", valorFlag(0), ($3.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-6);
			}
		}
	| /*LAMBDA*/
	;

cabecera_funcion 
	: tipo_dato ID 
		{
			if (!existeFuncion(TS,$2.nombre))
			{
				insertarSimbolo(&TS,$2.nombre,$1);
				desactivarFlag(5);
				activarFlag(1); 

				bzero(buffer,256);
				strcat(buffer,"\n#INICIO FUNCION");
				escribirFichero(buffer,1,0);

				if (valorFlag(2) == 1)
					printf("%s\n",buffer);
				
				bzero(buffer,256);				
				sprintf(buffer,"%s:",$2.nombre);
				
				escribirFichero(buffer,1,0);
				if (valorFlag(2) == 1)				
					printf("%s\n", buffer);
				
				bzero(funcionActual,256);
		   		strcat(funcionActual,$2.nombre);

				if ($1 != -2)
					activarFlag(12);
				
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Funcion %s ya definida\n", valorFlag(0), ($2.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-7);
			}
		}
	;
	
estructura_funcion
	: parametros cuerpo
	;
	
tipo_dato
	: INT
	| CHAR
	| SHORT
	| FLOAT
	| VOID
	;
	
dimension
	: CORCHETEIZ valor_dimension CORCHETEDE dimension
		{
			bzero(bufferCorto,32);
			sprintf(bufferCorto,"%s,%s",$2.nombre,$4);
			strcpy($$,bufferCorto);;
		}
	| CORCHETEIZ valor_dimension CORCHETEDE
		{
			bzero($$,32);
			strcpy($$,$2.nombre);
		}
	;
	
valor_dimension
	: ENTERO 
		{
			bzero($$.nombre,4096);
			sprintf(bufferCorto,"%d",$1);
			strcpy($$.nombre,bufferCorto);
			$$.tipo = 1;
		}
	| ID	//Se utiliza una constante para definir la dimensión
		{
			bzero($$.nombre,4096);
			
			if (existeVariableGlobal(TS,$1.nombre))
			{
				auxiliarArray = obtenerValoresVariable(TS,$1.nombre,1);
			
				$$.tipo = auxiliarArray;
			
				if ((auxiliarArray >= 0) || (auxiliarArray < 3))
				{
					auxiliarArray = atoi(obtenerValorConstante(TS,$1.nombre));
					sprintf(bufferCorto,"%d",auxiliarArray);
					strcpy($$.nombre,bufferCorto);
				}
				else
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): La constante %s no es de tipo entero.\n", valorFlag(0), ($1.nombre+3));
					cerrarFicheroIncompleto(1);
					exit(-8);
				}
			}
			else if ((existeVariableGlobal(TS,$1.nombre)) || (existeVariable(TS,$1.nombre)))
			{
				auxiliarTipo = obtenerValoresVariable(TS,$1.nombre,1);
			
				$$.tipo = auxiliarTipo;
				
				if ((auxiliarTipo >= 0) || (auxiliarTipo < 3))
				{
					sprintf(bufferCorto,"%s",$1.nombre);
					strcpy($$.nombre,bufferCorto);
				}
				else
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): La variable %s no es de tipo entero.\n", valorFlag(0), ($1.nombre+3));
					cerrarFicheroIncompleto(1);
					exit(-9);
				}
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): La contante o variable %s no esta definida.\n", valorFlag(0), ($1.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-10);
			}
		}
	;
	
dimension_operacion
	: CORCHETEIZ valor_dimension CORCHETEDE dimension_operacion
		{
			if (($2.tipo >= 0) && ($2.tipo < 3))
			{
				$$.entero      = $4.entero + 1;
				sprintf(bufferCorto,"%s,%s",$2.nombre,$4.dimensiones);
				strcpy($$.dimensiones,bufferCorto);
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Solo son validas posiciones enteras.\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-11);
			}
		}
	|  CORCHETEIZ valor_dimension CORCHETEDE
		{
			if (($2.tipo >= 0) && ($2.tipo < 3))
			{
				$$.entero = 1;
				strcpy($$.dimensiones,$2.nombre);
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Solo son validas posiciones enteras.\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-12);
			}
		}
	;
	
parametros
	: PARENTESISIZ lista_parametros PARENTESISDE 
	| PARENTESISIZ PARENTESISDE
	;

lista_parametros
	: elemento_parametro 
    | elemento_parametro lista_parametros
	;

elemento_parametro
	: tipo_dato elemento_base_parametro COMA 
		{
			if ((!existeVariableGlobal(TS,$2.nombre)) && (!existeVariable(TS,$2.nombre)))
				insertarNuevaVariable(TS,1,$2.nombre,$1,3,$2.entero,NULL,0); 
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s ya definida\n", valorFlag(0),($2.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-13);
			}
		}
	| tipo_dato elemento_base_parametro 
		{
			if ((!existeVariableGlobal(TS,$2.nombre)) && (!existeVariable(TS,$2.nombre)))
				insertarNuevaVariable(TS,1,$2.nombre,$1,3,$2.entero,NULL,0);
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s ya definida\n", valorFlag(0),($2.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-14);
			}
		}
	;
	
elemento_base_parametro
	: ID 
		{
			strcpy($$.nombre,$1.nombre); 
			$$.entero = 0;
		}
	| ID CORCHETEIZ CORCHETEDE 
		{
			strcpy($$.nombre,$1.nombre); 
			$$.entero = -1;
		}
	;
	
cabecera_principal
	: tipo_dato MAIN PARENTESISIZ PARENTESISDE
		{
			insertarSimbolo(&TS,"main",$1);
			desactivarFlag(5);
				
			escribirFichero("main:",1,0);
			if (valorFlag(2) == 1)
				printf("%s\n", "main:");
		}
	;
	
programa_principal
	: cabecera_principal cuerpo
	;

/****************************************************
  	FIN ZONA DE DECLARACION DE FUNCIONES
****************************************************/
	
identificador_base 
	: ID 
		{
			strcpy($$.nombre,$1.nombre);
			$$.entero = 0;
		}
	| ID dimension 
		{
			strcpy($$.nombre,$1.nombre);
			bzero($$.dimensiones,32);
			strcpy($$.dimensiones,$2);
		}
	;

identificador_operacion
	: ID
		{
			if ((!existeVariableGlobal(TS,$1.nombre)) && (!existeVariable(TS,$1.nombre)))
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s no definida\n", valorFlag(0), ($1.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-15);
			}
			else
			{
				$$.tipo = obtenerValoresVariable(TS,$1.nombre,1);
				
				if (obtenerValoresVariable(TS,$1.nombre,3) != 0)
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Utilizacion de array sin seleccionar posicion\n", valorFlag(0));
					cerrarFicheroIncompleto(1);
					exit(-16);
				}
				if (valorFlag(4))
					insertarAuxiliar(&lEx,$1.nombre,$$.tipo,2,-1,NULL);
			}

			strcpy($$.nombre,$1.nombre);
		}
	| ID dimension_operacion
		{
			if ((!existeVariableGlobal(TS,$1.nombre)) && (!existeVariable(TS,$1.nombre)))
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s no definida\n", valorFlag(0), ($1.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-17);
			}
			else
			{
			
				$$.tipo = obtenerValoresVariable(TS,$1.nombre,1);
			
				//printf("VALORES %d == %d\n",$2.entero,obtenerValoresVariable(TS,$1.nombre,3));

				if ($2.entero != obtenerValoresVariable(TS,$1.nombre,3))
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s numero de dimensiones incorrecto.\n", valorFlag(0), ($1.nombre+3));
					cerrarFicheroIncompleto(1);
					exit(-18);
				}
				else
				{
					if (obtenerValoresVariable(TS,$1.nombre,3) < 1)
					{
						fprintf(stderr,"ERROR SEMANTICO (Linea %d): Seleccion de posicion en una variable de tipo distinto Array.\n", valorFlag(0));
						cerrarFicheroIncompleto(1);
						exit(-19);
					}
					if (valorFlag(4))
						insertarAuxiliar(&lEx,$1.nombre,$$.tipo,2,$2.entero,$2.dimensiones);
					
					strcpy($$.nombre,$1.nombre);
					strcpy($$.dimensiones,$2.dimensiones);
				}
			}
		}
	;

numero
    : ENTERO
		{
			sprintf(buffer,"%d",$1);
			insertarAuxiliar(&lEx,buffer,2,1,-1,NULL);
			$$.entero = $1;
			
			if (($1 >= -128) && ($1 <= 127))
				$$.tipo = 0;  //Char
			else if (($1 >= -32728) && ($1 >= 32767))
				$$.tipo = 1;  //short
			else
				$$.tipo = 2;  //integer
		}
    | REAL
		{
			sprintf(buffer,"%f",$1);
			insertarAuxiliar(&lEx,buffer,3,1,-1,NULL);
			$$.real = $1;
			$$.tipo = 3;
		}
    ;

operador_asignacion
	: ASIGNACION
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"=",50,0,-1,NULL);
				
			retornoFuncion = 1;
		}
	| MASIGUAL
		{
			if (valorFlag(10) == 1)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Operacion no permitida en bucle for\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-20);
			}
			else
			{
				if (valorFlag(4))
				{
					insertarAuxiliar(&lEx,"=",50,0,-1,NULL);																						 
					insertarAuxiliar(&lEx,obtenerAuxiliar(ultimoAuxiliar(lEx)),obtenerTipoAuxiliar(ultimoAuxiliar(lEx)),obtenerTipoAuxiliar(ultimoAuxiliar(lEx)),obtenerPosicionAuxiliar(ultimoAuxiliar(lEx)),obtenerIndiceAuxiliar(ultimoAuxiliar(lEx)));
					insertarAuxiliar(&lEx,"+",50,0,-1,NULL);
				}
			}
		}
	| MENOSIGUAL
		{
			if (valorFlag(10) == 1)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Operacion no permitida en bucle for\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-21);
			}
			else
			{
				if (valorFlag(4))
				{
					insertarAuxiliar(&lEx,"=",50,0,-1,NULL);																						//
					insertarAuxiliar(&lEx,obtenerAuxiliar(ultimoAuxiliar(lEx)),obtenerTipoAuxiliar(ultimoAuxiliar(lEx)),obtenerTipoAuxiliar(ultimoAuxiliar(lEx)),obtenerPosicionAuxiliar(ultimoAuxiliar(lEx)),obtenerIndiceAuxiliar(ultimoAuxiliar(lEx)));
					insertarAuxiliar(&lEx,"-",50,0,-1,NULL);
				}
			}
		}
	| MULIGUAL
		{
			if (valorFlag(10) == 1)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Operacion no permitida en bucle for\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-22);
			}
			else
			{
				if (valorFlag(4))
				{
					insertarAuxiliar(&lEx,"=",50,0,-1,NULL);																						
					insertarAuxiliar(&lEx,obtenerAuxiliar(ultimoAuxiliar(lEx)),obtenerTipoAuxiliar(ultimoAuxiliar(lEx)),obtenerTipoAuxiliar(ultimoAuxiliar(lEx)),obtenerPosicionAuxiliar(ultimoAuxiliar(lEx)),obtenerIndiceAuxiliar(ultimoAuxiliar(lEx)));
					insertarAuxiliar(&lEx,"*",50,0,-1,NULL);
				}
			}
		}
	| DIVIGUAL
		{
			if (valorFlag(10) == 1)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Operacion no permitida en bucle for\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-23);
			}
			else
			{
				if (valorFlag(4))
				{
					insertarAuxiliar(&lEx,"=",50,0,-1,NULL);																					
					insertarAuxiliar(&lEx,obtenerAuxiliar(ultimoAuxiliar(lEx)),obtenerTipoAuxiliar(ultimoAuxiliar(lEx)),obtenerTipoAuxiliar(ultimoAuxiliar(lEx)),obtenerPosicionAuxiliar(ultimoAuxiliar(lEx)),obtenerIndiceAuxiliar(ultimoAuxiliar(lEx)));
					insertarAuxiliar(&lEx,"/",50,0,-1,NULL);
				}
			}
		}
	;

operador_matematico
   : MAS
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"+",50,0,-1,NULL);
		}
	| MENOS
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"-",50,0,-1,NULL);
		}
	| MULTIPLICACION
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"*",50,0,-1,NULL);
		}
	| DIVISION
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"/",50,0,-1,NULL);
		}
	| MODULO
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"%",50,0,-1,NULL);
		}
	;

operador_comparacion
	: COMPARACION 
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"==",50,0,-1,NULL);
		}
	| MEI
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"<=",50,0,-1,NULL);
		}	
	| MAI
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,">=",50,0,-1,NULL);
		}
	| DISTINTO
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"!=",50,0,-1,NULL);
		}
	| MAYOR
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,">",50,0,-1,NULL);
		}
	| MENOR 
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"<",50,0,-1,NULL);
		}
	;
	
operador_logico
    : AND
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"&",50,0,-1,NULL);
		}
	| OR 
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"|",50,0,-1,NULL);
		}
	;

operador_unario
	: MASUNARIO
		{
			if (valorFlag(10) == 0)
			{
				insertarAuxiliar(&lEx,"=",50,0,-1,NULL);
				insertarAuxiliar(&lEx,obtenerAuxiliar(lEx),obtenerDatoAuxiliar(lEx),obtenerTipoAuxiliar(lEx),obtenerPosicionAuxiliar(lEx),obtenerIndiceAuxiliar(lEx));
				insertarAuxiliar(&lEx,"+",50,0,-1,NULL);
				insertarAuxiliar(&lEx,"1",2,1,-1,NULL);

				lAux = generarPostFijo(lEx);
				lEx  = NULL;
				aAux = generarArbolExpresion(&lAux);
				tratarExpresion(aAux,TS);
			}
			else
			{
				insertarAuxiliar(&lEx,"+",50,0,-1,NULL);
				insertarAuxiliar(&lEx,"1",2,1,-1,NULL);
			}
			
			
		}
	| MENOSUNARIO
		{

			if (valorFlag(10) == 0)
			{
				insertarAuxiliar(&lEx,"=",50,0,-1,NULL);
				insertarAuxiliar(&lEx,obtenerAuxiliar(lEx),obtenerDatoAuxiliar(lEx),obtenerTipoAuxiliar(lEx),obtenerPosicionAuxiliar(lEx),obtenerIndiceAuxiliar(lEx));
				insertarAuxiliar(&lEx,"-",50,0,-1,NULL);
				insertarAuxiliar(&lEx,"1",2,1,-1,NULL);
					
				lAux = generarPostFijo(lEx); //PRoceso de eliminacion de lEx
				lEx  = NULL;
				aAux = generarArbolExpresion(&lAux);
				tratarExpresion(aAux,TS);
			}
			else
			{
				insertarAuxiliar(&lEx,"-",50,0,-1,NULL);
				insertarAuxiliar(&lEx,"1",2,1,-1,NULL);
			}
		}
	;

/****************************************************
  	DEFINICION EXPRESIONES MATEMATICA
****************************************************/

parentesis_izq_expresion
	: PARENTESISIZ
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"(",50,0,-1,NULL);
		}
	;

parentesis_der_expresion
	: PARENTESISDE
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,")",50,0,-1,NULL);
		}
	;
	
negacion
	: NOT
		{
			if (valorFlag(4))
				insertarAuxiliar(&lEx,"!",50,0,-1,NULL);
		}
	;

elemento_base
	: identificador_operacion
		{
			$$ = $1.tipo;
		}
	 | numero
		{
			$$ = $1.tipo;
		}
	 | TEXTO
	 	{
			$$ = -10;
		}
	;

expresion_matematica
	: elemento_expresion_matematica operador_matematico expresion_matematica
		{
			if (($1 >= 0) && ($3 >= 0))
			{
				$$ = mayor($1,$3);
			}
			else
			{
				$$ = -3;
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): error de tipos\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-24);
			}
		}
	| elemento_expresion_matematica
		{
			$$ = $1;
		}
	;

elemento_expresion_matematica
	: elemento_base
		{
			$$ = $1;
		}
	| parentesis_izq_expresion expresion_matematica parentesis_der_expresion
		{
			$$ = $2;
		}
    ;

operador
   : operador_logico
	| operador_comparacion
	;
	
expresion_logica
	: elemento_expresion_logica operador expresion_logica
		{
			if (($1 >= 0) && ($3 >= 0))
			{
				$$ = mayor($1,$3);
			}
			else
			{
				$$ = -3;
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): error de tipos\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-25);
			}
		}
	| elemento_expresion_logica
		{
			$$ = $1;
		}

	;

elemento_expresion_logica
	: base_expresion_logica
		{
			$$ = $1;
		}
	| parentesis_izq_expresion expresion_logica parentesis_der_expresion
		{
			$$ = $2;
		}
    ;
	
	
base_expresion_logica
    : negacion elemento_base
		{
			$$ = $2;
		}
	| elemento_base
		{
			$$ = $1;
		}
	;

	
fin_expresion
	:	PARENTESISDE
		{
			lAux = generarPostFijo(lEx);
			lEx  = NULL;
			aAux = generarArbolExpresion(&lAux);
			tratarExpresion(aAux,TS);
		}
	;

/****************************************************
  	DEFINICION EXTRUCTURAS
****************************************************/

cuerpo
 	: INICIO variables sentencias FIN
 		{
 			if (strcmp(funcionActual,"main") != 0) //El proceso de recarga de $ra y de salto solo se realiza en funciones no en el MAIN
			{
				if (obtenerTipo(ultimo(TS)) == -2)
				{
					bzero(registro,5);
					strcpy(registro,"$ra");
		
					generarInstruccionCarga(registro,NULL,"$sp",2,(obtenerOffsetFuncion(TS,funcionActual)-4));
			
					escribirFichero("jr $ra",1,0);
					if (valorFlag(2) == 1)
						printf("%s\n","jr $ra");
				}
			}
 		}
	;

variables
	: elemento_variable variables 
	| /*LAMBDA*/
		{
			activarFlag(4);
			eliminarListaAuxiliar(&lEx);
			if (valorFlag(1) == 1)
			{
				bzero(registro,5);
				strcpy(registro,"$ra");
			
				generarInstruccionAlmacenamiento(registro,NULL,"$sp",2,(obtenerOffsetFuncion(TS,funcionActual)-4));
			}
		}
	;
	
elemento_variable
	: tipo_dato identificador_base identificador_variable PUNTOYCOMA 
		{	
			if ((!existeVariableGlobal(TS,$2.nombre)) && (!existeVariable(TS,$2.nombre)))
			{
				insertarNuevaVariable(TS,1,$2.nombre,$1,1,$2.entero,$2.dimensiones,NULL); 
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s ya definida\n", valorFlag(0), ($2.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-25);
			}
		}
	;
	
identificador_variable
	: COMA identificador_base identificador_variable
		{
			if (!existeVariableGlobal(TS,$2.nombre))
			{
				insertarNuevaVariable(TS,1,$2.nombre,$<entero>-1,1,$2.entero,$2.dimensiones,NULL);
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable local %s ya definida\n", valorFlag(0),($2.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-26);
			}
		}
	| /*LAMBDA*/
	;

sentencias 
	: sentencia sentencias
	| sentencia
	;
 
sentencia
	: sentencia_asignacion
	| sentencia_if
	| sentencia_while
	| sentencia_for
	| sentencia_retorno
	| sentencia_entrada_salida
	| sentencia_llamada PUNTOYCOMA
		{
		bzero(buffer,256);
		sprintf(buffer,"%d",obtenerOffsetFuncion(TS,funcionLlamada));
		generarInstruccionOperacion("$sp","$sp",buffer,5,5);

		}
	| sentencia_especial PUNTOYCOMA
	;
   
cuerpo_sentencia 
	: INICIO sentencias FIN
	| sentencia
	;


/****************************************************
  	SENTENCIA TIPO: ESPECIAL
****************************************************/

sentencia_especial
	: identificador_operacion operador_unario
		{
			$$ = $1.tipo;
		}
	;

/****************************************************
  	SENTENCIA TIPO: ASIGNACION
****************************************************/

sentencia_asignacion
	: identificador_operacion operador_asignacion expresion_matematica PUNTOYCOMA
		{
		
			//printf("OPERADOR = %s\n", $1.nombre, $1.dimensiones);
			if (($1.tipo >= 0) && ($3 >= 0))
			{
				if ($1.tipo < $3)
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Perdida de precision numerica\n", valorFlag(0));
					cerrarFicheroIncompleto(1);
					exit(-27);
				}
			}
			else
			{
				if (($1.tipo != $3))
				{
					if (($1.tipo != 0) || ($3 != -1))
					{
						fprintf(stderr,"ERROR SEMANTICO (Linea %d): Asignacion de char o void incorrecta, %d %d \n", valorFlag(0), $1.tipo, $3);
						cerrarFicheroIncompleto(1);
						exit(-28);
					}
				}
			}

			lAux = generarPostFijo(lEx);
			lEx  = NULL;
			aAux = generarArbolExpresion(&lAux);
			tratarExpresion(aAux,TS);
			eliminarListaAuxiliar(&lEx);
			
			retornoFuncion == 0;

		}
	| identificador_operacion operador_asignacion sentencia_llamada PUNTOYCOMA
		{
			bzero(registro,5);
			
			obtenerRegistroOcupado($1.tipo,$1.nombre,registro);
			
			if (strlen(registro) == 0)
			{
				obtenerRegistroLibre($1.tipo,$1.nombre,registro,1,1);
				generarInstruccionCarga(registro,NULL,"$sp",$1.tipo,-1);
			}

			if ((valorFlag(1) == 1) && (!existeVariableGlobal(TS,$1.nombre)))
			{
				generarInstruccionAlmacenamiento(registro,NULL,"$sp",$1.tipo,obtenerOffsetVariableActual(ultimo(TS),$1.nombre));
			}
			else
				generarInstruccionAlmacenamiento(registro,NULL,$1.nombre,$1.tipo,0);
			
			bzero(buffer,256);
			sprintf(buffer,"%d",obtenerOffsetFuncion(TS,funcionLlamada));
			generarInstruccionOperacion("$sp","$sp",buffer,5,5);
			
			retornoFuncion == 0;
		}
	;

/****************************************************
  	SENTENCIA TIPO: CONDICION IF
****************************************************/

elemento_else
	: ELSE
		{
			bzero(buffer,256);
			strcat(buffer,"b ");
			strcat(buffer,desapilarEtiqueta(&pEti));
			strcat(buffer,"\n");
			strcat(buffer,desapilarEtiqueta(&pEti));
			if (valorFlag(2) == 1)
				printf("%s\n", buffer);
			escribirFichero(buffer,1,0);
			eliminarRegistroProfundos(valorFlag(11));
			desactivarFlag(11);
			activarFlag(11);
		}
	;
	
sentencia_if
	: IF PARENTESISIZ expresion_logica fin_expresion cuerpo_sentencia %prec LOWER_THAN_ELSE
		{
			desapilarEtiqueta(&pEti);
			bzero(buffer,256);
			strcat(buffer,desapilarEtiqueta(&pEti));
			if (valorFlag(2) == 1)
				printf("%s\n", buffer);
			escribirFichero(buffer,1,0);
			desapilarEtiqueta(&pEti);
			eliminarRegistroProfundos(valorFlag(11));
			desactivarFlag(11);	
		}
	| IF PARENTESISIZ expresion_logica fin_expresion cuerpo_sentencia elemento_else cuerpo_sentencia
		{
			bzero(buffer,256);
			strcat(buffer,desapilarEtiqueta(&pEti));
			if (valorFlag(2) == 1)
				printf("%s\n", buffer);
			escribirFichero(buffer,1,0);
			eliminarRegistroProfundos(valorFlag(11));			
			desactivarFlag(11);
		}
	;

/****************************************************
  	SENTENCIA TIPO: BUCLE FOR
****************************************************/

cabecera_for_asignacion
	: ID operador_asignacion expresion_matematica PUNTOYCOMA
		{

			if ((!existeVariableGlobal(TS,$1.nombre)) && (!existeVariable(TS,$1.nombre)))
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s no definida\n", valorFlag(0), ($1.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-29);
			}
			else
			{
				$1.tipo = obtenerValoresVariable(TS,$1.nombre,1);
				if (obtenerValoresVariable(TS,$1.nombre,3) != 0)
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Utilizacion de array sin seleccionar posicion\n", valorFlag(0));
					cerrarFicheroIncompleto(1);
					exit(-30);
				}
				
				insertarAuxiliar(&lEx,$1.nombre,$1.tipo,2,-1,NULL);
			}	
		
			if (($1.tipo < 0) || ($3 < 0))
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Asignacion de char y void incorrecta\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-31);
			}
			if ($1.tipo < $3)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Perdida de precision numerica\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-32);
			}

						

			lAux = generarPostFijo(lEx);
			lEx  = NULL;
			aAux = generarArbolExpresion(&lAux);

			tratarExpresion(aAux,TS);

		}
	;
	
cabecera_for_comparacion
	: ID operador_comparacion expresion_logica PUNTOYCOMA
		{
			if ((!existeVariableGlobal(TS,$1.nombre)) && (!existeVariable(TS,$1.nombre)))
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s no definida\n", valorFlag(0), $1.nombre);
				cerrarFicheroIncompleto(1);
				exit(-33);
			}
			else
			{
				$1.tipo = obtenerValoresVariable(TS,$1.nombre,1);
				
				if (obtenerValoresVariable(TS,$1.nombre,3) != 0)
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Utilizacion de array sin seleccionar posicion\n", valorFlag(0));
					cerrarFicheroIncompleto(1);
					exit(-34);
				}
				
				insertarAuxiliar(&lEx,$1.nombre,$1.tipo,2,-1,NULL);
			}
		
			if (($1.tipo < 0) || ($3 < 0))
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Asignacion de char y void incorrecta\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-35);
			}
			
			lAux = generarPostFijo(lEx);
			lEx  = NULL;
			aAux = generarArbolExpresion(&lAux);
			tratarExpresion(aAux,TS);

			insertarAuxiliar(&lEx,$1.nombre,$1.tipo,2,-1,NULL);			
			insertarAuxiliar(&lEx,"=",50,0,-1,NULL);	//Si tenemos un ++ debe ser eliminado
		}
	;

cabecera_for_modificacion
	: expresion_matematica
		{
			if ($1 < 0)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Asignacion de char y void incorrecta\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-37);
			}
			
			generarDatosFor(&lEx, &pFor);
			desactivarFlag(10);
			
		}
	| sentencia_especial
		{
			generarDatosFor(&lEx, &pFor);
			desactivarFlag(10);
		}
	;
	
sentencia_for 
	:  FOR PARENTESISIZ cabecera_for_asignacion cabecera_for_comparacion cabecera_for_modificacion PARENTESISDE cuerpo_sentencia
		{
			lEx = obtenerDatosFor(TS, &pFor);
			lAux = generarPostFijo(lEx);
			lEx  = NULL;
			aAux = generarArbolExpresion(&lAux);
			tratarExpresion(aAux,TS);

			bzero(buffer,256);
			strcat(buffer,desapilarEtiqueta(&pEti));
			strcat(buffer,"\n");
			strcat(buffer,desapilarEtiqueta(&pEti));
			if (valorFlag(2) == 1)			
				printf("%s\n", buffer);

			escribirFichero(buffer,1,0);
			eliminarRegistroProfundos(valorFlag(11));
			desactivarFlag(11);

				
		}
	;

/****************************************************
  	SENTENCIA TIPO: BUCLE WHILE
****************************************************/

cabecera_while
	: WHILE PARENTESISIZ expresion_logica fin_expresion
	;
	
sentencia_while 
	: cabecera_while cuerpo_sentencia
		{
			bzero(buffer,256);
			strcat(buffer,desapilarEtiqueta(&pEti));
			strcat(buffer,"\n");
			strcat(buffer,desapilarEtiqueta(&pEti));
			if (valorFlag(2) == 1)			
				printf("%s\n", buffer);
			escribirFichero(buffer,1,0);
			eliminarRegistroProfundos(valorFlag(11));
			desactivarFlag(11);
		}
	;


/****************************************************
  	SENTENCIA TIPO: ENTRADA/SALIDA
****************************************************/

sentencia_entrada_salida 
	: sentencia_escritura PUNTOYCOMA
		{
			eliminarListaAuxiliar(&lEx);
		}
	| sentencia_lectura PUNTOYCOMA
		{
			eliminarListaAuxiliar(&lEx);
		}
	;
 
sentencia_escritura
	: ESCRIBIR PARENTESISIZ elementos_entrada PARENTESISDE
	| ESCRIBIRLN PARENTESISIZ elementos_entrada PARENTESISDE
		{
			imprimirSaltoLinea();
		}
	;
	
elementos_entrada 
	: valor_entrada elementos_entrada
	| valor_entrada
	;
	
valor_entrada
	: identificador_operacion
		{
			if ((!existeVariableGlobal(TS,$1.nombre)) && (!existeVariable(TS,$1.nombre))) 
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s no definida\n", valorFlag(0), $1.nombre);
				cerrarFicheroIncompleto(1);
				exit(-38);
			}
			else
			{
				generarSalida(TS,$1.nombre,$1.dimensiones);
			}
		}
	;

sentencia_lectura
	: LEER PARENTESISIZ identificador_operacion PARENTESISDE
		{
			if ((!existeVariableGlobal(TS,$3.nombre)) && (!existeVariable(TS,$3.nombre))) 
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Variable %s no definida\n", valorFlag(0), ($3.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-39);
			}
			else if (obtenerValoresVariable(TS,$3.nombre,2) == 0)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): La constante %s no puede ser escrito\n", valorFlag(0), ($3.nombre+3));
				cerrarFicheroIncompleto(1);
				exit(-40);
			}
			else
			{
				generarEntrada(TS,$3.nombre,$3.dimensiones);
			}
		}
		
	;

	
/****************************************************
  	SENTENCIA TIPO: LLAMADA
****************************************************/
nombre_funcion
	: ID
		{
			if (!existeFuncion(TS,$1.nombre))
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Funcion Inexistente\n", valorFlag(0));
				cerrarFicheroIncompleto(1);			
				exit(-41);
			}

			//iniciarRegistros();
			
			bzero(buffer,256);
			sprintf(buffer,"-%d",obtenerOffsetFuncion(TS,$1.nombre));
			generarInstruccionOperacion("$sp","$sp",buffer,5,5);
			
			bzero(funcionLlamada,256);
		   	strcat(funcionLlamada,$1.nombre);
			
			numArgumento = 1;



		}
	;

sentencia_llamada
	: nombre_funcion PARENTESISIZ lista_argumentos PARENTESISDE
		{

			$$ = obtenerTipo(buscarSimbolo(TS,$1.nombre));
			//realizamos una llamada
			
			if (numeroArgumentos < numeroParametros(TS,$1.nombre))
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Faltan argumentos\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-42);
			}
			
			solicitarFuncion($1.nombre);
			eliminarListaAuxiliar(&lEx);

			//iniciarRegistros();
		}
	;

lista_argumentos
	:  lista_argumentos elemento_argumentos
	| /*lambda*/
	;

elemento_argumentos
	: COMA elemento_argumento
		{
			numeroArgumentos++;

			if (numParametros > -1)
			{
				if (numeroArgumentos > numParametros)
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Numero excesivo de argumentos\n", valorFlag(0));
					cerrarFicheroIncompleto(1);
					exit(-43);
				}
				else
				{
					auxiliarTipo = tipoParametro(TS,$<general>-2.nombre,numeroArgumentos);
					if (((auxiliarTipo == -1) && ($2 > -1)) || ((auxiliarTipo > -1) && ($2 == -1)))
					{
						fprintf(stderr,"ERROR SEMANTICO (Linea %d): Tipos imcompatibles\n", valorFlag(0));
						cerrarFicheroIncompleto(1);
						exit(-44);
					}
					else if (auxiliarTipo < $2)
					{
						fprintf(stderr,"ERROR SEMANTICO (Linea %d): Perdida de precision en la asignacion 3\n", valorFlag(0));
						cerrarFicheroIncompleto(1);
						exit(-45);
					}
				}
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Funcion Inexistente\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-46);
			}
		}
	| elemento_argumento
		{
			numeroArgumentos = 1;
			numParametros = numeroParametros(TS,$<general>-2.nombre);
		
			if (numParametros > -1)
			{
				if (numeroArgumentos > numParametros)
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Numero excesivo de argumentos\n", valorFlag(0));
					cerrarFicheroIncompleto(1);
					exit(-47);
				}
				else
				{
					numeroArgumentos = 1;
					auxiliarTipo = tipoParametro(TS,$<general>-2.nombre,numeroArgumentos);
					if (((auxiliarTipo == -1) && ($1 > -1)) || ((auxiliarTipo > -1) && ($1 == -1)))
					{
						fprintf(stderr,"ERROR SEMANTICO (Linea %d): Tipos imcompatibles\n", valorFlag(0));
						cerrarFicheroIncompleto(1);
						exit(-48);
					}
					else if (auxiliarTipo < $1)
					{
						fprintf(stderr,"ERROR SEMANTICO (Linea %d): Perdida de precision en la asignacion 4\n", valorFlag(0));
						cerrarFicheroIncompleto(1);
						exit(-49);
					}
				}
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): Funcion Inexistente\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-50);
			}
		}
	;
	
elemento_argumento
	: numero
		{

			$$ = $1.tipo;
			
			bzero(buffer,256);
			
			if ($1.tipo > 2)
				sprintf(buffer,"%f",$1.real);
			else
				sprintf(buffer,"%d",$1.entero);

			/*if (valorFlag(1) == 1)
				auxiliarOffSet = obtenerOffsetFuncion(TS,funcionActual) + ObtenerOffSetVariable(TS,funcionActual,$1.nombre);obtenerOffsetArgumento(TS,funcionActual,numArgumento);
			else*/
			
			auxiliarOffSet = obtenerOffsetArgumento(TS,funcionActual,numArgumento);


			bzero(registro,5);
			obtenerRegistroOcupado($1.tipo,buffer,registro);
			if (strlen(registro) == 0)
			{
				obtenerRegistroLibre($1.tipo,buffer,registro,1,1);
				
				if (valorFlag(1) == 1)
					generarInstruccionCarga(registro,NULL,"$sp",5,auxiliarOffSet);
				else
					generarInstruccionCarga(registro,NULL,buffer,5,0);
			}

			generarInstruccionAlmacenamiento(registro,NULL,"$sp",$1.tipo,obtenerOffsetArgumento(TS,funcionLlamada,numArgumento));
			
			numArgumento++;
			
		}
	| identificador_operacion
		{
			
			if (!existeVariableGlobal(TS,$1.nombre))
			{
				$$ = obtenerValoresVariable(TS,$1.nombre,1);

				if (valorFlag(1) == 1)
					auxiliarOffSet = obtenerOffsetFuncion(TS,funcionActual) + ObtenerOffSetVariable(TS,funcionActual,$1.nombre);
				else
					auxiliarOffSet = obtenerOffsetArgumento(TS,funcionActual,numArgumento);


				bzero(registro,5);
				obtenerRegistroOcupado($1.tipo,$1.nombre,registro);
				if (strlen(registro) == 0)
				{
					obtenerRegistroLibre($1.tipo,$1.nombre,registro,1,1);
				
					if (valorFlag(1) == 1)
						generarInstruccionCarga(registro,NULL,"$sp",$1.tipo,auxiliarOffSet);
					else
						generarInstruccionCarga(registro,NULL,$1.nombre,$1.tipo,0);
				}
			
				generarInstruccionAlmacenamiento(registro,NULL,"$sp",$1.tipo,obtenerOffsetArgumento(TS,funcionLlamada,numArgumento));
			
				numArgumento++;
			}
			else
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): No se permiten variable globales como argumento de llamadas a funcion\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-51);
			}
			
		}
	;
	
/****************************************************
  	SENTENCIA TIPO: RETORNO
****************************************************/

sentencia_retorno
	: RETURN identificador_operacion PUNTOYCOMA 
		{
			auxiliarTipo = obtenerTipo(ultimo(TS));
			if (auxiliarTipo == -2)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): La función no es de tipo Void\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-52);
			}
			else
			{
				if (((auxiliarTipo == -1) && ($2.tipo > -1)) || ((auxiliarTipo > -1) && ($2.tipo == -1)))
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Tipos imcompatibles\n", valorFlag(0));
					cerrarFicheroIncompleto(1);
					exit(-53);
				}
				else if (auxiliarTipo < $2.tipo)
				{
					fprintf(stderr,"ERROR SEMANTICO (Linea %d): Perdida de precision en la asignacion 4\n", valorFlag(0));
					cerrarFicheroIncompleto(1);
					exit(-54);
				}
				
				//printf("Esto es una prueba %d %s %s\n", $2.tipo, $2.nombre, registro);
			
				if (strcmp(funcionActual,"main") != 0) //El proceso de recarga de $ra y de salto solo se realiza en funciones no en el MAIN
				{
					
					if (!existeVariableGlobal(TS,$2.nombre))
					{
						bzero(registro,5);
						obtenerRegistroOcupado($2.tipo,$2.nombre,registro);
						if (strlen(registro) == 0)
						{
							obtenerRegistroLibre($2.tipo,$2.nombre,registro,1,1);
							generarInstruccionCarga(registro,NULL,"$sp",$2.tipo,obtenerOffsetVariableActual(ultimo(TS),$2.nombre));
						}
			
						generarInstruccionAlmacenamiento(registro,NULL,"$sp",$2.tipo,0);
			
						bzero(registro,5);
						strcpy(registro,"$ra");
		
						generarInstruccionCarga(registro,NULL,"$sp",2,(obtenerOffsetFuncion(TS,funcionActual)-4));
			
						escribirFichero("jr $ra",1,0);
						if (valorFlag(2) == 1)						
							printf("%s\n","jr $ra");

						eliminarListaAuxiliar(&lEx);

						desactivarFlag(12);
					}
					else
					{
						fprintf(stderr,"ERROR SEMANTICO (Linea %d): No se pueden retornar variable globales en una funcion\n", valorFlag(0));
						cerrarFicheroIncompleto(1);
						exit(-55);
					}
				}
			}
		}
	| RETURN PUNTOYCOMA
		{
			if (obtenerTipo(ultimo(TS)) != -2)
			{
				fprintf(stderr,"ERROR SEMANTICO (Linea %d): La funcion debe retornar un valor\n", valorFlag(0));
				cerrarFicheroIncompleto(1);
				exit(-56);
			}
			
			if (strcmp(funcionActual,"main") != 0) //El proceso de recarga de $ra y de salto solo se realiza en funciones no en el MAIN
			{
				bzero(registro,5);
				strcpy(registro,"$ra");
		
				generarInstruccionCarga(registro,NULL,"$sp",2,(obtenerOffsetFuncion(TS,funcionActual)-4));
			
				escribirFichero("jr $ra",1,0);
				if (valorFlag(2) == 1)				
					printf("%s\n","jr $ra");

				desactivarFlag(12);
			}
		}
	;

%%

void EscribirAyuda()
{
     system("clear");
     printf("Se ha solicitado la ayuda del compilador R3000\n");
     printf("\n");
     printf("Sintaxis:\n");
     printf("\n");
     printf("     r3000 [-t -help] fichero_entrada.c [-r fichero_salida.s] \n");
     printf("\n");
     printf("Donde la opciones son: \n");
     printf("\n");
     printf("     -t      Muestra por pantalla la traza de disparo de el código\n");
     printf("             ensamblador.\n");
     printf("\n");
     printf("     -r      Indica al programa el nombre del fichero al que se \n");
     printf("             escribira el codigo ensamblador.\n");
     printf("\n");
     printf("     -help   Muestra la ayuda de la aplicacion.\n");
     printf("\n");
     printf("Gracias por utilizar el compilador R3000.\n");
}

int tipocadena(char *cadena)
{
   int salida = -1;

   if (strcmp(cadena, "-help") == 0)
      salida = 1;
   if (strcmp(cadena, "-t") == 0)
      salida = 2;
   if (strcmp(cadena, "-l") == 0)
      salida = 3;
   if (strstr(cadena, ".txt") != NULL)
      salida = 4;
   if (strstr(cadena,".c") != NULL)
      salida = 5;
   if (strcmp(cadena, "-r") == 0)
      salida = 6;
   if (strstr(cadena,".s") != NULL)
      salida = 7;
   
   return salida;
}


int main( int argc, char **argv )
{
    FILE *entrada;
    
    char salida[256];
	
    int estado = 0;
    int error  = 0;
    int i;
                          /*-h  -t  -l LOG  IN  -r OUT*/
    int Mestados[7][7] = {
                          {  6,  1, -1, -1,  4, -1, -1}, //0
			  { -1, -1,  2, -1,  4, -1, -1}, //1
			  { -1, -1, -1,  3, -1, -1, -1}, //2
			  { -1, -1, -1, -1,  4, -1, -1}, //3
			  { -1, -1, -1, -1, -1,  5, -1}, //4
			  { -1, -1, -1, -1, -1, -1,  0}	 //5
                         };
    bzero(salida,256);
    
    iniciarControl(0);
    
    if ((argc > 1) && (argc <= 7))
    {
    
        for (i = 1; i < argc; i++)
        {
        
            switch (tipocadena(argv[i]))
            {
					case 1: 
						estado = Mestados[estado][0];
						break;
					case 2: 
						activarFlag(2);
						estado = Mestados[estado][1];
						break;
					case 3: 
						activarFlag(2);
						estado = Mestados[estado][2];
						break;
					case 4:
						abrirFichero(argv[i],2);
						estado = Mestados[estado][3];
						break;
					case 5:
						entrada = fopen(argv[i],"r");
						memcpy(salida,argv[i],strlen(argv[i]));
						if (entrada == NULL)
						{
							error = 2;
							break;
						}
						estado = Mestados[estado][4];
						break;
					case 6:
						estado = Mestados[estado][5];
						break;
					case 7:
						abrirFichero(argv[i],1);
						estado = Mestados[estado][6];
						break;
					case -1:
						fprintf(stderr,"ERROR: Se han introducido parametros incorrectos.\n");
						exit(-1);
            }
        }   
    }
    else
        error = 1;
       
    if (estado == -1)
    {
       	fprintf(stderr,"ERROR: Se han introducido parametros incorrectos.\n");
			exit(-1);
    }
    else
    {
 
    	if (error == 0)
    	{
			if (estado == 6)
				EscribirAyuda();        
			else
			{
				if (estado == 4)
				{
					salida[strlen(salida) - 1] = 's';
					abrirFichero(salida,1);

				}

				//system("clear");
			
				insertarSimbolo(&TS,"global",0);
				generarCabeceraPrograma();
				iniciarRegistros();				
				yyrestart(entrada);						

				//yydebug = 1;
				if ( yyparse() == 0 )
				{
					imprimirFinalPrograma();
					generarDefiniciones();	
					generarArraysPrograma(TS);						//ARRAYS
					generarVariablesPrograma(TS,0); 				//CONSTANTES
					imprimirCadenasExpeciales();
					generarVariablesPrograma(TS,2);				//VARIABLES GLOBALES
					generarVariablesPrograma(ultimo(TS),1); 	//VARIABLES LOCALES DEL MAIN
					printf("El proceso termino correctamente.\n");
				}
				
				eliminarLista(&TS);
				
				cerrarFichero(1);
				
				/*if (valorFlag(4) == 1)
					cerrarFichero(2);	*/
			}
		}
    	else
    	{
    		fprintf(stderr,"ERROR: Se ha producido un error con los parametros de entrada.\n");
			exit(-2);
    	}
    }
}
