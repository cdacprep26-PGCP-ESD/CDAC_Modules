//Check given number is amstrong or not

#include <stdio.h>
int main(){
	int num, temp, power;
	int sum = 0;
	int count = 0;
	int digit;

	printf("Enter the number :");
	scanf("%d", &num);
	
	temp = num;
	
	while(temp != 0){
		temp = temp/10;
		count++;
	}//while

	temp = num;

	while(temp != 0){

		digit = temp%10;
		power = 1;
		
		for(int i=1; i<=count; i++){
			power = power * digit;
		}//for

		sum = sum + power;
		temp/=10;
	
	}//while


	if(sum==num){
		printf("NUMBER IS AMSTRONG");
	}//if

	else{
		printf("NUMBER IS NOT AMSTRONG");
	}//else

	return 0;

}//main
