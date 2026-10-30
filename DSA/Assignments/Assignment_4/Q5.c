#include <stdio.h>
#include "circular_ll.h"
void menu();
int Choice();
void insertAtBeg(struct list **head,struct list** tail, int data);
void insertAtEnd(struct list **head,struct list **tail, int data);
void display(struct list *);
int get_Data();
void remAtBeg(struct list **head, struct list **tail);
void remAtEnd(struct list** head, struct list** tail);

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
                        remAtBeg(&head, &tail);
                        break;
                case 6:
                        remAtEnd(&head, &tail);
                        display(head);
                        break;
                default:
                        printf("Invalid Input !!\n");
                        break;
                }
        }
}
