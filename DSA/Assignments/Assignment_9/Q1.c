#include<stdio.h>
#include<stdlib.h>
#include"Q1.h"

Node **CreateArray(Node** , int* );
Node **takeInput(Node **, int );
void search(Node** , int, int);
void display(Node** , int);

int main(void)
{
    Node **table = NULL;
    int size = 0;
    int choice = 0;

    do
    {
        printf("\n----- Hash Table Menu -----\n");
        printf("1. Create table\n");
        printf("2. Insert element\n");
        printf("3. Search element\n");
        printf("4. Display table\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                if (table != NULL)
                {
                    printf("Table already exists (size %d).\n", size);
                    break;
                }
                table = CreateArray(table, &size);
                if (table == NULL)
                {
                    printf("Table creation failed.\n");
                }
                break;

            case 2:
                takeInput(table, size);
                break;

            case 3:
            {
                int key;
                printf("Enter element to search: ");
                if (scanf("%d", &key) == 1)
                {
                    search(table, key, size);
                }
                else
                {
                    while (getchar() != '\n');
                    printf("Invalid input.\n");
                }
                break;
            }

            case 4:
                display(table, size);
                break;

            case 5:
                for(int i = 0; i<size; i++)
                {
                    free(table[i]);
                }
                free(table);
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Enter a number from 1 to 5.\n");
        }
    } while (choice != 5);

    return 0;
}


