#include <stdio.h>


void menu()
{
    printf("1. Create array\n");
    printf("2. Search Element using iteration\n");
    printf("3. Search Element using recursion\n");
    printf("4. Print Input array ( iterative approach)\n");
    printf("5. Print Input array ( recursion approach)\n");
    printf("6. Print Input array reverse order ( iterative approach )\n");
    printf("7. Print Input array reverse order ( recursion approach )\n");
    printf("8. Exit\n\n");
}
void create(int *a, int n)
{
    printf("Enter the %d Elements of array\n", n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    printf("\n");
}

void search(int *a, int n, int key)
{
    int temp[n];
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        if (a[i] == key)
        {
            temp[j] = i;
            j++;
        }
    }

    for (int i = 0; i < j; i++)
    {
        printf("Key %d is at Position %d\n",key,temp[i]);
    }

    if (j == 0)
    {
        printf("key Not found\n");
    }
}

void print_arr(int *a, int n)
{
    printf("The array is:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void rev_arr(int *a, int n)
{
    printf("Reverse Array:\n");
    for (int i = n; i >= 0; i--)
    {
        printf("%d\t", a[i]);
    }
    printf("\n");
}


void rec_search(int *a, int n, int key, int m)
{
    if(key == a[0])
    {
        printf("Key found at %d Position\n", m - n);
        return ;
    }
    if(n == 0)
    {
        printf("Key Not Found\n");
        return;
    }
    rec_search(&a[1], n-1, key, m);
}

void print_rec(int* a, int n, int i)
{
    if(i==n)
    {
        return;
    }

    printf("%d\t", a[i]);
    print_rec(a, n, i+1);
}


void rev_rec(int* a, int n, int i)
{
    if(i==n)
    {
        return;
    }

    print_rec(a, n, i+1);
    printf("%d\t", a[i]);
}
/*void serach(int *a, int key, int n)
{
    int l,h,mid,found=0;
    l = 0;
    h = n-1;

    while(l<=h)
    {
        mid = (l+h)/2;
        printf("mid = %d\n",mid);

        if(a[mid] == key)
        {
            found = 1;
            break;
        }
        else if(a[mid]<key)
        {
            l = mid;
        }
        else
        {
            h = mid;
        }
    }

    if(found == 0)
    {
        printf("Key not found\n");
    }
    else
    {
        printf("Key found\n");
    }


}*/
