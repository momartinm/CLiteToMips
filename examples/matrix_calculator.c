#define msg "Inserte la opcion de calculo "
#define op1 "1 - Determinante"
#define op2 "2 - Suma de matrices"
#define op3 "3 - Matriz por escalar"
#define error "opcion seleccionada no valida"
#define matrix "Inserte los elementos de la matriz 2x2"
#define resultado "El resultado es: "
#define esc "Introduzca el escalar k"
#define vacio ""
#define espacio " "
#define deter "El determinante es: "
#define matrixReult "La matriz resultante es: "

int matriz [2][2];
int result [2][2];

int determinante ()
{
	int res;

	res = (matriz [0][0] * matriz [1][1]) - (matriz [0][1] * matriz [1][0]);
	return res;
}

void suma ()
{
	int i, j;
	for (i = 0; i < 2; i+1)
	{
		for (j = 0; j < 2; j+1)
			result [i][j] = result [i][j] + matriz [i][j];
	}
}

void mostrar ()
{
	int i;
	int k;

	for (i= 0; i< 2; i+1)
	{
		for (k=0; k <2; k+1)
			print (result [i][k] espacio);
		println (vacio);
	}
}

void escalar (int k)
{
	int i, j, aux;
	
	for (i = 0; i < 2; i+1)
	{
		for (j = 0; j < 2; j+1)
		{
			result [i][j] = matriz [i][j] * k;
		}
	}
}

void leerMatriz (int sel)
{
	int i, j, aux;
	
	println (matrix);
	
	for (i = 0; i < 2; i+1)
	{
		for (j = 0; j < 2; j+1)
		{
			scanf (aux);
			if (sel == 0)
			{
				matriz [i][j] = aux;
			}
			else
			{
				result [i][j] = aux;
			}
		}
	}
}

int main()
{
	int ret, escala, opcion, tipo;
	
	println (msg);
	println (op1);
	println (op2);
	println (op3);
	
	scanf (opcion);
	
	if (opcion == 1)
	{
		tipo = 0;
		leerMatriz (tipo);
		ret = determinante ();
		println (deter ret);
	}
	
	if (opcion == 2)
	{
		tipo = 0;
		leerMatriz (tipo);
		tipo = 1;
		leerMatriz (tipo);
		suma ();
		println (matrixReult);
		mostrar ();		
	}
	
	if (opcion == 3)
	{
		tipo = 0;
		leerMatriz (tipo);
		println (esc);
		scanf (escala);
		escalar (escala);
		println (matrixReult);
		mostrar ();	
	}
	
	if ((opcion < 1) & (opcion > 2))
		println (error);
}
