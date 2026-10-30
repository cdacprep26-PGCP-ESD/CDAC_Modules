//Q7.Testing Pointer Conversion between int* to const int* and vice veresa
#include <stdio.h>

void test(const int *p)
{
    printf("Value = %d\n", *p);

    // *p = 20; 
    // ERROR: cannot modify through const int *
}

void test2(int *p)
{
    *p = 20;
    printf("Modified value = %d\n", *p);
}

int main()
{
    int x = 10;
    const int y = 30;

    int *p = &x;
    const int *q = &y;

    printf("Calling test(p):\n");
    test(p); // int * to const int * is ALLOWED

    printf("\nCalling test(q):\n");
    test(q); // const int * to const int * is ALLOWED

    printf("\nCalling test2(p):\n");
    test2(p); // int * to int * is ALLOWED

    // test2(q); // WARNING/ERROR:
   // const int * cannot be passed to int *

    return 0;
   }
