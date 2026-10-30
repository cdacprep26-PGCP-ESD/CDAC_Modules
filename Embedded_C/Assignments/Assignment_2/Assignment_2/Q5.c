//To check the given number is prime or not
#include <stdio.h>

int main(){
	int num;
	int is_prime;
	printf("Enter the number :");
	scanf("%d", &num);

	for(int i=2; i<=num; i++){
		is_prime=1;
		
		if(num%i==0)
		{
			is_prime=0;
			break;
		}//if
		else{
			is_prime = 1;
			break;
		}//else
	
	}//for

	if(is_prime==1)
	{
		printf("Num is Prime");
	}//if
	else
	{
		printf("Not a prime");
	}//else

	return 0;

}//main





