#define si "El anno es bisiesto"
#define no "El anno no es bisiesto"
#define intro "Introduzca el anno a comprobar"

/*
Esta función calcula si un anno es o no bisiesto por medio de comprobaciones
matematicas. Finalmente imprime un mensaje con el resultado.
*/

void bisiesto (int year)
{
	int aux, aux1, aux2;
	aux = year % 4;
	aux1 = year % 100;
	aux2 = year % 400;
	
	if (((aux == 0) & (aux1 != 0)) | (aux2 == 0)) 
		println (si);
	else
		println (no);
}

int main ()
{
	int y;
	
	println (intro);
	
	scanf (y);
	
	bisiesto (y);
}
