#include <stdio.h>

// Function declarations
int* arr_add(int*);
int (*arr_ret(int [3]))[3];  // returns pointer to array of 3 ints

int main() {
    int arr[3] = {10, 20, 30};

    // Base address (pointer to first element)
    printf("Base address: %p\n", arr_add(arr));

    // Whole array address (pointer to entire block of 12 bytes)
    int (*whole)[3] = arr_ret(arr);
    printf("Whole array address: %p\n", whole);

    return 0;
}

// Return base address
int* arr_add(int* arr) {
    return arr;
}

// Return pointer to whole array
int (*arr_ret(int a[3]))[3] {
    return &a;  // &a is of type int (*)[3]
}

