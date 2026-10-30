#include<stdio.h>
#include"Q1.h"

void preorder(struct Node *root);
void inorder(struct Node *root);
void postorder(struct Node *root);
struct Node* search(struct Node *root,int key);

int main()
{
    struct Node *root=NULL;

    root = insert(root, 100);
    root = insert(root, 50);
    root = insert(root, 150);
    root = insert(root, 25);
    root = insert(root, 125);
    root = insert(root, 175);
    root = insert(root, 75);

    while(1)
    {
        int choice,key;
        struct Node *result;
        printf("\n\n");
        printf("0. Exit\n");
        printf("1. Preorder\n");
        printf("2. Inorder\n");
        printf("3. Postorder\n");
        printf("4. Search node\n");
        printf("Enter your choice:\n");
        scanf("%d",&choice);

        switch(choice)
        {
            case 0:
                printf("Exit\n");
                break;
            case 1:
                preorder(root);
                break;
            case 2:
                inorder(root);
                break;
            case 3:
                postorder(root);
                break;

            case 4:
                printf("Enter the key:\n");
                scanf("%d",&key);
                result = search(root, key);
                if(result == NULL)
                {
                    printf("Key not found\n");
                }
                else{
                    printf("Key found\n");
                }
                break;
                
            default:
                printf("Invalide Input\n");
                break;
        }

    }
    free(root);
    return 0;

}