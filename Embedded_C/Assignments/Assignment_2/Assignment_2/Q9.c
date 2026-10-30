//C program to have recursive sum of digits in a number

#include <stdio.h>

//function to find sum of digits
int SumOfDigits(int n){
	if(n==0)
		return 0;

	return(n%10) + SumOfDigits(n/10);
	
}

//function to find repeatedly find sum until single digit
int recursiveSum(int n){
	if(n<10)
		return n;

	return recursiveSum(SumOfDigits(n));
	
}

int main(){
	int n, result;

	printf("Enter the number:");
	scanf("%d", &n);

	result = recursiveSum(n);//stored the return value in result

	printf("final single digit = %d", result);

	return 0;
}

