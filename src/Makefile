MAKE			= make -s
CC				= gcc
FLEX			= flex
BISON			= bison
RM				= rm
FL				= -l
FB				= -d -v
SOURCE_PATH	= ./
OBJECTS		= control.c sintactico.tab.c lex.yy.c tSimbolos.c lVariables.c lDimensiones.c arbol.c pilaArbol.c lAuxiliar.c pila.c transformador.c pilaEtiquetas.c intermedio.c fichero.c
EXE			= r3000

all:
	@echo "                              Compilando "
	@echo -n "                              "
	@echo -n "Compilador de C para MIPS R3000 [.."
	@$(BISON) $(FB) sintactico.y
	@$(FLEX) $(FL) lexico.l
	@$(CC) $(OBJECTS) -o $(EXE)
	@echo "..]"
	@echo "                              R3000 Compilado con Exito!"
clean:
	@echo "                              Eliminando ficheros!"
	@$(RM) -f *.o $(EXE) lex.yy.c sintactico.tab.c sintactico.tab.h sintactico.output *~
	
