//To check the given number is perfect or not

#include <stdio.h>

int main(){

	int num, sum = 0;

	printf("Enter the number :");
	scanf("%d", &num);

	for(int i=1; i<=num; i++){

		if(num%i==0){
			sum = sum+i;
		}//if

	}//for

	if(sum/2 == num){
		printf("THE NUMBER IS PERFECT NUMBER");
	}

	else{
		printf("THE NUMBER IS NOT PERFECT NUMBER");
	}

	return 0;

}//main
