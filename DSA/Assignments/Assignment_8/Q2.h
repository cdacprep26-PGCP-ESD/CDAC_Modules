#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
}qNode;

struct node *front = NULL;
struct node *rear = NULL;

qNode *create(int value)
{
    qNode *temp = NULL;
    temp = (qNode *)malloc(sizeof(qNode));
    temp->data = value;
    temp->next = NULL;
    return temp;
}

int empty()
{
   return front == NULL;
}
void enqueue(int value)
{
    qNode *newnode = create(value);

    if(empty())
    {
        front = newnode;
        rear = newnode;
    }
    else
    {
        rear->next = newnode;
        rear = newnode;

    }
}

void dequeue()
{
    qNode *temp;
    if(empty())
    {
        printf("Queue is empty\n");
        return;
    }

    temp = front;
    printf("Deleted node:%d\n",temp->data);
    front = front->next;

    if(front == NULL)
    {
        rear = NULL;
    }
    free(temp);

}

void front_val()
{
    if(empty())
    {
        printf("Queue is empty\n");
        return;
    }

    printf("First value:%d\n",front->data);
}

void rear_val()
{
    if(empty())
    {
        printf("Queue is empty\n");
        return;
    }

    printf("Last value:%d\n",rear->data);
}

void display()
{
    qNode *temp = front;
    if(empty())
    {
        printf("Queue is empty\n");
        return;
    }
    else{
        while(temp != NULL)
        {
            printf("%d\t",temp->data);
            temp = temp->next;
        }
    }
}

