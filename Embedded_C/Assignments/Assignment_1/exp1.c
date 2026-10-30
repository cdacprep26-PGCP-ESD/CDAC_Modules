// Swapping without 3rd Variable
#include<stdio.h>

int main()
{
	int a,b;
	a=2;
	b=3;
	printf("A and B before swapping A=%d B=%d\n", a,b);
	a=a^b;
	b=a^b;
	a=a^b;
	printf("A and B after swapping A=%d B=%d", a,b);
	return 0;
}//main
