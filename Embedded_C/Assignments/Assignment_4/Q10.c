// Finding day of the week for given date 
#include<stdio.h>

int main()
{
    int d , m , y , ry = 1970, days = 0, days2 = 0, days3, day_i, n;
    int arr[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    char* w_days [] = { "Thursday" ,"Friday", "Saturday", "Sunday", "Monday", "Tuesday", "Wednessday"};
	
    printf("Enter Date : ");
    scanf("%d %d %d" , &d, &m, &y);

    if( m> 12 || d>31)
    {
	    printf("Invalid Input!!\n");
	    return 0;
    }
   
    n = y - 1970;
    
    for ( int i = 1 ; i<= n; i++) // Calculatiing total days till year-1
    {
        if( ( (ry%4 == 0) && (ry%100 != 0) ) || (ry%400 == 0) )
        {
            days = days + 366;
        }
        else
        {
            days = days + 365;
        }
        ry++ ;
    }

    
    for( int i = 0; i<m-1; i++) // Calculating days till month-1
    {
        days2 = days2 + arr[i];
    }
    
    days3 = days + days2;

    if( (( (y%4 == 0) && (y%100 != 0) ) || (y%400 == 0)) && m > 2)
    {
     days3 = days3 + 1;
    }


    days3 = days3 + d - 1;// Calculaiing days till the given date
    day_i = days3 % 7; // Finding exact day
    

    printf("Day is %s\n", w_days[day_i]); 
    return 0;

} // main
