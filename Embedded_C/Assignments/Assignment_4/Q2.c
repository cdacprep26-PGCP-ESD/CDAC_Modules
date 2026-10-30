//Swapping two numbers
#include<stdio.h>

int main()
{
	int a,b;
	printf("Enter the two numbers:");
	scanf("%d %d", &a, &b);
        //without using third variable
	a=a+b;
	b=a-b;
	a=a-b;

	printf("The swappeded numbers without using third variable are a=%d and b=%d :\n", a,b);
	//using third variable
	int temp;
	
	temp = a;
	a = b;
	b = temp;

	printf("The swappeded numbers using third variable are a=%d and b=%d :\n", a,b);
	//using XOR in one line
	int d;
	a != b ? ( a = a^b, b = a^b, a = a^b ) : printf("Numbers are same ");
	printf(" Swapped Variables using one line XOR operation a = %d and b = %d\n", a,b);
	return 0;
}
