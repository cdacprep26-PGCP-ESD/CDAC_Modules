#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *Next;
} sNode;

sNode *CreateNode(int data)
{
    sNode *temp = NULL;
    temp = (sNode *)malloc(sizeof(sNode));
    temp->data = data;
    temp->Next = NULL;
    return temp;
}

void push(int data, sNode **top)
{
    sNode *temp = NULL;
    temp = CreateNode(data);
    if (*top == NULL)
    {
        *top = temp;
    }
    else
    {
        temp->Next = *top;
        *top = temp;
    }
}

int isEmpty(sNode *top)
{
    if (top == NULL)
    {
        printf("Stack is Empty\n");
        return 1;
    }

    return 0;
}

int pop(sNode **top)
{
    if (isEmpty(*top) == 1)
    {
        return 0;
    }
    sNode *temp = *top;
    *top = temp->Next;
    printf("Popped Element: %d", temp->data);
    free(temp);
}

int peek(sNode *top)
{
    if (isEmpty(top) == 1)
    {
        return 0;
    }
    return top->data;
}

void display(sNode *temp)
{

    if (isEmpty(temp) == 1)
    {
        return;
    }
    while (temp != NULL)
    {
        printf("%d\t", temp->data);
        temp = temp->Next;
    }

    printf("\n");
}