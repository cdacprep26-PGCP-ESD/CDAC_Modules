#include <stdio.h>
#include <stdlib.h>
#include"Q1.h"

void in_Sort(int *a, int n);
void Display(int *a, int n);
void accept(int *a, int n);
void sel_sort(int *a, int n);
void bubl_sort(int *a, int n);

int main()
{
    int n;

    printf("enter the length of array:\n");
    scanf("%d",&n);

    int *a = malloc(n * sizeof(int));
    
    accept(a, n);
    
    while(1)
    {
        int choice;

        printf("1. Selection sort\n");
        printf("2. Insertion sort\n");
        printf("3. Bubble sort\n");
        printf("4. Exit\n");

        printf("Enter the choice:\n");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                    sel_sort(a, n);
                    break;
            case 2:
                    in_Sort(a, n);
                    break;
            case 3:
                    bubl_sort(a, n);
                    break;
            case 4:
                    printf("Exit\n");
                    break;
            default:
                    printf("Invalid Input\n");
                    break;
        }
    }
    return 0;
}

