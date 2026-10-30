//Q9
#include<stdio.h>
union A
{
	int x; 
	float y; 
	double z;
	int arr[2]; 
}a1; 

union B
{
	int x; 
	short int y; 
	char ch;
	char carr[4];
}b1;

int main()
{
	//a1.y=6; 
	//printf("y=%x\n",a1.y); 
	a1.z=0x1234567812345678;
	printf("a1.arr[1] : %x\na1.arr[0] : %x\n",a1.arr[1],a1.arr[0]);

	b1.x=0x41424344;
	printf("b1.y : %x\n", b1.y);
	printf("b1.ch : %x\n", b1.ch);
	for(int i = 0; i<4; i++)
	{
		printf("carr[%d] : %x\n", i, b1.carr[i]);
	}
	
	return 0;
}
	
	
	














