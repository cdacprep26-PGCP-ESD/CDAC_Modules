#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mrgSort(int *a, int low, int high);
void merge(int *a, int low, int mid, int high);

int main()
{
    int n;
    int *arr;

    printf("Enter Number of Elements in the Array : ");
    scanf("%d", &n);

    arr = (int *)malloc(n * sizeof(int));

    printf("Enter Array Elements : \n");
    for(int i = 0; i<n; i++)
    {
        scanf("%d", &arr[i]);
    }

    mrgSort(arr, 0, n-1);

    printf("Entered Array : \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t", arr[i]);
    }

    printf("\n");
}

void mrgSort(int *a, int low, int high)
{
    if (low < high)
    {
        int mid = (low + high) / 2;
        mrgSort(a, low, mid);
        mrgSort(a, mid + 1, high);
        merge(a, low, mid, high);
    }
}

void merge(int *a, int low, int mid, int high)
{
    int n1 = (mid - low) + 1;
    int n2 = high - mid;

    int L[n1];
    int R[n2];

    for (int i = 0; i < n1; i++)
    {
        L[i] = a[low + i];
    }

    for (int i = 0; i < n2; i++)
    {
        R[i] = a[mid + 1 + i];
    }

    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            a[low + k] = L[i];
            i++;
            k++;
        }
        else
        {
            a[low + k] = R[j];
            j++;
            k++;
        }
    }

    while (i < n1)
    {
        a[low + k] = L[i];
        k++;
        i++;
    }

    while (j < n2)
    {
        a[low + k] = R[j];
        k++;
        j++;
    }
}
