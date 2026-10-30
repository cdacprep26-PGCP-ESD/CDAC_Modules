//Q3.called functions putchar(toupper(ch)) & sqrt(pow(2,abs(x))) from different libraries
#include<stdio.h>
#include<math.h> // math library
#include<stdlib.h> 
#include<ctype.h>

int main()
{
	int x;
	char ch;
	
	
	printf("Enter any Alphabate betwen a-z : ");
	scanf("%c",&ch);
	printf("\n");
	
	printf("Upper Case is : ");
	putchar(toupper(ch));
	printf("\n");
	
	
	printf("Enter Number : ");
	scanf("%d",&x);
	printf("\n");
	
	printf("%f", sqrt(pow(2,abs(x))) );
	printf("\n");
	
	return 0;
}
