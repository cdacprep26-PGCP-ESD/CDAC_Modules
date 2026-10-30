//Q7.Recursive Number Conversion
#include<stdio.h>

//Function Declarations
void decimalToBin(int);
int binToDecimal(int,int,int); 
void decimalToOct(int);
int OctToDecimal(int,int,int);

int main()
{
	int n,n2,n3;
	int ch;

	printf("1. Decimal to Binary\n2. Binary to Decimal\n3.Decimal to Octal\n4. Octal to Decimal\n");
	printf("Enter Your choice \n");
	
	scanf("%d",&ch);
	switch(ch) //Switch statement for selecting conversion choice selection
	{
		case 1: 
			printf("Enter Decimal number\n");
			scanf("%d",&n); 
			decimalToBin(n);//call Decimal to Binary conversion funcction
			printf("\n");
			break;
		case 2:
			printf("Enter binary \n");
			scanf("%d",&n2);
			printf("%d",binToDecimal(n2, 1, n2%10));//call Binary to Decimal function and printing decimal
			printf("\n");
			break;
		case 3:
			printf("Enter Decimal number\n");
			scanf("%d",&n);
        		decimalToOct(n);//call Decimal to Octal conversion funcction
        		printf("\n");
			break;
        	case 4:
        		printf("Enter Octal \n");
        		scanf("%d",&n3);
        		printf("%d",OctToDecimal(n3, 1, n3%10));//call Octal to Decimal function and printing decimal
        		printf("\n");
        		break;
        	default:
        			printf("Invalid Input !!\n");
        }//switch
	return 0;
}//main

void decimalToBin(int n)//Recursive Decimal to Binary Function Definition
{
	if(n ==0)
	{
		return;
	}

	decimalToBin(n/2);
	printf("%d",n%2);
}//decimalToBin

int binToDecimal(int n, int base, int digit)//Recursive Binary to Decimal Function Definition
{
	if(n == 0)
	{
		return 0 ;
	}
        return binToDecimal(n/10, base*2, (n/10)%10) + digit*base;
}//binToDecimal

void decimalToOct(int n)//Recursive Decimal to Octal Function Definition
{
        if(n ==0)
        {
                return;
        }

        decimalToOct(n/8);
        printf("%d",n%8);
}//decimalToOct

int OctToDecimal(int n, int base, int digit)//Recursive Octal to Decimal Function Definition
{
        if(n == 0)
        {
                return 0 ;
        }
        return OctToDecimal(n/10, base*8, (n/10)%10) + digit*base;
}//OctToDecimal
