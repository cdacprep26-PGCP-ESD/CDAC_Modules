//Q8
#include<stdio.h>
#include<stddef.h>

union A
{
	int x; 
	int y; 
	char ch;
}; 

int main()
{
	union A a1;
	a1.x=0x05; 	
	a1.y=0x1121;  
	
	printf("Value of a1.x : %x\n",a1.x);
	printf("Value of a1.y : %x\n",a1.y);
	printf("Size of union %ld\n", sizeof(union A));//Prints the size of union (Gives the size of largest member) 
	
	//Prints the offset of each member
	//All members of Union has same address
	printf("Offset of x : %zu\n", offsetof(union A, x));
	printf("Offset of y : %zu\n", offsetof(union A, y));
	printf("Offset of ch : %zu\n", offsetof(union A, ch));
	 
	
	
	return 0;
}
	
	
	
