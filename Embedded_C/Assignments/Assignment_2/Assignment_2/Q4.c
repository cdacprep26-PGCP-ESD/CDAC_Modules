// Calculate GCD and LCM of two numbers
#include <stdio.h>

int main(){
	int a, b;
	int lcm , gcd;

	printf("Enter the first number :\n");
	scanf("%d", &a);

	printf("Enter the second number :\n");
        scanf("%d", &b);

	//for finding GCD
	for(int i=1; i<=a && i<=b; i++){
		if(a%i == 0 && b%i == 0){
			gcd = i;
		}//if
	}

	//for LCM
	lcm = (a*b)/gcd;

	printf("LCM of numbers is : %d\n", lcm);
	printf("GCD of numbers is : %d\n", gcd);

	return 0;
}//main
