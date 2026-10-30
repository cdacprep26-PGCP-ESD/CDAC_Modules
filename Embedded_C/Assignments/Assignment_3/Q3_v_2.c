//Q3.Recursively taking sum of N Numbers
#include<stdio.h>

int sum(int n)
{
	if(n==1)
	{
		return 1;
	}
	else
	{
		return n+sum(n-1);
	}
}//sum

int main()
{
	int n=5, a; //Variable declaration

	a=sum(n); //calling recursive sum function

	printf("Sum is:%d",a);

	return 0;
}//main
