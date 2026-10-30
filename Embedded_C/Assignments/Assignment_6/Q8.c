//Q8 Checking pointer is Null using assert Macro
#include<stdio.h>
#include<assert.h>

int main()
{
	int x=10;
	int *ptr = NULL;
	//Check if pointer is Null or not using assert
	assert(ptr != NULL);
	
	//Dereferncing Pointer 
	printf("Dereferncing Pointer %d\n",*ptr);

	return 0;
}
