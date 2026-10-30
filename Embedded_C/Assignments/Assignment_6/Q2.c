//Q2 Conversion of short integer from little endian to big endian
#include<stdio.h>

int main()
{
	short int a;
	printf("Enter Integer : ");//Enter short integer
	scanf("%hx",&a);
	printf("\n");
	
	printf("Converting to Big Endian\n");	
	short int Big_E;
	Big_E = (a<<8) | (a>>8);//Converting to Big Endian
	printf("After Conversion to Big Endian 0x%04X\n",Big_E);
	
	printf("Converting to Little Endinan \n");
	short int lit_E;
	lit_E = (Big_E << 8 ) | (Big_E >> 8);//Converting to Little Endian
	printf("After Conversion to Little Endian 0x%04X\n",lit_E);
	
	return 0;
}//main
