//Q1 Checking the endianess of current system
#include<stdio.h>

int main()
{
	int x=0x00000001;
	
	char *ptr=(char *)&x;
	
	if(*ptr==01)
	{
		printf("Little Endian\n");
	}
	else
	{
		printf("Big Endian\n");
	}
	
	return 0;
}//main
