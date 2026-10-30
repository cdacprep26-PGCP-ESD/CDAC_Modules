//Finding Biggest using Ternary Operator
#include<stdio.h>

int main()
{
	int a, b, c, d;
	
	printf("Enter 1st number \n");
	scanf("%d",&a);
	printf("Enter 2nd number \n");
	scanf("%d",&b);
        printf("Enter 3rd number \n");
	scanf("%d",&c);
	
	d = a > b ? ( a > c ? a:c ) : ( b > c ? b : c ); // Ternary Operator

	printf("The Biggest of three numbers is %d\n", d);

	return 0;
}// nain
