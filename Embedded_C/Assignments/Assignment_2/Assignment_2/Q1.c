// Calculator
#include <stdio.h>
int main(){
	int a, b, add, subtract, divide, multiply, choice;  // Declared Variables
	printf("Enter two numbers : ");  // Taking Input
	scanf("%d %d",&a, &b); 
	printf("Enter Your Choice (1.addition 2.Subtration 3.Multiplication 4.Division) : ");  // Choice For Operation
	scanf("%d",&choice);

        // Used Switch Case 
	switch (choice){
		case 1 : //Addition
			add = a + b;
			printf("ADDITION is : %d\n",add);
			break;

		case 2 :  //Subtraction
			subtract = a - b;
                        printf("SUBSTRACTION is : %d\n",subtract);
                        break;

		case 3 ://Multiplication
			multiply = a * b;
                        printf("MULTIPLICAION is : %d\n",multiply);
                        break;

		case 4 ://Divide
			divide = a / b;
                        printf("DIVISION is : %d\n",divide);
                        break;
		default:
			printf("INVALID NUMBERS");
	return 0;
	}//switch
}//main
