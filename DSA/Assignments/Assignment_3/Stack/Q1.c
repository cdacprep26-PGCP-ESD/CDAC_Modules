#include <stdio.h>
#include <stdlib.h>
#include "Q1.h"

void initStack(stack *s);
int isEmpty(stack s);
int isFull(stack s);
void push(stack *s);
void pop(stack* s);
void peek(stack s);
void display(stack s);

int main()
{
    stack s;
    int ch;

    printf("\n--- STACK OPERATIONS MENU ---\n");
    printf("0. Exit\n");
    printf("1. Initialize Stack\n");
    printf("2. Check if Empty\n");
    printf("3. Check if Full\n");
    printf("4. Push\n");
    printf("5. Pop\n");
    printf("6. Peek\n");
    printf("7. Display\n");
    printf("-----------------------------\n");

    while (1)
    {
        printf("Enter Choice : ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 0:
            free(s.stack);
            printf("Exiting !!\n");
            return 0;
        case 1:
            initStack(&s);
            break;
        case 2:
            isEmpty(s);
            break;
        case 3:
            isFull(s);
            break;
        case 4:
            push(&s);
            break;
        case 5:
            pop(&s);
            break;
        case 6:
            peek(s);
            break;
        case 7:
            display(s);
            break;
        default:
            printf("Invalid Input\n");
            break;
        }
    }

    return 0;
}
