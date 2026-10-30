//Q6 Test arithmatic operation on void pointer

#include <stdio.h>
int main(){
	int arr[3] = {10, 20, 30};
	
    	void *vp = arr;
	
    	printf("Address before increment : %p\n", vp);
    	vp++; // Arithmetic operation on void pointer
    	printf("Address after increment : %p\n", vp);

	vp = arr;

	printf("\nAddress before increment : %p\n", vp);
        ++vp; // Arithmetic operation on void pointer
        printf("Address after increment : %p\n", vp);

	vp = arr;

        printf("\nAddress before decrement : %p\n", vp);
        vp--; // Arithmetic operation on void pointer
        printf("Address after decrement : %p\n", vp);

	vp = arr;

        printf("\nAddress before decrement : %p\n", vp);
        --vp; // Arithmetic operation on void pointer
        printf("Address after decrement : %p\n", vp);

	
    	return 0;

}//main
