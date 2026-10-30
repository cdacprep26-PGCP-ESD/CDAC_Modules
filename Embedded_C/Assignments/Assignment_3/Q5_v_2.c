//Q5.Recursive Calculation of GCD
#include<stdio.h>

int gcd(int a,int b) //Recursive GCD Function definiton 
{
	if(b==0)
	{
		return a;
	}
	else
	{
		return gcd(b, a%b);

	}
}//gcd

int main()
{
	int a, b, GCD; // Variable Declaration

	printf("Enter numbers:\n");//Input two Numbers for GCD calculation
	scanf("%d %d", &a,&b);

	GCD=gcd(a,b);//Call of Recursive GCD function
	
	printf("The GCD of numbers : %d\n", GCD);

	return 0;
}//main
