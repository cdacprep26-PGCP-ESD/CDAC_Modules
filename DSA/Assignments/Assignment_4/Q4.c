#include <stdio.h>
#include "doubly_ll.h"
void menu();
int Choice();
void insertAtBeg(struct list **head,struct list** tail, int data);
void insertAtEnd(struct list **head,struct list **tail, int data);
void display(struct list *);
void insertAtMid(struct list *, int, int);
int get_Data();
int get_Index();
void remAtBeg(struct list **head);
void remAtEnd(struct list** tail);
void remATPos(struct list *temp);

int main()
{
        struct list *head = NULL;
        struct list *tail = NULL;

        int data, Index, choice;

        menu();
        while (1)
        {

                choice = Choice();

                switch (choice)
                {
                case 0:
                        printf("Exiting !!\n");
                        return 0;
                case 1:
                        data = get_Data();
                        insertAtBeg(&head, &tail, data);
                        break;
                case 2:
                        data = get_Data();
                        insertAtBeg(&head, &tail, data);
                        break;
                case 3:
                        data = get_Data();
                        insertAtEnd(&head, &tail, data);
                        break;
                case 4:
                        display(head);
                        break;
                case 5:
                        Index = get_Index();
                        data = get_Data();
                        insertAtMid(head, data, Index);
                        break;
                case 6:
                        remAtBeg(&head);
                        break;
                case 7:
                        remAtEnd(&tail);
                         display(head);
                        break;
                case 8:
                        remATPos(head);
                        display(head);
                        break;

                default:
                        printf("Invalid Input !!\n");
                        break;
                }
        }
}
