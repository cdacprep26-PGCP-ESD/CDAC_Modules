//Q4 Pointer arithmetic
#include <stdio.h>

int main()
{
    int a[5] = {10, 20, 30, 40, 50};
    int *p, *q;

    /* *p++ */
    p = a;
    printf("*p++ = %d\n", *p++);

    /* *++p */
    p = a;
    printf("*++p = %d\n", *++p);

    /* (*p)++ */
    p = a;
    printf("(*p)++ = %d\n", (*p)++);

    /* ++(*p) */
    p = a;
    printf("++(*p) = %d\n", ++(*p));

    /* ++*p */
    p = a;
    printf("++*p = %d\n", ++*p);

    /* *(p++) */
    p = a;
    printf("*(p++) = %d\n", *(p++));

    /* *(++p) */
    p = a;
    printf("*(++p) = %d\n", *(++p));


    /* *q-- */
    q = *(&a + 1) - 1;
    printf("\n*q-- = %d\n", *q--);

    /* *--q */
    q = *(&a + 1) - 1;
    printf("*--q = %d\n", *--q);

    /* --(*q) */
    q = *(&a + 1) - 1;
    printf("--(*q) = %d\n", --(*q));

    /* --*q */
    q = *(&a + 1) - 1;
    printf("--*q = %d\n", --*q);

    /* (*q)-- */
    q = *(&a + 1) - 1;
    printf("(*q)-- = %d\n", (*q)--);

    /* *(q--) */
    q = *(&a + 1) - 1;
    printf("*(q--) = %d\n", *(q--));

    /* *(--q) */
    q = *(&a + 1) - 1;
    printf("*(--q) = %d\n", *(--q));

    return 0;
}//main

