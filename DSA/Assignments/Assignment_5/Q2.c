#include <stdio.h>
#include "Q1.h"

void initQueue(queue *q);
int isEmpty(queue q);
int isFull(queue q);
void enQueue(queue *q);
int deQueue(queue *q);
void display(queue q);

int main()
{
    queue q;
    int choice;
    printf("\n--- QUEUE OPERATIONS MENU ---\n");
    printf("0. Exit\n");
    printf("1. Initialize Queue\n");
    printf("2. Check if Empty\n");
    printf("3. Check if Full\n");
    printf("4. Enqueue\n");
    printf("5. Dequeue\n");
    printf("6. Display\n");

    while (1)
    {
        printf("Enter the choice:");
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            free(q.queue);
            printf("Exit\n");
            break;
        case 1:
            initQueue(&q);
            break;
        case 2:
            isEmpty(q);
            break;
        case 3:
            isFull(q);
            break;
        case 4:
            enQueue(&q);
            break;
        case 5:
            deQueue(&q);
            break;
        case 6:
            display(q);
            break;
        default:
            printf("Invalide Input\n");
            break;
        }
    }
    return 0;
}