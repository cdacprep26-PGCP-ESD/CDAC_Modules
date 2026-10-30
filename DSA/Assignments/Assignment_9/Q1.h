#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node **CreateArray(Node** a, int* size)
{
    printf("Enter Size : ");
    scanf("%d", size);
    printf("\n");
    a = (Node **)malloc(*size * sizeof(Node *));
    for (int i = 0; i < *size; i++)
    {
        a[i] = NULL;
    }
    return a;
}

Node **takeInput(Node **a, int size)
{
    int ele, index;
    printf("Enter Element : ");
    scanf("%d", &ele);

    index = ((ele % size)+size)%size;

    if (a[index] == NULL)
    {
        a[index] = (Node *)malloc(sizeof(Node *));
        if(a[index] == NULL)
        {
            printf("Memoery Not Alloted\n");
            return a;
        }
        a[index]->data = ele;
        a[index]->next = NULL;
    }
    else
    {
        Node *temp = NULL;
        temp = (Node *)malloc(sizeof(Node *));
        temp->data = ele;
        temp->next = a[index];
        a[index] = temp;
        
    }
    return a;
}

void search(Node** a, int data, int size)
{
    int count = 0;
    int index = ((data % size)+size)%size;

    Node* temp = a[index];
    while(temp != NULL)
    {
        if(temp->data == data)
        {
            printf("Element Found in Bucket : %d\n", index);
            return;
        }
        temp = temp->next;
    }

    printf("Element Not Found\n");    
}

void display(Node** a, int size)
{
    
    for(int i = 0; i<size; i++)
    {
        printf("%d: ", i);
        Node* temp = a[i];
        while(temp != NULL)
        {
            printf("%d->",temp->data);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}
