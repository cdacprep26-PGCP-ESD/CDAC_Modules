//Q4.Factorial of number using Recursion
#include<stdio.h>

int fact(int n) //Function defination
{
	if(n==1)
	{
		return 1;
	}
	else
	{
		return n*fact(n-1); //Recursive Fuction
	}
}

int main()
{
	int n=5, factorial;

	factorial=fact(n); //Function calling

	printf("The factorial of number:%d\n", factorial);

	return 0;
}//main
