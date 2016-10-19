#define msg "Inserte la opcion de calculo "
#define op1 "1 - suma"
#define op2 "2 - multiplicacion"
#define op3 "3 - division"
#define op4 "4 - modulo (solo enteros)"
#define error "opcion seleccionada no valida"
#define argf "Inserte los flotantes"
#define argi "Inserte los enteros"
#define resultado "El resultado es: "

float suma (float s1, float s2) //Esta funcion suma 2 flotantes
{
	float res;
	res = s1 + s2;
	return res;
}

float multiplicacion (float s1, float s2) //Esta función multiplica 2 flotantes
{
	float res;
	res = s1 * s2;
	return res;
}

float division (float s1, float s2) //Esta funcion divides el primer argumento entre el segundo
{
	float res;
	res = s1 / s2;
	return res;
}

int modulo (int s1, int s2) //Esta funcion calcula la operacion argumento_1 modulo argumento_2
{
	int res;
	res = s1 % s2;
	return res;
}

int main() //Comienza el main de la aplicacion
{
	int retInt, opcion;
	float p1, p2, retFloat;
	int m1, m2;
	
	println (msg);
	println (op1);
	println (op2);
	println (op3);
	println (op4);
	
	scanf (opcion);
	
	if ((opcion == 1) | (opcion == 2) | (opcion == 3)) 
	{
		println (argf);
		scanf (p1);
		scanf (p2);
	}
	
	if (opcion == 1)
	{
		retFloat = suma (p1, p2);
		println (resultado retFloat);		
	}
	
	if (opcion == 2)
	{
		retFloat = multiplicacion (p1, p2);
		println (resultado retFloat);		
	}
	
	if (opcion == 3)
	{
		retFloat = division (p1, p2);
		println (resultado retFloat);			
	}
	
	if (opcion == 4)
	{
		println (argi);
		scanf (m1);
		scanf (m2);
		retInt = modulo (m1, m2);
		println (resultado retInt);			
	}
 	
}
