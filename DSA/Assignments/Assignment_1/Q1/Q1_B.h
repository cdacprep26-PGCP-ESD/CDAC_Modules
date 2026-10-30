#include<stdio.h>
#include<stdlib.h>


int len;
float avge;
int val, Ind, cnt = 0, sum = 0;
int *arr = NULL;

void createArr()
{
    printf("Enter size of Array : ");
    scanf("%d", &len);
    arr = (int*)calloc(len, sizeof(int));
}

 

void getV_I()
{
    
    printf("Enter Index:");
    scanf("%d", &Ind);
    printf("Enter Value: ");
    scanf("%d", &val);
}


void insertAt()
{
    getV_I();
    char ch_temp;

    if(arr[Ind] != 0)
    {
        printf("Value is already assigned at Index %d\n", Ind);
        printf("Do you Want to Override y or n");
        scanf(" %c", &ch_temp);
        if(ch_temp == 'y')
        {
            printf("Value Updated Successflly\n");
            arr[Ind] = val;
        }
        else if(ch_temp == 'n')
        {
            printf("Not Updated");
        }
        else
        {
            printf("Invalid Input\n");
        } 
    }

    else
    {
        arr[Ind] = val;
    }
}

void display()
{
    for(int i=0; i<len; i++)
    {
        if(arr[i]!= 0)
        {
            printf("%d\t",arr[i]);
        }
    }
    printf("\n");
}

void revdis()
{
    for(int i=len-1; i>=0; i--)
    {
        if(arr[i] != 0)
        {
            printf("%d\t",arr[i]);
        }

    }
}

void max()
{
    int max = arr[0];

    for(int i=1; i<len; i++)
    {
        if(max < arr[i])
        {
            max = arr[i];
        }
    }
    printf("The maximum element is %d\n", max);
}

void min()
{
    int min = arr[0];

    for(int i=1; i<len; i++)
    {
        if(min > arr[i])
        {
            min = arr[i];
        }
    }
    printf("The minimun element is %d\n",min);
}

void search()
{
    int value;
    printf("Enter the value that u want to search\n");
    scanf("%d",&value);

    for(int i=0; i<len; i++)
    {
        if(arr[i] == value)
        {
            printf("The value %d is at index %d\n",value,i);
        }
    }
}


void count()
{
    for(int i = 0; i<len; i++)
    {
        if(arr[i] != 0)
        {
            cnt ++;
        }
    }
    printf("Count : %d\n", cnt);
}

void avg()
{
    int n = 0;
    for(int i = 0; i<len; i++)
    {
        if(arr[i] != 0)
        {
            sum = sum + arr[i];
            n++;
        }
    }
    avge = sum/n;

    printf("Average : %f\n", avge);
}


void duplicate()
{
    int found = 0;

    for (int i = 0; i < len; i++)
    {
        if (arr[i] == 0)
            continue;                 // empty slot

        // skip if this value already appeared before index i
        int already = 0;
        for (int k = 0; k < i; k++)
        {
            if (arr[k] == arr[i])
            {
                already = 1;
                break;
            }
        }
        if (already)
            continue;

        // count total occurrences of arr[i]
        int cnt_ele = 1;
        for (int j = i + 1; j < len; j++)
        {
            if (arr[j] == arr[i])
                cnt_ele++;
        }

        if (cnt_ele > 1)
        {
            printf("Element %d is repeated %d times\n", arr[i], cnt_ele);
            found = 1;
        }
    }

    if (!found)
        printf("No duplicate elements\n");
}

void rev_mem()
{
    int i = 0;
    int j = len;
    int temp;
    while(i<j)
    {
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;

        i++;
        j--;
    }
    display();
}

void del_at()
{
    int Index;
    printf("Enter Index to Deleted : ");
    scanf("%d",&Index);
     if(arr[Index] != 0)
     {
        arr[Index = 0];
     }
     else
     {
        printf("No Element is present at %d", Index);
     }

}
