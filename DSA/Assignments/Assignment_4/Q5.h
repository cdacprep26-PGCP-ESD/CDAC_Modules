#include <stdio.h>
#include <stdlib.h>

struct list
{
    int data;
    struct list *Next;
};

int n = 0;

void menu()
{
    printf("1.Create Singly Linked List\n");
    printf("2.Insert AT Begining\n");
    printf("3.Insert At End\n");
    printf("4.Display\n");
    printf("5.Remove the node at the beginning of the list\n");
    printf("6.Remove the node at the end of the list\n\n\n");
    printf("0. Exit\n");
}

int Choice()
{
    int ch;
    printf("Enter Choice : ");
    scanf("%d", &ch);
    return ch;
}

void insertAtBeg(struct list **head, struct list **tail, int data)
{
    struct list *temp = NULL;
    temp = (struct list *)malloc(sizeof(struct list));
    if (temp == NULL)
    {
        printf("Memory NOt Allocated\n");
    }
    else
    {
        temp->data = data;
        if (*head == NULL)
        {

            *tail = temp;
            *head = temp;
            (*tail)->Next = *head;
            printf("Linked List Created\n");
            n++;
        }
        else
        {
            temp->Next = *head;
            *head = temp;
            (*tail)->Next = *head;
            printf("Node Created\n");
            n++;
        }
    }
}

void insertAtEnd(struct list **head, struct list **tail, int data)
{
    struct list *temp = NULL;

    temp = (struct list *)malloc(sizeof(struct list));
    if (temp == NULL)
    {
        printf("Memory NOt Allocated\n");
        return;
    }

    if (*tail == NULL)
    {
        printf("List is Empty \nCreating List\n");
        insertAtBeg(head, tail, data);
    }
    else
    {
        temp->data = data;
        (*tail)->Next = temp;
        temp->Next = *head;
        (*tail) = temp;
        n++;
    }
}

void display(struct list *head)
{

    if (head == NULL)
    {
        printf("Linked List is empty\n");
    }
    else
    {
        struct list *temp = NULL;
        while (temp != head)
        {
            if (temp == NULL)
            {
                temp = head;
            }
            printf("%d->", temp->data);
            temp = temp->Next;
        }
        printf("\n");
    }
}

int get_Data()
{
    int data;
    printf("Enter Data : ");
    scanf("%d", &data);
    return data;
}

void remAtBeg(struct list **head, struct list **tail)
{
    if (*head == NULL)
    {
        printf("List is Empty\n");
        return;
    }
    struct list *temp = *head;
    if (*head == *tail)
    {
        *head = NULL;
        *tail = NULL;
    }
    else
    {

        *head = (*head)->Next;
        (*tail)->Next = *head;
    }
    free(temp);
    display(*head);
    n--;
}

void remAtEnd(struct list **head, struct list **tail)
{
    if (*tail == NULL)
    {
        printf("List is Empty\n");
        return;
    }
    struct list *temp = *head;

    if (*head == *tail)
    {
        *head = NULL;
        *tail = NULL;
    }
    else
    {

        while (temp->Next->Next != *head)
        {
            temp = temp->Next;
        }
        free(*tail);
        *tail = temp;
        (*tail)->Next = *head;
    }

    display(*head);
    n--;
}
