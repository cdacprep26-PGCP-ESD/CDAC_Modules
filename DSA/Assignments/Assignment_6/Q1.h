#include<stdio.h>
#include<stdlib.h>

struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node* create(int data)
{
    struct Node *newnode;
    newnode =(struct Node *)malloc(sizeof(struct Node));

    if(newnode == NULL)
    {
        return NULL;
    }
    newnode->data = data;
    newnode->left = NULL;
    newnode->right = NULL;

    return newnode;
}

struct Node* insert(struct Node *root, int data)
{
    if(root == NULL)
    {
        return create(data);
    }

    if(data < root->data)
    {
        root->left = insert(root->left, data);
    }
    else if(data > root->data)
    {
        root->right = insert(root->right, data);
    }

    return root;
}

void preorder(struct Node *root)
{
    if(root == NULL)
    {
        return;
    }

    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);

}

void inorder(struct Node *root)
{
    if(root == NULL)
    {
        return;
    }

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void postorder(struct Node *root)
{
    if(root == NULL)
    {
        return;
    }

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);

}

struct Node* search(struct Node *root,int key)
{
    if(root == NULL || key == root->data)
    {
        return root;
    }
    if(key < root->data)
    {
        return search(root->left,key);
    }
    if(key > root->data)
    {
        return search(root->right,key);
    }
}


struct Node* findMin(struct Node* root)
{
    while(root != NULL && root->left != NULL)
    {
        root = root->left;
    }
    return root;
}


struct Node* delNode(struct Node* root, int key)
{
    if(root == NULL)
    {
        printf("Root Not Found\n");
        return root;
    }

    if(root->data > key)
    {
        root->left = delNode(root->left, key);
    }
    else if(root->data < key)
    {
        root->right = delNode(root->right, key);
    }
    else
    {
        if(root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }

        if(root->left == NULL)
        {
            struct Node* temp = root->right;
            free(root);
            return temp;
        }

         if(root->right == NULL)
        {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        if(root->left != NULL && root->right != NULL)
        {
            struct Node* temp = findMin(root->right);
            root->data = temp->data;
            root->right = (root->right, temp->data);
        }
    }
    return root;
}

