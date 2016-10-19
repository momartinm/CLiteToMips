#define mov "Movemos disco de torre "
#define to " a torre "
#define intro "Introduzca el numero de discos"

void hanoi (int n, char com, char aux, char fin)
{
	int m;
	m = n - 1;

	if (n == 1)
		println (mov com to fin);
	else 
	{
		hanoi (m, com, fin, aux);
		println (mov com to fin);
		hanoi (m, aux, com, fin);   
	}
}

int main() 
{
	char comm, auxm, finm;
	int n;

	comm = 65;
	auxm = 66;
	finm = 67;

	println (intro);

	scanf (n);

	hanoi (n, comm, auxm, finm);
}
