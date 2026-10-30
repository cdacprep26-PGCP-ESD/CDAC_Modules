#include <stdio.h>
#include <stdlib.h>

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
    if (q.front == -1)
    {
        printf("Queue is Empty\n");
        return 1;
    }
    else
        return 0;
}

int isFull(queue q)
{
    if (((q.rear == q.size - 1) && (q.front == 0)) || ((q.rear + 1) == q.front))
    {
        printf("Queue is full\n");
        return 1;
    }
    else
        return 0;
}

void enQueue(queue *q)
{
    int ele;

    if (isFull(*q) == 1)
    {
        return;
    }

    if (isEmpty(*q) == 1)
    {
        q->front = 0;
        q->rear = 0;
    }
    else if (q->rear == q->size - 1)
    {
        q->rear = 0;
    }
    else
    {
        q->rear++;
    }

    printf("Enter Element : ");
    scanf("%d", &ele);
    printf("\n");

    q->queue[q->rear] = ele;
}

int deQueue(queue *q)
{
    int ele;
    if (isEmpty(*q) == 1)
    {
        return 1;
    }

    ele = q->queue[q->front];
    printf("The element which is dequeue : %d \n", ele);
    if (q->front == q->rear)
    {
        q->front = -1;
        q->rear = -1;
    }
    else if (q->front == (q->size) - 1)
    {
        q->front = 0;
    }
    else
    {
        q->front++;
    }

    return 1;
}

void display(queue q)
{
    if (isEmpty(q) == 1)
    {
        return;
    }

    int i = q.front;
    while (1)
    {
        printf("%d\t", q.queue[i]);
        if (i == q.rear)
        {
            break;
        }
        else if (i == (q.size - 1))
        {
            i = 0;
        }
        else
        {
            i++;
        }
    }
}
