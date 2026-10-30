//Q5 one Pointer to another Conversion
#include <stdio.h>

int main()
{
    int num = 100;
    int *ip;
    float *fp;
    void *vp;

    // int pointer to void pointer 
    ip = &num;
    vp = ip;

    printf("Value using int pointer = %d\n", *ip);

    // void pointer to float pointer 
    fp = (float *)vp;

    printf("Address of num = %p\n", (void *)&num);
    printf("Address using int pointer = %p\n", (void *)ip);
    printf("Address using void pointer= %p\n", vp);
    printf("Address using float ptr = %p\n", (void *)fp);

    return 0;
}//main
