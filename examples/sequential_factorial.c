#define resultado "El factorial de "
#define es " es: "
#define over "El factorial de numeros mayores a 13 desborda 32 bits"
#define intro "Introduzca el numero a calcular su factorial"

int factorial (int n) 
{
	int i, aux;   
	aux = 1;
	i = n;

	while (i > 0)
	{
		aux = aux * i;
		i = i - 1;
	}

	return aux;
}

int main ()
{
	int arg1, res;

	println (intro);

	scanf (arg1);

	if (arg1 < 14) 
	{
		res = factorial (arg1);
		println (resultado arg1 es res);
	} 
	else
		println (over);    
}
