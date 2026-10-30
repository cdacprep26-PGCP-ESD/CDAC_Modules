#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void qkSort(int *a, int low, int high);
int partition(int *a, int low, int high);

int main()
{
    int n;
    int *arr;

    printf("Enter Number of Elements in the Array : ");
    scanf("%d", &n);

    arr = (int *)malloc(n * sizeof(int));

    printf("Enter Array Elements : \n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    qkSort(arr, 0, n - 1);

    printf("Enterd Array : \n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t", arr[i]);
    }

    printf("\n");
}

void qkSort(int *a, int low, int high)
{
    if (low < high)
    {
        int p = partition(a, low, high);

        qkSort(a, low, p - 1);
        qkSort(a, p + 1, high);
    }
}

int partition(int *a, int low, int high)
{
    int L[high - low], R[high - low];
    int pivot = a[high];

    int i, j = 0, k = 0;
    for (i = low; i < high; i++)
    {
        if (a[i] <= pivot)
        {
            L[j] = a[i];
            j++;
        }
        else
        {
            R[k] = a[i];
            k++;
        }
    }

    for (i = 0; i < j; i++)
    {
        a[low + i] = L[i];
    }

    a[low + j] = pivot;

    for (i = 0; i < k; i++)
    {
        a[low + j + 1 + i] = R[i];
    }

    return low + j;
}