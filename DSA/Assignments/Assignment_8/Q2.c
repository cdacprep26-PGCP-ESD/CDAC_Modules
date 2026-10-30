#include<stdio.h>
#include"Q2.h"

int empty();
void enqueue(int value);
void dequeue();
void front_val();
void rear_val();
void display();

int main()
{
    int choice, value;

    while(1)
    {
        printf("0. Exit\n");
        printf("1. to check queue is empty\n");
        printf("2. Enqueue\n");
        printf("3. Dequeue\n");
        printf("4. front (first value)\n");
        printf("5. Rear (last value)\n");
        printf("6. Display\n");

        printf("Enter your choice:\n");
        scanf("%d",&choice);

        switch(choice)
        {
            case 0:
                printf("Exit\n");
                break;
            case 1:
                empty();
                break;
            case 2:
                printf("Enter the value:\n");
                scanf("%d",&value);
                enqueue(value);
                break;
            case 3:
                dequeue();
                break;
            case 4:
                front_val();
                break;
            case 5:
                rear_val();
                break;
            case 6:
                display();
                break;
            default:
                printf("Invlide value\n");
                break;
        }
    }
}

