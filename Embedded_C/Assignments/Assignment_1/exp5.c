// Finding leap year
#include<stdio.h>

int main()
{
	int Y;
	printf("Enter year you want to check :\n");
        scanf("%d",&Y);
        if((Y%400==0) || (Y%4==0))
	{
		printf("Leap Year!!!\n");
	}
	else
	{
		printf("Not Leap Year\n");
	}

	return 0;
}//main

