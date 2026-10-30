//Q7.Iterative Number conversion
#include<stdio.h>

void decimalToBinary( int n ) // Decimal to Binary Conversion
{
        int i = 0;
	int binary[32];
        while( n > 0)
        {
                binary[i]= n % 2;
                n = n/2;
                i++;
        }

        printf("Binary=");

        for(i= i-1; i>= 0; i--)
        {
                printf("%d\n", binary[i]);
        }
}

int  binaryToDecimal(int n) // Binary to Decimal Conversion
{ 
	int base = 1;
	int decimal = 0;
	int digit;

	while( n!= 0)
	{ 
		digit = n%10;
		decimal = decimal + digit * base;
		base = base * 2;
		n = n/10;
	} 
	return decimal;
}


void decimalToOctal( int n ) // Decimal to Ocatal Conversion
{ 
	int i = 0; 
	int octal[32];

	while( n > 0)
	{
		octal[i]= n % 8;
		n = n/8;
		i++;
	}

	printf("Octal=");

	for(i=i-1; i>=0; i--)
	{
		printf("%d\n", octal[i]);
	}
}

int  OctalToDecimal(int n) // Octal to Decimal Conversion
{ 
	int base = 1;
	int decimal = 0;
	int digit;

	while( n!= 0)
	{ 
		digit = n%10;
		decimal = decimal + digit * base;
		base = base * 8;
		n = n/10;
	} 
	return decimal;
}


int main()
{
      int choice;
      int number;
      int result;

      printf("Number Conversion\n");

      printf("1. Decimal to Binary\n");
      printf("2. Binary  to Decimal \n");
      printf("3. Decimal to Octal\n");
      printf("4. Octal to Decimal\n");
      
      scanf("%d",&choice);

      switch(choice) // Taking Choice for Conversion
      {
	case 1 :
	      printf("Enter decimal:\n");
	      scanf("%d",&number);
	      decimalToBinary(number);
	      break;
	case 2 :
	      printf("Enter Binary:\n");
	      scanf("%d",&number);
	      printf("Decimal %d", binaryToDecimal(number));
	      break;
	case 3:
              printf("Enter Decimal \n:");
              scanf("%d",&number);
              decimalToOctal(number);
              break;
	case 4:
	      printf("Enter Octal\n");
	      scanf("%d", &number);
	      printf("Decimal is %d",OctalToDecimal(number));
	      break;

	default  :
	       printf("Invalid Input\n");
		      break;
      }// switch
      printf("\n");

      return 0;
}// main
