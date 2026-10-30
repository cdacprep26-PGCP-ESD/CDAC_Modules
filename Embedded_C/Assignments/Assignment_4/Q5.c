// HMS to Seconds and Vice Vaers time Conversion
#include<stdio.h>

int hmsToSec(int,int,int); // function declaration
int SecToHms(int);

int main()
{
	int h, m, s;
	int sec; // variable declaration
	int n;
	while (1)
	{
		printf("Enter 1 to Convert H:M:S to Seconds \n Enter 2 to Convert Seconds to H:M:S \n Enter 3 to exit \n ");
		scanf("%d",&n);

		switch(n) 
		{
			case 1:
				printf("Enter time in H:M:S \n");
				scanf("%d %d %d", &h,&m,&s);
				printf("After Conversion: %d\n: ",hmsToSec(h,m,s)); // function call
				break;
			case 2 :
				printf("Enter time in Seconds \n");
				scanf("%d",&sec);
				SecToHms(sec); // function call
				break;
			case 3 : 
				return 0;  //exit
			default : 
				printf("Invalid Input !! \n");

		} // switch
	}
	return 0;
} // main

int hmsToSec( int h, int m, int s) // HMS to Seconds Conversion
{
	return h*3600 + m*60 + s;
}

int SecToHms( int sec ) // Seconds to HMS conversion
{
	int h, m, s;
	
	h = sec / 3600;
	sec = sec % 3600;

	m = sec / 60 ; 
	sec = sec % 60 ;
	s = sec ;

	printf("After Conversion %d: %d: %d\n " , h,m,s);
	
	return 0;
}

	
