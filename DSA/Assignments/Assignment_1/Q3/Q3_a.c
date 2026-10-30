#include<stdio.h>
#include<stdlib.h>

int main()
{
    int*arr = NULL;
    int size;
    printf("Enter Size of an Array\n");
    scanf("%d",&size);
    arr = (int*)malloc(size*sizeof(int));

    printf("Enter Elements\n");
    for(int i = 0; i<size; i++)
    {
        scanf("%d",&arr[i]);
    }

    printf("Printing Elements\n");
    for(int i = 0; i<size; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("\n");
    
}