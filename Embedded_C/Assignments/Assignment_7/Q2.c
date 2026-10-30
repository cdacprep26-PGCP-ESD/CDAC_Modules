//Q2.Try to global and decalre variable using register storage class
#include<stdio.h>

//register int b =20; // Can not declare global variable with register storage class
int main()
{ 
	register int a = 10;
	// program won't compile, address of register storage class varibale can't be accessed using &
	//printf("Address of Local register variable %d\n", &a);
	// printf("Address of gloabl register variable %d\n", &b);
	
	return 0;
}//main
