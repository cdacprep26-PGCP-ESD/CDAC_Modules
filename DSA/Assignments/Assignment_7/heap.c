#include<stdio.h>
#include<stdlib.h>

int main()
{
    int a[7] = {234, 354, 6456, 3432, 76, 446, 45};
    heapSort(a, 7);
    return 0;
}

void heapSort(int a[], int n)
{
    for(int i = (n/2)-1; i>=0; i-- )
    {
        heapyFy(a, i, n);
    }

    for(int i = n-1; i>0; i--)
    {
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;

        heapyFy(a, 0, n);
    }
    
}

void heapyFy(int a[], int i, int n)
{
    int largest = i;
    int left = (2*i) + 1;
    int right = (2*i) + 2;

    if(a[left] > a[largest] )
    {
        largest = left;
    }

    if(a[right] > a[largest] )
    {
        largest = right;
    }
}