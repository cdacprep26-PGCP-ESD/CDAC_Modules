#include<stdio.h>
#include<stdlib.h>

typedef struct queueArr
{
    int size;
    int front;
    int rear;
    int *queue;
} queue;

void initQueue(queue *q)
{
    int n;
    printf("Enter Size of Queue : ");
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("Size must be between 1 to N\n\n");
        return;
    }

    q->size = n;
    q->front = -1;
    q->rear = -1;
    q->queue = (int *)calloc(q->size, sizeof(int));

    if (q->queue == NULL)
    {
        printf("Queue Not Initallised\n\n");
    }
}

int isEmpty(queue q)
{
    if(q.front == -1)
    {
        printf("Queue is Empty\n");
        return 1;
    }
    else
        return 0;
}

int isFull(queue q)
{
    if(q.rear == q.size-1)
    {
        printf("Queue is full\n");
        return 1;
    }
    else
        return 0;
}

void enQueue(queue *q)
{
    if(isFull(*q) == 1)
    {
        return;
    }
    int ele;
    printf("Enter Element : ");
    scanf("%d", &ele);
    printf("\n");

    q->rear++;

    q->queue[q->rear] = ele;

    if(q->front == -1)
    {
        q->front++;
    }
}

int deQueue(queue *q)
{
    int ele;
    if(isEmpty(*q) == 1)
    {
        return 1;
    }
   
    else
    {
        ele = q->queue[q->front];
        printf("The element which is dequeue : %d \n", ele);
        q->front++;
        if(q->front > (q->size)-1)
        {
            q->front = -1;
            q->front = -1;
        }
    }
    return 1;
}

void display(queue q)
{
    if(isEmpty(q) == 1)
    {
        return;
    }
    else
    {
        for (int i = q.front; i<= q.rear; i++)
        printf("Element %d : %d\n", i, q.queue[i]);
    }
}

