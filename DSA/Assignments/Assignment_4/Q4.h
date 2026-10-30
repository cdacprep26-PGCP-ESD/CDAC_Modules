#include <stdio.h>
#include <stdlib.h>

struct list
{
    int data;
    struct list* Next;
    struct list* prev;
};

int n = 0;

void menu()
{
    printf("1.Create Singly Linked List\n");
    printf("2.Insert AT Begining\n");
    printf("3.Insert At End\n");
    printf("4.Display\n");
    printf("5.Inser At Position\n");
    printf("6.Remove the node at the beginning of the list\n");
    printf("7.Remove the node at the end of the list\n");
    printf("8.Delete a node at a given position in the list\n\n\n");
    printf("0. Exit\n");
}

int Choice()
{
    int ch;
    printf("Enter Choice : ");
    scanf("%d", &ch);
    return ch;
}

void insertAtBeg(struct list **head,struct list** tail, int data)
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
            temp->Next = NULL;
            temp->prev = NULL;
            *tail = temp;
            *head = temp;      
            printf("Linked List Created\n");
            n++;
        }
        else
        {
            (*head)->prev = temp;
            temp->Next = *head;
            temp->prev = NULL;
            *head = temp;
            printf("Node Created\n");
            n++;
        }
    }
}

void insertAtEnd(struct list **head,struct list **tail, int data)
{
    struct list *temp = NULL;

    temp = (struct list *)malloc(sizeof(struct list));
    if (temp == NULL)
    {
        printf("Memory NOt Allocated\n");
        return ;
    }
    
        temp->data = data;
        (*tail)->Next = temp;
        temp->Next = NULL;
        temp->prev = temp;
        (*tail) = temp;
        n++;
       
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
        temp = head;
        while (temp != NULL)
        {
            printf("%d->", temp->data);
            temp = temp->Next;
        }
        printf("\n");
    }
}

void insertAtMid(struct list *head, int data, int Index)
{
    struct list* temp = head;
    struct list* newN = NULL;

    newN = (struct list *)malloc(sizeof(struct list));
    

    for (int i = 1; i < Index; i++)
    {
       
        temp = temp->Next;
        
    }
    newN->data = data;
    newN->Next = temp->Next;
    newN->prev = temp;
    temp->Next = newN;
    display(head);
}

int get_Data()
{
    int data;
    printf("Enter Data : ");
    scanf("%d", &data);
    return data;
}

int get_Index()
{
    int Index;
    printf("Enter Index : ");
    scanf("%d", &Index);
    return Index;
}



void remAtBeg(struct list **head)
{
    struct list *temp = *head;
    (*head)->Next->prev = NULL; 
    *head = (*head)->Next;
    free(temp);
    display(*head);
}

void remAtEnd(struct list** tail)
{
    struct list *temp = *tail;
    (*tail)->prev->Next = NULL;
    *tail = temp->prev;
    free(temp);

}

void remATPos(struct list *temp)
{
    int pos;
    printf("Enter Positon : ");
    scanf("%d", &pos);

    if(pos>=n)
    {
        printf("Position is Out of Bound\n");
        return;
    }


    for(int i = 1; i<pos; i++)
    {
      //  printf("%d\t", temp->data);
        temp = temp ->Next;
    }
    printf("\n");
    temp->prev->Next = temp->Next;
    temp->Next->prev = temp->prev;

    free(temp);
    
    
}


