#include<stdio.h>
#include<stdlib.h>
#include"Q1.h"

void create(int *a, int n);
void search(int *a, int key, int n);
void rec_search(int *a, int n, int key, int m);
void print_arr(int *a, int n);
void print_rec(int* a, int n, int i);
void rev_arr(int *a, int n);
void rev_rec(int* a, int n, int i);


int main()
{
    int n,key;

    printf("Enter the length of array:\n");
    scanf("%d",&n);

    int *a = malloc(n * sizeof(int));

    if(a == NULL)
    {
        printf("Memory allocation is failed\n");
        return 1;
    }

    menu();

    while(1)
    {
        int choice;
        printf("Enter choice:\n");
        scanf("%d",&choice);

        switch (choice)
        {
        case 1:
                create(a, n);
                break;
        case 2:
                printf("Enter the key:\n");
                scanf("%d",&key);
                search(a, n, key);
                break;
        case 3:
                printf("Enter the key:\n");
                scanf("%d",&key);
                rec_search(a, n, key, 0);
                break;
        case 4:
                print_arr(a, n);
                break;
        case 5:
                print_rec(a, n, 0);
                break;
        case 6:
                rev_arr(a, n);
                break;
        case 7:
                rev_rec(a, n, 0);
                break;
        case 8:
                printf("Exit\n");
                break;
        default:
                printf("Invalide Input\n");
                break;

        }
}
        free(a);

        return 0;
}