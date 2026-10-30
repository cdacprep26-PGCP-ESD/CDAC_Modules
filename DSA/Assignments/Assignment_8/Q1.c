#include<stdio.h>
#include"Q1.h"


void push(int data, sNode **top);
int isEmpty(sNode *top);
int pop(sNode **top);
int peek(sNode *top);
void display(sNode *temp);
int main()
{
    sNode *top = NULL;

    while(1)
    {
        int choice,data;
        printf("\n");
        printf("0. Exit\n");
        printf("1. Push Node\n");
        printf("2. Check if stack is empty\n");
        printf("3. Pop Node\n");
        printf("4. Peek the Node\n");
        printf("5. Display stack\n");

        printf("Enter your choice\n");
        scanf("%d",&choice);

        switch(choice)
        {
            case 0: 
                printf("Exit\n");
                break;
            case 1:
                printf("Enter the data:\n");
                scanf("%d",&data);
                push(data, &top);
                break;
            case 2:
                isEmpty(top);
                break;
            case 3:
                pop(&top);
                break;
            case 4:
                int d = peek(top);
                printf("The top of stack:%d\n",d);
                break;
            case 5:
                display(top);
                break;
            default:
                printf("Invalide Input\n");
                break;
        }

                

    }
}
