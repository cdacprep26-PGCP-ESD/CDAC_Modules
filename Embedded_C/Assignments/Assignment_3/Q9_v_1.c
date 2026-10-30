//Q9.Iterative Calculation of Power of n 
#include<stdio.h>

int main()
{
	int a, n, i, power=1;//Variable Declaration

	printf("Enter base:");//Number Input
	scanf("%d", &a);

	printf("Enter power:");//Power Input
	scanf("%d",&n);

	for(i=1; i<=n; i++)//Iterative Power Calculation
	{
		power=power*a;
	}
	
	printf("The power is %d\n", power);
	return 0;
}//main
