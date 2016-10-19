#define numero "Introduzca numero para la posicion "
#define resultado "La media del array es: "

float array [5];

float media ()
{
	int i;
	float res, aux;
	
	aux = 0.0;
	
	for (i = 0; i < 5; i+1)
		aux = aux + array [i];
	
	res = aux / 5.0;
	
	return res;	
}

void rellenar ()
{
	int i;
	float aux;
	
	for (i = 0; i < 5; i+1)
	{
		println (numero i);
		scanf (aux);
		array [i] = aux;
	}
}

int main ()
{
	float ret;
	
	rellenar ();
	
	ret = media ();
	
	println (resultado ret);
}
