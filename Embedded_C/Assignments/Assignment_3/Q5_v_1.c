//Q5.Iterative Calculation of GCD 
#include<stdio.h>

int main()
{
	int a, b, rem; //Variable Declaration

	printf("Enter two values:\n");
	scanf("%d %d", &a,&b);

	while(b!=0) //Iterative Calculation of GCD
	{
		rem=a%b;
		a=b;
		b=rem;
	}

	printf("GCD : %d\n", a);

	return 0;
}//main
