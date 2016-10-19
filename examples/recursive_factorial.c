#define resultado "El factorial de "
#define es " es: "
#define over "El factorial de numeros mayores a 13 desborda 32 bits"
#define intro "Introduzca el numero a calcular su factorial"

int factorial (int num) 
{ 
	int aux, aux1, aux2;

	if (num == 0)
	{ 
		aux = 1;
		return aux;
	} 
	else 
	{
		aux1 = num - 1;
		aux2 = factorial (aux1);
		aux = aux2 * num;
		return aux; 
	} 
}

int main()
{
	int num, ret;
	
	println (intro);

	scanf (num);

	if (num < 14)
	{
		ret = factorial (num);
		println (resultado num es ret);
	}
	else
		println (over);
}
