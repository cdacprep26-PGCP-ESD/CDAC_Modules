#include <stdio.h>

void accept(int *a, int n)
{
    printf("Enter the array element\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }
}

void Display(int *a, int n)
{
    printf("The array is:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void sel_sort(int *a, int n)
{
    int temp;
    for (int i = 0; i < n - 1; i++)
    {
        int index = i;
        for (int j = i + 1; j < n; j++)
        {
            if (a[index] > a[j])
            {
                temp = a[index];
                a[index] = a[j];
                a[j] = temp;
            }
        }
    }
    Display(a, n);
}

void in_Sort(int *a, int n)
{
    int pivot, j, cont = 0;
    for (int i = 1; i < n; i++)
    {
        pivot = a[i];
        for (j = i - 1; j >= 0; j--)
        {
            if (a[j] > pivot)
            {
                cont = 1; 
                continue;
            }
            else
            {
                break; 
            }
        }

        if (cont == 1)
        {
            int k = i - 1;
            while (k > j) 
            {
                a[k + 1] = a[k];
                k--;
            }
            a[j + 1] = pivot; 
            cont = 0;
        }
    }

    Display(a, n);
}

void bubl_sort(int *a, int n)
{
    int temp;
    for(int i = 0; i< n; i++)
    {
        for(int j = 0; j<n-1; j++)
        {
            if (a[j] > a[j+1])
            {
                temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        
        }
    }
    Display(a, n);
    
}
