//Q6.Fixing Function definition
#include<stdio.h>

int *test( int x)
{
	static int y; //Fix: declare y as a static variable to solve problem of dangling pointer 
	y = x*x;
	return &y;
}

int main()
{
	int x =10;
	printf("%d\n", *test(10)); //dereferencing address return by function call
	
	return 0;
}//main
