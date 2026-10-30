#include <stdio.h>
#include <stdlib.h>

typedef struct StackArr
{
    int size;
    int top;
    int *stack;
} stack;

void initStack(stack *s)
{
    int n;
    printf("Enter Size of Stack : ");
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("Size must be between 1 to N\n\n");
        return;
    }

    s->size = n;
    s->top = -1;
    s->stack = (int *)calloc(s->size, sizeof(int));

    if (s->stack == NULL)
    {
        printf("Stack Not Initallised\n\n");
    }
}

int isEmpty(stack s)
{
    if (s.top == -1)
    {
        printf("Stack is Emptyl\n\n");
        return 1;
    }
    return 0;
}

int isFull(stack s)
{
    if (s.top == (s.size - 1))
    {
        printf("Stack is full\n\n");
        return 1;
    }
    return 0;
}

void push(stack *s)
{
    int ele;
    if (isFull(*s) == 1)
    {
        return;
    }
    else
    {
        printf("\n");
        printf("Enter Element : ");
        scanf("%d", &ele);
        printf("\n\n");

        s->top++;

        s->stack[s->top] = ele;
    }
}

void pop(stack* s)
{

    if (isEmpty(*s) == 1)
    {
        return;
    }
    else
    {
        printf("Popped Element : %d \n\n", (*s).stack[(*s).top]);
        s->top --;
    }
}

void peek(stack s)
{

    if (isEmpty(s) == 1)
    {
        return;
    }
    else
    {
        printf("Element at Top : %d \n\n", s.stack[s.top]);
    }
}

void display(stack s)
{

    if (isEmpty(s) == 1)
    {
        printf("Stack is Empty\n");
    }
    else
    {
        for (int i = s.top; i >= 0; i--)
            printf("Element %d : %d\n", i, s.stack[i]);
    }
}