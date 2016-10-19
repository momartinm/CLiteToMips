%{
#include <stdio.h>
#include "sintactico.tab.h"
#include "control.h"
%}

%x comentario
%x comentarioLinea

digito					[0-9]
letra					[a-zA-Z]
alpha					[a-zA-Z0-9_]

%%

-?{digito}+				{yylval.entero = atol(yytext); return(ENTERO);}   
-?{digito}+"."{digito}+			{yylval.real   = atof(yytext); return(REAL);}
					
"float"					{yylval.entero =  3; return(FLOAT);}
"int"					{yylval.entero =  2; return(INT);}
"short"					{yylval.entero =  1; return(SHORT);}
"char"					{yylval.entero =  0; return(CHAR);}
"void"					{yylval.entero = -2; return(VOID);}
"if"					{activarFlag(8);activarFlag(11); return(IF);}
"else"					return(ELSE);
"for"					{activarFlag(10);activarFlag(11); return(FOR);}
"while"					{activarFlag(9);activarFlag(11); return(WHILE);}
"main"					return(MAIN);
"return"				return(RETURN);
"#define"				return(DEFINE);
"print"					return(ESCRIBIR);
"println"				return(ESCRIBIRLN);
"scanf"					return(LEER);
"+"					return(MAS);
"++"					return(MASUNARIO);
"--"					return(MENOSUNARIO);
"-"					return(MENOS);
"*"					return(MULTIPLICACION);
"/"					return(DIVISION);
"%"					return(MODULO);
"+="					return(MASIGUAL);
"-="					return(MENOSIGUAL);
"*="					return(MULIGUAL);
"/="					return(DIVIGUAL);
"=="					return(COMPARACION);
"<="					return(MEI);
">="					return(MAI);
"!="					return(DISTINTO);
"<"					return(MENOR);
">"					return(MAYOR);
"="					return(ASIGNACION);
"&"					return(AND);
";"					return(PUNTOYCOMA);
"|"					return(OR);
","					return(COMA);
"("					return(PARENTESISIZ);
")"					return(PARENTESISDE);
"["					return(CORCHETEIZ);
"]"					return(CORCHETEDE);
":"					return(DOSPUNTOS);
"{"					return(INICIO);
"}"					return(FIN);
"\""					return(COMILLAS);
"'"					return(COMILLASIMPLE);

{letra}?\'(\\.|[^\\'])+\'		{strcpy(yylval.cadena,yytext);return(CARACTER);}
{letra}?\"(\\.|[^\\"])*\"		{strcpy(yylval.cadena,yytext);return(TEXTO);}
{letra}{alpha}*				{
						sprintf(yylval.general.nombre,"ID_%s",yytext);
						return(ID);
					}

[ \t\v\f]
[ \n]					{incrementarLinea();}

"//"					BEGIN(comentarioLinea);
<comentarioLinea>[^\n]*			/* se come cualquier cosa que no sea un '\n' */
<comentarioLinea>\n			{incrementarLinea();BEGIN(INITIAL);}

"/*"					BEGIN(comentario);
<comentario>[^*\n]*			/* se come cualquier cosa que no sea un '*' */
<comentario>"*"+[^*/\n]*		/* se come '*'s que no continuen con '/' */
<comentario>\n				{incrementarLinea();}
<comentario>"*"+"/"			BEGIN(INITIAL);

.

%%

int yywrap (void) 
{
    return 1;
}

int yyerror(char *s) 
{
	printf("Se ha producido un error sintactivo en la linea %d en el token %s\n", valorFlag(0), yytext);
	return -1;
}
