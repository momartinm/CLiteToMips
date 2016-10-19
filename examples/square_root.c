#define resultado "La raiz de "
#define es " es "
#define intro "Introduzca el numero a calcular su raiz"

float sqrt (float m)
{
	float i,j;
	float x1,x2;
	int c;

	i = 0.0;
	j = 0.0;

	while (j <= m)
	{
		j = i * i;
		i = i + 0.1;
	}

	x1 = i;

	for (c = 0; c < 10; c+1)
	{
		x2 = m;
		x2 = x2 / x1;
		x2 = x2 + x1;
		x2 = x2 / 2;
		x1 = x2;
	}

	return x2;
}

int main()
{
	float num, res;

	println (intro);

	scanf (num);

	res = sqrt (num); 	
	
	println (resultado num es res);
}
